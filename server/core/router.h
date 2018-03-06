/**
 * Place to decide should the client be allowed to connect
 */
void on_connection_request(librg_event_t *event) {
    if (mod.settings.password.size() == 0) {
        return;
    }

    // read password
    u32 size = librg_data_ru32(event->data);
    std::string password = "";
    for (usize i = 0; i < size; ++i) {
        password += librg_data_ru8(event->data);
    }

    // if not matches - reject
    if (password != mod.settings.password) {
        librg_event_reject(event);
    }
}

/**
 * On client connected
 */
void on_connect_accepted(librg_event_t *event) {
    auto entity = event->entity;
    // entity->position = vec3(-421.75f, 479.31f, 0.05f);

    mod_log("spawning player %u at: %f %f %f\n",
        entity->id,
        entity->position.x,
        entity->position.y,
        entity->position.z
    );

    entity->stream_range = 20.0f;
    entity->user_data = new ped_t();
    librg_entity_control_set(event->ctx, event->entity->id, event->entity->client_peer);
}

void on_connect_disconnect(librg_event_t *event) {
    delete (char *)event->entity->user_data;
}

void on_entity_create(librg_event_t *event) {
    mod_log("[info] sending a create packet for entity: %d\n", event->entity->id);

    switch (event->entity->type) {
        case TYPE_PED: { on_ped_create(event); } break;
        case TYPE_CAR: { on_car_create(event); } break;
    }
}

void on_entity_update(librg_event_t *event) {
    switch (event->entity->type) {
        case TYPE_PED: { auto ped = get_ped(event->entity); librg_data_wptr(event->data, &ped->stream, sizeof(ped->stream)); } break;
        case TYPE_CAR: { auto car = get_car(event->entity); librg_data_wptr(event->data, &car->stream, sizeof(car->stream)); } break;
    }
}

void on_entity_client_update(librg_event_t *event) {
    switch (event->entity->type) {
        case TYPE_PED: { auto ped = get_ped(event->entity); librg_data_rptr(event->data, &ped->stream, sizeof(ped->stream)); } break;
        case TYPE_CAR: { auto car = get_car(event->entity); librg_data_rptr(event->data, &car->stream, sizeof(car->stream)); } break;
    }
}

void on_entity_remove(librg_event_t *event) {
    if (!event->entity) return; // entity has been deleted
    mod_log("[info] sending a remove packet for entity: %d\n", event->entity->id);

    if (event->entity->flags & LIBRG_ENTITY_CONTROLLED) {
        librg_entity_control_remove(event->ctx, event->entity->id);
    }

    switch (event->entity->type) {
        case TYPE_PED: { on_ped_remove(event); } break;
        case TYPE_CAR: { on_car_remove(event); } break;
    }
}

void mod_register_routes(librg_ctx_t *ctx) {
    librg_event_add(ctx, LIBRG_CONNECTION_REQUEST,      on_connection_request);
    librg_event_add(ctx, LIBRG_CONNECTION_ACCEPT,       on_connect_accepted);
    librg_event_add(ctx, LIBRG_CONNECTION_DISCONNECT,   on_connect_disconnect);

    librg_event_add(ctx, LIBRG_ENTITY_CREATE,           on_entity_create);
    librg_event_add(ctx, LIBRG_ENTITY_UPDATE,           on_entity_update);
    librg_event_add(ctx, LIBRG_ENTITY_REMOVE,           on_entity_remove);
    librg_event_add(ctx, LIBRG_CLIENT_STREAMER_UPDATE,  on_entity_client_update);

    librg_network_add(ctx, MOD_PED_CREATE,              on_ped_create_command); /* testing */
    librg_network_add(ctx, MOD_CAR_CREATE,              on_car_create_command); /* testing */
    librg_network_add(ctx, MOD_CAR_ENTER_START,         on_car_enter_start);
    librg_network_add(ctx, MOD_CAR_ENTER_FINISH,        on_car_enter_finish);
    librg_network_add(ctx, MOD_CAR_EXIT_START,          on_car_exit_start);
    librg_network_add(ctx, MOD_CAR_EXIT_FINISH,         on_car_exit_finish);
}
