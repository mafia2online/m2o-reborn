static M2::Wrappers::GameModelManager *pPedModelManager = nullptr;

librg_entity_t lastcar;

void module_car_callback_create(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_CAR) return;

    mod_log("creating vehicle\n");

    car_t car = {0};
    //librg_data_rptr(event->data, &car, sizeof(car_t));

    auto transform = librg_fetch_transform(event->entity);
    print_posm(transform->position, "creating vehicle at:");

    std::string dir;
    std::string model;
    M2::Models::GetVehicleModelFromID(0, &dir, &model);

    M2::Wrappers::ModelManager *pModelManager = new M2::Wrappers::ModelManager();
    mod_assert(pModelManager);

    M2::Wrappers::GameModelManager *pPedModelManager = pModelManager->Load(dir.c_str(), model.c_str());
    mod_assert(pPedModelManager);

    M2::C_Car *object = M2::C_EntityFactory::Get()->CreateEntity<M2::C_Car>(M2::EntityTypes::Entity_Car);
    mod_assert(object);

    M2::C_Model *pModel = M2::C_Core::Get()->AllocateModel(2);
    mod_assert(pModel);

    pModel->CloneHierarchy(pPedModelManager->GetModelManager()->m_pModel);
    pModel->SetName("m2online_car");
    pModel->MarkForNotify(6);

    reinterpret_cast<M2::C_Entity *>(object)->SetModel(pModel);

    object->Init(NULL);
    object->m_nSlotSDS = pPedModelManager->GetModelManager()->m_pSlot->m_iSlotNumber;
    object->Setup();

    DWORD flags = reinterpret_cast<M2::C_Entity *>(object)->m_dwFlags & 0xFFFFB7BF | 0x4800;
    reinterpret_cast<M2::C_Entity *>(object)->m_dwFlags = flags;

    if (reinterpret_cast<M2::C_Entity *>(object)->m_dwFlags & 0x20)
        mod_log("Flags set sucessfully!\n");

    reinterpret_cast<M2::C_Entity *>(object)->Activate();

    if (reinterpret_cast<M2::C_Entity *>(object)->IsActive())
        mod_log("Entity active !\n");

    reinterpret_cast<M2::C_Entity *>(object)->SetPosition(transform->position);

    auto gm = librg_attach_gamedata(event->entity, { (M2::C_Entity *)object });
    librg_attach_interpolate(event->entity, {0});
    librg_attach_car(event->entity, car);

    mod_log("Created at %x with GUID: %lu!\n", object, gm->object->m_dwGUID);
}
