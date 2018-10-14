// =======================================================================//
// !
// ! Generic entity events
// !
// =======================================================================//

#define M2O_PED_NICKNAME_FONT_SIZE  18
#define M2O_PED_HEALTHBAR_WIDTH     64
#define M2O_PED_HEALTHBAR_HEIGH     12
#define M2O_PED_HEALTHBAR_MARGIN    32 /* margin from the nickname */
#define M2O_PED_HEALTHBAR_PADDING   1  /* internal border padding */

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

    { /* healthbar and nickname stuff */
        ped->nickname_value  = gfx_create_text(0, M2O_PED_NICKNAME_FONT_SIZE, ped->name, vec4f(255,255,255,255));
        ped->healthbar_base  = gfx_create_rect(0, 0, M2O_PED_HEALTHBAR_WIDTH, M2O_PED_HEALTHBAR_HEIGH, vec4f(50,50,50,170));
        ped->healthbar_value = gfx_create_rect(
            M2O_PED_HEALTHBAR_PADDING,
            M2O_PED_HEALTHBAR_PADDING,
            M2O_PED_HEALTHBAR_WIDTH - M2O_PED_HEALTHBAR_PADDING * 2,
            M2O_PED_HEALTHBAR_HEIGH - M2O_PED_HEALTHBAR_PADDING * 2,
            vec4f(255,122,122,250)
        );

        gfx_render_add(ped->nickname_value,  24);
        gfx_render_add(ped->healthbar_base,  22);
        gfx_render_add(ped->healthbar_value, 22);
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

        if (ped->tasks.movedir) delete ped->tasks.movedir;
    }

    gfx_destroy(ped->nickname_value);
    gfx_destroy(ped->healthbar_base);
    gfx_destroy(ped->healthbar_value);

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

void m2o_callback_ped_namechange(librg_message_t *msg) {
    auto entity = librg_entity_fetch(msg->ctx, librg_data_ru32(msg->data)); mod_assert(entity);
    auto ped = m2o_ped_get(entity); mod_assert(ped);

    auto size_actual = librg_data_ru8(msg->data);
    auto size = zpl_clamp(0, size_actual, 127);

    zpl_zero_item(ped->name);
    librg_data_rptr(msg->data, ped->name, size);
    mod_log("[info] set new name for client %u: %s\n", entity->id, ped->name);

    gfx_destroy(ped->nickname_value);
    ped->nickname_value = gfx_create_text(0, 14, ped->name, vec4f(255,255,255,255));
    gfx_render_add(ped->nickname_value,  24);
}

/**
 * The entity in our stream zone gets updated
 */
void m2o_callback_ped_update(librg_event_t *event) {
    auto entity = event->entity;
    auto ped    = m2o_ped_get(entity);

    // read up the dynaymicly changed data
    librg_data_rptr(event->data, &ped->stream, sizeof(ped->stream));

    // skip the udpate if we have removed ped cuz he was inside a removed car
    if (!ped->CEntity) return;

    // // apply movement anim
    //     auto pos_diff  = zplm_vec3_mag2(ped->interp.last - entity->position);
    //     auto real_diff = zplm_vec3_mag2(ped->CEntity->GetPosition() - entity->position);

    //     if (pos_diff > zplm_square(0.05f)) {
    //         ped->CHuman->GetScript()->ScrMoveV(
    //             &ped->sync, entity->position, (M2::eHumanMoveMode)ped->stream.move,
    //             vec3f(ped->stream.dirx, ped->stream.diry, 0.0f), true
    //         );
    //     }

    //     if (real_diff > zplm_cube(10.0f)) {
    //         ped->CHuman->SetPos(entity->position);
    //     }
    // }

    /* ped tasks late-init (sync init right after ped creation causes crash sometimes) */
    if (!ped->tasks.init && ++ped->tasks.counter > 20) {
        ped->tasks.init = true;
        mod_log("[info] calling late init for ped tasks...");

        //ped->tasks.stand = CIE_Alloc(0x1E); // zpl_zero_item(ped->tasks.stand);
        //ped->CHuman->AddCommand(M2::E_Command::COMMAND_STAND, ped->tasks.stand);

        // ped->tasks.movedir = CIE_Alloc(0x58); //zpl_zero_item(ped->tasks.movedir);
        ped->tasks.movedir = new char[0x58]; //zpl_zero_item(ped->tasks.movedir);
        ped->CHuman->AddCommand(M2::E_Command::COMMAND_MOVEDIR, ped->tasks.movedir);
    }

    if (ped->state == PED_ON_GROUND && ped->tasks.init) {
        auto pos_diff = zplm_vec3_mag2(ped->interp.pos.target - entity->position);

        if (pos_diff > 0) {
            auto cmd = (M2::S_HumanCommandMoveDir *)ped->tasks.movedir; {
                cmd->speedMultiplier = 1.0f;
                cmd->moveSpeed = ped->stream.move;
                cmd->potentialMoveVector = { ped->stream.dirx, ped->stream.diry };
            }

            ped->CHuman->m_iCurrentCommand = 1;
            ped->CHuman->m_aCommandsArray[1].m_pCommand = ped->tasks.movedir;
        } else {
            // ped->CHuman->m_iCurrentCommand = 0;
            // ped->CHuman->m_aCommandsArray[0].m_pCommand = ped->tasks.stand;
        }
    }

    // Get the interpolation interval
    ped->interp.pos.startTime = zpl_time_now();
    ped->interp.pos.finishTime = ped->interp.pos.startTime + (1.0f / M2O_TICKRATE_SERVER);
    ped->interp.pos.lastAlpha = 0.0f;

    ped->interp.pos.start = ped->interp.pos.target;
    ped->interp.pos.target = entity->position;

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
            case M2::E_MOVEMENT_WALK:     ped->stream.move = 0; break;
            case M2::E_MOVEMENT_JOG:      ped->stream.move = 1; break;
            case M2::E_MOVEMENT_SPRINT:   ped->stream.move = 2; break;
            // case M2::E_MOVEMENT_IDLE:     ped->stream.move = M2::HUMAN_MOVE_MODE_BREATH; break;
            // case M2::E_MOVEMENT_STOPPING: ped->stream.move = M2::HUMAN_MOVE_MODE_END; break;
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
    auto ped = m2o_ped_get(entity);

    // skip the udpate if we have removed ped cuz he was inside a removed car
    if (!ped->CEntity) return;

    f64 currentTime = zpl_time_now();
    f32 alpha = zplm_unlerp(currentTime, ped->interp.pos.startTime, ped->interp.pos.finishTime);

    // Don't let it overcompensate the error too much
    alpha = zplm_clamp(0.0f, alpha, 1.5f);

    // Get the current error portion to compensate
    f32 currentAlpha = alpha - ped->interp.pos.lastAlpha;
    ped->interp.pos.lastAlpha = alpha;

    // Apply the error compensation
    vec3 compensation;
    zplm_vec3_lerp(&compensation, ped->interp.pos.start, ped->interp.pos.target, alpha);

    // If we finished compensating the error, finish it for the next pulse
    if (alpha == 1.5f) {
        ped->interp.pos.finishTime = 0;
    }

    ped->CHuman->SetPos(compensation);

    // // last delta tick against constant tick delay
    // f32 alpha = ped->inter_delta / ctx->timesync.server_delay;
    // ped->inter_delta += (f32)mod.last_delta;

    // if (ped->stream.state != PED_ON_GROUND) {
    //     return;
    // }

    // ped->CHuman->SetPos(cubic_hermite_v3_interpolate(&ped->inter_pos, alpha));


    { /* drawing names and healthbars */
        f32 vdmag = zplm_vec3_mag2(mod.player->position - compensation);
        f32 scale = (1.0f - zplm_clamp01(vdmag / zplm_square(35.0f)));

        vec3 screen;
        gfx_util_world2screen(compensation + vec3f(0, 0, 2.5f), &screen);

        /* setup nickaname stuff */
        gfx_position_set(ped->nickname_value, screen.x - 50.f * scale, screen.y);
        gfx_visible_set(ped->nickname_value, (screen.z > 1.0));
        gfx_scale_set(ped->nickname_value, scale, scale);

        /* setup healthbar stuff */
        int hbasex = screen.x - (M2O_PED_HEALTHBAR_WIDTH * 0.5f) * scale;
        int hevalx = screen.x - (M2O_PED_HEALTHBAR_WIDTH * 0.5f) * scale + M2O_PED_HEALTHBAR_PADDING * scale;

        gfx_position_set(ped->healthbar_base,  hbasex, screen.y + M2O_PED_HEALTHBAR_MARGIN * scale);
        gfx_position_set(ped->healthbar_value, hevalx, screen.y + M2O_PED_HEALTHBAR_MARGIN * scale + M2O_PED_HEALTHBAR_PADDING * scale);

        gfx_visible_set(ped->healthbar_base,  (screen.z > 1.0));
        gfx_visible_set(ped->healthbar_value, (screen.z > 1.0));
        gfx_scale_set(ped->healthbar_base,  scale, scale);
        gfx_scale_set(ped->healthbar_value, scale, scale /* TODO: multiply by health percentage */);
    }

}

// =======================================================================//
// !
// ! Custom events
// !
// =======================================================================//

void module_ped_init() {

}
