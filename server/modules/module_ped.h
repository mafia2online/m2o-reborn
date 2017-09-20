void module_ped_callback_create(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_PED) return;
    librg_attach_ped(event->entity, {0});
}

void module_ped_callback_clientstream(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_PED) return;
    auto ped = librg_fetch_ped(event->entity); mod_assert(ped);
    librg_data_rptr(event->data, ped, sizeof(ped_t));
}

void module_ped_callback_update(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_PED) return;
    auto ped = librg_fetch_ped(event->entity); mod_assert(ped);
    librg_data_wptr(event->data, ped, sizeof(ped_t));
}

inline void module_ped_init() {
    librg_event_add(LIBRG_ENTITY_CREATE, module_ped_callback_create);
    librg_event_add(LIBRG_ENTITY_UPDATE, module_ped_callback_update);
    librg_event_add(LIBRG_CLIENT_STREAMER_UPDATE, module_ped_callback_clientstream);
}

inline void module_ped_tick() {

}
