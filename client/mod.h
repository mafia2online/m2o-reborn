bool mod_init()
{
    Mem::Initialize();

    printf("<CGame::HirePreHookers> Current Thread ID: %x\n", GetCurrentThreadId());

    tools::filepatcher_install();
    tools::steam_drm_install();
    tools::gamehooks_install();

    // Disable loading screen
    Mem::Utilites::PatchAddress(0x08CA820, 0xC300B0); // mov al, 0; retn

    // Disable DLC loadings
    //Mem::Utilites::PatchAddress(0x11A62C0, 0xC300B0); // mov al, 0; retn

    return true;
}

/**
 * Register all current mod paths
 * @param module
 */
void mod_path_register(HMODULE module)
{
    char temp_path_raw[MAX_PATH] = { '\0' };
    GetModuleFileName(module, temp_path_raw, MAX_PATH);

    auto temp_path = std::string(temp_path_raw);
    auto temp_pos  = temp_path.rfind("\\");

    temp_path.erase(temp_pos, std::string::npos);

    mod.paths.index = temp_path;
    mod.paths.files = librg::fs::path(temp_path, "files");
    mod.paths.debug = librg::fs::path(temp_path, "debug.log");
    mod.paths.game_files = librg::fs::path(temp_path, "game_files");
}

/**
 * Trigger attachment to process
 * @param module
 */
void mod_attach(HMODULE module)
{
    // console, yay
    tools::console_attach();
    tools::console_color_fg(3);
    {
        printf("the\nm2o-reborn\n");
        printf("starting...\n");
    }
    tools::console_color_fg(7);

    // path n basics
    mod_path_register(module);
    mod.debug_steam.open(mod.paths.debug);
    mod.module = module;

    // setup manual client mode
    librg::core_initialize(librg::mode_client_manual);

    // setup callbacks
    librg::events::add(librg::events::on_log, on_mod_log);
    librg::events::add(librg::events::on_tick, game_tick);
    librg::events::add(librg::events::on_inter, entity_inter);
    librg::events::add(librg::events::on_create, entity_create);
    librg::events::add(librg::events::on_update, entity_update);
    librg::events::add(librg::events::on_remove, entity_remove);
    librg::events::add(librg::events::on_connect, client_connect);
    librg::events::add(librg::events::on_disconnect, client_disconnect);
    librg::events::add(librg::events::on_client_stream_entity, clientstream_update);

    auto cfg = librg::config_t{};
    cfg.ip = "localhost";
    cfg.port = 27010;
    cfg.world_size = HMM_Vec3(5000.00, 5000.00, 5000.00);
    cfg.tick_delay = 32;
    cfg.max_connections = 8;
    cfg.platform_id = 1;
    cfg.proto_version = 1;
    cfg.build_version = 1;

    // start the client
    librg::core::start(cfg);

    if (global_gfx.Init() == false) {
        return mod_exit("Unable to init Graphics Manager");
    }

    if (mod_init() == false) {
        return mod_exit("unable to initialize the mod");
    }

    if (ExceptionHandler::Install() == false) {
        return mod_exit("Unable to install exception handler");
    }

    // if (m_clientSettings.LoadFile(CClientSettings::DEFAULT_SETTINGS_FILENAME) == false) {
    //     mod_exit("Unable to parse config file");
    // }

    rawinput::on_mousemove = [&](RAWMOUSE me) {
        mod.mouse.x += me.lLastX;
        mod.mouse.y += me.lLastY;
        mod.mouse.flags = me.usButtonFlags;
    };

    CDirectInput8Hook::Install();
    rawinput::hook();

    mod.state = MOD_TITLE_STATE;
}

/**
 * Windows window message handler
 * @return if message has been handled
 */
bool mod_wndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (!mod.window) {
        mod.window = hWnd;
    }

    // MSG msg;
    // msg.hwnd = hWnd;
    // msg.lParam = lParam;
    // msg.wParam = wParam;
    // msg.message = uMsg;

    return false; // todo

    // it was not there
    // but its part of the stuff
    // ZAK do it

    // if (uMsg == WM_LBUTTONDOWN) {
    //     mod_log("Pojeb sa ty pica!");
    // }

    // if (nk_ctx) {
    //     if (uMsg == WM_MOUSEMOVE) {
    //         mouse_pos pos = *(mouse_pos *)lParam;
    //         // we shall call it in a tight loop since WM_MOUSEMOVE IS not triggered by the game...
    //         nk_input_motion(nk_ctx, pos.x, pos.y);
    //         return true;
    //     }
    //     else if (nk_d3d9_handle_event(hWnd, uMsg, wParam, lParam))
    //         return true;
    // }

    // if (uMsg == WM_KEYDOWN) {
    //     if (wParam == VK_RETURN && !CDebugConsole::Instance().HasFocus()) {
    //         CDebugConsole::Instance().Focus();
    //         return true;
    //     }
    // }


    // return false; // not handled
}

/**
 * Trigger exiting from the mod
 * @param reason
 */
void mod_exit(std::string reason)
{
    mod_log("exiting %s", reason.c_str());

    librg::core_terminate();
    mod.debug_steam.close();

    MessageBoxA(nullptr, reason.c_str(), "Well.. Something went wrong!", MB_OK);

    exit(0);
}
