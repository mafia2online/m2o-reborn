void module_car_callback_remove(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_CAR) return;

    auto gamedata  = librg_fetch_gamedata(event->entity);
    gamedata->object->Deactivate();
    mod_log("remvoing vehicle\n");
}
