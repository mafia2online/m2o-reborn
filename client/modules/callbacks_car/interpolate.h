#define MOD_ENTITY_KEEP_ALIVE 1
#define MOD_ENTITY_POSITION_THRESHOLD 0.035

void module_car_callback_interpolate(librg_ctx_t *ctx, librg_entity_id id) {
    auto entity = librg_entity_fetch(ctx, id);
    mod_assert(entity);

    if (librg_entity_type(ctx, entity->id) != TYPE_CAR) return;

    auto car = (car_t *)entity->user_data;
    librg_assert(car && car->object);

    // last delta tick against constant tick delay
    car->interpolate.delta += (mod.last_delta / 16.666f);
    // mod_log("%f\n", interpolate->delta);

    auto curr_pos = car->object->GetPosition();
    auto diff_pos = curr_pos - entity->position;

    // TODO: make proper validation
    if (zpl_abs(diff_pos.x) < MOD_ENTITY_POSITION_THRESHOLD
     && zpl_abs(diff_pos.y) < MOD_ENTITY_POSITION_THRESHOLD) {
        return;
    }

    // position
    if (car->interpolate.lposition != car->interpolate.tposition) {
        vec3_t dposition;
        car->interpolate.step = 0;
        zplm_vec3_lerp(&dposition, car->interpolate.lposition, car->interpolate.tposition, car->interpolate.delta);
        car->object->SetPosition(dposition);
    }

    // rotation TODO:
    if (car->interpolate.lrotation != car->interpolate.trotation) {
        auto last = car->interpolate.lrotation;
        auto dest = car->interpolate.trotation;

        quat_t drotation;
        zplm_quat_nlerp(&drotation, zplm_quat_dot(last, dest) < 0 ? -last : last, dest, car->interpolate.delta);
        car->object->SetRotation(drotation);
    }

    car->interpolate.step++;
}
