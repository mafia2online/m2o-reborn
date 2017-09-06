void ped_oncreate(librg_event_t *event)
{
    auto transform = librg_fetch_transform(event->entity);

    mod_log("creating player\n");

    M2::C_Human2 *ent = M2::C_EntityFactory::Get()->CreateEntity<M2::C_Human2>(M2::EntityTypes::Entity_Human);

    librg_assert_msg(ent, "player entity should be created!");

    M2::C_Model *own_model = M2::C_Core::Get()->AllocateModel(2);
    own_model->CloneHierarchy(M2::C_PlayerModelManager::Get()->GetInterface()->localPlayerModel);

    own_model->SetName("lawl");
    own_model->MarkForNotify(2);

    reinterpret_cast<M2::C_Entity *>(ent)->SetModel(own_model);
    reinterpret_cast<M2::C_Entity *>(ent)->Setup();

    // set flagsF
    DWORD flags = *(DWORD *)(ent + 32) & 0xFFFFFBF | 0x4800;
    *(DWORD *)(ent + 32) = flags;

    if (flags & 0x20)
        mod_log("Flags set sucessfully!\n");

    reinterpret_cast<M2::C_Entity *>(ent)->Activate();

    if (reinterpret_cast<M2::C_Entity *>(ent)->IsActive())
        mod_log("Entity active !\n");

    reinterpret_cast<M2::C_Entity *>(ent)->SetPosition(transform->position);

    librg_attach_gamedata(event->entity, { (M2::C_Entity*)ent });

    mod_log("Created at %x!\n", ent);
}

void ped_onclient(librg_event_t *event)
{
    auto transform = librg_fetch_transform(event->entity);
    auto gamedata  = librg_fetch_gamedata(event->entity);

    zplm_vec3_t position;
    zplm_quat_t rotation;

    Mem::InvokeFunction<Mem::call_this, void>(gamedata->object->m_pVFTable->GetPosition, gamedata->object, &position);
    Mem::InvokeFunction<Mem::call_this, void>(gamedata->object->m_pVFTable->GetRotation, gamedata->object, &rotation);

    vec3_t direction;
    Mem::InvokeFunction<Mem::call_this, void>(gamedata->object->m_pVFTable->GetDirection, gamedata->object, &direction);

    librg_log("sending looking at: %f %f %f\n", direction.x, direction.y, direction.z);

    transform->position = position;
    transform->rotation = rotation;

    librg_data_wptr(&event->data, &direction, sizeof(direction));
}

void ped_onupdate(librg_event_t *event)
{
    auto transform = librg_fetch_transform(event->entity);
    auto gamedata  = librg_fetch_gamedata(event->entity);
    librg_assert(gamedata && gamedata->object);

    zplm_vec3_t direction;
    librg_data_rptr(&event->data, &direction, sizeof(direction));

    // librg_log("looking at: %f %f %f\n", direction.x, direction.y, direction.z);

    M2::C_SyncObject *pSyncObject = nullptr;
    gamedata->object->SetPosition(transform->position);
    ((M2::C_Human2*)gamedata->object)->GetScript()->ScrLookAt(&pSyncObject, (M2::C_Entity *)gamedata->object, direction, true);
}
