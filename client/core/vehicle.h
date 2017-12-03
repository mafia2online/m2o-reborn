// =======================================================================//
// !
// ! Generic entity events
// !
// =======================================================================//

void module_car_init() {
    //librg_network_add(ctx, MOD_CAR_ENTER, module_car_remote_enter);
}

/**
 * The entity enters the stream zone
 */
void module_car_callback_create(librg_event_t *event) {
    mod_log("creating vehicle\n");

    auto entity = event->entity;
    auto car = new car_t();
    //librg_data_rptr(event->data, &car, sizeof(car_t));

    print_posm(entity->position, "creating vehicle at:");

    std::string dir;
    std::string model;
    M2::Models::GetVehicleModelFromID(0, &dir, &model);

    M2::Wrappers::ModelManager *pModelManager = new M2::Wrappers::ModelManager();
    mod_assert(pModelManager);

    M2::Wrappers::GameModelManager *pPedModelManager = pModelManager->Load(dir.c_str(), model.c_str());
    mod_assert(pPedModelManager);

    M2::C_Car *object = M2::C_EntityFactory::Get()->CreateEntity<M2::C_Car>(M2::EntityTypes::Entity_Car);
    mod_assert(object);

    M2::C_Model *pModel = M2::C_Core::Get()->AllocateModel(2);
    mod_assert(pModel);

    pModel->CloneHierarchy(pPedModelManager->GetModelManager()->m_pModel);
    pModel->SetName("m2online_car");
    pModel->MarkForNotify(2);

    reinterpret_cast<M2::C_Entity *>(object)->SetModel(pModel);

    if (object->Init(NULL)) {
        object->m_nSlotSDS = pPedModelManager->GetModelManager()->m_pSlot->m_iSlotNumber;
        object->Setup();

        DWORD flags = reinterpret_cast<M2::C_Entity *>(object)->m_dwFlags & 0xFFFFFFBF | 0x4800;
        reinterpret_cast<M2::C_Entity *>(object)->m_dwFlags = flags;

        if (reinterpret_cast<M2::C_Entity *>(object)->m_dwFlags & 0x20)
            mod_log("Flags set sucessfully!\n");

        reinterpret_cast<M2::C_Entity *>(object)->Activate();

        if (reinterpret_cast<M2::C_Entity *>(object)->IsActive())
            mod_log("Entity active !\n");

        reinterpret_cast<M2::C_Entity *>(object)->SetPosition(entity->position);

        car->object = (M2::C_Entity *)object;

        mod_log("Created at %x with GUID: %lu!\n", object, car->object->m_dwGUID);
    }

    event->flags |= MOD_ENTITY_INTERPOLATED;
    event->entity->user_data = car;
}

/**
 * The entity exists the stream zone
 */
void module_car_callback_remove(librg_event_t *event) {
    // TODO: add vehicle removing
    //auto gamedata  = librg_fetch_gamedata(event->entity);
    //gamedata->object->Deactivate();
    mod_log("remvoing vehicle\n");
}

// =======================================================================//
// !
// ! Update/sync entity events
// !
// =======================================================================//

/**
 * The entity in our stream zone gets updated
 */
void module_car_callback_update(librg_event_t *event) {
    auto car = (car_t *)event->entity->user_data;
    auto interpolate = &car->interpolate;

    interpolate->lposition = interpolate->tposition;
    interpolate->lrotation = interpolate->trotation;

    interpolate->tposition = event->entity->position;
    interpolate->trotation = car->stream.rotation;

    interpolate->delta = 0.0f;
}

/**
 * The entity is streamed by us, and we are sending an update with new data
 */
void module_car_callback_clientstream(librg_event_t *event) {
    auto car = (car_t *)event->entity->user_data;

    librg_assert(car && car->object);

    // TODO: add rotations
    event->entity->position = car->object->GetPosition();
    car->stream.rotation = car->object->GetRotation();

    librg_data_wptr(event->data, &car->stream, sizeof(car->stream));
}

// =======================================================================//
// !
// ! Interpolation
// !
// =======================================================================//

#define MOD_ENTITY_KEEP_ALIVE 1
#define MOD_ENTITY_POSITION_THRESHOLD 0.035

void module_car_callback_interpolate(librg_entity_t *entity) {
    mod_assert(entity);

    if (librg_entity_type(ctx, entity->id) != TYPE_CAR) return;

    auto car = (car_t *)entity->user_data;
    librg_assert(car && car->object);

    // last delta tick against constant tick delay
    car->interpolate.delta += (mod.last_delta / 16.666f);
    mod_log("%f\n", car->interpolate.delta);

    auto curr_pos = car->object->GetPosition();
    auto diff_pos = curr_pos - entity->position;

    // TODO: make proper validation
    if (zpl_abs(diff_pos.x) < MOD_ENTITY_POSITION_THRESHOLD
     && zpl_abs(diff_pos.y) < MOD_ENTITY_POSITION_THRESHOLD) {
        return;
    }

    // position
    if (car->interpolate.lposition != car->interpolate.tposition) {
        vec3_t dposition;
        car->interpolate.step = 0;
        zplm_vec3_lerp(&dposition, car->interpolate.lposition, car->interpolate.tposition, car->interpolate.delta);
        car->object->SetPosition(dposition);
    }

    // rotation TODO:
    if (car->interpolate.lrotation != car->interpolate.trotation) {
        mod_log("interpolating rotation");

        auto last = car->interpolate.lrotation;
        auto dest = car->interpolate.trotation;

        quat_t drotation;
        zplm_quat_nlerp(&drotation, zplm_quat_dot(last, dest) < 0 ? -last : last, dest, car->interpolate.delta);
        car->object->SetRotation(drotation);
    }

    car->interpolate.step++;
}

// =======================================================================//
// !
// ! Custom events
// !
// =======================================================================//

/**
 * Event when local player enters a car
 */
void module_car_local_enter(void *custom) {
    auto expected = (M2::C_Entity *)custom;
    auto ped = (ped_t*)mod.player->user_data;

    mod_assert(expected && ped);
    ped->stream.state = PED_IN_CAR;

    // send vehicle create request onto server
    mod_entity_iterate(ctx, LIBRG_ENTITY_ALIVE, [&](librg_entity_t *entity) {
        if (entity->type != TYPE_CAR) return;
        auto car = (car_t *)entity->user_data;

        if (car->object == expected) {
            mod_message_send(ctx, MOD_CAR_ENTER, [&](librg_data_t *data) { librg_data_wu32(data, entity->id); });
        }
    });
}

/**
 * Event when a remote ped starts entering the car
 */
void module_car_remote_enter(librg_message_t *msg) {
    mod_log("received message for EVERYYYBODY\n");

    auto player  = librg_entity_fetch(ctx, librg_data_rent(msg->data));
    auto vehicle = librg_entity_fetch(ctx, librg_data_rent(msg->data));
    mod_assert(player && vehicle);

    auto ped = (ped_t *)player->user_data;
    auto car = (car_t *)vehicle->user_data;
    mod_assert_msg(ped->object, "trying to put ped in invalid car");
    mod_assert_msg(car->object, "trying to put invalid ped in car");

    mod_log("im not putting anyone anywhere fuck u\n");
    return;

    mod_log("putting ped: %u in the car: %u\n", player->id, vehicle->id);

    M2::C_SyncObject *pSyncObject = nullptr;
    ((M2::C_Human2*)ped->object)->GetScript()->ScrDoAction(
        &pSyncObject,
        reinterpret_cast<M2::C_Vehicle *>(car->object),
        true, M2::E_VehicleSeat::E_SEAT_DRIVER, false
    );
}
