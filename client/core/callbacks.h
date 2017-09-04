/**
 * When client connects :D
 */
void client_connect(librg_event_t *event)
{
    auto entity = event->entity;

    mod.player = entity;
    mod_log("connected to the server\n");

    librg_attach_gamedata(event->entity, { (M2::C_Entity*)M2::C_Game::Get()->GetLocalPed() });

    mod_log("spawned player!\n");
}

/**
 * On client disconnects
 */
void client_disconnect(librg_event_t *event)
{
    auto entity = event->entity;

    mod_log("disconnected form the server\n");

    librg_detach_gamedata(event->entity);

    mod_log("unspawned player!\n");
}

/**
 * Local entity gets send from our client
 */
void clientstream_update(librg_event_t *event)
{
    // auto remote     = event->entity.component<gamedata_t>();
    auto transform = librg_fetch_transform(event->entity);
    auto gamedata  = librg_fetch_gamedata(event->entity);

    zplm_vec3_t position;
    zplm_quat_t rotation;
    zplm_vec3_t direction;

    Mem::InvokeFunction<Mem::call_this, void>(gamedata->object->m_pVFTable->GetPosition, gamedata->object, &position);
    Mem::InvokeFunction<Mem::call_this, void>(gamedata->object->m_pVFTable->GetRotation, gamedata->object, &rotation);
    Mem::InvokeFunction<Mem::call_this, void>(gamedata->object->m_pVFTable->GetDirection, gamedata->object, &direction);

    transform->position = position;
    transform->rotation = rotation;
    // transform->scale = direction;
}

/**
 * Remote entity is added to our local streamer
 */
void entity_create(librg_event_t *event)
{
    auto transform  = librg_fetch_transform(event->entity);
    auto entitytype = librg_entity_type(event->entity);

    // make them inteprpolable
    if (entitytype == TYPE_PLAYER || entitytype == TYPE_VEHICLE) {
        // event->entity.assign<librg::interpolable_t>(*transform);
    }

    mod_log("creating entity\n");

    switch (entitytype)
    {
        case TYPE_PLAYER:  return ped_oncreate(event);
        case TYPE_VEHICLE: return vehicle_oncreate(event);
    }
}

/**
 * Remote entity is updated in our local streamer
 */
void entity_update(librg_event_t *event)
{
    auto transform = librg_fetch_transform(event->entity);
    auto gamedata  = librg_fetch_gamedata(event->entity);

    gamedata->object->SetPosition(transform->position);
}

/**
 * Remote entity is removed from our local streamer
 */
void entity_remove(librg_event_t *event)
{

}
