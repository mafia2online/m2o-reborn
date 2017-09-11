void module_ped_callback_remove(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_PED) return;
}
