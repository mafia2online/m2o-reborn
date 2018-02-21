m2o_plugin_vtable m2o_api_init() {
    m2o_plugin_vtable api;

    api.set_last_error = m2o_set_last_error;
    api.get_last_error = m2o_get_last_error;
    api.vehicle_create = m2o_vehicle_create;
    api.vehicle_destroy = m2o_vehicle_destroy;
    api.vehicle_position_set = m2o_vehicle_position_set;
    api.vehicle_position_get = m2o_vehicle_position_get;

    return api;
}