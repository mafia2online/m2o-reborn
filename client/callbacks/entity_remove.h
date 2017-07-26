/**
* Entity remove from streamer
*/
void entity_remove(librg::events::event_t* evt)
{
    librg::core::log("removing entity");
    auto event = (librg::events::event_remove_t*) evt;

    switch (event->type)
    {
    case TYPE_PLAYER:

        auto remote = event->entity.component<gamedata_t>();
        // PED::DELETE_PED(&remote->object);
        break;
    }
}
