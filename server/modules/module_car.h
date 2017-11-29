// TODO: remove (TESTING)
librg_entity_id lastcar;

void module_car_create(librg_message_t *msg) {
    auto playerid  = librg_entity_find(msg->ctx, msg->peer);
    auto vehicleid = librg_entity_create(msg->ctx, TYPE_CAR);

    auto player  = librg_entity_fetch(msg->ctx, playerid);
    auto vehicle = librg_entity_fetch(msg->ctx, vehicleid);

    vehicle->position = vec3(
        player->position.x + 3.0f,
        player->position.y,
        player->position.z
    );

    auto car = new car_t();

    car->stream.model = librg_data_ru16(msg->data);
    car->stream.brake = 0.0f;

    vehicle->user_data = car;

    lastcar = vehicle->id;

    // log
    print_posm(vehicle->position, "created a vehicle at: ");
}

void module_car_enter(librg_message_t *msg) {
    auto playerid  = librg_entity_find(msg->ctx, msg->peer);
    auto vehicleid = librg_data_ru32(msg->data);

    mod_assert_msg(librg_entity_valid(msg->ctx, vehicleid), "trying to enter invalid car");
    auto ped = librg_entity_fetch(msg->ctx, vehicleid);
    auto car = librg_entity_fetch(msg->ctx, vehicleid);

    mod_assert(ped && car);

    if (car->flags & LIBRG_ENTITY_CONTROLLED) {
        mod_log("the car has a driver already\n");
        return;
    }

    mod_log("ped: %lu becomes driver of: %lu\n", playerid, vehicleid);
    librg_entity_control_set(msg->ctx, vehicleid, msg->peer);

    // sorry
    ((ped_t *)ped->user_data)->stream.state = PED_IN_CAR;

    librg_data_t data;
    librg_data_init(&data);
    mod_log("sending enter message to EVERYYYYBODY\n");
    librg_data_wu32(&data, playerid);
    librg_data_wu32(&data, vehicleid);
    librg_message_send_instream(msg->ctx, MOD_CAR_ENTER, playerid, data.rawptr, librg_data_get_wpos(&data));
    librg_data_free(&data);
}

void module_car_exit(librg_message_t *msg) {
    librg_entity_control_remove(msg->ctx, lastcar);

}
