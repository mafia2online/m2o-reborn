// =======================================================================//
// !
// ! Generic entity events
// !
// =======================================================================//

/**
 * The entity enters the stream zone
 */
void m2o_callback_ped_create(librg_event_t *event) {
    auto ped = m2o_ped_alloc(NULL);

    ped->model = librg_data_ru16(event->data);
    ped->state = (ped_state)librg_data_ru8(event->data);

    auto namelen = librg_data_ru8(event->data);
    librg_data_rptr(event->data, ped->name, namelen);

    /* handle state specific casese*/
    switch (ped->state) {
        case PED_ON_GROUND: break;
        case PED_IN_CAR: {
            ped->vehicle = librg_data_ru32(event->data);
            ped->seat    = librg_data_ru8(event->data);
        } break;
    }

    event->entity->flags |= M2O_ENTITY_INTERPOLATED;
    event->entity->user_data = ped;

    M2::C_Entity *entity = M2::Wrappers::CreateEntity(M2::eEntityType::MOD_ENTITY_PED, ped->model);

    if (entity->IsActive()) {
        print_posm(event->entity->position, "[info] creating ped at");
        ped->CEntity = entity;
        ped->CEntity->SetPosition(event->entity->position);

        /* remote peds need can be killed only from remote client side */
        ped->CHuman->GetScript()->SetDemigod(true);
    } else {
        mod_log("[warning] could not create a ped for entity: %d\n", event->entity->id);
    }
}

/**
 * The entity exists the stream zone
 */
void m2o_callback_ped_remove(librg_event_t *event) {
    // TODO: check if still occurs - and remove
    if (event->entity->id == mod.player->id) {
        mod_assert_msg(false, "m2o_callback_ped_remove: event->entity->id == mod.player->id");
        return;
    }

    mod_log("destroying ped entity %d\n", event->entity->id);
    auto ped = m2o_ped_get(event->entity);

    if (ped->CEntity) { /* maybe it was deleted when we removed car from streamzone */
        M2::Wrappers::DestroyEntity(ped->CEntity, M2::eEntityType::MOD_ENTITY_PED);
        ped->CEntity = NULL;
    }

    m2o_ped_free(ped);
}

// =======================================================================//
// !
// ! Update/sync entity events
// !
// =======================================================================//

// void module_ped_tasks_update(ped_t *ped) {
//     if (ped->state == PED_ENTERING_CAR && ped->target_entityid != M2O_INVALID_ENTITY) {
//         if (librg_entity_valid(ctx, ped->target_entityid)) {
//             mod_log("[info] found a valid car, trying to put player ped in");

//             ped->vehicle = librg_entity_fetch(ctx, ped->target_entityid);
//             auto car = get_car(ped->vehicle);

//             M2::C_SyncObject *pSyncObject = nullptr;
//             ((M2::C_Human2 *)ped->CEntity)->GetScript()->ScrDoAction(
//                 &pSyncObject, (M2::C_Vehicle *)car->CEntity,
//                 true, (M2::E_VehicleSeat)ped->seat, false
//             );

//             ped->target_entityid = M2O_INVALID_ENTITY;
//         }
//     }
// }

/**
 * The entity in our stream zone gets updated
 */
void m2o_callback_ped_update(librg_event_t *event) {
    auto entity = event->entity;
    auto ped    = m2o_ped_get(entity);

    // skip the udpate if we have removed ped cuz he was inside a removed car
    if (!ped->CEntity) return;

    // read up the dynaymicly changed data
    librg_data_rptr(event->data, &ped->stream, sizeof(ped->stream));

    // apply movement anim
    if (ped->state == PED_ON_GROUND) {
        auto pos_diff  = zplm_vec3_mag2(ped->interp.last - entity->position);
        auto real_diff = zplm_vec3_mag2(ped->CEntity->GetPosition() - entity->position);

        if (pos_diff > zplm_square(0.05f)) {
            ped->CHuman->GetScript()->ScrMoveV(
                &ped->sync, entity->position, (M2::eHumanMoveMode)ped->stream.move,
                vec3f(ped->stream.dirx, ped->stream.diry, 0.0f), true
            );
        }

        if (real_diff > zplm_cube(2.5f)) {
            ped->CHuman->SetPos(entity->position);
        }
    }

    ped->interp.last = entity->position;

    // module_ped_tasks_update(ped);
}

/**
 * The entity is streamed by us, and we are sending an update with new data
 */
void m2o_callback_ped_clientstream(librg_event_t *event) {
    auto entity = event->entity;
    auto ped    = m2o_ped_get(entity);

    /* if ped is local player, get movement type */
    /* else try to calculate it from pos difference */
    if (event->entity->id != mod.player->id) {
        // lower limits
        // 0.05 - 0.06 - walking
        // 0.13 - 0.14 - running
        // 0.19 - 0.20 - sprinting

        auto pos_differ = ped->CEntity->GetPosition() - entity->position;
        auto ped_speed  = zplm_vec3_mag2(pos_differ);

        /**/ if (ped_speed > zplm_square(0.19f)) {
            ped->stream.move = (u8)M2::HUMAN_MOVE_MODE_SPRINT;
        }
        else if (ped_speed > zplm_square(0.13f)) {
            ped->stream.move = (u8)M2::HUMAN_MOVE_MODE_RUN;
        }
        else if (ped_speed > zplm_square(0.01f)) {
            ped->stream.move = (u8)M2::HUMAN_MOVE_MODE_WALK;
        }
        else {
            ped->stream.move = (u8)M2::HUMAN_MOVE_MODE_STEP;
        }
    }
    else {
        auto movestate = ped->CPlayer->m_pPlayerControls.m_ePlayerMovementState;

        // convert local player movement to human movement
        switch (movestate) {
            case M2::E_MOVEMENT_WALK:     ped->stream.move = M2::HUMAN_MOVE_MODE_WALK; break;
            case M2::E_MOVEMENT_JOG:      ped->stream.move = M2::HUMAN_MOVE_MODE_RUN; break;
            case M2::E_MOVEMENT_SPRINT:   ped->stream.move = M2::HUMAN_MOVE_MODE_SPRINT; break;
            case M2::E_MOVEMENT_IDLE:     ped->stream.move = M2::HUMAN_MOVE_MODE_BREATH; break;
            case M2::E_MOVEMENT_STOPPING: ped->stream.move = M2::HUMAN_MOVE_MODE_END; break;
        }
    }

    entity->position = ped->CEntity->GetPosition();
    auto direction   = ped->CEntity->GetDirection();

    ped->stream.dirx = direction.x;
    ped->stream.diry = direction.y;

    // write up the dynamicly changing data
    librg_data_wptr(event->data, &ped->stream, sizeof(ped->stream));
}

// =======================================================================//
// !
// ! Interpolation
// !
// =======================================================================//

void m2o_callback_ped_interpolate(librg_entity_t *entity) {
    // auto ped = get_ped(entity);

    // // skip the udpate if we have removed ped cuz he was inside a removed car
    // if (!ped->CEntity) return;

    // // last delta tick against constant tick delay
    // f32 alpha = ped->inter_delta / ctx->timesync.server_delay;
    // ped->inter_delta += (f32)mod.last_delta;

    // if (ped->stream.state != PED_ON_GROUND) {
    //     return;
    // }

    // ped->CHuman->SetPos(cubic_hermite_v3_interpolate(&ped->inter_pos, alpha));
}

// =======================================================================//
// !
// ! Custom events
// !
// =======================================================================//

void module_ped_init() {

}
