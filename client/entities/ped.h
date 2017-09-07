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

    DWORD flags = *(DWORD *)(human + 32) & 0xFFFFFBF | 0x4800;
    *(DWORD *)(human + 32) = flags;

    if (flags & 0x20)
        mod_log("Flags set sucessfully!\n");

    reinterpret_cast<M2::C_Entity *>(human)->Activate();

    if (reinterpret_cast<M2::C_Entity *>(human)->IsActive())
        mod_log("Entity active !\n");

    reinterpret_cast<M2::C_Entity *>(human)->SetPosition(transform->position);

    mod_log("Created at %x!\n", human);
}

void ped_onclient(librg_event_t *event)
{
    auto transform = librg_fetch_transform(event->entity);
    auto gamedata  = librg_fetch_gamedata(event->entity);

    transform->position = gamedata->object->GetPosition();
    transform->rotation = gamedata->object->GetRotation();

    vec3_t direction = gamedata->object->GetDirection();
    direction.x = 0.0f;
    librg_data_wptr(&event->data, &direction, sizeof(direction));
}

void ped_onupdate(librg_event_t *event)
{
    auto transform = librg_fetch_transform(event->entity);
    auto gamedata  = librg_fetch_gamedata(event->entity);
    librg_assert(gamedata && gamedata->object);

    zplm_vec3_t direction;
    librg_data_rptr(&event->data, &direction, sizeof(direction));

    gamedata->object->SetPosition(transform->position);
    gamedata->object->SetDirection(direction);

    // TODO: look at
    // M2::C_SyncObject *pSyncObject = nullptr;
    // ((M2::C_Human2*)gamedata->object)->GetScript()->ScrLookAt(&pSyncObject, (M2::C_Entity *)gamedata->object, direction, true);
}
