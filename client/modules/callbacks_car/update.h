void module_car_callback_clientstream(librg_event_t *event) {
    if (librg_entity_type(ctx, event->entity->id) != TYPE_CAR) return;

    auto ped = (ped_t *)event->entity->user_data;

    librg_assert(ped && ped->object);

    // TODO: add rotations
    event->entity->position = ped->object->GetPosition();
    //event->entity->rotation = ped->object->GetRotation();
}


void module_car_callback_update(librg_event_t *event) {
    if (librg_entity_type(ctx, event->entity->id) != TYPE_CAR) return;

    auto ped = (ped_t *)event->entity->user_data;
    auto interpolate = &ped->interpolate;

    interpolate->lposition = interpolate->tposition;
    interpolate->lrotation = interpolate->trotation;

    interpolate->tposition = event->entity->position;
    //interpolate->trotation = transform->rotation;

    interpolate->delta = 0.0f;
}
