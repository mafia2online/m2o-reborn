/**
 * Entity interpolate callback
 */
void entity_inter(librg::events::event_t* evt)
{
    auto event = (librg::events::event_entity_t*) evt;

    auto transform  = event->entity.component<librg::transform_t>();
    auto inter      = event->entity.component<librg::interpolable_t>();
    auto remote     = event->entity.component<gamedata_t>();

    *transform = *(librg::transform_t*)event->data;

    remote->object->SetPosition(transform->position);
    // remote->object->SetRotation(HMM_QuaternionV4(transform->rotation));
}

