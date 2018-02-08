// =======================================================================//
// !
// ! Generic entity events
// !
// =======================================================================//

#define MOD_CAR_SPEED_TRESHOLD 0.6f
#define MOD_CAR_POSITION_TRESHOLD 0.02f
#define MOD_CAR_ROTATION_TRESHOLD 0.15f

/**
 * The entity enters the stream zone
 */
void module_car_callback_create(librg_event_t *event) {
    mod_log("[info] trying to spawn a car\n");
    M2::C_Entity *entity = M2::Wrappers::CreateEntity(M2::eEntityType::MOD_ENTITY_CAR, 42);

    if (entity->IsActive()) {
        print_posm(event->entity->position, "[info] creating car at");

        event->entity->flags |= MOD_ENTITY_INTERPOLATED;
        event->entity->user_data = new car_t(event->entity);

        get_car(event->entity)->setCEntity(entity);
        get_car(event->entity)->CCar->SetPos(event->entity->position);
        //get_car(event->entity)->CCar->SetRot(event->entity->position);
        get_car(event->entity)->CCar->m_pVehicle.SetEngineOn(true, false);
        get_car(event->entity)->dbg();
    }
}

/**
 * The entity exists the stream zone
 */
void module_car_callback_remove(librg_event_t *event) {
    auto car = get_car(event->entity); mod_assert(car && car->CEntity);
    M2::Wrappers::DestroyEntity(car->CEntity, M2::eEntityType::MOD_ENTITY_CAR);
    delete car;
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
    auto car = get_car(event->entity);

    // make sure we have all objects
    mod_assert(car && car->CEntity);
    librg_data_rptr(event->data, &car->stream, sizeof(car->stream));

    /* update interpolation tables */
    cubic_hermite_v3_value(&car->inter_pos, event->entity->position);

    // car->CCar->SetRot(car->stream.rotation);
    car->CCar->m_pVehicle.SetSteer(car->stream.steer);

    car->inter_steer.last = car->inter_steer.targ;
    car->inter_steer.targ = car->stream.steer;

    car->inter_rot.last = car->inter_rot.targ;
    car->inter_rot.targ = car->stream.rotation;

    quat_t empty_quat = { 0 };
    if (car->inter_rot.last == empty_quat) {
        car->inter_rot.last = car->stream.rotation;
    }

    car->inter_delta = 0.0f;
}

/**
 * The entity is streamed by us, and we are sending an update with new data
 */
void module_car_callback_clientstream(librg_event_t *event) {
    auto car = get_car(event->entity);
    mod_assert(car && car->CEntity);

    event->entity->position = car->CEntity->GetPosition();
    car->stream.rotation    = car->CEntity->GetRotation();
    car->stream.steer       = car->CCar->m_pVehicle.m_fSteer;
    car->stream.speed       = car->CCar->m_pVehicle.m_vSpeed;

    librg_data_wptr(event->data, &car->stream, sizeof(car->stream));
}

// =======================================================================//
// !
// ! Interpolation
// !
// =======================================================================//


void module_car_callback_interpolate(librg_entity_t *entity) {
    auto car = get_car(entity); mod_assert(car && car->CEntity);

    // last delta tick against constant tick delay
    f32 alpha = car->inter_delta / (f32)MOD_SERVER_TICK_DELAY;
    car->inter_delta += mod.last_delta;

    f32 steer_alpha = zpl_clamp01(car->inter_delta / /*MOD_SERVER_TICK_DELAY*/ 32.f);
    f32 dest_steer = zplm_lerp(car->inter_steer.last, car->inter_steer.targ, steer_alpha);

    car->CCar->m_pVehicle.SetSteer(dest_steer);
    car->CCar->SetPos(cubic_hermite_v3_interpolate(&car->inter_pos, alpha));

    if (zplm_vec3_mag2(car->stream.speed) > MOD_CAR_SPEED_TRESHOLD) {
        car->CCar->m_pVehicle.SetSpeed(car->stream.speed);
    } else {
        car->CCar->m_pVehicle.SetSpeed(zplm_vec3_zero());
    }

    /* rotation interpolation */
    if (car->inter_rot.last != car->inter_rot.targ) {
        auto curr_rot = car->CEntity->GetRotation();
        auto diff_rot = zplm_quat_angle(curr_rot - car->stream.rotation);

        if (diff_rot > MOD_CAR_ROTATION_TRESHOLD) {
            auto last = car->inter_rot.last;
            auto dest = car->inter_rot.targ;

            quat_t dest_rotation;
            zplm_quat_nlerp(&dest_rotation, zplm_quat_dot(last, dest) < 0 ? -last : last, dest, alpha);
            car->CCar->SetRot(dest_rotation);
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
    auto ped     = get_ped(mod.player);

    mod_assert(vehicle && ped);
    u8 seat = *(u8*)event->data;

    // send vehicle create request onto server
    mod_entity_iterate(ctx, LIBRG_ENTITY_ALIVE, [&](librg_entity_t *entity) {
        if (entity->type != TYPE_CAR) return;
        auto car = get_car(entity);

        if (car->CEntity == vehicle) {
            // set the driver data
            ped->stream.state = PED_ENTERING_CAR;
            ped->seat         = seat;
            ped->vehicle      = entity;

            mod_message_send(ctx, MOD_CAR_ENTER_START, [&](librg_data_t *data) {
                librg_data_wu32(data, entity->id);
                librg_data_wu8(data, seat);
            });
        }
    });
}

/**
 * Local ped finished entering the car
 * @param event
 */
void module_car_local_enter_finish(librg_event_t *event) {
    auto ped = get_ped(mod.player);

    mod.player->flags |= MOD_ENTITY_DRIVER;
    ped->stream.state = PED_IN_CAR;

    mod_message_send(ctx, MOD_CAR_ENTER_FINISH, nullptr);
}

/**
 * Event when local player starts leaving local car
 * @param event
 */
void module_car_local_exit_start(librg_event_t *event) {
    auto ped = get_ped(mod.player);

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
    auto ped = get_ped(mod.player);

    ped->seat = 0;
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

    auto seat = librg_data_ru8(msg->data);
    auto ped = get_ped(player); mod_assert_msg(ped->CEntity, "trying to put ped in invalid car");
    auto car = get_car(vehicle); mod_assert_msg(car->CEntity, "trying to put invalid ped in car");

    mod_log("[info] putting ped: %u in the car: %u\n", player->id, vehicle->id);

    ped->vehicle = vehicle;
    ped->seat = seat;

    // TODO: add seat sync
    M2::C_SyncObject *pSyncObject = nullptr;
    ((M2::C_Human2 *)ped->CEntity)->GetScript()->ScrDoAction(
        &pSyncObject, (M2::C_Vehicle *)car->CEntity,
        true, (M2::E_VehicleSeat)seat, 1
    );
}

/**
 * Remote ped finished entering the car
 * @param msg
 */
void module_car_remote_enter_finish(librg_message_t *msg) {
    auto player = librg_entity_fetch(ctx, librg_data_rent(msg->data)); mod_assert(player);
    //auto ped = get_ped(player); mod_assert(ped && ped->object && ped->vehicle);
    //auto car = get_car(ped->vehicle); mod_assert(car && car->object);

    // TODO: add PutPlayerInVehicle focred if not in the car yet
}

/**
 * Remote ped started exiting the car
 * @param msg
 */
void module_car_remote_exit_start(librg_message_t *msg) {
    auto player = librg_entity_fetch(ctx, librg_data_rent(msg->data)); mod_assert(player);
    auto ped = get_ped(player); mod_assert(ped && ped->CEntity && ped->vehicle);
    auto car = get_car(ped->vehicle); mod_assert(car && car->CEntity);

    mod_log("[info] removing ped: %u from the car: %u\n", player->id, ped->vehicle->id);

    // TODO: add seat sync
    M2::C_SyncObject *pSyncObject = nullptr;
    ((M2::C_Human2 *)ped->CEntity)->GetScript()->ScrDoAction(
        &pSyncObject, (M2::C_Vehicle *)car->CEntity,
        true, (M2::E_VehicleSeat)ped->seat, 1
    );
}

/**
 * Remote ped finished exiting the car
 * @param msg
 */
void module_car_remote_exit_finish(librg_message_t *msg) {
    auto player = librg_entity_fetch(ctx, librg_data_rent(msg->data)); mod_assert(player);
    auto ped = get_ped(player); mod_assert(ped && ped->CEntity);

    ped->vehicle = nullptr;

    // TODO: add forced exit for player
}

// =======================================================================//
// !
// ! Initializers, misc
// !
// =======================================================================//

void module_car_interaction_finish(librg_event_t *event) {
    auto ped = get_ped(mod.player);

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
