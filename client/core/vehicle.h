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

    static M2::C_Entity *carEntity = M2::Wrappers::CreateEntity(M2::eEntityType::MOD_ENTITY_CAR, 0);
    if (carEntity == nullptr) {
        return;
    }
    carEntity->SetPosition(entity->position);
    if(carEntity->IsActive()){
        car->object = carEntity;

        event->entity->flags |= MOD_ENTITY_INTERPOLATED;
        event->entity->user_data = car;

        reinterpret_cast<M2::C_Car*>(carEntity)->m_pVehicle.SetEngineOn(true, false);
    }
}

/**
 * The entity exists the stream zone
 */
void module_car_callback_remove(librg_event_t *event) {
    auto car = (car_t *)event->entity->user_data;
    M2::Wrappers::DestroyEntity(car->object);
    delete event->entity->user_data;
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
        //((M2::C_Car *)car->object)->m_pVehicle.SetSpeedFloat(car->stream.speed);
        //((M2::C_Car *)car->object)->SetSpeedFloat(100.0f);
    }
}

f32 mod_fract_round(f32 x, i32 exp) {
    f32 fract = zplm_pow(10.0f, exp);
    return zplm_round(x * fract) / fract;
}

vec3_t mod_fract_round_v3(vec3_t vec, i32 exp) {
    return vec3(
        mod_fract_round(vec.x, exp),
        mod_fract_round(vec.y, exp),
        mod_fract_round(vec.z, exp)
    );
}

quat_t mod_fract_round_quat(quat_t vec, i32 exp) {
    return zplm_quat(
        mod_fract_round(vec.x, exp),
        mod_fract_round(vec.y, exp),
        mod_fract_round(vec.z, exp),
        mod_fract_round(vec.w, exp)
    );
}

/**
 * The entity is streamed by us, and we are sending an update with new data
 */
void module_car_callback_clientstream(librg_event_t *event) {
    auto car = (car_t *)event->entity->user_data;

    librg_assert(car && car->object);

    //mod_log("%x\n", car->object);
    //vec3_t vec = ((M2::C_Car *)car->object)->m_test;
    //print_posm(vec, "applying speed: \n");
    // TODO: add rotations
    event->entity->position = car->object->GetPosition();
    //event->entity->position = mod_fract_round_v3(car->object->GetPosition(), 2);
    car->stream.rotation = car->object->GetRotation();
    //car->stream.rotation    = mod_fract_round_quat(car->object->GetRotation(), 5);
    //car->stream.speed       = ((M2::C_Car *)car->object)->m_fSpeedDir;
    car->stream.steer       = ((M2::C_Car *)car->object)->m_pVehicle.m_fSteer;

    librg_data_wptr(event->data, &car->stream, sizeof(car->stream));
}

// =======================================================================//
// !
// ! Interpolation
// !
// =======================================================================//

#define MOD_CAR_ROTATION_TRESHOLD 0.15f

void module_car_callback_interpolate(librg_entity_t *entity) {
    auto car = (car_t *)entity->user_data;
    librg_assert(car && car->object);

    // skip entity if we are the driver
    if (mod.player->flags & MOD_ENTITY_DRIVER && ((ped_t *)mod.player->user_data)->vehicle == entity) {
        if (((ped_t *)mod.player->user_data)->vehicle->client_peer != mod.player->client_peer)
            return;
    }

    // last delta tick against constant tick delay
    car->interpolate.delta += (mod.last_delta / 40.666f);
    car->interpolate.delta = zplm_clamp(car->interpolate.delta, 0.f, 1.0f);

    /* steering */
    ((M2::C_Car *)car->object)->m_pVehicle.SetSteer(car->stream.steer/* * (180.0f / ZPLM_PI)*/);

    /* position interpolation */
    if (car->interpolate.lposition != car->interpolate.tposition && car->interpolate.step++ > 16) {
        auto curr_pos = car->object->GetPosition();
        auto diff_pos = zplm_vec3_mag2(curr_pos - entity->position);

        vec3_t dposition;
        zplm_vec3_lerp(&dposition, car->interpolate.lposition, car->interpolate.tposition, car->interpolate.delta);
        reinterpret_cast<M2::C_Car*>(car->object)->SetPos(dposition);
    }

    /* rotation interpolation */
    if (car->interpolate.lrotation != car->interpolate.trotation) {
        auto curr_rot = car->object->GetRotation();
        auto diff_rot = zplm_quat_angle(curr_rot - car->stream.rotation);

        if (diff_rot > MOD_CAR_ROTATION_TRESHOLD) {
            auto last = car->interpolate.lrotation;
            auto dest = car->interpolate.trotation;

            quat_t drotation;
            zplm_quat_nlerp(&drotation, zplm_quat_dot(last, dest) < 0 ? -last : last, dest, car->interpolate.delta);
            reinterpret_cast<M2::C_Car*>(car->object)->SetRot(drotation);
        }
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

    ped->vehicle = vehicle;

    // TODO: add seat sync
    M2::C_SyncObject *pSyncObject = nullptr;
    ((M2::C_Human2 *)ped->object)->GetScript()->ScrDoAction(
        &pSyncObject, (M2::C_Vehicle *)car->object,
        true, M2::E_VehicleSeat::E_SEAT_DRIVER, 1
    );
}

/**
 * Remote ped finished entering the car
 * @param msg
 */
void module_car_remote_enter_finish(librg_message_t *msg) {
    auto player = librg_entity_fetch(ctx, librg_data_rent(msg->data)); mod_assert(player);
    //auto ped = (ped_t *)player->user_data; mod_assert(ped && ped->object && ped->vehicle);
    //auto car = (car_t *)ped->vehicle->user_data; mod_assert(car && car->object);

    // TODO: add PutPlayerInVehicle focred if not in the car yet
}

/**
 * Remote ped started exiting the car
 * @param msg
 */
void module_car_remote_exit_start(librg_message_t *msg) {
    auto player = librg_entity_fetch(ctx, librg_data_rent(msg->data)); mod_assert(player);
    auto ped = (ped_t *)player->user_data; mod_assert(ped && ped->object && ped->vehicle);
    auto car = (car_t *)ped->vehicle->user_data; mod_assert(car && car->object);

    mod_log("[info] removing ped: %u from the car: %u\n", player->id, ped->vehicle->id);

    // TODO: add seat sync
    M2::C_SyncObject *pSyncObject = nullptr;
    ((M2::C_Human2 *)ped->object)->GetScript()->ScrDoAction(
        &pSyncObject, (M2::C_Vehicle *)car->object,
        true, M2::E_VehicleSeat::E_SEAT_DRIVER, 1
    );
}

/**
 * Remote ped finished exiting the car
 * @param msg
 */
void module_car_remote_exit_finish(librg_message_t *msg) {
    auto player = librg_entity_fetch(ctx, librg_data_rent(msg->data)); mod_assert(player);
    auto ped = (ped_t *)player->user_data; mod_assert(ped && ped->object);

    ped->vehicle = nullptr;

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
