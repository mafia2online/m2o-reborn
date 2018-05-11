

void m2o_set_last_error(m2o_error error);
m2o_error m2o_get_last_error();

void m2o_args_init(m2o_args *arg);
void m2o_args_free(m2o_args *arg);
usize m2o_args_size(m2o_args *arg);
m2o_arg_value *m2o_args_get(m2o_args *arg, usize i);
void m2o_args_push_string(m2o_args *arg, const char *string);
void m2o_args_push_integer(m2o_args *arg, i64 integer);
void m2o_args_push_real(m2o_args *arg, f64 real);
void m2o_args_push_pointer(m2o_args *arg, void *pointer);

void m2o_event_trigger(const char* name, const m2o_args* args);;

u32 m2o_vehicle_create();
bool m2o_vehicle_destroy(u32 vehicleid);
bool m2o_vehicle_position_set(u32 vehicleid, vec3_t position);
vec3_t m2o_vehicle_position_get(u32 vehicleid);

u32 m2o_ped_create();
bool m2o_ped_destroy(u32 pedid);
bool m2o_ped_position_set(u32 pedid, vec3_t position);
vec3_t m2o_ped_position_get(u32 pedid);

typedef void (m2o_api_set_last_error)(m2o_error error);
typedef m2o_error (m2o_api_get_last_error)();
typedef void (m2o_api_args_init)(m2o_args *arg);
typedef void (m2o_api_args_free)(m2o_args *arg);
typedef usize (m2o_api_args_size)(m2o_args *arg);
typedef m2o_arg_value *(m2o_api_args_get)(m2o_args *arg, usize i);
typedef void (m2o_api_args_push_string)(m2o_args *arg, const char *string);
typedef void (m2o_api_args_push_integer)(m2o_args *arg, i64 integer);
typedef void (m2o_api_args_push_real)(m2o_args *arg, f64 real);
typedef void (m2o_api_args_push_pointer)(m2o_args *arg, void *pointer);
typedef void (m2o_api_event_trigger)(const char* name, const m2o_args* args);;
typedef u32 (m2o_api_vehicle_create)();
typedef bool (m2o_api_vehicle_destroy)(u32 vehicleid);
typedef bool (m2o_api_vehicle_position_set)(u32 vehicleid, vec3_t position);
typedef vec3_t (m2o_api_vehicle_position_get)(u32 vehicleid);
typedef u32 (m2o_api_ped_create)();
typedef bool (m2o_api_ped_destroy)(u32 pedid);
typedef bool (m2o_api_ped_position_set)(u32 pedid, vec3_t position);
typedef vec3_t (m2o_api_ped_position_get)(u32 pedid);

typedef struct m2o_api_vtable {
    m2o_api_set_last_error *set_last_error;
    m2o_api_get_last_error *get_last_error;
    m2o_api_args_init *args_init;
    m2o_api_args_free *args_free;
    m2o_api_args_size *args_size;
    m2o_api_args_get *args_get;
    m2o_api_args_push_string *args_push_string;
    m2o_api_args_push_integer *args_push_integer;
    m2o_api_args_push_real *args_push_real;
    m2o_api_args_push_pointer *args_push_pointer;
    m2o_api_event_trigger *event_trigger;
    m2o_api_vehicle_create *vehicle_create;
    m2o_api_vehicle_destroy *vehicle_destroy;
    m2o_api_vehicle_position_set *vehicle_position_set;
    m2o_api_vehicle_position_get *vehicle_position_get;
    m2o_api_ped_create *ped_create;
    m2o_api_ped_destroy *ped_destroy;
    m2o_api_ped_position_set *ped_position_set;
    m2o_api_ped_position_get *ped_position_get;
} m2o_api_vtable;

void m2o_api_init(m2o_api_vtable *api) {
    api->set_last_error = m2o_set_last_error;
    api->get_last_error = m2o_get_last_error;
    api->args_init = m2o_args_init;
    api->args_free = m2o_args_free;
    api->args_size = m2o_args_size;
    api->args_get = m2o_args_get;
    api->args_push_string = m2o_args_push_string;
    api->args_push_integer = m2o_args_push_integer;
    api->args_push_real = m2o_args_push_real;
    api->args_push_pointer = m2o_args_push_pointer;
    api->event_trigger = m2o_event_trigger;
    api->vehicle_create = m2o_vehicle_create;
    api->vehicle_destroy = m2o_vehicle_destroy;
    api->vehicle_position_set = m2o_vehicle_position_set;
    api->vehicle_position_get = m2o_vehicle_position_get;
    api->ped_create = m2o_ped_create;
    api->ped_destroy = m2o_ped_destroy;
    api->ped_position_set = m2o_ped_position_set;
    api->ped_position_get = m2o_ped_position_get;
}