// =======================================================================//
// !
// ! Generic game events
// !
// =======================================================================//

void mod_connected(librg_event_t *);
void mod_disconnected(librg_event_t *);
void mod_entity_create(librg_event_t *);
void mod_entity_update(librg_event_t *);
void mod_entity_remove(librg_event_t *);
void mod_entity_client(librg_event_t *);
void mod_entity_interpolate(librg_ctx_t *, librg_entity_t *);

void mod_game_init() {
    mod_log("GameInit \\(^o^)/ (Thread: %x)\n", GetCurrentThreadId());
    tools::gamehooks_install_late();
    M2::C_GameGuiModule::Get()->FaderFadeIn(1); // therotically we shouldn't call it here but because it's a sync object it's fine itll work but the local player isn't created just yet.
    M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetTime(0.5); /* 0.0 .. 1.0 - time of the day */

    // setup callbacks
    librg_event_add(ctx, LIBRG_CONNECTION_ACCEPT, mod_connected);
    librg_event_add(ctx, LIBRG_CONNECTION_REFUSE, mod_disconnected);
    librg_event_add(ctx, LIBRG_ENTITY_CREATE, mod_entity_create);
    librg_event_add(ctx, LIBRG_ENTITY_UPDATE, mod_entity_update);
    librg_event_add(ctx, LIBRG_ENTITY_REMOVE, mod_entity_remove);
    librg_event_add(ctx, LIBRG_CLIENT_STREAMER_UPDATE, mod_entity_client);
}

void mod_game_tick() {
    mod.last_delta  = (zpl_utc_time_now() - mod.last_update) / 1000.f;
    mod.last_update = zpl_utc_time_now();

    librg_tick(ctx);
    librg_entity_iterate(ctx, (LIBRG_ENTITY_ALIVE | MOD_ENTITY_INTERPOLATED), mod_entity_interpolate);
    updateDiscordPresence();

    /* show/hide mouse */
    if (GetAsyncKeyState(VK_F1) & 0x1) {
        mod.input_blocked = !mod.input_blocked;
    }
    /* connect to the server */
    if (GetAsyncKeyState(VK_F5) & 0x1 && !mod.spawned) {
        //game_connect();
        mod.spawned = true;
    }

#if 0
    if (GetAsyncKeyState(VK_LEFT) & 0x1) {
        ztime -= 0.1f;
        if (ztime < 0)
            ztime = 0;
        M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetTime(ztime);
        mod_log("Time shift!\n");
    }

    if (GetAsyncKeyState(VK_RIGHT) & 0x1) {
        ztime += 0.1f;
        if (ztime > 1.0f)
            ztime = 1.0f;
        M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetTime(ztime);
        mod_log("Time shift!\n");
    }


    if (GetAsyncKeyState(VK_F8) & 0x1) {
    }

    if (GetAsyncKeyState(VK_F6) & 0x1) {
        //librg_fetch_gamedata(lastcar)->object->Deactivate();
    }

    if (GetAsyncKeyState(VK_F7) & 0x1) {
        //librg_fetch_gamedata(lastcar)->object->Activate();
    }

    if (GetAsyncKeyState(VK_F9) & 0x1) {
    }
#endif
}

// =======================================================================//
// !
// ! Generic network events
// !
// =======================================================================//

void mod_connected(librg_event_t *event) {
    mod_log("connected to the server\n");
    mod.player = event->entity;

    auto ped = new ped_t();
    ped->object = (M2::C_Entity *)M2::C_Game::Get()->GetLocalPed();

    event->entity->user_data = ped;
    mod_log("my ped guid: %lu\n", (u32)ped->object->m_dwGUID);

    mod.state = MOD_DEBUG_STATE;
}

void mod_disconnected(librg_event_t *event) {
    mod_log("disconnected form the server\n");
    delete event->entity->user_data;
}

// =======================================================================//
// !
// ! Entity network events, routing
// !
// =======================================================================//

void mod_entity_create(librg_event_t *event) {
    switch (event->entity->type) {
        case TYPE_PED: { module_ped_callback_create(event); } break;
        case TYPE_CAR: { module_car_callback_create(event); } break;
    }
}

void mod_entity_update(librg_event_t *event) {
    switch (event->entity->type) {
        case TYPE_PED: { module_ped_callback_update(event); } break;
        case TYPE_CAR: { module_car_callback_update(event); } break;
    }
}

void mod_entity_remove(librg_event_t *event) {
    switch (event->entity->type) {
        case TYPE_PED: { module_ped_callback_remove(event); } break;
        case TYPE_CAR: { module_car_callback_remove(event); } break;
    }
}

void mod_entity_client(librg_event_t *event) {
    switch (event->entity->type) {
        case TYPE_PED: { module_ped_callback_clientstream(event); } break;
        case TYPE_CAR: { module_car_callback_clientstream(event); } break;
    }
}

void mod_entity_interpolate(librg_ctx_t *ctx, librg_entity_t *entity) {
    switch (entity->type) {
        case TYPE_PED: { module_ped_callback_interpolate(entity); } break;
        case TYPE_CAR: { module_car_callback_interpolate(entity); } break;
    }
}

// ARCHIVE

// void game_connect() {
//     mod.state = MOD_DEBUG_STATE;
//     mod_log("spawning and connecting...\n");

//     if (M2::C_SDSLoadingTable::Get()) {
//         M2::C_SDSLoadingTable::Get()->ActivateStreamMapLine("free_joe_load");
//         M2::C_SDSLoadingTable::Get()->ActivateStreamMapLine("free_summer_load");

//         M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetDayTemplate("DT_RTRclear_day_late_afternoon");
//     }

//     auto ped = (M2::C_Entity*)M2::C_Game::Get()->GetLocalPed();
//     ((M2::C_Player2*)ped)->LockControls(false);
//     ped->SetPosition(vec3(-421.75f, 479.31f, 0.05f));

//     char *arg = GetCommandLine();
//     LPCSTR tok = strrchr(arg, '"');
//     if (tok == NULL) {
//         tok = strtok(arg, " ");
//         tok = strtok(NULL, " ");
//     }
//     else {
//         char buf[MAX_PATH] = { 0 };
//         tok = strtok(arg, "\"");
//         tok = strtok(NULL, "\"");

//         if (tok != NULL) {
//             strcpy(buf, tok);
//         }
//     }

//     if (tok != NULL) {
//         while (zpl_char_is_space(*tok)) ++tok;

//         char hostname[16] = { 0 }; char hr = 0;
//         char port_raw[5]  = { 0 }; char pr = 0;
//         int port = 27010;

//         while (*tok && (*tok != ':' && !zpl_char_is_space(*tok))) {
//             hostname[hr++] = *tok++;
//         }

//         if (*tok++ == ':') {
//             while (*tok && zpl_char_is_digit(*tok)) {
//                 port_raw[pr++] = *tok++;
//             }

//             port = atoi(port_raw);
//         }

//         librg_network_start(ctx, { port, hostname });
//     }
//     else {
//         librg_network_start(ctx, { 27010, "localhost" });
//     }
// }

// void game_disconnect() {
//     mod_log("disconnecting...\n");
//     librg_network_stop(ctx);
// }
