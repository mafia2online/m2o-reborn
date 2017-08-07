void game_init()
{
    mod_log("GameInit \\(^o^)/ (Thread: %x)", GetCurrentThreadId());
    tools::gamehooks_install_late();
    M2::C_GameGuiModule::Get()->FaderFadeIn(1); // therotically we shouldn't call it here but because it's a sync object it's fine itll work but the local player isn't created just yet.
}

// todo refactor
bool spawned = false;
float ztime = 0; // debugging time stuff, nice to have for now

void game_tick(librg::events::event_t* evt)
{
    auto event  = (librg::events::event_tick_t*) evt;
    auto player = mod.player.component<gamedata_t>();

    if (!player->object) {
        auto local_ped = M2::C_Game::Get()->GetLocalPed();

        if (!local_ped) {
            return mod_log("someting is not right, can this even happen????");
        }

        player->object = (M2::C_Entity*)local_ped;
    }

    if (M2::C_SDSLoadingTable::Get()) {
        M2::C_SDSLoadingTable::Get()->ActivateStreamMapLine("free_joe_load");
        M2::C_SDSLoadingTable::Get()->ActivateStreamMapLine("free_summer_load");

        M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetDayTemplate("DT_RTRclear_day_late_afternoon");
    }

    if (GetAsyncKeyState(VK_LEFT) & 0x1) {
        ztime -= 0.1f;
        if (ztime < 0)
            ztime = 0;
        M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetTime(ztime);
        mod_log("Time shift!");
    }

    if (GetAsyncKeyState(VK_RIGHT) & 0x1) {
        ztime += 0.1f;
        if (ztime > 1.0f)
            ztime = 1.0f;
        M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetTime(ztime);
        mod_log("Time shift!");
    }

    if (GetAsyncKeyState(VK_F4) & 0x1) {
        M2::S_ExplosionInit *fire = new M2::S_ExplosionInit;
        M2::C_ShotManager::Get()->CreateExplosion(fire);
        mod_log("Created fire!");
    }

    if (GetAsyncKeyState(VK_F5) & 0x1 && !spawned) {
        mod_log("spawning and connecting...");
        ((M2::C_Player2*)(player->object))->LockControls(false);
        
        Mem::InvokeFunction<Mem::call_this, void>(
            player->object->m_pVFTable->SetPosition, 
            player->object, 
            &HMM_Vec3(-421.758942, 479.316925, 0.051288)
        );

        librg::network::start();
        spawned = true;
    }
}
