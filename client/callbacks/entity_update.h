const float DISTANCE_BEFORE_TELEPORT = 50 * 50;

/**
* Entity update in streamer
*/
void entity_update(librg::events::event_t* evt)
{
    // auto event = (librg::events::event_bs_entity_t*) evt;

    // auto transform  = event->entity.component<librg::transform_t>();
    // auto inter      = event->entity.component<librg::interpolable_t>();
    // auto remote     = event->entity.component<gamedata_t>();

    // auto distance   = HMM_LengthSquaredVec3(HMM_SubtractVec3(inter->latest.position, transform->position));

    // // if distance further, override interpolation
    // if (distance > DISTANCE_BEFORE_TELEPORT) {
    //     inter->latest = *transform;

    //     ENTITY::SET_ENTITY_COORDS_NO_OFFSET(remote->object, transform->position.x, transform->position.y, transform->position.z, true, true, true);
    //     ENTITY::SET_ENTITY_QUATERNION(remote->object, transform->rotation.x, transform->rotation.y, transform->rotation.z, transform->rotation.w);
    // }
}
