bool mod_init()
{
    Mem::Initialize();

    mod_log("<CGame::HirePreHookers> Current Thread ID: %x\n", GetCurrentThreadId());

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
    mod.paths.files = temp_path + "\\files";
    mod.paths.debug = temp_path + "\\debug.log";
    mod.paths.game_files = temp_path + "\\game_files";
}

/**
 * Trigger attachment to process
 * @param module
 */
void mod_attach(HMODULE module)
{
    zpl_mutex_init(&mod.mutexes.log);

    // console, yay
    tools::console_attach();
    tools::console_color_fg(3);
    {
        mod_log("the\nm2o-reborn\n");
        mod_log("starting...\n");
    }
    tools::console_color_fg(7);

    // path n basics
    mod_path_register(module);
    mod.debug_stream.open(mod.paths.debug);
    mod.module = module;

    // setup manual client mode
    librg_config_t config;
    config.tick_delay = 32;
    config.mode = LIBRG_MODE_CLIENT;
    config.world_size = zplm_vec2(5000.0f, 5000.0f);
    config.max_entities = 16000;

    librg_init(config);

    // setup callbacks
    librg_event_add(LIBRG_CONNECTION_ACCEPT, client_connect);
    librg_event_add(LIBRG_CONNECTION_REFUSE, client_disconnect);
    librg_event_add(LIBRG_ENTITY_CREATE, entity_create);
    librg_event_add(LIBRG_ENTITY_UPDATE, entity_update);
    librg_event_add(LIBRG_ENTITY_REMOVE, entity_remove);
    librg_event_add(LIBRG_CLIENT_STREAMER_UPDATE, clientstream_update);

    if (graphics_init() == false) {
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

    CDirectInput8Hook::Install();
    mod.state = MOD_TITLE_STATE;
}

/**
 * Trigger exiting from the mod
 * @param reason
 */
void mod_exit(std::string reason)
{
    mod_log("exiting %s\n", reason.c_str());

    librg_free();
    mod.debug_stream.close();

    zpl_mutex_destroy(&mod.mutexes.log);

    MessageBoxA(nullptr, reason.c_str(), "Well.. Something went wrong!", MB_OK);

    exit(0);
}
