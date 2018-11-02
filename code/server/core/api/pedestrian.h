M2O_SERVER_API u32 m2o_ped_create() {
    auto entity = librg_entity_create(ctx, M2O_ENTITY_DUMMY_PED);
    entity->user_data = m2o_ped_alloc(NULL);
    return entity->id;
}

M2O_SERVER_API bool m2o_ped_destroy(u32 pedid) {
    M2O_FETCH_ENTITY(entity, pedid, false);
    m2o_ped_free((m2o_ped *)entity->user_data);
    librg_entity_destroy(ctx, entity->id);
    return true;
}

M2O_SERVER_API bool m2o_ped_position_set(u32 pedid, vec3 position) {
    M2O_FETCH_ENTITY(entity, pedid, false);
    entity->position = position;
    return true;
}

M2O_SERVER_API vec3 m2o_ped_position_get(u32 pedid) {
    M2O_FETCH_ENTITY(entity, pedid, zplm_vec3f_zero());
    return entity->position;
}
