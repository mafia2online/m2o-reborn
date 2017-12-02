void module_car_callback_clientstream(librg_event_t *event) {
    if (librg_entity_type(ctx, event->entity->id) != TYPE_CAR) return;

    auto car = (car_t *)event->entity->user_data;

    librg_assert(car && car->object);

    // TODO: add rotations
    event->entity->position = car->object->GetPosition();
    car->stream.rotation = car->object->GetRotation();

    librg_data_wptr(event->data, &car->stream, sizeof(car->stream));
}


void module_car_callback_update(librg_event_t *event) {
    if (librg_entity_type(ctx, event->entity->id) != TYPE_CAR) return;

    auto car = (car_t *)event->entity->user_data;
    auto interpolate = &car->interpolate;

    interpolate->lposition = interpolate->tposition;
    interpolate->lrotation = interpolate->trotation;

    interpolate->tposition = event->entity->position;
    interpolate->trotation = car->stream.rotation;

    interpolate->delta = 0.0f;
}
