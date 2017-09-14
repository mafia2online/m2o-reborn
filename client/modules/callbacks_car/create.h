void module_car_callback_create(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_CAR) return;
    create_game_entity(event->entity, M2::Entity_Car, 1);
}
