void module_ped_callback_interpolate(librg_ctx_t *ctx, librg_entity_id id) {
    auto entity = librg_entity_fetch(ctx, id); mod_assert(entity);
    if (entity->type != TYPE_PED) return;

    auto ped = (ped_t *)(entity->user_data);

    // make sure we have all objects
    mod_assert(ped && ped->object);

    // last delta tick against constant tick delay
    ped->interpolate.delta += (mod.last_delta / 33.666f);
    //mod_log("%f\n", interpolate->delta);

    if (ped->stream.state != PED_ON_GROUND) return;

    vec3_t dposition;
    zplm_vec3_lerp(&dposition, ped->interpolate.lposition, ped->interpolate.tposition, ped->interpolate.delta);

    //if (dposition == interpolate->tposition) return;
    ped->object->SetPosition(dposition);
}
