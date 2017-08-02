uint32_t game_base;

void game_init()
{
    game_base = Mem::GetModuleAddress(nullptr);
    Mem::Initialize();

    printf("<CGame::HirePreHookers> Current Thread ID: %x\n", GetCurrentThreadId());

    FilePatcher_Initialize();
    SteamDRMPatch_Apply();
    GameHooks_Install();

    printf("hey\n");

    // Disable loading screen
    Mem::Utilites::PatchAddress(0x08CA820, 0xC300B0); // mov al, 0; retn

    // Disable DLC loadings
    //Mem::Utilites::PatchAddress(0x11A62C0, 0xC300B0); // mov al, 0; retn
}


void game_on_init()
{
    corelog("GameInit \\(^o^)/ (Thread: %x)", GetCurrentThreadId());
    GameHooks_InstallLate();

    M2::C_GameGuiModule::Get()->FaderFadeIn(1); // therotically we shouldn't call it here but because it's a sync object it's fine itll work but the local player isn't created just yet.
}

void game_on_tick()
{
    librg::core::tick();

    // testing dont complain bitch
    if (!dwLocalPlayer)
    {
        dwLocalPlayer = reinterpret_cast<M2::C_Player2*>(M2::C_Game::Get()->GetLocalPed());

        if (dwLocalPlayer)
        {
            if (M2::C_SDSLoadingTable::Get())
            {
                M2::C_SDSLoadingTable::Get()->ActivateStreamMapLine("free_joe_load");
                M2::C_SDSLoadingTable::Get()->ActivateStreamMapLine("free_summer_load");

                M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetDayTemplate("DTFreerideNight");

                CommandProcessor::RegisterCommand("lock",
                    [](const std::string& params)->void
                {
                    int lock = atoi(params.c_str());
                    dwLocalPlayer->LockControls(lock);
                    corelog("Controls %s!", lock ? ("locked") : ("unlocked"));
                });

                CommandProcessor::RegisterCommand("ent",
                    [](const std::string& params)->void
                {
                    ent = M2::C_EntityFactory::Get()->CreateEntity<M2::C_Human2>(M2::EntityTypes::Entity_Human);
                    if (ent)
                    {
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
                            corelog("Flags set sucessfully!");

                        reinterpret_cast<M2::C_Entity *>(ent)->Activate();

                        if(reinterpret_cast<M2::C_Entity *>(ent)->IsActive())
                            corelog("Entity active !");

                        Vector3 pos;
                        Mem::InvokeFunction<Mem::call_this, void>(reinterpret_cast<M2::C_Entity *>(dwLocalPlayer)->m_pVFTable->GetPosition, reinterpret_cast<M2::C_Entity*>(dwLocalPlayer), &pos);
                        reinterpret_cast<M2::C_Entity *>(ent)->SetPosition(pos);
                    }

                    corelog("Created at %x!", ent);
                });

                CommandProcessor::RegisterCommand("time",
                    [=](const std::string& params)->void
                {
                    float time = atof(params.c_str());
                    M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetTime(time);
                    corelog("Set time to %f", time);
                });
            }

            CommandProcessor::RegisterCommand("spawn",
                [=](const std::string& params)->void
            {
                Mem::InvokeFunction<Mem::call_this, void>(reinterpret_cast<M2::C_Entity*>(dwLocalPlayer)->m_pVFTable->SetPosition, dwLocalPlayer, &HMM_Vec3(-421.758942, 479.316925, 0.051288));
            });

        //     CommandProcessor::RegisterCommand("shake", 
        //         [=](const std::string& params)->void
        //     {
        //         M2::ShakeCommandData data;
        //         data.speed = 50;
        //         data.strength = 5;
        //         data.duration = 3;
        //         M2::C_CameraModule::Get()->GetCamera(1)->BroadcastCommand(M2::CAMCOMMAND_SHAKE, &data, 0);
        //     });

        //     CommandProcessor::RegisterCommand("fpv",
        //         [=](const std::string& params)->void
        //     {
        //         if (M2::C_CameraModule::Get()->GetCamera(1)->ModeGetActiveTypeTop() != M2::CAMMODE_FPV)
        //             M2::C_CameraModule::Get()->GetCamera(1)->ModeChange(M2::CAMMODE_FPV, dwLocalPlayer, true, true);
        //         else
        //             M2::C_CameraModule::Get()->GetCamera(1)->ModePop(1, 1);
        //     });
        }
    }

    if (GetAsyncKeyState(VK_LEFT) & 0x1)
    {
        ztime -= 0.1f;
        if (ztime < 0)
            ztime = 0;
        M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetTime(ztime);
        corelog("Time shift!");
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x1)
    {
        ztime += 0.1f;
        if (ztime > 1.0f)
            ztime = 1.0f;
        M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetTime(ztime);
        corelog("Time shift!");
    }
    if (GetAsyncKeyState(VK_F2) & 0x1)
    {
        M2::C_SyncObject *pSyncObject = nullptr;
        ent->GetScript()->LockControls(true, true);
        ent->GetScript()->ScriptAnimPlay(&pSyncObject, "TAXI_CALL-A", true, 0, 0, 0.0f, 0.30000001f, 0.3000000f);
    }

    if (GetAsyncKeyState(VK_F3) & 0x1)
    {
        M2::C_SyncObject *pSyncObject = nullptr;
        ent->GetScript()->LockControls(true, true);
        ent->GetScript()->ScriptAnimPlay(&pSyncObject, "TAXI_CALL-B", true, 0, 0, 0.0f, 0.30000001f, 0.3000000f);
    }

    if (GetAsyncKeyState(VK_F4) & 0x1)
    {
        M2::S_ExplosionInit *fire = new M2::S_ExplosionInit;
        M2::C_ShotManager::Get()->CreateExplosion(fire);
        corelog("Created fire!");
    }
    if (GetAsyncKeyState(VK_F5) & 0x1)
    {
        librg::network::start();
        // if (CNetworkManager::Instance().Connect("127.0.0.1", 1234, "")) {
        //     corelog("Connection accepted");
        // }
        // else {
        //     corelog("Cannot connect");
        // }
    }
    if (GetAsyncKeyState(VK_F6) & 0x1)
    {
        // if (CNetworkManager::Instance().Disconnect())
        //     corelog("Disconnected");
        // else
        //     corelog("Cannot disconnect");
    }
}

bool game_on_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    MSG msg;
    msg.hwnd = hWnd;
    msg.lParam = lParam;
    msg.wParam = wParam;
    msg.message = uMsg;

    if (uMsg == WM_KEYDOWN) {
        if (wParam == VK_RETURN && !CDebugConsole::Instance().HasFocus()) {
            CDebugConsole::Instance().Focus();
            return true;
        }
    }

    if (m_graphicsmanager.GetGwenManager()->GetInput().ProcessMessage(msg))
        return true;

    return false; // not handled
}
