void module_car_callback_interpolate(librg_entity_t entity) {
    auto transform   = librg_fetch_transform(entity);
    auto interpolate = librg_fetch_interpolate(entity);
    auto gamedata    = librg_fetch_gamedata(entity);

    // last delta tick against constant tick delay
    interpolate->delta += (mod.last_delta / 16.666f);
    // mod_log("%f\n", interpolate->delta);

    librg_assert(gamedata && gamedata->object);

    vec3_t dposition;
    zplm_vec3_lerp(&dposition, interpolate->lposition, interpolate->tposition, interpolate->delta);
    gamedata->object->SetPosition(dposition);

    auto last = interpolate->lrotation;
    auto dest = interpolate->trotation;

    if (last == dest) return;

    quat_t drotation;
    zplm_quat_nlerp(&drotation, zplm_quat_dot(last, dest) < 0 ? -last : last, dest, interpolate->delta);
    gamedata->object->SetRotation(drotation);
}
