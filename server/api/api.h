void m2o_set_last_error(u32 error);
u32 m2o_get_last_error();

u32 m2o_vehicle_create();
bool m2o_vehicle_destroy(u32 vehicleid);
bool m2o_vehicle_position_set(u32 vehicleid, vec3_t position);
vec3_t m2o_vehicle_position_get(u32 vehicleid);

typedef void (api_set_last_error)(u32 error);
typedef u32 (api_get_last_error)();
typedef u32 (api_vehicle_create)();
typedef bool (api_vehicle_destroy)(u32 vehicleid);
typedef bool (api_vehicle_position_set)(u32 vehicleid, vec3_t position);
typedef vec3_t (api_vehicle_position_get)(u32 vehicleid);

typedef struct m2o_api_vtable {
    api_set_last_error *set_last_error;
    api_get_last_error *get_last_error;
    api_vehicle_create *vehicle_create;
    api_vehicle_destroy *vehicle_destroy;
    api_vehicle_position_set *vehicle_position_set;
    api_vehicle_position_get *vehicle_position_get;
} m2o_api_vtable;

m2o_api_vtable m2o_api_init() {
    m2o_api_vtable api;

    api.set_last_error = m2o_set_last_error;
    api.get_last_error = m2o_get_last_error;
    api.vehicle_create = m2o_vehicle_create;
    api.vehicle_destroy = m2o_vehicle_destroy;
    api.vehicle_position_set = m2o_vehicle_position_set;
    api.vehicle_position_get = m2o_vehicle_position_get;

    return api;
}