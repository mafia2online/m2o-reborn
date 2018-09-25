M2O_SERVER_API u32 m2o_vehicle_create() {
    mod_log("creating a vehicle\n");
    auto entity = librg_entity_create(ctx, M2O_ENTITY_CAR);
    entity->user_data = m2o_car_alloc(NULL);
    return entity->id;
}

M2O_SERVER_API bool m2o_vehicle_destroy(u32 vehicleid) {
    M2O_FETCH_ENTITY(entity, vehicleid, false);
    m2o_car_free((m2o_car *)entity->user_data);
    librg_entity_destroy(ctx, entity->id);
    return true;
}

M2O_SERVER_API bool m2o_vehicle_position_set(u32 vehicleid, vec3_t position) {
    M2O_FETCH_ENTITY(entity, vehicleid, false);
    entity->position = position;
    return true;
}

M2O_SERVER_API vec3_t m2o_vehicle_position_get(u32 vehicleid) {
    M2O_FETCH_ENTITY(entity, vehicleid, zplm_vec3f_zero());
    return entity->position;
}
