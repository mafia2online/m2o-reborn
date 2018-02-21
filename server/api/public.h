typedef void (api_set_last_error)(u32 error);
typedef u32 (api_get_last_error)();
typedef u32 (api_vehicle_create)();
typedef bool (api_vehicle_destroy)(u32 vehicleid);
typedef bool (api_vehicle_position_set)(u32 vehicleid, vec3_t position);
typedef vec3_t (api_vehicle_position_get)(u32 vehicleid);

typedef struct m2o_plugin_vtable {
    api_set_last_error *set_last_error;
    api_get_last_error *get_last_error;
    api_vehicle_create *vehicle_create;
    api_vehicle_destroy *vehicle_destroy;
    api_vehicle_position_set *vehicle_position_set;
    api_vehicle_position_get *vehicle_position_get;
} m2o_plugin_vtable;