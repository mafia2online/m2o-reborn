m2o_export u32 m2o_ped_create() {
    auto entity = librg_entity_create(ctx, TYPE_PED);
    entity->user_data = new ped_t();
    return entity->id;
}

m2o_export bool m2o_ped_destroy(u32 pedid) {
    M2O_FETCH_ENTITY(entity, pedid, false);
    delete (char *)entity->user_data;
    librg_entity_destroy(ctx, entity->id);
    return true;
}

m2o_export bool m2o_ped_position_set(u32 pedid, vec3_t position) {
    M2O_FETCH_ENTITY(entity, pedid, false);
    entity->position = position;
    return true;
}

m2o_export vec3_t m2o_ped_position_get(u32 pedid) {
    M2O_FETCH_ENTITY(entity, pedid, zplm_vec3f_zero());
    return entity->position;
}
