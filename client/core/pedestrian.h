// =======================================================================//
// !
// ! Generic entity events
// !
// =======================================================================//

/**
 * The entity enters the stream zone
 */
void module_ped_callback_create(librg_event_t *event) {
    const char *directory = "/sds/traffic/";
    const char *model = "cvezjon";

    M2::Wrappers::ModelManager *pModelManager = new M2::Wrappers::ModelManager();
    mod_assert(pModelManager);

    M2::Wrappers::GameModelManager *pPedModelManager = pModelManager->Load(directory, model);
    mod_assert(pPedModelManager);

    M2::C_Human2 *human = M2::C_EntityFactory::Get()->CreateEntity<M2::C_Human2>(M2::EntityTypes::Entity_Human);
    mod_assert(human);

    M2::C_Model *pModel = M2::C_Core::Get()->AllocateModel(2);
    mod_assert(pModel);

    pModel->CloneHierarchy(pPedModelManager->GetModelManager()->m_pModel);
    mod_assert(pModel);

    pModel->SetName("m2online_ped");
    pModel->MarkForNotify(2);

    reinterpret_cast<M2::C_Entity *>(human)->SetModel(pModel);
    reinterpret_cast<M2::C_Entity *>(human)->Setup();

    DWORD flags = reinterpret_cast<M2::C_Entity *>(human)->m_dwFlags &= 0xFFFFB7BF | 0x4800;
    reinterpret_cast<M2::C_Entity *>(human)->m_dwFlags = flags;

    if (reinterpret_cast<M2::C_Entity *>(human)->m_dwFlags & 0x20)
        mod_log("Flags set sucessfully!\n");
    else {
        reinterpret_cast<M2::C_Entity *>(human)->Release();
    }

    reinterpret_cast<M2::C_Entity *>(human)->Activate();

    if (reinterpret_cast<M2::C_Entity *>(human)->IsActive())
        mod_log("Entity active !\n");

    reinterpret_cast<M2::C_Entity *>(human)->SetPosition(event->entity->position);

    mod_log("Created at %x!\n", human);

    auto ped = new ped_t();

    ped->object = (M2::C_Entity*)human;
    ped->pGameModelManager = pPedModelManager;

    event->entity->flags |= MOD_ENTITY_INTERPOLATED;
    event->entity->user_data = ped;
}

/**
 * The entity exists the stream zone
 */
void module_ped_callback_remove(librg_event_t *event) {
    // TODO: remove ped
    librg_log("very important, remove ped pls tnx\n");
    delete event->entity->user_data;
}

// =======================================================================//
// !
// ! Update/sync entity events
// !
// =======================================================================//

/**
 * The entity in our stream zone gets updated
 */
void module_ped_callback_update(librg_event_t *event) {
    auto entity = event->entity; mod_assert(entity);
    auto ped = (ped_t *)(event->entity->user_data);

    // make sure we have all objects
    mod_assert(ped && ped->object);

    interpolate_t *interpolate = &ped->interpolate;

    // interpolation stuff
    interpolate->lposition = interpolate->tposition;
    interpolate->tposition = entity->position;
    interpolate->delta = 0.0f;

    librg_data_rptr(event->data, &ped->stream, sizeof(ped->stream));

    // apply movement anim
    if (ped->stream.state == PED_ON_GROUND) {
        auto extr_shift = ped->stream.direction * 3.0f; /* create extrapolated shift for ped */
        auto targ_pos = entity->position + extr_shift;

        targ_pos.z = entity->position.z;
        ped->object->SetDirection(vec3(ped->stream.direction.x, ped->stream.direction.y, 0.0f));
        return;

        M2::C_SyncObject *pSyncObject = nullptr;
        ((M2::C_Human2*)ped->object)->GetScript()->ScrMoveV(
            &pSyncObject, targ_pos, (M2::eHumanMoveMode)ped->stream.move_state,
            vec3(ped->stream.direction.x, ped->stream.direction.y, 0.0f), true
        );
    } else {
        // if we are not on the ground
        // our prev position will be always our current
        // so that after we appear on the ground again
        // we won't be interpolated from somewhere far away
        interpolate->lposition = entity->position;
    }
}

#define valid_dir(x) (zplm_abs(x) > 0.0f && zplm_abs(x) < 1.0f)

/**
 * The entity is streamed by us, and we are sending an update with new data
 */
void module_ped_callback_clientstream(librg_event_t *event) {
    auto entity = event->entity; mod_assert(entity);
    auto ped = (ped_t *)(event->entity->user_data);

    // make sure we have all objects
    mod_assert(ped && ped->object);

    // TODO: add checks for being on the ground

    // read new values of entity
    auto new_position = ped->object->GetPosition();
    auto diff_position = new_position - entity->position;
    entity->position = new_position;

    if (event->entity->id != mod.player->id) {
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
    librg_data_wptr(event->data, &ped->stream, sizeof(ped->stream));
}

// =======================================================================//
// !
// ! Interpolation
// !
// =======================================================================//

void module_ped_callback_interpolate(librg_entity_t *entity) {
    mod_assert(entity);
    auto ped = (ped_t *)(entity->user_data);

    // make sure we have all objects
    mod_assert(ped && ped->object);

    // last delta tick against constant tick delay
    ped->interpolate.delta += (mod.last_delta / 40.666f);
    // ped->interpolate.delta = zplm_clamp01(ped->interpolate.delta);

    if (ped->stream.state != PED_ON_GROUND) {
        return;
    }

    vec3_t dposition;
    zplm_vec3_lerp(&dposition, ped->interpolate.lposition, ped->interpolate.tposition, ped->interpolate.delta);

    //if (dposition == interpolate->tposition) return;
    ped->object->SetPosition(dposition);
}

// =======================================================================//
// !
// ! Custom events
// !
// =======================================================================//

void module_ped_init() {

}
