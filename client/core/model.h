static M2::Wrappers::ModelManager *model_manager = nullptr;

struct mod_model_node_t {
    M2::Wrappers::ModelManager *manager;
    M2::C_Model *model;
    b32 cached;
    u32 count;
};

struct {
    mod_model_node_t car[M2_CAR_MODELS];
    mod_model_node_t ped[M2_PED_MODELS];
} mod_model_cache = {0};


void model_init() {
    mod_assert(model_manager == nullptr);

    model_manager = new M2::Wrappers::ModelManager();
    mod_assert(model_manager);
}

/**
 * Method for fetching a model
 * If model with same name has been used before,
 * it will take it from the cache
 * Else, it will create it, cache it, and return
 */
mod_model_node_t *model_fetch_node(M2::EntityTypes type, i32 model) {
    mod_assert(model_manager);
    mod_model_node_t *node;

    switch (type) {
        case M2::Entity_Human:  node = &mod_model_cache.ped; mod_assert(model < M2_PED_MODELS); break;
        case M2::Entity_Car:    node = &mod_model_cache.car; mod_assert(model < M2_CAR_MODELS); break;
    }

    if (!node[model]->cached) {
        node[model]->cached = true;

        char *native_dir;
        char *native_model;

        switch (type) {
            case M2::Entity_Human:  M2::Models::GetPlayerModelFromID(model, native_dir, native_model); break;
            case M2::Entity_Car:    M2::Models::GetVehicleModelFromID(model, native_dir, native_model); break;
        }

        node[model]->manager = model_manager->Load(native_dir, native_model); mod_assert(node[model]->manager);
        node[model]->model   = M2::C_Core::Get()->AllocateModel(2); mod_assert(model);
        node[model]->model->CloneHierarchy(node[model]->manager->GetModelManager()->m_pModel);

        switch (type) {
            case M2::Entity_Human:  model->SetName("m2online_ped"); model->MarkForNotify(2); break;
            case M2::Entity_Car:    model->SetName("m2online_car"); model->MarkForNotify(6); break;
        }
    }

    node[model]->count++;
    return node[model];
}

void model_free() {
    // TODO: add freeing of models

    mod_assert(model_manager);
    model_manager->Clear();
    delete model_manager;
}


void create_game_entity(librg_entity_t entity, M2::EntityTypes type, i32 model) {
    mod_assert(model_manager);

    auto trans  = librg_fetch_transform(entity);
    auto mnode  = model_fetch_node(M2::Entity_Player, 0);
    auto object = M2::C_EntityFactory::Get()->CreateEntity<M2::C_Entity>(type); mod_assert(game_object);

    // setting up the model
    /**/ if (type == M2::Entity_Human) {
        object->SetModel(mnode->model)
        object->Setup();
    }
    else if (type == M2::Entity_Car) {
        object->Init(NULL); // NOTE: should it also be in the human ?
        object->SetModel(mnode->model)
        object->m_nSlotSDS = mnode->manager->GetModelManager()->m_pSlot->m_iSlotNumber;
        object->Setup();
    }

    // setting flags
    object->m_dwFlags = object->m_dwFlags &= 0xFFFFB7BF | 0x4800;
    mod_assert(object->m_dwFlags & 0x20);

    // activation and default stuff
    object->Activate(); mod_assert(object->IsActive());
    object->SetPosition(trans->position);

    // attach default comps
    librg_attach_gamedata(entity, { object });
}

void destroy_game_entity(librg_enitty_t entity) {
    auto gamedata = librg_fetch_gamedata(entity);
    mod_assert(gamedata && gamedata->object);
    librg_detach_gamedata(entity);

    gamedata->object->Deactivate();
    gamedata->object->Destructor();

    delete gamedata->object;
}
