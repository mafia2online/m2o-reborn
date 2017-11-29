#define MOD_ENTITY_KEEP_ALIVE 1
#define MOD_ENTITY_POSITION_THRESHOLD 0.035

void module_car_callback_interpolate(librg_ctx_t *ctx, librg_entity_id id) {
    auto entity = librg_entity_fetch(ctx, id);
    mod_assert(entity);

    if (librg_entity_type(ctx, entity->id) != TYPE_CAR) return;

    auto ped = (ped_t *)entity->user_data;
    librg_assert(ped && ped->object);

    // last delta tick against constant tick delay
    ped->interpolate.delta += (mod.last_delta / 16.666f);
    // mod_log("%f\n", interpolate->delta);

    auto curr_pos = ped->object->GetPosition();
    auto diff_pos = curr_pos - entity->position;

    print_posm(diff_pos, "curr diff");

    // TODO: make proper validation
    if (zpl_abs(diff_pos.x) < MOD_ENTITY_POSITION_THRESHOLD
     && zpl_abs(diff_pos.y) < MOD_ENTITY_POSITION_THRESHOLD) {
        return;
    }

    // position
    if (ped->interpolate.lposition != ped->interpolate.tposition) {
        vec3_t dposition;
        ped->interpolate.step = 0;
        zplm_vec3_lerp(&dposition, ped->interpolate.lposition, ped->interpolate.tposition, ped->interpolate.delta);
        ped->object->SetPosition(dposition);
    }

    // rotation TODO:
    if (ped->interpolate.lrotation != ped->interpolate.trotation) {
        auto last = ped->interpolate.lrotation;
        auto dest = ped->interpolate.trotation;

        quat_t drotation;
        zplm_quat_nlerp(&drotation, zplm_quat_dot(last, dest) < 0 ? -last : last, dest, ped->interpolate.delta);
        ped->object->SetRotation(drotation);
    }

    ped->interpolate.step++;
}
