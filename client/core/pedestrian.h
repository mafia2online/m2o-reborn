// =======================================================================//
// !
// ! Generic entity events
// !
// =======================================================================//

/**
 * The entity enters the stream zone
 */
void module_ped_callback_create(librg_event_t *event) {
    auto ped = new ped_t();

    ped->model = librg_data_ru16(event->data);
    ped->state = librg_data_ru8(event->data);

    // state on the moment player is created
    if (ped->state == PED_IN_CAR || ped->state == PED_ENTERING_CAR) {
        ped->state  = PED_ENTERING_CAR;
        ped->seat   = librg_data_ru8(event->data);
        ped->target_entityid = librg_data_ru32(event->data);
    }

    event->entity->flags |= MOD_ENTITY_INTERPOLATED;
    event->entity->user_data = ped;

    M2::C_Entity *entity = M2::Wrappers::CreateEntity(M2::eEntityType::MOD_ENTITY_PED, ped->model);

    if (entity->IsActive()) {
        print_posm(event->entity->position, "[info] creating ped at");
        entity->SetPosition(event->entity->position);

        ped->CEntity = entity;
    }
}

/**
 * The entity exists the stream zone
 */
void module_ped_callback_remove(librg_event_t *event) {
    if (event->entity->id == mod.player->id) return;
    mod_log("destroying entity %d\n", event->entity->id);

    auto ped = get_ped(event->entity); mod_assert(ped && ped->CEntity);
    M2::Wrappers::DestroyEntity(ped->CEntity);

    delete ped;
}

// =======================================================================//
// !
// ! Update/sync entity events
// !
// =======================================================================//

void module_ped_tasks_update(ped_t *ped) {
    if (ped->state == PED_ENTERING_CAR && ped->target_entityid != MOD_INVALID_ENTITY) {
        if (librg_entity_valid(ctx, ped->target_entityid)) {
            mod_log("[info] found a valid car, trying to put player ped in");

            ped->vehicle = librg_entity_fetch(ctx, ped->target_entityid);
            auto car = get_car(ped->vehicle);

            // TODO: add seat sync
            M2::C_SyncObject *pSyncObject = nullptr;
            ((M2::C_Human2 *)ped->CEntity)->GetScript()->ScrDoAction(
                &pSyncObject, (M2::C_Vehicle *)car->CEntity,
                true, (M2::E_VehicleSeat)ped->seat, false
            );

            ped->target_entityid = MOD_INVALID_ENTITY;
        }
    }
}

/**
 * The entity in our stream zone gets updated
 */
void module_ped_callback_update(librg_event_t *event) {
    auto entity = event->entity; mod_assert(entity);
    auto ped = get_ped(event->entity);

    // make sure we have all objects
    mod_assert(ped && ped->CEntity);
    librg_data_rptr(event->data, &ped->stream, sizeof(ped->stream));

    /* update interpolation tables */
    cubic_hermite_v3_value(&ped->inter_pos, event->entity->position);

    // ped->inter_pos.targ = event->entity->position;
    // ped->inter_pos.last_speed = ped->inter_pos.targ_speed;
    // ped->inter_pos.targ_speed = ped->stream.vspeed;
    // ped->inter_rot.last = ped->inter_rot.targ;
    // ped->inter_rot.targ = ped->stream.rotation;


    // ped->CHuman->GetScript()->ScrLookAt(
    //     &ped->sync, nullptr, ped->stream.look_at, true
    // );

    // // apply movement anim
    // if (ped->stream.state == PED_ON_GROUND) {
    //     auto extr_shift = ped->stream.direction * 13.0f; /* create extrapolated shift for ped */
    //     auto targ_pos = entity->position + extr_shift;

    //     targ_pos.z = entity->position.z;
    //     ped->CHuman->SetDir(vec3(ped->stream.direction.x, ped->stream.direction.y, 0.0f));
    //     // return;

    //     //if (interpolate->step++ > 10) {
    //         interpolate->step = 0;

    //         return;
    //         if (ped->sync) {
    //             ped->sync->Done();
    //         }
    //         ped->CHuman->GetScript()->ScrMoveV(
    //             &ped->sync, targ_pos, (M2::eHumanMoveMode)ped->stream.move_state,
    //             vec3(ped->stream.direction.x, ped->stream.direction.y, 0.0f), true
    //         );
    //     //}
    // } else {
    //     // if we are not on the ground
    //     // our prev position will be always our current
    //     // so that after we appear on the ground again
    //     // we won't be interpolated from somewhere far away
    //     interpolate->last_position = entity->position;
    // }

    ped->inter_delta = 0.0f;

    module_ped_tasks_update(ped);
}

#define valid_dir(x) (zplm_abs(x) > 0.0f && zplm_abs(x) < 1.0f)

/**
 * The entity is streamed by us, and we are sending an update with new data
 */
void module_ped_callback_clientstream(librg_event_t *event) {
    auto entity = event->entity; mod_assert(entity);
    auto ped = get_ped(event->entity);

    // make sure we have all objects
    mod_assert(ped && ped->CEntity);

    // TODO: add checks for being on the ground

    // read new values of entity
    auto new_position   = ped->CEntity->GetPosition();
    auto diff_position  = new_position - entity->position;

    entity->position    = new_position;
    ped->stream.vspeed = diff_position;

    // if (event->entity->id != mod.player->id) {
    //     // lower limits
    //     // 0.05 - 0.06 - walking
    //     // 0.13 - 0.14 - running
    //     // 0.19 - 0.20 - sprinting
    //     f32 ped_speed = zplm_vec3_mag(diff_position);

    //     /**/ if (ped_speed > 0.19f) {
    //         ped->stream.move_state = (u8)M2::HUMAN_MOVE_MODE_SPRINT;
    //     }
    //     else if (ped_speed > 0.13f) {
    //         ped->stream.move_state = (u8)M2::HUMAN_MOVE_MODE_RUN;
    //     }
    //     else if (ped_speed > 0.01f) {
    //         ped->stream.move_state = (u8)M2::HUMAN_MOVE_MODE_WALK;
    //     }
    //     else {
    //         ped->stream.move_state = (u8)M2::HUMAN_MOVE_MODE_BREATH;
    //     }
    // }
    // else {
    //     auto movestate = ped->CPlayer->m_pPlayerControls.m_ePlayerMovementState;

    //     // convert local player movement to human movement
    //     switch (movestate) {
    //         case M2::E_MOVEMENT_WALK:     ped->stream.move_state = M2::HUMAN_MOVE_MODE_WALK; break;
    //         case M2::E_MOVEMENT_JOG:      ped->stream.move_state = M2::HUMAN_MOVE_MODE_RUN; break;
    //         case M2::E_MOVEMENT_SPRINT:   ped->stream.move_state = M2::HUMAN_MOVE_MODE_SPRINT; break;
    //         case M2::E_MOVEMENT_IDLE:     ped->stream.move_state = M2::HUMAN_MOVE_MODE_BREATH; break;
    //         case M2::E_MOVEMENT_STOPPING: ped->stream.move_state = M2::HUMAN_MOVE_MODE_END; break;
    //     }
    // }

    // f32 ped_speed = zplm_vec3_mag(diff_position);
    // ped->stream.is_accelerating = (ped_speed > ped->stream.speed);
    // ped->stream.speed = ped_speed;

    // // assign and send new values
    // vec3_t newdir; zplm_vec3_norm0(&newdir, diff_position);
    // if ((valid_dir(newdir.x) || valid_dir(newdir.y)) && ped->stream.is_accelerating) {
    //     ped->stream.direction = (ped->stream.direction + newdir) * 0.5f; // calc average
    // }

    librg_data_wptr(event->data, &ped->stream, sizeof(ped->stream));
}

// =======================================================================//
// !
// ! Interpolation
// !
// =======================================================================//

void module_ped_callback_interpolate(librg_entity_t *entity) {
    auto ped = get_ped(entity); mod_assert(ped && ped->CEntity);

    // last delta tick against constant tick delay
    f32 alpha = ped->inter_delta / (f32)MOD_SERVER_TICK_DELAY;
    ped->inter_delta += mod.last_delta;

    if (ped->stream.state != PED_ON_GROUND) {
        return;
    }

    ped->CHuman->SetPos(cubic_hermite_v3_interpolate(&ped->inter_pos, alpha));
}

// =======================================================================//
// !
// ! Custom events
// !
// =======================================================================//

void module_ped_init() {

}
