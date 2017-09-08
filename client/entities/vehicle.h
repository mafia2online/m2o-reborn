static M2::Wrappers::GameModelManager *pPedModelManager = nullptr;

void vehicle_oncreate(librg_event_t *event)
{
    mod_log("creating vehicle\n");

    auto transform = librg_fetch_transform(event->entity);

    const char *directory = SDS_LOAD_DIR_CARS;
    std::string model;
    M2::Models::GetVehicleModelFromID(0, &model);

    M2::Wrappers::ModelManager *pModelManager = new M2::Wrappers::ModelManager();
    if (!pModelManager) {
        return;
    }

    M2::Wrappers::GameModelManager *pPedModelManager = pModelManager->Load(directory, model.c_str());
    if (!pPedModelManager) {
        return;
    }

    M2::C_Car *car = M2::C_EntityFactory::Get()->CreateEntity<M2::C_Car>(M2::EntityTypes::Entity_Car);
    if (!car) {
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

    pModel->SetName("m2online_car");
    pModel->MarkForNotify(6);

    reinterpret_cast<M2::C_Entity *>(car)->SetModel(pModel);

    car->Init(NULL);
    car->m_nSlotSDS = pPedModelManager->GetModelManager()->m_pSlot->m_iSlotNumber;
    car->Setup();

    DWORD flags = reinterpret_cast<M2::C_Entity *>(car)->m_dwFlags & 0xFFFFB7BF | 0x4800;
    reinterpret_cast<M2::C_Entity *>(car)->m_dwFlags = flags;

    if (reinterpret_cast<M2::C_Entity *>(car)->m_dwFlags & 0x20)
        mod_log("Flags set sucessfully!\n");

    reinterpret_cast<M2::C_Entity *>(car)->Activate();

    if (reinterpret_cast<M2::C_Entity *>(car)->IsActive())
        mod_log("Entity active !\n");

    reinterpret_cast<M2::C_Entity *>(car)->SetPosition(transform->position);

    librg_attach_gamedata(event->entity, { (M2::C_Entity *)car });

    mod_log("Created at %x!\n", car);
}


void vehicle_onclient(librg_event_t *event)
{

}


void vehicle_onupdate(librg_event_t *event)
{
    auto transform = librg_fetch_transform(event->entity);
    auto gamedata  = librg_fetch_gamedata(event->entity);

    librg_assert(gamedata && gamedata->object);

    gamedata->object->SetPosition(transform->position);
}
