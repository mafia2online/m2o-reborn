librg_entity_t lastcar;

void module_car_create(librg_message_t *msg) {
    auto player  = librg_get_client_entity(msg->peer);
    auto vehicle = librg_entity_create(TYPE_CAR);

    auto pt = librg_fetch_transform(player);
    auto vt = librg_fetch_transform(vehicle);

    vt->position = vec3(
        pt->position.x + 5.0f,
        pt->position.y,
        pt->position.z
    );

    lastcar = vehicle;

    // log
    print_posm(vt->position, "creating vehicle at: ");
}

void module_car_enter(librg_message_t *msg) {
    librg_streamer_client_set(lastcar, msg->peer);
}

inline void module_car_init() {
    // librg_event_add(LIBRG_ENTITY_CREATE, module_car_callback_create);
    // librg_event_add(LIBRG_ENTITY_UPDATE, module_car_callback_update);
    // librg_event_add(LIBRG_ENTITY_REMOVE, module_car_callback_remove);
    // librg_event_add(LIBRG_CLIENT_STREAMER_UPDATE, module_car_callback_clientstream);

    librg_network_add(MOD_VEHICLE_CREATE, module_car_create);
    librg_network_add(MOD_VEHICLE_ENTER, module_car_enter);
}

inline void module_car_tick() {

}
