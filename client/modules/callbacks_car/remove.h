void module_car_callback_remove(librg_event_t *event) {
    if (event->entity->id != TYPE_CAR) return;

    // TODO: add vehicle removing
    //auto gamedata  = librg_fetch_gamedata(event->entity);
    //gamedata->object->Deactivate();
    mod_log("remvoing vehicle\n");
}
