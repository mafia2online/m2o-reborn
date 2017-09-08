/**
 * When client connects :D
 */
void client_connect(librg_event_t *event)
{
    mod_log("connected to the server\n");
    mod.player = event->entity;
    librg_attach_gamedata(event->entity, { (M2::C_Entity*)M2::C_Game::Get()->GetLocalPed() });
}

/**
 * On client disconnects
 */
void client_disconnect(librg_event_t *event)
{
    mod_log("disconnected form the server\n");
    librg_detach_gamedata(event->entity);
}

/**
 * Local entity gets send from our client
 */
void clientstream_update(librg_event_t *event)
{
    switch (librg_entity_type(event->entity)) {
        case TYPE_PLAYER:  return ped_onclient(event);
        case TYPE_VEHICLE: return vehicle_onclient(event);
    }
}

/**
 * Remote entity is added to our local streamer
 */
void entity_create(librg_event_t *event)
{
    mod_log("creating entity\n");

    switch (librg_entity_type(event->entity)) {
        case TYPE_PLAYER:  return ped_oncreate(event);
        case TYPE_VEHICLE: return vehicle_oncreate(event);
    }
}

/**
 * Remote entity is updated in our local streamer
 */
void entity_update(librg_event_t *event)
{
    switch (librg_entity_type(event->entity)) {
        case TYPE_PLAYER:  return ped_onupdate(event);
        case TYPE_VEHICLE: return vehicle_onupdate(event);
    }
}

/**
 * Remote entity is removed from our local streamer
 */
void entity_remove(librg_event_t *event)
{

}
