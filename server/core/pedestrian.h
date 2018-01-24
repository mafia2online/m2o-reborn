void module_ped_create(librg_message_t *msg) {
    auto player = librg_entity_find(msg->ctx, msg->peer);
    auto ped = librg_entity_create(msg->ctx, TYPE_PED);
    mod_assert(player && ped);

    ped->user_data = new ped_t();
    ped->position = vec3(
        player->position.x + 1.0f,
        player->position.y,
        player->position.z + 0.05f
    );

    // log
    print_posm(ped->position, "created a ped at: ");
}

void module_ped_callback_create(librg_event_t *event) {
    auto ped = get_ped(event->entity);

    librg_data_wu16(event->data, ped->model);
    librg_data_wu8(event->data, ped->state);

    // we are in the car
    if (ped->state == PED_IN_CAR) {
        librg_data_wu32(event->data, ped->vehicle->id);
        librg_data_wu8(event->data, ped->seat);
    }
}

void module_ped_callback_remove(librg_event_t *event) {

}
