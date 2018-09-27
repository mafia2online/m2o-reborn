#define M2O_SERVER_API_REGION
// =======================================================================//
// !
// ! Basic types
// !
// =======================================================================//

#ifndef M2O_API_DISABLE_VEC3

    typedef union vec3 {
        struct {
            float x;
            float y;
            float z;
        };

        float xyz[3];
    } vec3;

#endif // M2O_API_DISABLE_VEC3

    typedef enum {
        M2O_ERROR_NONE,
        M2O_UNDEFINED_ENTITY,
    } m2o_error;

// =======================================================================//
// !
// ! Plugins/events structs
// !
// =======================================================================//

    typedef enum m2o_event_type {
        M2O_EVENT_NONE,

        M2O_EVENT_PLUGIN_INIT,
        M2O_EVENT_PLUGIN_TICK,
        M2O_EVENT_PLUGIN_STOP,

        M2O_EVENT_SERVER_CONSOLE, /* not implemented */

        M2O_EVENT_PLAYER_CONNECT,
        M2O_EVENT_PLAYER_DISCONNECT,
        M2O_EVENT_PLAYER_NAME,
        M2O_EVENT_PLAYER_CHAT,  /* not implemented */

        M2O_EVENT_PED_WEAPON_CHANGE, /* not implemented */
        M2O_EVENT_PED_CAR_ENTER,
        M2O_EVENT_PED_CAR_EXIT,
        M2O_EVENT_PED_DEATH, /* not implemented */
        M2O_EVENT_PED_HEALTH_CHANGE, /* not implemented */

        M2O_EVENT_CUSTOM,
    } m2o_event_type;

    typedef enum m2o_event_result {
        M2O_EVENT_RESULT_NONE       = 0,
        M2O_EVENT_RESULT_REJECTED   = (1 << 0),
    } m2o_event_result;

    typedef enum m2o_arg_type {
        M2O_ARG_POINTER,
        M2O_ARG_STRING,
        M2O_ARG_INTEGER,
        M2O_ARG_REAL,
    } m2o_arg_type;

    typedef struct m2o_arg_value {
        m2o_arg_type type;
        usize size;

        union {
            void *pointer;
            char *string;
            i64 integer;
            f64 real;
        };
    } m2o_arg_value;

    typedef struct m2o_args {
        m2o_arg_value* values;
        usize size;
    } m2o_args;

    typedef void (m2o_callback)(const m2o_args *, m2o_event_result *result);

    typedef struct {
        const char *name;
        const char *author;
        u32 version;

        struct {
            m2o_callback *plugin_init;
            m2o_callback *plugin_tick;
            m2o_callback *plugin_stop;

            m2o_callback *server_console;

            m2o_callback *player_connect;
            m2o_callback *player_disconnect;
            m2o_callback *player_name;
            m2o_callback *player_chat;

            m2o_callback *ped_car_enter;
            m2o_callback *ped_car_exit;
            m2o_callback *ped_weapon_change;
            m2o_callback *ped_health_change;
            m2o_callback *ped_death;

            m2o_callback *custom_event;
        } callbacks;
    } m2o_plugin;

#undef M2O_SERVER_API_REGION
// =======================================================================//
// !
// ! Errors
// !
// =======================================================================//

    static m2o_error m2o_last_error = M2O_ERROR_NONE;

    M2O_SERVER_API void m2o_error_set(m2o_error error) {
        m2o_last_error = error;
    }

    M2O_SERVER_API m2o_error m2o_error_get() {
        m2o_error last_error = m2o_last_error;
        m2o_last_error = M2O_ERROR_NONE;
        return last_error;
    }

// =======================================================================//
// !
// ! Plugins/events methods
// !
// =======================================================================//

    M2O_SERVER_API void m2o_event_trigger(m2o_event_type type, const m2o_args* args);
    M2O_SERVER_API void m2o_event_trigger_result(m2o_event_type type, const m2o_args* args, m2o_event_result *result);

    M2O_SERVER_API void m2o_args_init(m2o_args *arg) {
        zpl_array_init(arg->values, zpl_heap());
    }

    M2O_SERVER_API void m2o_args_free(m2o_args *arg) {
        zpl_array_free(arg->values);
    }

    M2O_SERVER_API usize m2o_args_size(m2o_args *arg) {
        return arg->size;
    }

    M2O_SERVER_API m2o_arg_value *m2o_args_get(m2o_args *arg, usize i) {
        ZPL_ASSERT(arg && i < zpl_array_count(arg->values));
        return &arg->values[i];
    }

    M2O_SERVER_API void m2o_args_push_string(m2o_args *arg, const char *string) {
        m2o_arg_value value = {M2O_ARG_STRING, 0}; {
            value.size = zpl_strlen(string);
            value.string = (char *)string;
        }

        zpl_array_append(arg->values, value);
        arg->size++;
    }

    M2O_SERVER_API void m2o_args_push_integer(m2o_args *arg, i64 integer) {
        m2o_arg_value value = {M2O_ARG_INTEGER, 0}; {
            value.integer = integer;
        }

        zpl_array_append(arg->values, value);
        arg->size++;
    }

    M2O_SERVER_API void m2o_args_push_real(m2o_args *arg, f64 real) {
        m2o_arg_value value = {M2O_ARG_REAL, 0}; {
            value.real = real;
        }

        zpl_array_append(arg->values, value);
        arg->size++;
    }

    M2O_SERVER_API void m2o_args_push_pointer(m2o_args *arg, void *pointer) {
        m2o_arg_value value = {M2O_ARG_POINTER, 0}; {
            value.pointer = pointer;
        }

        zpl_array_append(arg->values, value);
        arg->size++;
    }
