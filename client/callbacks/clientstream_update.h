/**
 * Entity remove from streamer
 */
void clientstream_update(librg::events::event_t* evt)
{
    auto event = (librg::events::event_bs_entity_t*) evt;
    auto game_entity    = event->entity.component<gamedata_t>();
    auto transform      = event->entity.component<librg::transform_t>();

    switch (event->type)
    {
        case TYPE_PLAYER:
        case TYPE_VEHICLE: {
            float x, y, z, w;
            hmm_v3 position;
            hmm_v4 rotation;

            Mem::InvokeFunction<Mem::call_this, void>(
                game_entity->object->m_pVFTable->GetPosition,
                game_entity->object,
                &position
            );

            Mem::InvokeFunction<Mem::call_this, void>(
                game_entity->object->m_pVFTable->GetRotation,
                reinterpret_cast<M2::C_Entity*>(game_entity->object),
                &rotation
            );

            transform->position = position;
            transform->rotation = rotation;

            corelog("rotation: %f %f %f %f",
                rotation.x,
                rotation.y,
                rotation.z,
                rotation.w
            );

            // transform->rotation = HMM_Vec4(
            //     rotation.x,
            //     rotation.y,
            //     rotation.z,
            //     rotation.w
            // );
        }
    }
}
