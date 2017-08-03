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

    //reinterpret_cast<M2::C_Entity *>(remote->object)->SetPosition(transform->position);
    // ENTITY::SET_ENTITY_COORDS_NO_OFFSET(remote->object, transform->position.x, transform->position.y, transform->position.z, true, true, true);
    // ENTITY::SET_ENTITY_QUATERNION(remote->object, transform->rotation.X, transform->rotation.y, transform->rotation.z, transform->rotation.w);
}

