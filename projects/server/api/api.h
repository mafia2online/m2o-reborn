/* Mafia 2 Online Server Interal API header */
/* Generated on Tue Sep 25 2018 11:48:58 GMT+0300 (EEST) */
void m2o_error_set(m2o_error error);
m2o_error m2o_error_get();
void m2o_event_trigger(m2o_event_type type, const m2o_args* args);;
void m2o_args_init(m2o_args *arg);
void m2o_args_free(m2o_args *arg);
usize m2o_args_size(m2o_args *arg);
m2o_arg_value *m2o_args_get(m2o_args *arg, usize i);
void m2o_args_push_string(m2o_args *arg, const char *string);
void m2o_args_push_integer(m2o_args *arg, i64 integer);
void m2o_args_push_real(m2o_args *arg, f64 real);
void m2o_args_push_pointer(m2o_args *arg, void *pointer);

typedef void (m2o_api_error_set)(m2o_error error);
typedef m2o_error (m2o_api_error_get)();
typedef void (m2o_api_event_trigger)(m2o_event_type type, const m2o_args* args);;
typedef void (m2o_api_args_init)(m2o_args *arg);
typedef void (m2o_api_args_free)(m2o_args *arg);
typedef usize (m2o_api_args_size)(m2o_args *arg);
typedef m2o_arg_value *(m2o_api_args_get)(m2o_args *arg, usize i);
typedef void (m2o_api_args_push_string)(m2o_args *arg, const char *string);
typedef void (m2o_api_args_push_integer)(m2o_args *arg, i64 integer);
typedef void (m2o_api_args_push_real)(m2o_args *arg, f64 real);
typedef void (m2o_api_args_push_pointer)(m2o_args *arg, void *pointer);

typedef struct m2o_api_vtable {
    m2o_api_error_set *error_set;
    m2o_api_error_get *error_get;
    m2o_api_event_trigger *event_trigger;
    m2o_api_args_init *args_init;
    m2o_api_args_free *args_free;
    m2o_api_args_size *args_size;
    m2o_api_args_get *args_get;
    m2o_api_args_push_string *args_push_string;
    m2o_api_args_push_integer *args_push_integer;
    m2o_api_args_push_real *args_push_real;
    m2o_api_args_push_pointer *args_push_pointer;
} m2o_api_vtable;

void m2o_api_init(m2o_api_vtable *api) {
    api->error_set = m2o_error_set;
    api->error_get = m2o_error_get;
    api->event_trigger = m2o_event_trigger;
    api->args_init = m2o_args_init;
    api->args_free = m2o_args_free;
    api->args_size = m2o_args_size;
    api->args_get = m2o_args_get;
    api->args_push_string = m2o_args_push_string;
    api->args_push_integer = m2o_args_push_integer;
    api->args_push_real = m2o_args_push_real;
    api->args_push_pointer = m2o_args_push_pointer;
}