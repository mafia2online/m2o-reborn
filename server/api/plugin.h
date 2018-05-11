//@ M2O_EXPORT_REGION_START
typedef void (m2o_callback)(const m2o_args *);
typedef void (m2o_event_callback)(const char *, const m2o_args *);

typedef struct {
    const char *name;
    const char *author;
    u32 version;

    struct {
        m2o_callback *plugin_init;
        m2o_callback *plugin_tick;
        m2o_callback *plugin_stop;
        m2o_event_callback *plugin_event;
    } callbacks;
} m2o_plugin;
//@ M2O_EXPORT_REGION_END

m2o_export void m2o_event_trigger(const char* name, const m2o_args* args);
