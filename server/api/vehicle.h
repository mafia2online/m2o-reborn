#define M2O_FETCH_ENTITY(dest, id, returned) \
    auto dest = librg_entity_fetch(ctx, id); \
    if (!dest) { m2o_set_last_error(M2O_UNDEFINED_ENTITY); return returned; }

m2o_export u32 m2o_vehicle_create() {
    auto vehicle = librg_entity_create(ctx, TYPE_CAR);
    vehicle->user_data = new car_t(vehicle);
    return vehicle->id;
}

m2o_export bool m2o_vehicle_destroy(u32 vehicleid) {
    M2O_FETCH_ENTITY(entity, vehicleid, false);
    delete (char *)entity->user_data;
    librg_entity_destroy(ctx, entity->id);
    return true;
}

m2o_export bool m2o_vehicle_position_set(u32 vehicleid, vec3_t position) {
    M2O_FETCH_ENTITY(entity, vehicleid, false);
    entity->position = position;
    return true;
}

m2o_export vec3_t m2o_vehicle_position_get(u32 vehicleid) {
    M2O_FETCH_ENTITY(entity, vehicleid, zplm_vec3_zero());
    return entity->position;
}
