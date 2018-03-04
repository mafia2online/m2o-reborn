typedef void (m2o_plugin_init)(m2o_api_vtable *);

void m2o_plugins_init(m2o_api_vtable *api) {
    // TODO: refactor to proper platform methods
    // doing some wierd system command stuff for now :p

    char buffer[1024];
    // zpl_system_command("mkdir -p plugins", buffer);
    zpl_zero_item(buffer);

    char filename[128];

    // WARNING: ugly code below
    // TODO: refactor with proper string and platform methods
    // for (int i = 0, j = 0; i < zpl_strlen(buffer); ++i, ++j) {
    //     if (buffer[i] == '\n') {
    //         filename[j] = '\0';
    //         // mod_log("[info] trying to load: %s\n", filename);

    //         char filepath[256];
    //         zpl_snprintf(filepath, 256, "plugins/%s", filename);
    //         zpl_dll_handle_t dll = zpl_dll_load(filepath);

    //         if (!dll) {
    //             mod_log("[error] could not load a plugin: %s\n", filename);
    //         } else {
    //             auto plugin_main = (m2o_plugin_init *)zpl_dll_proc_address(dll, "m2o_plugin_main");

    //             if (!plugin_main) {
    //                 mod_log("[error] could not load a plugin: %s\n", filename);
    //             } else {
    //                 mod_log("[info] successfully loaded plugin: %s\n", filename);
    //                 plugin_main(api);
    //             }
    //         }

    //         zpl_zero_item(filename); j = -1;
    //     } else {
    //         filename[j] = buffer[i];
    //     }
    // }
}
