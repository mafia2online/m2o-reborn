void on_ped_create_command(librg_message_t *msg) {
    auto player = librg_entity_find(msg->ctx, msg->peer);
    mod_assert(player);

    auto ped = m2o_ped_create();
    m2o_ped_position_set(ped, vec3(
        player->position.x + 1.0f,
        player->position.y,
        player->position.z + 0.05f
    ));

    // log
    print_posm(m2o_ped_position_get(ped), "created a ped at: ");
}

void on_ped_create(librg_event_t *event) {
    auto ped = get_ped(event->entity);

    librg_data_wu16(event->data, ped->model);
    librg_data_wu8(event->data, ped->state);

    // we are in the car
    if (ped->state == PED_IN_CAR) {
        librg_data_wu32(event->data, ped->vehicle->id);
        librg_data_wu8(event->data, ped->seat);
    }
}

void on_ped_remove(librg_event_t *event) {

}
