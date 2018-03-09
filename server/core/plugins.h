#define M2O_PLUGIN_DIRECTORY "plugins"
typedef void (m2o_plugin_init)(m2o_api_vtable *);

void m2o_plugins_init(m2o_api_vtable *api) {
    /* create folder if doesnt exist */
    zpl_path_mkdir(M2O_PLUGIN_DIRECTORY, 0744);

    zpl_string_t filelst = {0};
    filelst = zpl_path_dirlist(zpl_heap(), M2O_PLUGIN_DIRECTORY, false);
    char **files = zpl_str_split_lines(zpl_heap(), filelst, false);

    /* for now just iterate all files inside plugin folder and try to load */
    for (i32 i = 0; i < zpl_array_count(files); ++i) {
        zpl_dll_handle dll = zpl_dll_load(files[i]);

        if (!dll) {
            mod_log("[error] could not load a plugin: %s\n", files[i]);
        } else {
            auto plugin_main = (m2o_plugin_init *)zpl_dll_proc_address(dll, "m2o_plugin_main");

            if (!plugin_main) {
                mod_log("[error] could not load a plugin: %s\n", files[i]);
            } else {
                mod_log("[info] successfully loaded plugin: %s\n", files[i]);
                plugin_main(api);
            }
        }

    }

    zpl_string_free(filelst);
    zpl_array_free(files);
}
