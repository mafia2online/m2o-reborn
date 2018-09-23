void on_ped_create_command(librg_message_t *msg) {
    auto player = librg_entity_find(msg->ctx, msg->peer);
    mod_assert(player);

    auto ped = m2o_ped_create();
    m2o_ped_position_set(ped, vec3f(
        player->position.x + 1.0f,
        player->position.y,
        player->position.z + 0.05f
    ));

    // log
    print_posm(m2o_ped_position_get(ped), "created a ped at: ");
}

void on_ped_create(librg_event_t *event) {
    auto ped = m2o_ped_get(event->entity);

    librg_data_wu16(event->data, ped->model);
    librg_data_wu8(event->data, ped->state);
    librg_data_wu8(event->data, zpl_strlen(ped->name));
    librg_data_wptr(event->data, ped->name, zpl_strlen(ped->name));

    // we are in the car
    if (ped->state == PED_IN_CAR) {
        librg_data_wu32(event->data, ped->vehicle);
        librg_data_wu8(event->data, ped->seat);
    }
}

void on_ped_remove(librg_event_t *event) {

}
