// =======================================================================//
// !
// ! General stuff
// !
// =======================================================================//

void module_car_create(librg_message_t *msg) {
    auto player  = librg_entity_find(msg->ctx, msg->peer);
    auto vehicle = librg_entity_create(msg->ctx, TYPE_CAR);
    mod_assert(player && vehicle);

    vehicle->user_data = new car_t();
    vehicle->position = vec3(
        player->position.x + 3.0f,
        player->position.y,
        player->position.z + 1.0f
    );

    // log
    print_posm(vehicle->position, "created a vehicle at: ");
}

// =======================================================================//
// !
// ! Vehicle events
// !
// =======================================================================//

void module_car_enter_start(librg_message_t *msg) {
    auto player  = librg_entity_find(msg->ctx, msg->peer);
    auto vehicle = librg_entity_fetch(msg->ctx, librg_data_ru32(msg->data));
    mod_assert_msg(vehicle && player, "trying to enter invalid vehicle");

    auto seat = librg_data_ru8(msg->data);
    auto ped = (ped_t *)player->user_data;

    ped->vehicle = vehicle;
    ped->seat = seat;

    if (seat == 1 || !(vehicle->flags & LIBRG_ENTITY_CONTROLLED)) {
        librg_entity_control_set(msg->ctx, vehicle->id, player->client_peer);
    }

    mod_log("[info] ped: %lu becomes member of: %lu on seat: %u\n", player->id, vehicle->id, seat);
    mod_message_send_instream_except(msg->ctx, MOD_CAR_ENTER_START, player->id, player->client_peer, [&](librg_data_t *data) {
        librg_data_wu32(data, player->id);
        librg_data_wu32(data, vehicle->id);
        librg_data_wu8(data, seat);
    });
}

void module_car_enter_finish(librg_message_t *msg) {
    auto player = librg_entity_find(msg->ctx, msg->peer);
    player->flags |= MOD_ENTITY_DRIVER;

    mod_message_send_instream_except(msg->ctx, MOD_CAR_ENTER_FINISH, player->id, player->client_peer, [&](librg_data_t *data) {
        librg_data_wu32(data, player->id);
    });
}

void module_car_exit_start(librg_message_t *msg) {
    auto player = librg_entity_find(msg->ctx, msg->peer);
    mod_log("player: %d is trying to leave his current car\n", player->id);

    player->flags &= ~MOD_ENTITY_DRIVER;

    // TODO: does he still need to control and stream that car? most probably yea
    // librg_entity_control_remove(msg->ctx, player->vehicle->id);

    mod_message_send_instream_except(msg->ctx, MOD_CAR_EXIT_START, player->id, player->client_peer, [&](librg_data_t *data) {
        librg_data_wu32(data, player->id);
    });
}

void module_car_exit_finish(librg_message_t *msg) {
    auto player  = librg_entity_find(msg->ctx, msg->peer);
    auto ped = (ped_t *)player->user_data;
    ped->vehicle = nullptr;
    ped->seat = 0;

    mod_message_send_instream_except(msg->ctx, MOD_CAR_EXIT_FINISH, player->id, player->client_peer, [&](librg_data_t *data) {
        librg_data_wu32(data, player->id);
    });
}
