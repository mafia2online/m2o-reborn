m2o_export u32 m2o_vehicle_create() {
    mod_log("creating a vehicle\n");
    auto entity = librg_entity_create(ctx, TYPE_CAR);
    entity->user_data = new car_t(entity);
    return entity->id;
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
