#define settings_default                    \
    "{\n"                                   \
    "    /* json5 supported */\n\n"         \
                                            \
    "    /* connection settings */\n"       \
    "    hostname: \"my m2o server\",\n"    \
    "    password: \"\",\n"                 \
    "    max_connections: 128,\n"           \
    "    port: 27010,\n\n"                  \
                                            \
    "    /* server parameters */\n"         \
    "    stream_range: 250.0,\n"            \
    "    tick_delay: 100.0,\n\n"            \
                                            \
    "    /* masterlist parameters */\n"     \
    "    masterlist_login: \"\",\n"         \
    "    masterlist_secret: \"\",\n"        \
    "}\n"

zpl_json_object *settings_read_value(zpl_json_object *obj, char *name) {
    for (i32 i = 0; i < zpl_array_count(obj->nodes); ++i) {
        zpl_json_object *ptr = (obj->nodes + i);
        if (!strcmp(ptr->name, name)) {
            return ptr;
        }
    }

    return NULL;
}

#define settings_readto(handle, name, type, dest) do { \
        zpl_json_object *element = settings_read_value(handle, (char *)name); \
        if (element) { dest = element->type; } \
    } while(0)

void settings_read(librg_ctx_t *ctx, librg_address_t *address, mod_t *mod) {
    zpl_file_t file;
    zplFileError error = zpl_file_open(&file, "server.json");

    // create file if didnt exist
    if (error != 0) {
        zpl_file_create(&file, "server.json");
        zpl_file_write(&file, settings_default, zpl_strlen(settings_default));
        zpl_file_seek(&file, 0);
    }

    // read and copy the data
    isize file_size = (isize)zpl_file_size(&file);
    char *content = (char *)zpl_malloc(file_size+1);
    zpl_file_read(&file, content, file_size);
    content[file_size] = 0;
    zpl_file_close(&file);

    // parse json
    zpl_json_object root = {0}; u8 error2;
    zpl_json_parse(&root, file_size, content, zpl_heap_allocator(), true, &error2);

    // read up the data
    settings_readto(&root, "port", integer, address->port);
    settings_readto(&root, "tick_delay", real, ctx->tick_delay);
    settings_readto(&root, "max_connections", integer, ctx->max_connections);

    f32 stream_range = 250.0f;
    settings_readto(&root, "stream_range", real, stream_range);
    if (!stream_range) { stream_range = 250; }
    librg_option_set(LIBRG_DEFAULT_STREAM_RANGE, (u32)stream_range);

    settings_readto(&root, "hostname", string, mod->settings.hostname);
    settings_readto(&root, "password", string, mod->settings.password);

    settings_readto(&root, "masterlist_login", string, mod->settings.masterlist_login);
    settings_readto(&root, "masterlist_secret", string, mod->settings.masterlist_secret);

    mod->settings.port = address->port;
    mod->settings.max_connections = ctx->max_connections;

    if (mod->settings.hostname.size() > 256) {
        mod_log("[error] server hostname can't be longer than 256 symbols");
        mod_assert(0);
        return;
    }

    // free
    zpl_json_free(&root);
    zpl_mfree(content);
}
