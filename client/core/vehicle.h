// =======================================================================//
// !
// ! Generic entity events
// !
// =======================================================================//

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

    event->entity->flags |= MOD_ENTITY_INTERPOLATED;
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
    auto entity = event->entity; mod_assert(entity);
    auto car = (car_t *)(event->entity->user_data);

    // make sure we have all objects
    mod_assert(car && car->object);

    interpolate_t *interpolate = &car->interpolate;

    // interpolation stuff
    interpolate->lposition = interpolate->tposition;
    interpolate->lrotation = interpolate->trotation;
    interpolate->tposition = entity->position;
    interpolate->trotation = car->stream.rotation;
    interpolate->delta = 0.0f;

    librg_data_rptr(event->data, &car->stream, sizeof(car->stream));

    if (car->stream.speed > 0.0f) {
        ((M2::C_Car *)car->object)->SetSpeedFloat(car->stream.speed);
    }
}

/**
 * The entity is streamed by us, and we are sending an update with new data
 */
void module_car_callback_clientstream(librg_event_t *event) {
    auto car = (car_t *)event->entity->user_data;

    librg_assert(car && car->object);

    // TODO: add rotations
    event->entity->position = car->object->GetPosition();
    car->stream.rotation    = car->object->GetRotation();
    car->stream.speed       = ((M2::C_Car *)car->object)->m_fSpeed;

    librg_data_wptr(event->data, &car->stream, sizeof(car->stream));
}

// =======================================================================//
// !
// ! Interpolation
// !
// =======================================================================//

#define MOD_ENTITY_KEEP_ALIVE 1
#define MOD_ENTITY_POSITION_THRESHOLD 0.035
#define MOD_ENTITY_ROTATION_THRESHOLD 0.005

void module_car_callback_interpolate(librg_entity_t *entity) {
    auto car = (car_t *)entity->user_data;
    librg_assert(car && car->object);

    // skip entity if we are the driver
    if (mod.player->flags & MOD_ENTITY_DRIVER && ((ped_t *)mod.player->user_data)->vehicle == entity) {
        return;
    }

    // last delta tick against constant tick delay
    car->interpolate.delta += (mod.last_delta / 40.666f);
    car->interpolate.delta = zplm_clamp(car->interpolate.delta, 0.f, 1.0f);

    /* position interpolation */
    if (car->interpolate.lposition != car->interpolate.tposition) {
        auto curr_pos = car->object->GetPosition();
        auto diff_pos = curr_pos - entity->position;

        if (zpl_abs(diff_pos.x) > MOD_ENTITY_POSITION_THRESHOLD
         || zpl_abs(diff_pos.y) > MOD_ENTITY_POSITION_THRESHOLD) {
            vec3_t dposition;
            zplm_vec3_lerp(&dposition, car->interpolate.lposition, car->interpolate.tposition, car->interpolate.delta);
            car->object->SetPosition(dposition);
        }
    }

    /* rotation interpolation */
    if (car->interpolate.lrotation != car->interpolate.trotation) {
        auto curr_rot = car->object->GetRotation();
        auto diff_rot = curr_rot - car->stream.rotation;

        // if (zpl_abs(diff_rot.x) > MOD_ENTITY_POSITION_THRESHOLD
        //  || zpl_abs(diff_rot.y) > MOD_ENTITY_POSITION_THRESHOLD
        //  || zpl_abs(diff_rot.z) > MOD_ENTITY_POSITION_THRESHOLD
        //  || zpl_abs(diff_rot.w) > MOD_ENTITY_POSITION_THRESHOLD) {
            auto last = car->interpolate.lrotation;
            auto dest = car->interpolate.trotation;

            quat_t drotation;
            zplm_quat_nlerp(&drotation, zplm_quat_dot(last, dest) < 0 ? -last : last, dest, car->interpolate.delta);
            car->object->SetRotation(drotation);
        // }
    }
}

// =======================================================================//
// !
// ! Local car events
// !
// =======================================================================//

/**
 * Event when local player starts entering a local car
 */
void module_car_local_enter_start(librg_event_t *event) {
    auto vehicle = (M2::C_Entity *)event->user_data;
    auto ped     = (ped_t*)mod.player->user_data;

    mod_assert(vehicle && ped);

    // send vehicle create request onto server
    mod_entity_iterate(ctx, LIBRG_ENTITY_ALIVE, [&](librg_entity_t *entity) {
        if (entity->type != TYPE_CAR) return;
        auto car = (car_t *)entity->user_data;

        if (car->object == vehicle) {
            // set the driver data
            ped->stream.state = PED_ENTERING_CAR;
            ped->vehicle      = entity;

            mod_message_send(ctx, MOD_CAR_ENTER_START, [&](librg_data_t *data) { librg_data_wu32(data, entity->id); });
        }
    });
}

/**
 * Local ped finished entering the car
 * @param event
 */
void module_car_local_enter_finish(librg_event_t *event) {
    auto ped = (ped_t*)mod.player->user_data;

    mod.player->flags |= MOD_ENTITY_DRIVER;
    ped->stream.state = PED_IN_CAR;

    mod_message_send(ctx, MOD_CAR_ENTER_FINISH, nullptr);
}

/**
 * Event when local player starts leaving local car
 * @param event
 */
void module_car_local_exit_start(librg_event_t *event) {
    auto ped = (ped_t*)mod.player->user_data;

    // reset the driver data
    mod.player->flags &= ~MOD_ENTITY_DRIVER;
    ped->stream.state = PED_EXITING_CAR;

    mod_message_send(ctx, MOD_CAR_EXIT_START, nullptr);
}

/**
 * Event when local player finished exiting local car
 * @param event
 */
void module_car_local_exit_finish(librg_event_t *event) {
    auto ped = (ped_t*)mod.player->user_data;

    ped->vehicle = nullptr;
    ped->stream.state = PED_ON_GROUND;

    mod_message_send(ctx, MOD_CAR_EXIT_FINISH, nullptr);
}

// =======================================================================//
// !
// ! Remote car events
// !
// =======================================================================//

/**
 * Event when a remote ped starts entering the car
 */
void module_car_remote_enter_start(librg_message_t *msg) {
    auto player  = librg_entity_fetch(ctx, librg_data_rent(msg->data));
    auto vehicle = librg_entity_fetch(ctx, librg_data_rent(msg->data));
    mod_assert(player && vehicle);

    auto ped = (ped_t *)player->user_data;
    auto car = (car_t *)vehicle->user_data;
    mod_assert_msg(ped->object, "trying to put ped in invalid car");
    mod_assert_msg(car->object, "trying to put invalid ped in car");
    mod_log("[info] putting ped: %u in the car: %u\n", player->id, vehicle->id);

    // TODO: add seat sync
    M2::C_SyncObject *pSyncObject = nullptr;
    ((M2::C_Human2 *)ped->object)->GetScript()->ScrDoAction(
        &pSyncObject,
        reinterpret_cast<M2::C_Vehicle *>(car->object),
        true, M2::E_VehicleSeat::E_SEAT_DRIVER, 1
    );
}

/**
 * Remote ped finished entering the car
 * @param msg
 */
void module_car_remote_enter_finish(librg_message_t *msg) {
    auto player  = librg_entity_fetch(ctx, librg_data_rent(msg->data));
    auto vehicle = librg_entity_fetch(ctx, librg_data_rent(msg->data));
    mod_assert(player && vehicle);

    auto ped = (ped_t *)player->user_data;
    auto car = (car_t *)vehicle->user_data;
    mod_assert_msg(ped->object, "trying to put ped in invalid car");
    mod_assert_msg(car->object, "trying to put invalid ped in car");

    // TODO: add PutPlayerInVehicle focred
}

/**
 * Remote ped started exiting the car
 * @param msg
 */
void module_car_remote_exit_start(librg_message_t *msg) {
    auto player = librg_entity_fetch(ctx, librg_data_rent(msg->data)); mod_assert(player);
    auto ped = (ped_t *)player->user_data; mod_assert(ped && ped->object);

    mod_assert(ped && ped->object && ped->vehicle);

    M2::C_SyncObject *pSyncObject = nullptr;
    ((M2::C_Human2 *)ped->object)->GetScript()->ScrDoAction(
        &pSyncObject,
        reinterpret_cast<M2::C_Vehicle *>(ped->vehicle),
        false, M2::E_VehicleSeat::E_SEAT_DRIVER, 1
    );
}

/**
 * Remote ped finished exiting the car
 * @param msg
 */
void module_car_remote_exit_finish(librg_message_t *msg) {
    auto player = librg_entity_fetch(ctx, librg_data_rent(msg->data)); mod_assert(player);
    auto ped = (ped_t *)player->user_data; mod_assert(ped && ped->object);

    // TODO: add forced exit for player
}

// =======================================================================//
// !
// ! Initializers, misc
// !
// =======================================================================//

void module_car_interaction_finish(librg_event_t *event) {
    auto ped = (ped_t *)mod.player->user_data;

    /**/ if (ped->stream.state == PED_ENTERING_CAR) {
        module_car_local_enter_finish(event);
    }
    else if (ped->stream.state == PED_EXITING_CAR) {
        module_car_local_exit_finish(event);
    }
}

void module_car_init() {
    librg_event_add(ctx, MOD_CAR_INTERACTION_FINISH,  module_car_interaction_finish);

    librg_event_add(ctx, MOD_CAR_ENTER_START,   module_car_local_enter_start);
    librg_event_add(ctx, MOD_CAR_EXIT_START,    module_car_local_exit_start);

    librg_network_add(ctx, MOD_CAR_ENTER_START,     module_car_remote_enter_start);
    librg_network_add(ctx, MOD_CAR_ENTER_FINISH,    module_car_remote_enter_finish);
    librg_network_add(ctx, MOD_CAR_EXIT_START,      module_car_remote_exit_start);
    librg_network_add(ctx, MOD_CAR_EXIT_FINISH,     module_car_remote_exit_finish);
}
