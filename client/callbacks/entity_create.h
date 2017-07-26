/**
* Entity add to streamer
*/
void entity_create(librg::events::event_t* evt)
{
    librg::core::log("creating entity");
    auto event      = (librg::events::event_create_t*) evt;
    auto transform  = event->entity.component<librg::transform_t>();
    auto inter      = event->entity.assign<librg::interpolable_t>(*transform);

    switch (event->type)
    {
        case TYPE_PLAYER: {
            librg::core::log("creating ped");
        } break;
        case TYPE_VEHICLE: {
            librg::core::log("creating vehicle");
        }break;
    }

    librg::core::log("entity succeessfully created");
}

