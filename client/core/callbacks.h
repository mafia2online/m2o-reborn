const float DISTANCE_BEFORE_TELEPORT = 50 * 50;

/**
 * On something getting logged
 * into the debug console
 */
void on_mod_log(librg::events::event_t* evt)
{
    auto event  = (librg::events::event_log_t*)evt;

    mod.debug_stream << event->output;
    printf("%s", event->output.c_str());
}

/**
 * When client connects :D
 */
void client_connect(librg::events::event_t* evt)
{
    auto event  = (librg::events::event_entity_t*)evt;
    auto entity = event->entity;

    mod.player = entity;
    mod_log("connected to the server");

    entity.assign<librg::streamable_t>();
    entity.assign<gamedata_t>(reinterpret_cast<M2::C_Entity*>(M2::C_Game::Get()->GetLocalPed()));

    mod_log("spawned player!");
}

/**
 * On client disconnects
 */
void client_disconnect(librg::events::event_t* evt)
{
    auto event  = (librg::events::event_entity_t*)evt;
    auto entity = event->entity;

    mod_log("disconnected form the server");

    entity.remove<gamedata_t>();
    entity.remove<librg::streamable_t>();

    mod_log("unspawned player!");
}

/**
 * Local entity gets send from our client
 */
void clientstream_update(librg::events::event_t* evt)
{
    auto event      = (librg::events::event_bs_entity_t*) evt;
    auto remote     = event->entity.component<gamedata_t>();
    auto transform  = event->entity.component<librg::transform_t>();

    hmm_v3 position;
    hmm_v4 rotation;

    Mem::InvokeFunction<Mem::call_this, void>(remote->object->m_pVFTable->GetPosition, remote->object, &position);
    Mem::InvokeFunction<Mem::call_this, void>(remote->object->m_pVFTable->GetRotation, remote->object, &rotation);

    transform->position = position;
    transform->rotation = rotation;
}

/**
 * Remote entity is added to our local streamer
 */
void entity_create(librg::events::event_t* evt)
{
    auto event      = (librg::events::event_entity_t*) evt;
    auto transform  = event->entity.component<librg::transform_t>();

    // make them inteprpolable
    if (event->type == TYPE_PLAYER || event->type == TYPE_VEHICLE) {
        event->entity.assign<librg::interpolable_t>(*transform);
    }

    mod_log("creating entity");

    switch (event->type)
    {
        case TYPE_PLAYER:  return ped_oncreate(event);
        case TYPE_VEHICLE: return vehicle_oncreate(event);
    }
}

/**
 * Remote entity is updated in our local streamer
 */
void entity_update(librg::events::event_t* evt)
{
    auto event = (librg::events::event_entity_t*) evt;

    auto transform  = event->entity.component<librg::transform_t>();
    auto remote     = event->entity.component<gamedata_t>();

    if (transform && remote && remote->object) {
        remote->object->SetRotation(HMM_Quaternion(0, 0, 0, 1));
    }
}

/**
 * Remote entity is removed from our local streamer
 */
void entity_remove(librg::events::event_t* evt)
{
    auto event = (librg::events::event_remove_t*) evt;
    auto remote = event->entity.component<gamedata_t>();
}

/**
 * Entity interpolation callback
 */
void entity_inter(librg::events::event_t* evt)
{
    auto event = (librg::events::event_entity_t*) evt;

    auto transform  = event->entity.component<librg::transform_t>();
    auto inter      = event->entity.component<librg::interpolable_t>();
    auto remote     = event->entity.component<gamedata_t>();

    // assing interpolated treansofrm, as our current transoform
    *transform = *(librg::transform_t*)event->data;

    remote->object->SetPosition(transform->position);
    // remote->object->SetRotation(HMM_QuaternionV4(transform->rotation));
}
