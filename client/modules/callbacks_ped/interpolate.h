void module_ped_callback_interpolate(librg_entity_t entity) {
    if (librg_entity_type(entity) != TYPE_PED) return;

    auto transform   = librg_fetch_transform(entity);
    auto interpolate = librg_fetch_interpolate(entity);
    auto gamedata    = librg_fetch_gamedata(entity);
    auto ped         = librg_fetch_ped(entity);

    // last delta tick against constant tick delay
    interpolate->delta += (mod.last_delta / 33.666f);
    //mod_log("%f\n", interpolate->delta);

    librg_assert(ped && gamedata && gamedata->object);

    vec3_t dposition;
    zplm_vec3_lerp(&dposition, interpolate->lposition, interpolate->tposition, interpolate->delta);

    //if (dposition == interpolate->tposition) return;
    gamedata->object->SetPosition(dposition);
}
