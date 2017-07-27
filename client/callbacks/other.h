void spawn_player(librg::entity_t entity)
{
    entity.assign<librg::streamable_t>();
    // entity.assign<gamedata_t>(PLAYER::PLAYER_PED_ID());
    entity.assign<gamedata_t>(1);
    entity.assign<ped_t>();

    librg::core::log("spawned player!");
}

void unspawn_player(librg::entity_t entity)
{
    entity.remove<ped_t>();
    entity.remove<gamedata_t>();
    entity.remove<librg::streamable_t>();

    librg::core::log("unspawned player!");
}

void client_connect(librg::events::event_t* evt)
{
    auto event = (librg::events::event_entity_t*)evt;
    auto entity = event->entity;

    local_player = entity;

    librg::core::log("connected to the server");
    spawn_player(entity);
}

void client_disconnect(librg::events::event_t* evt)
{
    auto event = (librg::events::event_entity_t*)evt;
    auto entity = event->entity;

    librg::core::log("disconnected form the server");
    unspawn_player(entity);
}
