#define valid_dir(x) (zplm_abs(x) > 0.0f && zplm_abs(x) < 1.0f)

void module_ped_callback_clientstream(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_PED) return;

    auto transform = librg_fetch_transform(event->entity);
    auto gamedata = librg_fetch_gamedata(event->entity);
    auto ped = librg_fetch_ped(event->entity);

    // make sure we have all objects
    mod_assert(ped && gamedata && gamedata->object);

    // read new values of entity
    auto movestate = ((M2::C_Player2*)gamedata->object)->m_pPlayerControls.m_ePlayerMovementState;
    auto new_position = gamedata->object->GetPosition();
    auto diff_position = new_position - transform->position;
    transform->position = new_position;

    // convert player movement to human movement
    switch (movestate) {
        case M2::E_MOVEMENT_WALK:     ped->move_state = M2::HUMAN_MOVE_MODE_WALK; break;
        case M2::E_MOVEMENT_JOG:      ped->move_state = M2::HUMAN_MOVE_MODE_RUN; break;
        case M2::E_MOVEMENT_SPRINT:   ped->move_state = M2::HUMAN_MOVE_MODE_SPRINT; break;
        case M2::E_MOVEMENT_IDLE:     ped->move_state = M2::HUMAN_MOVE_MODE_BREATH; break;
        case M2::E_MOVEMENT_STOPPING: ped->move_state = M2::HUMAN_MOVE_MODE_END; break;
    }

    f32 ped_speed = zplm_vec3_mag(diff_position);
    ped->is_accelerating = (ped_speed > ped->speed);
    ped->speed = ped_speed;

    // assign and send new values
    vec3_t newdir; zplm_vec3_norm0(&newdir, diff_position);
    if ((valid_dir(newdir.x) || valid_dir(newdir.y)) && ped->is_accelerating) {
        ped->direction = (ped->direction + newdir) * 0.5f; // calc average
    }

    // write last valid direction
    librg_data_wptr(event->data, ped, sizeof(ped_t));
}

void module_ped_callback_update(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_PED) return;

    auto interpolate = librg_fetch_interpolate(event->entity);
    auto transform = librg_fetch_transform(event->entity);
    auto gamedata  = librg_fetch_gamedata(event->entity);
    auto ped       = librg_fetch_ped(event->entity);

    // make sure we have all objects
    librg_assert(ped && gamedata && gamedata->object);

    // interpolation stuff
    if (interpolate) {
        interpolate->lposition = interpolate->tposition;
        interpolate->lrotation = interpolate->trotation;

        interpolate->tposition = transform->position;
        interpolate->trotation = transform->rotation;

        interpolate->delta = 0.0f;
    }

    librg_data_rptr(event->data, ped, sizeof(ped_t));

    // apply movement anim
    {
        auto extr_shift = ped->direction * 3.0f; /* create extrapolated shift for ped */
        auto targ_pos = transform->position + extr_shift;

        targ_pos.z = transform->position.z;
        gamedata->object->SetDirection(vec3(ped->direction.x, ped->direction.y, 0.0f));

        M2::C_SyncObject *pSyncObject = nullptr;
        ((M2::C_Human2*)gamedata->object)->GetScript()->ScrMoveV(
            &pSyncObject, targ_pos, (M2::eHumanMoveMode)ped->move_state, vec3(ped->direction.x, ped->direction.y, 0.0f), true
        );
    }
}
