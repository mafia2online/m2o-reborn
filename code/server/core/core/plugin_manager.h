#define M2O_PLUGIN_DIRECTORY "plugins"
typedef void (m2o_plugin_init)(const m2o_api_vtable *, m2o_plugin *);

static zpl_array(m2o_plugin) m2o_plugins;

void m2o_plugins_init(librg_ctx_t *ctx, mod_t *mod) {
    zpl_array_init(m2o_plugins, zpl_heap());

    /* create folder if doesnt exist */
    zpl_path_mkdir(M2O_PLUGIN_DIRECTORY, 0744);

    zpl_string_t filelst = {0};
    filelst = zpl_path_dirlist(zpl_heap(), M2O_PLUGIN_DIRECTORY, false);
    char **files = zpl_str_split_lines(zpl_heap(), filelst, false);

    /* for now just iterate all files inside plugin folder and try to load */
    for (i32 i = 0; i < zpl_array_count(files); ++i) {
        zpl_dll_handle dll = zpl_dll_load(files[i]);
        if (!dll) {
            mod_log("[error] (%s) is not a valid dynamic library, skipping...\n", files[i]);
            continue;
        }

        auto plugin_main = (m2o_plugin_init *)zpl_dll_proc_address(dll, "m2o_plugin_main");
        if (!plugin_main) {
            mod_log("[error] (%s) doesnt contain m2o_plugin_main, skipping...\n", files[i]);
            continue;
        }

        m2o_plugin plugin = {0};
        plugin_main((const m2o_api_vtable *)mod->api, &plugin);

        if (!plugin.name) {
            plugin.name = "undefined";
            plugin.author = "unknown";
        }

        mod_log("[info] successfully loaded plugin: %s@%d.%d.%d by %s\n", plugin.name,
            M2O_VERSION_GET_MAJOR(plugin.version),
            M2O_VERSION_GET_MINOR(plugin.version),
            M2O_VERSION_GET_PATCH(plugin.version),
            plugin.author
        );

        zpl_array_append(m2o_plugins, plugin);
    }

    m2o_event_trigger(M2O_EVENT_PLUGIN_INIT, NULL);

    zpl_string_free(filelst);
    zpl_array_free(files);
}

void m2o_plugins_tick(librg_ctx_t *ctx, mod_t *mod) {
    m2o_event_trigger(M2O_EVENT_PLUGIN_TICK, NULL);
}

void m2o_plugins_stop(librg_ctx_t *ctx, mod_t *mod) {
    m2o_event_trigger(M2O_EVENT_PLUGIN_STOP, NULL);
    zpl_array_free(m2o_plugins);
}

void m2o_event_trigger_result(m2o_event_type type, const m2o_args* args, m2o_event_result *result) {
    for (int i = 0; i < zpl_array_count(m2o_plugins); ++i) {
        m2o_callback *callback = NULL;

        /* find needed event */
        switch (type) {
            case M2O_EVENT_NONE: break;

            case M2O_EVENT_PLUGIN_INIT:         callback = m2o_plugins[i].callbacks.plugin_init;        break;
            case M2O_EVENT_PLUGIN_TICK:         callback = m2o_plugins[i].callbacks.plugin_tick;        break;
            case M2O_EVENT_PLUGIN_STOP:         callback = m2o_plugins[i].callbacks.plugin_stop;        break;

            case M2O_EVENT_SERVER_CONSOLE:      callback = m2o_plugins[i].callbacks.server_console;     break;

            case M2O_EVENT_PLAYER_CONNECT:      callback = m2o_plugins[i].callbacks.player_connect;     break;
            case M2O_EVENT_PLAYER_DISCONNECT:   callback = m2o_plugins[i].callbacks.player_disconnect;  break;
            case M2O_EVENT_PLAYER_NAME:         callback = m2o_plugins[i].callbacks.player_name;        break;
            case M2O_EVENT_PLAYER_CHAT:         callback = m2o_plugins[i].callbacks.player_chat;        break;

            case M2O_EVENT_PED_WEAPON_CHANGE:   callback = m2o_plugins[i].callbacks.ped_car_enter;      break;
            case M2O_EVENT_PED_CAR_ENTER:       callback = m2o_plugins[i].callbacks.ped_car_exit;       break;
            case M2O_EVENT_PED_CAR_EXIT:        callback = m2o_plugins[i].callbacks.ped_weapon_change;  break;
            case M2O_EVENT_PED_DEATH:           callback = m2o_plugins[i].callbacks.ped_health_change;  break;
            case M2O_EVENT_PED_HEALTH_CHANGE:   callback = m2o_plugins[i].callbacks.ped_death;          break;

            case M2O_EVENT_CUSTOM:              callback = m2o_plugins[i].callbacks.custom_event;       break;
        }

        /* trigger the plugin event, if handler exists */
        if (callback) { callback(args, result); }
    }
}

void m2o_event_trigger(m2o_event_type type, const m2o_args* args) {
    m2o_event_result result = M2O_EVENT_RESULT_NONE;
    m2o_event_trigger_result(type, args, &result);
}
