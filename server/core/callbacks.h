/**
 * Place to decide should the client be allowed to connect
 */
void on_connection_request(librg_event_t *event) {
    if (mod_settings.password.size() == 0) {
        return;
    }

    // read password
    u32 size = librg_data_ru32(event->data);
    std::string password = "";
    for (usize i = 0; i < size; ++i) {
        password += librg_data_ru8(event->data);
    }

    // if not matches - reject
    if (password != mod_settings.password) {
        librg_event_reject(event);
    }
}

/**
 * On client connected
 */
void on_connect_accepted(librg_event_t *event) {
    auto transform = librg_fetch_transform(event->entity);
    auto client    = librg_fetch_client(event->entity);

    mod_log("spawning player %u at: %f %f %f\n",
        event->entity,
        transform->position.x,
        transform->position.y,
        transform->position.z
    );

    librg_attach_ped(event->entity, {0});
    librg_streamer_client_set(event->entity, client->peer);
}

/**
 * Remote entity is added to particular client
 */
void entity_create(librg_event_t *event)
{
    switch (librg_entity_type(event->entity)) {
        case TYPE_PLAYER:  return ped_oncreate(event);
        // case TYPE_VEHICLE: return vehicle_oncreate(event);
    }
}

/**
 * Remote entity is updated for particular client
 */
void entity_update(librg_event_t *event)
{
    switch (librg_entity_type(event->entity)) {
        case TYPE_PLAYER:  return ped_onupdate(event);
        // case TYPE_VEHICLE: return vehicle_onupdate(event);
    }
}

/**
 * Remote entity is removed from particular client
 */
void entity_remove(librg_event_t *event)
{
    // switch (librg_entity_type(event->entity)) {
        // case TYPE_PLAYER:  return ped_onremove(event);
        // case TYPE_VEHICLE: return vehicle_onremove(event);
    // }
}

/**
 * Local entity gets send from our client
 */
void clientstream_update(librg_event_t *event)
{
    switch (librg_entity_type(event->entity)) {
        case TYPE_PLAYER:  return ped_onclient(event);
        // case TYPE_VEHICLE: return vehicle_onclient(event);
    }
}
