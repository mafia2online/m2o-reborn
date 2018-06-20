static M2::Wrappers::ModelManager *pModelManager = new M2::Wrappers::ModelManager();

void M2::Wrappers::PreloadModels()
{
    if (!pModelManager) {
        return;
    }
    pModelManager->PreloadAllModels();
}
M2::C_Entity *M2::Wrappers::CreateEntity(eEntityType type, int modelID)
{
    if (!pModelManager) {
        return nullptr;
    }
    M2::Wrappers::GameModelManager *pPedModelManager = nullptr;
    m2sdk_assert(pModelManager);

    M2::C_Entity *entity = nullptr;
    std::string dir;
    std::string model;
    switch (type)
    {
        case MOD_ENTITY_PED:
        {
            M2::Models::GetPlayerModelFromID(modelID, &dir, &model);

            pPedModelManager = pModelManager->Load(dir.c_str(), model.c_str());
            m2sdk_assert(pPedModelManager);

            entity = reinterpret_cast<M2::C_Entity*>(M2::C_EntityFactory::Get()->CreateEntity<M2::C_Human2>(M2::EntityTypes::Entity_Human));
            m2sdk_assert(entity);
        }
        break;

        case MOD_ENTITY_CAR:
        {
            M2::Models::GetVehicleModelFromID(modelID, &dir, &model);

            pPedModelManager = pModelManager->Load(dir.c_str(), model.c_str());
            m2sdk_assert(pPedModelManager);

            entity = reinterpret_cast<M2::C_Entity*>(M2::C_EntityFactory::Get()->CreateEntity<M2::C_Car>(M2::EntityTypes::Entity_Car));
            m2sdk_assert(entity);
        }
        break;
    }

    M2::C_Model *pModel = M2::C_Core::Get()->AllocateModel(2);
    m2sdk_assert(pModel);

    pModel->CloneHierarchy(pPedModelManager->GetModelManager()->m_pModel);
    pModel->SetName("m2o_entity");
    pModel->MarkForNotify(2);

    entity->SetModel(pModel);

    switch (type)
    {
        case MOD_ENTITY_PED:
        {
            entity->Setup();

            DWORD flags = entity->m_dwFlags &= 0xFFFFB7BF | 0x4800;
            entity->m_dwFlags = flags;
        }
        break;

        case MOD_ENTITY_CAR:
        {
            if (reinterpret_cast<M2::C_Car*>(entity)->Init(NULL) == false) {
                return nullptr;
            }

            reinterpret_cast<M2::C_Car*>(entity)->m_nSlotSDS = pPedModelManager->GetModelManager()->m_pSlot->m_iSlotNumber;
            reinterpret_cast<M2::C_Car*>(entity)->Setup();

            DWORD flags = entity->m_dwFlags & 0xFFFFFFBF | 0x4800;
            entity->m_dwFlags = flags;
        }
        break;
    }

    if (entity->m_dwFlags & 0x20)
        m2sdk_log("[ModelManager] Flags set sucessfully!\n");
    else {
        entity->Release();
        return nullptr;
    }

    entity->Activate();
    return entity;
}

bool M2::Wrappers::DestroyEntity(C_Entity *entity, eEntityType type)
{
    if (entity == nullptr) {
        m2sdk_log("entity null\n");
        return false;
    }
    entity->Deactivate();
    if (entity->IsActive()) {
        return false;
    }
    entity->Release();
    if (entity) {
        entity->Destructor();
    }
    if (entity && type == eEntityType::MOD_ENTITY_PED) {
        Mem::InvokeFunction<Mem::call_cdecl, int>(0x4019F0, entity);
    }
    if (entity) {
        entity = nullptr;
    }
    return true;
}

