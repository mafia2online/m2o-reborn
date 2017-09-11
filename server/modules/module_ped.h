void module_ped_callback_clientstream(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_PED) return;

    when_assert(librg_fetch_ped(event->entity), ped) {
        librg_data_rptr(event->data, &ped->direction, sizeof(ped->direction));
    }
}

void module_ped_callback_update(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_PED) return;

    when_assert(librg_fetch_ped(event->entity), ped) {
        librg_data_wptr(event->data, &ped->direction, sizeof(ped->direction));
    }
}

inline void module_ped_init() {
    librg_event_add(LIBRG_ENTITY_UPDATE, module_ped_callback_update);
    librg_event_add(LIBRG_CLIENT_STREAMER_UPDATE, module_ped_callback_clientstream);
}

inline void module_ped_tick() {

}
