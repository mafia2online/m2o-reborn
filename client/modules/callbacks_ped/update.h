#define valid_dir(x) (zplm_abs(x) > 0.0f && zplm_abs(x) < 1.0f)

void module_ped_callback_clientstream(librg_event_t *event) {
    if (event->entity->type != TYPE_PED) return;

    auto entity = event->entity; mod_assert(entity);
    auto ped = (ped_t *)(event->entity->user_data);

    // make sure we have all objects
    mod_assert(ped && ped->object);

    // read new values of entity
    auto new_position = ped->object->GetPosition();
    auto diff_position = new_position - entity->position;
    entity->position = new_position;

    if (event->entity != mod.player) {
        // lower limits
        // 0.05 - 0.06 - walking
        // 0.13 - 0.14 - running
        // 0.19 - 0.20 - sprinting
        f32 ped_speed = zplm_vec3_mag(diff_position);

        /**/ if (ped_speed > 0.19f) {
            ped->stream.move_state = (u8)M2::HUMAN_MOVE_MODE_SPRINT;
        }
        else if (ped_speed > 0.13f) {
            ped->stream.move_state = (u8)M2::HUMAN_MOVE_MODE_RUN;
        }
        else if (ped_speed > 0.01f) {
            ped->stream.move_state = (u8)M2::HUMAN_MOVE_MODE_WALK;
        }
        else {
            ped->stream.move_state = (u8)M2::HUMAN_MOVE_MODE_BREATH;
        }
    }
    else {
        auto movestate = ((M2::C_Player2*)ped->object)->m_pPlayerControls.m_ePlayerMovementState;

        // convert local player movement to human movement
        switch (movestate) {
            case M2::E_MOVEMENT_WALK:     ped->stream.move_state = M2::HUMAN_MOVE_MODE_WALK; break;
            case M2::E_MOVEMENT_JOG:      ped->stream.move_state = M2::HUMAN_MOVE_MODE_RUN; break;
            case M2::E_MOVEMENT_SPRINT:   ped->stream.move_state = M2::HUMAN_MOVE_MODE_SPRINT; break;
            case M2::E_MOVEMENT_IDLE:     ped->stream.move_state = M2::HUMAN_MOVE_MODE_BREATH; break;
            case M2::E_MOVEMENT_STOPPING: ped->stream.move_state = M2::HUMAN_MOVE_MODE_END; break;
        }
    }

    f32 ped_speed = zplm_vec3_mag(diff_position);
    ped->stream.is_accelerating = (ped_speed > ped->stream.speed);
    ped->stream.speed = ped_speed;

    // assign and send new values
    vec3_t newdir; zplm_vec3_norm0(&newdir, diff_position);
    if ((valid_dir(newdir.x) || valid_dir(newdir.y)) && ped->stream.is_accelerating) {
        ped->stream.direction = (ped->stream.direction + newdir) * 0.5f; // calc average
    }

    // write last valid direction
    librg_data_wptr(event->data, ped, sizeof(ped_t));
}

void module_ped_callback_update(librg_event_t *event) {
    if (event->entity->type != TYPE_PED) return;

    auto entity = event->entity; mod_assert(entity);
    auto ped = (ped_t *)(event->entity->user_data);

    // make sure we have all objects
    mod_assert(ped && ped->object);

    interpolate_t *interpolate = &ped->interpolate;

    // interpolation stuff
    interpolate->lposition = interpolate->tposition;
    interpolate->tposition = entity->position;
    interpolate->delta = 0.0f;
    
    librg_data_rptr(event->data, ped, sizeof(ped_t));

    // apply movement anim
    if (ped->stream.state == PED_ON_GROUND) {
        auto extr_shift = ped->stream.direction * 3.0f; /* create extrapolated shift for ped */
        auto targ_pos = entity->position + extr_shift;

        targ_pos.z = entity->position.z;
        ped->object->SetDirection(vec3(ped->stream.direction.x, ped->stream.direction.y, 0.0f));

        M2::C_SyncObject *pSyncObject = nullptr;
        ((M2::C_Human2*)ped->object)->GetScript()->ScrMoveV(
            &pSyncObject, targ_pos, (M2::eHumanMoveMode)ped->stream.move_state,
            vec3(ped->stream.direction.x, ped->stream.direction.y, 0.0f), true
        );
    }
}
