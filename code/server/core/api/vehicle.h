M2O_SERVER_API u32 m2o_vehicle_create(u16 model_id) {
    auto entity = librg_entity_create(ctx, M2O_ENTITY_CAR);
    auto car    = m2o_car_alloc(NULL);

    /* fill up default params */
    car->state = CAR_SPAWNED;
    car->model = zpl_clamp(0, model_id, 54 /* M2_CAR_MODELS */);

    entity->user_data = car;
    return entity->id;
}

M2O_SERVER_API int m2o_vehicle_destroy(u32 vehicleid) {
    M2O_FETCH_ENTITY(entity, vehicleid, false);
    m2o_car_free((m2o_car *)entity->user_data);
    librg_entity_destroy(ctx, entity->id);
    return 0;
}

// TODO: create a macro for simple api generation

M2O_SERVER_API vec3 m2o_vehicle_position_get(u32 vehicleid) {
    M2O_FETCH_ENTITY(entity, vehicleid, zplm_vec3f_zero());
    return entity->position;
}

M2O_SERVER_API int m2o_vehicle_position_set(u32 vehicleid, vec3 position) {
    M2O_FETCH_ENTITY(entity, vehicleid, false);
    entity->position = position;
    return 0;
}

M2O_SERVER_API vec3 m2o_vehicle_rotation_get(u32 vehicleid) {
    M2O_FETCH_ENTITY(entity, vehicleid, zplm_vec3f_zero());
    auto car = m2o_car_get(entity);
    return car->stream.rotation;
}

M2O_SERVER_API int m2o_vehicle_rotation_set(u32 vehicleid, vec3 rotation) {
    M2O_FETCH_ENTITY(entity, vehicleid, false);
    auto car = m2o_car_get(entity);
    car->stream.rotation = rotation;
    return 0;
}
