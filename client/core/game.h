float ztime = 0; // debugging time stuff, nice to have for now

void game_init()
{
    mod_log("GameInit \\(^o^)/ (Thread: %x)\n", GetCurrentThreadId());
    tools::gamehooks_install_late();
    M2::C_GameGuiModule::Get()->FaderFadeIn(1); // therotically we shouldn't call it here but because it's a sync object it's fine itll work but the local player isn't created just yet.
}

void game_connect()
{
    mod.state = MOD_DEBUG_STATE;
    mod_log("spawning and connecting...\n");

    if (M2::C_SDSLoadingTable::Get()) {
        M2::C_SDSLoadingTable::Get()->ActivateStreamMapLine("free_joe_load");
        M2::C_SDSLoadingTable::Get()->ActivateStreamMapLine("free_summer_load");

        M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetDayTemplate("DT_RTRclear_day_late_afternoon");
    }

    auto ped = (M2::C_Entity*)M2::C_Game::Get()->GetLocalPed();
    ((M2::C_Player2*)ped)->LockControls(false);
    ped->SetPosition(vec3(-421.75f, 479.31f, 0.05f));
	
	char *arg = GetCommandLine();
	LPCSTR tok = strrchr(arg, '"');
	if (tok == NULL) {
		tok = strtok(arg, " ");
		tok = strtok(NULL, " ");
	}
	else {
		char buf[MAX_PATH] = { 0 };
		tok = strtok(arg, "\"");
		tok = strtok(NULL, "\"");

		if (tok != NULL) {
			strcpy(buf, tok);
		}
	}

	if (tok != NULL) {
		while (zpl_char_is_space(*tok)) ++tok;
		
		char hostname[16] = { 0 }; char hr = 0;
		char port_raw[5]  = { 0 }; char pr = 0;
		int port = 27010;

		while (*tok && (*tok != ':' && !zpl_char_is_space(*tok))) {
			hostname[hr++] = *tok++;
		}

		if (*tok++ == ':') {
			while (*tok && zpl_char_is_digit(*tok)) {
				port_raw[pr++] = *tok++;
			}

			port = atoi(port_raw);
		}
		
		librg_network_start({ hostname, port });
	}
	else {
		librg_network_start({ "localhost", 27010 });
	}
}

void game_disconnect()
{
    mod_log("disconnecting...\n");
    librg_network_stop();
}

void game_tick()
{
    librg_tick();

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

    if (GetAsyncKeyState(VK_F1) & 0x1) {
        mod.input_blocked = !mod.input_blocked;
    }

    if (GetAsyncKeyState(VK_F8) & 0x1) {
        // send vehicle create request onto server
        librg_send(MOD_VEHICLE_CREATE, data, {});
    }

    if (GetAsyncKeyState(VK_F9) & 0x1) {
        // send vehicle create request onto server
        librg_send(MOD_VEHICLE_ENTER, data, {});
    }

    if (GetAsyncKeyState(VK_F5) & 0x1 && !mod.spawned) {
        game_connect();
        mod.spawned = true;
    }
}
