typedef struct { vec3_t position; u8 step; } librg_component(lastdata);

void ped_oncreate(librg_event_t *event)
{
    auto transform = librg_fetch_transform(event->entity);

    const char *directory = "/sds/traffic/";
    const char *model = "cvezjon";

    M2::Wrappers::ModelManager *pModelManager = new M2::Wrappers::ModelManager();
    if (!pModelManager) {
        return;
    }

    M2::Wrappers::GameModelManager *pPedModelManager = pModelManager->Load(directory, model);
    if (!pPedModelManager) {
        return;
    }

    M2::C_Human2 *human = M2::C_EntityFactory::Get()->CreateEntity<M2::C_Human2>(M2::EntityTypes::Entity_Human);
    if (!human) {
        return;
    }

    M2::C_Model *pModel = M2::C_Core::Get()->AllocateModel(2);
    if (!pModel) {
        return;
    }

    pModel->CloneHierarchy(pPedModelManager->GetModelManager()->m_pModel);
    if (!pModel) {
        return;
    }

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

    reinterpret_cast<M2::C_Entity *>(human)->SetPosition(transform->position);

    mod_log("Created at %x!\n", human);

    librg_attach_gamedata(event->entity, { (M2::C_Entity*)human, pPedModelManager });
    librg_attach_lastdata(event->entity, {transform->position});
}

void ped_onclient(librg_event_t *event)
{
    auto transform = librg_fetch_transform(event->entity);
    auto gamedata  = librg_fetch_gamedata(event->entity);

    transform->position = gamedata->object->GetPosition();
    transform->rotation = gamedata->object->GetRotation();

    zplm_vec4_t direction;// = ((M2::C_Human2 *)gamedata->object)->GetDirection();
    Mem::InvokeFunction<Mem::call_this, void>(gamedata->object->m_pVFTable->GetDirection, (M2::C_Player2 *)gamedata->object, &direction);
    // direction.x = 0.0f;

    librg_log("%f\n", direction.w);
    librg_data_wptr(event->data, &direction, sizeof(direction));
}

void ped_onupdate(librg_event_t *event)
{
    auto transform = librg_fetch_transform(event->entity);
    auto gamedata  = librg_fetch_gamedata(event->entity);
    auto lastdata  = librg_fetch_lastdata(event->entity);
    librg_assert(gamedata && gamedata->object);

    vec3_t direction;
    librg_data_rptr(event->data, &direction, sizeof(direction));

    // trying out scripted movement
    // its shit, dont use it
    // if (lastdata->step++ > 16) {
    //     // gamedata->object->SetDirection(direction);
    //     auto ped = ((M2::C_Human2*)gamedata->object);
    //     M2::C_SyncObject *pSyncObject = nullptr;
    //     ped->GetScript()->ScrMoveV(&pSyncObject, lastdata->position, M2::HUMAN_MOVE_MODE_WALK, transform->position, true);
    //     lastdata->position = transform->position;
    //     lastdata->step = 0;
    // }

    gamedata->object->SetPosition(transform->position);

    // TODO: look at
    // M2::C_SyncObject *pSyncObject = nullptr;
    // ((M2::C_Human2*)gamedata->object)->GetScript()->ScrLookAt(&pSyncObject, (M2::C_Entity *)gamedata->object, direction, true);
}
