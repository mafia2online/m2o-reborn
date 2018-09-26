void mod_register_routes(librg_ctx_t *ctx) {
    // =======================================================================//
    // !
    // ! Connection handling callbacks
    // !
    // =======================================================================//

    librg_event_add(ctx, LIBRG_CONNECTION_REQUEST, [](librg_event_t *event) {
        if (mod.settings.password.size() == 0) { return; }

        // read password
        std::string password = "";
        u32 size = librg_data_ru32(event->data);
        for (usize i = 0; i < size; ++i) {
            password += librg_data_ru8(event->data);
        }

        // if not matches - reject
        if (password != mod.settings.password) {
            librg_event_reject(event);
        }
    });

    librg_event_add(ctx, LIBRG_CONNECTION_ACCEPT, [](librg_event_t *event) {
        auto entity = event->entity;
        // entity->position = vec3f(-421.75f, 479.31f, 0.05f);

        mod_log("spawning player %u at: %f %f %f\n",
            entity->id,
            entity->position.x,
            entity->position.y,
            entity->position.z
        );

        entity->user_data = m2o_ped_alloc(NULL);
        librg_entity_control_set(event->ctx, event->entity->id, event->entity->client_peer);

        m2o_args args = {0};
        m2o_args_init(&args);
        m2o_args_push_integer(&args, event->entity->id);
        m2o_event_trigger(M2O_EVENT_PLAYER_CONNECT, &args);
        m2o_args_free(&args);
    });

    librg_event_add(ctx, LIBRG_CONNECTION_DISCONNECT, [](librg_event_t *event) {
        auto ped = m2o_ped_get(event->entity);

        m2o_args args = {0};
        m2o_args_init(&args);
        m2o_args_push_integer(&args, event->entity->id);
        m2o_event_trigger(M2O_EVENT_PLAYER_DISCONNECT, &args);
        m2o_args_free(&args);

        librg_entity_iteratex(event->ctx, LIBRG_ENTITY_ALIVE, entityid, {
            auto entity = librg_entity_fetch(event->ctx, entityid);
            if (entity->type != M2O_ENTITY_CAR) continue;
            if (!(entity->flags & LIBRG_ENTITY_CONTROLLED)) continue;

            if (librg_entity_control_get(event->ctx, entity->id) == event->entity->client_peer) {
                mod_log("[info] removing control on %d for disconnected client\n", entity->id);
                librg_entity_control_remove(event->ctx, entity->id); // remove control from our ped
            }
        });

        m2o_ped_alloc(ped);
    });

    // =======================================================================//
    // !
    // ! General routing callbacks
    // !
    // =======================================================================//

    librg_event_add(ctx, LIBRG_ENTITY_CREATE, [](librg_event_t *event) {
        switch (event->entity->type) {
            case M2O_ENTITY_PLAYER_PED:
            case M2O_ENTITY_DUMMY_PED:  { on_ped_create(event); } break;
            // case M2O_ENTITY_CAR:        { on_car_create(event); } break;
        }
    });

    librg_event_add(ctx, LIBRG_ENTITY_UPDATE, [](librg_event_t *event) {
        switch (event->entity->type) {
            case M2O_ENTITY_PLAYER_PED:
            case M2O_ENTITY_DUMMY_PED:  { auto ped = m2o_ped_get(event->entity); librg_data_wptr(event->data, &ped->stream, sizeof(ped->stream)); } break;
            // case M2O_ENTITY_CAR:        { auto car = m2o_car_get(event->entity); librg_data_wptr(event->data, &car->stream, sizeof(car->stream)); } break;
        }
    });

    librg_event_add(ctx, LIBRG_ENTITY_REMOVE, [](librg_event_t *event) {
        if (!event->entity) return; // entity has been deleted
        mod_log("[info] sending a remove packet for entity: %d\n", event->entity->id);

        if (librg_entity_control_get(event->ctx, event->entity->id) == event->peer) {
            mod_log("removing control of entity: %d for peer: %llx\n", event->entity->id, (u64)event->peer);
            librg_entity_control_remove(event->ctx, event->entity->id);
        }

        switch (event->entity->type) {
            case M2O_ENTITY_PLAYER_PED:
            case M2O_ENTITY_DUMMY_PED:  { on_ped_remove(event); } break;
            // case M2O_ENTITY_CAR:        { on_car_remove(event); } break;
        }
    });

    librg_event_add(ctx, LIBRG_CLIENT_STREAMER_UPDATE, [](librg_event_t *event) {
        switch (event->entity->type) {
            case M2O_ENTITY_PLAYER_PED:
            case M2O_ENTITY_DUMMY_PED:  { auto ped = m2o_ped_get(event->entity); librg_data_rptr(event->data, &ped->stream, sizeof(ped->stream)); } break;
            // case M2O_ENTITY_CAR:        { auto car = m2o_car_get(event->entity); librg_data_rptr(event->data, &car->stream, sizeof(car->stream)); } break;
        }
    });

    // =======================================================================//
    // !
    // ! General user action messages
    // !
    // =======================================================================//

    librg_network_add(ctx, M2O_USER_SET_NAME, [](librg_message_t *msg) {
        auto entity = librg_entity_find(msg->ctx, msg->peer);
        auto ped    = m2o_ped_get(entity);

        char buffer[128] = {0};
        auto size = librg_data_ru8(msg->data);
        size = zpl_min(size, 127);
        librg_data_rptr(msg->data, buffer, size);

        mod_log("[info] client %d requested name change to: %s\n", entity->id, buffer);

        m2o_event_result result = M2O_EVENT_RESULT_NONE; {
            m2o_args args = {0};
            m2o_args_init(&args);
            m2o_args_push_string(&args, buffer);
            m2o_event_trigger_result(M2O_EVENT_PLAYER_NAME, &args, &result);
            m2o_args_free(&args);
        }

        /* prevent execution if we rejected the event */
        if (result & M2O_EVENT_RESULT_REJECTED) { return; }

        mod_message_send_instream_except(msg->ctx, M2O_USER_SET_NAME, entity->id, msg->peer, [&](librg_data_t *data) {
            librg_data_wu32(data, entity->id);
            librg_data_wu8(data, size);
            librg_data_wptr(data, (void *)ped->name, size);
        });
    });

    librg_network_add(ctx, M2O_USER_MESSAGE, [](librg_message_t *msg) {
        char message_buffer[632], input_buffer[512];

        u32 strsize = librg_data_ru32(msg->data);
        strsize = zpl_min(strsize, 511);

        librg_data_rptr(msg->data, input_buffer, strsize);
        input_buffer[strsize] = '\0';

        for (int i = 0; i < strsize; i++) input_buffer[i] = input_buffer[i] == '%' ? '\045' : input_buffer[i];

        auto entity = librg_entity_find(msg->ctx, msg->peer);
        auto ped = m2o_ped_get(entity);

        zpl_snprintf(message_buffer, 632, "%s (%d): %s", ped->name, entity->id, input_buffer);

        mod_log("[chat] %s \n", message_buffer);

        mod_message_send(msg->ctx, M2O_USER_MESSAGE, [&](librg_data_t *data) {
            librg_data_wu32(data, zpl_strlen(message_buffer));
            librg_data_wptr(data, message_buffer, zpl_strlen(message_buffer));
        });
    });

    // =======================================================================//
    // !
    // ! Other callbacks
    // !
    // =======================================================================//

    librg_network_add(ctx, M2O_PED_CREATE,   on_ped_create_command); /* testing */
    // librg_network_add(ctx, M2O_CAR_CREATE,   on_car_create_command); /* testing */
    // librg_network_add(ctx, M2O_CAR_ENTER,    on_car_enter_start);
    // librg_network_add(ctx, M2O_CAR_EXIT,     on_car_exit_start);

}
