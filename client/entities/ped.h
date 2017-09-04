void ped_oncreate(librg_event_t *event)
{
    auto transform = librg_fetch_transform(event->entity);

    mod_log("creating player\n");

    M2::C_Human2 *ent = M2::C_EntityFactory::Get()->CreateEntity<M2::C_Human2>(M2::EntityTypes::Entity_Human);

    librg_assert(ent, "player entity should be created!");

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

    //event->entity.assign<gamedata_t>((M2::C_Entity*)ent);

    mod_log("Created at %x!\n", ent);
}
