void module_ped_callback_remove(librg_event_t *event) {
    if (event->entity->type != TYPE_PED) return;
    // TODO: remove ped
    librg_log("very important, remove ped pls tnx\n");
    delete event->entity->user_data;
}
