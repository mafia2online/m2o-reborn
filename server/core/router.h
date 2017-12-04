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

    //auto object = new ped_t;
    //zpl_zero_item(object); // fill object with 0s
    entity->user_data = new ped_t();

    // TODO: allocate stuff
    librg_entity_control_set(event->ctx, event->entity->id, event->entity->client_peer);
}

void entity_on_create(librg_event_t *event) {
    // emtpy
}

void entity_on_update(librg_event_t *event) {
    switch (event->entity->type) {
        case TYPE_PED: { auto ped = (ped_t *)event->entity->user_data; librg_data_wptr(event->data, &ped->stream, sizeof(ped->stream)); } break;
        case TYPE_CAR: { auto car = (car_t *)event->entity->user_data; librg_data_wptr(event->data, &car->stream, sizeof(car->stream)); } break;
    }
}
void entity_on_csupdate(librg_event_t *event) {
    switch (event->entity->type) {
        case TYPE_PED: { auto ped = (ped_t *)event->entity->user_data; librg_data_rptr(event->data, &ped->stream, sizeof(ped->stream)); } break;
        case TYPE_CAR: { auto car = (car_t *)event->entity->user_data; librg_data_rptr(event->data, &car->stream, sizeof(car->stream)); } break;
    }
}

void entity_on_remove(librg_event_t *event) {
    // emtpty
}

void mod_register_routes(librg_ctx_t *ctx) {
    librg_event_add(ctx, LIBRG_CONNECTION_REQUEST,  on_connection_request);
    librg_event_add(ctx, LIBRG_CONNECTION_ACCEPT,   on_connect_accepted);

    librg_event_add(ctx, LIBRG_ENTITY_CREATE, entity_on_create);
    librg_event_add(ctx, LIBRG_ENTITY_UPDATE, entity_on_update);
    librg_event_add(ctx, LIBRG_ENTITY_REMOVE, entity_on_remove);
    librg_event_add(ctx, LIBRG_CLIENT_STREAMER_UPDATE, entity_on_csupdate);

    librg_network_add(ctx, MOD_CAR_CREATE,          module_car_create);
    librg_network_add(ctx, MOD_CAR_ENTER_START,     module_car_enter_start);
    librg_network_add(ctx, MOD_CAR_ENTER_FINISH,    module_car_enter_finish);
    librg_network_add(ctx, MOD_CAR_EXIT_START,      module_car_exit_start);
    librg_network_add(ctx, MOD_CAR_EXIT_FINISH,     module_car_exit_finish);
}
