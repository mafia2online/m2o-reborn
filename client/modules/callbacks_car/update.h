void module_car_callback_clientstream(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_CAR) return;

    auto transform = librg_fetch_transform(event->entity);
    auto gamedata  = librg_fetch_gamedata(event->entity);

    librg_assert(gamedata && gamedata->object);

    transform->position = gamedata->object->GetPosition();
    transform->rotation = gamedata->object->GetRotation();
}


void module_car_callback_update(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_CAR) return;

    auto transform   = librg_fetch_transform(event->entity);
    auto interpolate = librg_fetch_interpolate(event->entity);

    // if this car is not interpolable
    if (!interpolate) return;

    interpolate->lposition = interpolate->tposition;
    interpolate->lrotation = interpolate->trotation;

    interpolate->tposition = transform->position;
    interpolate->trotation = transform->rotation;

    interpolate->delta = 0.0f;
}
