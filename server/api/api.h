void m2o_set_last_error(u32 error);
u32 m2o_get_last_error();

u32 m2o_vehicle_create();
bool m2o_vehicle_destroy(u32 vehicleid);
bool m2o_vehicle_position_set(u32 vehicleid, vec3_t position);
vec3_t m2o_vehicle_position_get(u32 vehicleid);