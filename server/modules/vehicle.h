void module_car_create(librg_message_t *msg) {
    auto player  = librg_entity_find(msg->ctx, msg->peer);
    auto vehicle = librg_entity_create(msg->ctx, TYPE_CAR);
    mod_assert(player && vehicle);

    vehicle->user_data = new car_t();
    vehicle->position = vec3(
        player->position.x + 3.0f,
        player->position.y,
        player->position.z
    );

    // log
    print_posm(vehicle->position, "created a vehicle at: ");
}

void module_car_enter(librg_message_t *msg) {
    auto player  = librg_entity_find(msg->ctx, msg->peer);
    auto vehicle = librg_entity_fetch(msg->ctx, librg_data_ru32(msg->data));
    mod_assert_msg(vehicle && player, "trying to enter invalid vehicle");

    if (vehicle->flags & LIBRG_ENTITY_CONTROLLED) {
        mod_log("the vehicle has a driver already\n");
        return;
    }

    mod_log("ped: %lu becomes driver of: %lu\n", player->id, vehicle->id);
    librg_entity_control_set(msg->ctx, vehicle->id, player->client_peer);

    // sorry
    ((ped_t *)player->user_data)->stream.state = PED_IN_CAR;

    mod_log("sending enter message to EVERYYYYBODY\n");
    mod_message_send_instream(msg->ctx, MOD_CAR_ENTER, player->id, [&](librg_data_t *data) {
        librg_data_wu32(data, player->id);
        librg_data_wu32(data, vehicle->id);
    });
}

void module_car_exit(librg_message_t *msg) {
    // librg_entity_control_remove(msg->ctx, lastcar);
}
