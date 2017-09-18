void module_ped_callback_clientstream(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_PED) return;

    auto transform = librg_fetch_transform(event->entity);
    auto gamedata  = librg_fetch_gamedata(event->entity);

    transform->position = gamedata->object->GetPosition();
    transform->rotation = gamedata->object->GetRotation();

    zplm_vec3_t direction;// = ((M2::C_Human2 *)gamedata->object)->GetDirection();
    Mem::InvokeFunction<Mem::call_this, void>(gamedata->object->m_pVFTable->GetDirection, (M2::C_Player2 *)gamedata->object, &direction);
    direction.x = 0.0f;

    // mod_log("%f\n", direction.w);
    librg_data_wptr(event->data, &direction, sizeof(direction));
}

#define zplm_sign(x) ((x) >= 0 ? (x) == 0 ? 0 : 1 : -1)

void module_ped_callback_update(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_PED) return;

    auto transform = librg_fetch_transform(event->entity);
    auto gamedata  = librg_fetch_gamedata(event->entity);
    auto interpolate = librg_fetch_interpolate(event->entity);

    librg_assert(gamedata && gamedata->object);

    vec3_t direction;
    librg_data_rptr(event->data, &direction, sizeof(direction));

    // if this car is not interpolable
    if (!interpolate) return;

    interpolate->lposition = interpolate->tposition;
    interpolate->lrotation = interpolate->trotation;

    interpolate->tposition = transform->position;
    interpolate->trotation = transform->rotation;

    interpolate->delta = 0.0f;

    // apply movement anim
    {
        // get direction
        vec3_t dir = interpolate->tposition - interpolate->lposition;

        //print_posm(dir, "going to");
        // round it up
        //dir.x = zplm_sign(dir.x);
        //dir.y = zplm_sign(dir.y);
        const float coef = 100.0f;
        dir.x = zplm_clamp(dir.x * 1000.0f, -coef, coef);
        dir.y = zplm_clamp(dir.y * 1000.0f, -coef, coef);
        dir.z = 0.0f;

        

        // skip next steps
        if (dir.x == 0.0f && dir.y == 0.0f) {
            if (interpolate->step++ > 5) {
                interpolate->step = 0;

                mod_log("stopping player\n");
                auto ped = ((M2::C_Human2*)gamedata->object);

                M2::C_SyncObject *pSyncObject = nullptr;
                ped->GetScript()->ScrMoveV(&pSyncObject, interpolate->tposition, M2::HUMAN_MOVE_MODE_END, interpolate->tposition, true);

                return;
            }
        }

        vec3_t ext; // extrapolated value
        zplm_vec3_mul(&ext, dir, 15.0f);

        vec3_t tar = ext + transform->position;

        auto ped = ((M2::C_Human2*)gamedata->object);
        M2::C_SyncObject *pSyncObject = nullptr;
        ped->GetScript()->ScrMoveV(&pSyncObject, tar, M2::HUMAN_MOVE_MODE_RUN, tar, true);
    }

}
