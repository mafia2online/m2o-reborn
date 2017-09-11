void module_ped_callback_clientsteam(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_PED) return;

    auto transform = librg_fetch_transform(event->entity);
    auto gamedata  = librg_fetch_gamedata(event->entity);

    transform->position = gamedata->object->GetPosition();
    transform->rotation = gamedata->object->GetRotation();

    zplm_vec3_t direction;// = ((M2::C_Human2 *)gamedata->object)->GetDirection();
    Mem::InvokeFunction<Mem::call_this, void>(gamedata->object->m_pVFTable->GetDirection, (M2::C_Player2 *)gamedata->object, &direction);
    direction.x = 0.0f;

    // mod_log("%f\n", direction.w);
    librg_data_wptr(event->data, &direction, sizeof(direction));
}

void module_ped_callback_update(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_PED) return;

    auto transform = librg_fetch_transform(event->entity);
    auto gamedata  = librg_fetch_gamedata(event->entity);

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
}
