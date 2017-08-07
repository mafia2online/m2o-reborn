/**
* Entity add to streamer
*/
void entity_create(librg::events::event_t* evt)
{
    librg::core::log("creating entity");
    auto event      = (librg::events::event_create_t*) evt;
    auto transform  = event->entity.component<librg::transform_t>();
    auto inter      = event->entity.assign<librg::interpolable_t>(*transform);

    switch (event->type)
    {
        case TYPE_PLAYER: {
            mod_log("creating player");

            M2::C_Human2 *ent = M2::C_EntityFactory::Get()->CreateEntity<M2::C_Human2>(M2::EntityTypes::Entity_Human);
            
            librg_assert(ent, "player entity should be created!");

            DWORD coreInstance = *(DWORD*)(0x1AC2778);

            M2::C_Model *own_model = Mem::InvokeFunction<Mem::call_this, M2::C_Model*>((*(Address*)(*(DWORD*)coreInstance + 0x94)), coreInstance, 2);
            own_model->CloneHierarchy(M2::C_PlayerModelManager::Get()->GetInterface()->localPlayerModel);

            own_model->SetName("lawl");
            own_model->MarkForNotify(2);

            reinterpret_cast<M2::C_Entity *>(ent)->SetModel(own_model);
            reinterpret_cast<M2::C_Entity *>(ent)->Setup();

            // set flagsF
            DWORD flags = *(DWORD *)(ent + 32) & 0xFFFFFBF | 0x4800;
            *(DWORD *)(ent + 32) = flags;

            if (flags & 0x20)
                mod_log("Flags set sucessfully!");

            reinterpret_cast<M2::C_Entity *>(ent)->Activate();

            if (reinterpret_cast<M2::C_Entity *>(ent)->IsActive())
                mod_log("Entity active !");

            reinterpret_cast<M2::C_Entity *>(ent)->SetPosition(transform->position);

            event->entity.assign<gamedata_t>((M2::C_Entity*)ent);

            mod_log("Created at %x!", ent);

        } break;
        case TYPE_VEHICLE: {
            librg::core::log("creating vehicle");
        }break;
    }

    librg::core::log("entity succeessfully created");
}

