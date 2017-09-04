void game_init()
{
    mod_log("GameInit \\(^o^)/ (Thread: %x)\n", GetCurrentThreadId());
    tools::gamehooks_install_late();
    M2::C_GameGuiModule::Get()->FaderFadeIn(1); // therotically we shouldn't call it here but because it's a sync object it's fine itll work but the local player isn't created just yet.
}

void game_connect()
{
    mod_log("spawning and connecting...\n");

    if (M2::C_SDSLoadingTable::Get()) {
        M2::C_SDSLoadingTable::Get()->ActivateStreamMapLine("free_joe_load");
        M2::C_SDSLoadingTable::Get()->ActivateStreamMapLine("free_summer_load");

        M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetDayTemplate("DT_RTRclear_day_late_afternoon");
    }

    auto ped = (M2::C_Entity*)M2::C_Game::Get()->GetLocalPed();
    ((M2::C_Player2*)ped)->LockControls(false);

    Mem::InvokeFunction<Mem::call_this, void>(
        ped->m_pVFTable->SetPosition, ped,
        &zplm_vec3(-421.75f, 479.31f, 0.05f)
    );

    mod.state = MOD_DEBUG_STATE;

    librg_network_start({ "localhost", 27010 });
}

bool spawned = false;
float ztime = 0; // debugging time stuff, nice to have for now
void game_tick()
{
    librg_tick();

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

    static M2::C_Human2 *ent = nullptr;

    if (GetAsyncKeyState(VK_F6) & 0x1 && spawned) {
        ent = M2::C_EntityFactory::Get()->CreateEntity<M2::C_Human2>(M2::EntityTypes::Entity_Human);

        librg_assert(ent, "player entity should be created!");

        DWORD coreInstance = *(DWORD*)(0x1AC2778);

        M2::C_Model *own_model = Mem::InvokeFunction<Mem::call_this, M2::C_Model*>((*(Address*)(*(DWORD*)coreInstance + 0x94)), coreInstance, 2);
        own_model->CloneHierarchy(M2::C_PlayerModelManager::Get()->GetInterface()->localPlayerModel);

        own_model->SetName("lawl");
        own_model->MarkForNotify(2);

        reinterpret_cast<M2::C_Entity *>(ent)->SetModel(own_model);
        reinterpret_cast<M2::C_Entity *>(ent)->Setup();

        // set flagsF
        DWORD flags = *(DWORD *)(ent + 32) & 0xFFFFFBF | 0x4800;
        *(DWORD *)(ent + 32) = flags;

        if (flags & 0x20)
            mod_log("Flags set sucessfully!");

        reinterpret_cast<M2::C_Entity *>(ent)->Activate();

        if (reinterpret_cast<M2::C_Entity *>(ent)->IsActive())
            mod_log("Entity active !");

        reinterpret_cast<M2::C_Entity *>(ent)->SetPosition(zplm_vec3(-421.75f, 479.31f, 0.05f));


        mod_log("Created at %x!", ent);
    }

    if (GetAsyncKeyState(VK_F7) & 0x1) {
        M2::C_SyncObject *pSyncObject = nullptr;

        mod_log("Trying to move vec");
        if (ent != nullptr) {
            auto vecPosition = zplm_vec3(-421.75f, 479.31f, 0.05f);
            auto vecDirection = zplm_vec3(-421.75f, 489.31f, 0.05f);
            ent->GetScript()->ScrLookAt(&pSyncObject, reinterpret_cast<M2::C_Entity *>(ent), vecDirection, true);
        }
    }

    if (GetAsyncKeyState(VK_F5) & 0x1 && !spawned) {
        game_connect();
        spawned = true;
    }
}
