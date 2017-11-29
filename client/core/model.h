static M2::Wrappers::ModelManager *model_manager = nullptr;

struct mod_model_node_t {
    M2::Wrappers::GameModelManager *manager;
    b32 cached;
};

struct {
    mod_model_node_t car[M2_CAR_MODELS];
    mod_model_node_t ped[M2_PED_MODELS];
} mod_model_cache = {0};


void model_init() {
    mod_assert(model_manager == nullptr);
    mod_log("initializing model manager\n");
    model_manager = new M2::Wrappers::ModelManager();
    mod_assert(model_manager);
}

/**
 * Method for fetching a model
 * If model with same name has been used before,
 * it will take it from the cache
 * Else, it will create it, cache it, and return
 */
M2::C_Model *model_fetch_node(M2::EntityTypes type, i32 model) {
    mod_assert(model_manager);
    mod_model_node_t *node = nullptr;

    switch (type) {
        case M2::Entity_Human: mod_assert(model < M2_PED_MODELS); node = &mod_model_cache.ped[model]; break;
        case M2::Entity_Car:   mod_assert(model < M2_CAR_MODELS); node = &mod_model_cache.car[model]; break;
    }

    // node should be selected
    mod_assert(node);

    if (!node->cached) {
        node->cached = true;

        std::string native_dir;
        std::string native_model;

        switch (type) {
            case M2::Entity_Human: M2::Models::GetPlayerModelFromID(model, &native_dir, &native_model); break;
            case M2::Entity_Car:   M2::Models::GetVehicleModelFromID(model, &native_dir, &native_model); break;
        }

mod_log("loading %s %s\n", native_dir.c_str(), native_model.c_str());
        node->manager = model_manager->Load(native_dir.c_str(), native_model.c_str());
        mod_assert(node->manager);

        mod_log("model_fetch_node: added to cache\n");
    }
    else {
        mod_log("model_fetch_node: using from cache\n");
    }

    M2::C_Model *native_model = nullptr;

    native_model = M2::C_Core::Get()->AllocateModel(2); mod_assert(native_model);
    native_model->CloneHierarchy(node->manager->GetModelManager()->m_pModel);

    switch (type) {
        case M2::Entity_Human: native_model->SetName("m2online_ped"); native_model->MarkForNotify(2); break;
        case M2::Entity_Car:   native_model->SetName("m2online_car"); native_model->MarkForNotify(6); break;
    }

    return native_model;
}

void model_free() {
    // TODO: add freeing of models

    mod_assert(model_manager);
    model_manager->Clear();
    delete model_manager;
}


void create_game_entity(librg_entity_t entity, M2::EntityTypes type, i32 model) {
    mod_assert(model_manager);

    auto object = M2::C_EntityFactory::Get()->CreateEntity<M2::C_Entity>(type); mod_assert(object);

    // setting up the model
    object->SetModel(model_fetch_node(type, 0));

    /**/ if (type == M2::Entity_Human) {
        object->Setup();
    }
    else if (type == M2::Entity_Car) {
        ((M2::C_Car*)object)->Init(NULL); // NOTE: should it also be in the human ?
        ((M2::C_Car*)object)->m_nSlotSDS = mod_model_cache.car[model].manager->GetModelManager()->m_pSlot->m_iSlotNumber;
        ((M2::C_Car*)object)->Setup();
    }

    // setting flags
    // object->m_dwFlags = object->m_dwFlags &= 0xFFFFB7BF | 0x4800;
    // mod_assert(object->m_dwFlags & 0x20);
    DWORD flags = object->m_dwFlags & 0xFFFFB7BF | 0x4800;
    object->m_dwFlags = flags;

    if (object->m_dwFlags & 0x20)
        mod_log("Flags set sucessfully!\n");


    // activation and default stuff
    object->Activate(); mod_assert(object->IsActive());
    object->SetPosition(trans->position);
mod_log("activated and set position\n");
print_posm(trans->position, "spawning at: ");

    // attach default comps
    librg_attach_gamedata(entity, { object });
}

void destroy_game_entity(librg_entity_t entity) {
    mod_log("removing the entity\b");

    auto gamedata = librg_fetch_gamedata(entity);
    mod_assert(gamedata && gamedata->object);
    librg_detach_gamedata(entity);

    gamedata->object->Deactivate();
    gamedata->object->Destructor();

    delete gamedata->object;
}
