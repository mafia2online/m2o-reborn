librg_entity_t lastcar;

void mod_vehicle_crate(librg_message_t *msg) {
    auto player  = librg_get_client_entity(msg->peer);
    auto vehicle = librg_entity_create(TYPE_VEHICLE);

    auto pt = librg_fetch_transform(player);
    auto vt = librg_fetch_transform(vehicle);

    vt->position = vec3(
        pt->position.x + 5.0f,
        pt->position.y,
        pt->position.z
    );

    lastcar = vehicle;

    // log
    librg_log("creating a vehicle at %f %f %f\n",
        vt->position.x, vt->position.y, vt->position.z
    );

    print_posm(vt->position, "creating vehicle at: ");
}

void mod_vehicle_enter(librg_message_t *msg) {
    librg_streamer_client_set(lastcar, msg->peer);
}
