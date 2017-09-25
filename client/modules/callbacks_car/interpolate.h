#define MOD_ENTITY_KEEP_ALIVE 32

void module_car_callback_interpolate(librg_entity_t entity) {
    if (librg_entity_type(entity) != TYPE_CAR) return;

    auto transform   = librg_fetch_transform(entity);
    auto interpolate = librg_fetch_interpolate(entity);
    auto gamedata    = librg_fetch_gamedata(entity);

    // last delta tick against constant tick delay
    interpolate->delta += (mod.last_delta / 16.666f);
    // mod_log("%f\n", interpolate->delta);

    librg_assert(gamedata && gamedata->object);

    // position
    if (interpolate->lposition != interpolate->tposition || interpolate->step == MOD_ENTITY_KEEP_ALIVE) {
        vec3_t dposition;
        interpolate->step = 0;
        zplm_vec3_lerp(&dposition, interpolate->lposition, interpolate->tposition, interpolate->delta);
        gamedata->object->SetPosition(dposition);
    }

    // rotation
    if (interpolate->lrotation != interpolate->trotation || interpolate->step == MOD_ENTITY_KEEP_ALIVE) {
        auto last = interpolate->lrotation;
        auto dest = interpolate->trotation;

        quat_t drotation;
        zplm_quat_nlerp(&drotation, zplm_quat_dot(last, dest) < 0 ? -last : last, dest, interpolate->delta);
        gamedata->object->SetRotation(drotation);
    }

    interpolate->step++;
}
