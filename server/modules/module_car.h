librg_entity_t lastcar;

void module_car_create(librg_message_t *msg) {
    auto player  = librg_get_client_entity(msg->peer);
    auto vehicle = librg_entity_create(TYPE_CAR);

    auto pt = librg_fetch_transform(player);
    auto vt = librg_fetch_transform(vehicle);

    vt->position = vec3(
        pt->position.x + 3.0f,
        pt->position.y,
        pt->position.z
    );

    auto car = librg_attach_car(vehicle, { 0 });

    car->model = librg_data_ru16(msg->data);
    car->brake = 0.0f;

    lastcar = vehicle;

    // log
    print_posm(vt->position, "created a vehicle at: ");
}

void module_car_callback_create(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_CAR) return;
    auto car = librg_fetch_car(event->entity); mod_assert(car);
    //librg_data_wptr(event->data, car, sizeof(car_t));
}

void module_car_callback_update(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_CAR) return;
    //auto car = librg_fetch_car(event->entity); mod_assert(car);
    //librg_data_wptr(event->data, car, sizeof(car_t));
}

void module_car_callback_clientstream(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_CAR) return;
    //auto car = librg_fetch_car(event->entity); mod_assert(car);
    //librg_data_wptr(event->data, car, sizeof(car_t));
}


void module_car_enter(librg_message_t *msg) {
    auto entped = librg_get_client_entity(msg->peer);
    auto entcar = librg_data_ru32(msg->data);

    mod_assert_msg(librg_entity_valid(entcar), "trying to enter invalid car");

    mod_log("ped: %lu becomes driver of: %lu\n", entped, entcar);
    librg_streamer_client_set(entcar, msg->peer);

    auto ped = librg_fetch_ped(entped); mod_assert(ped);
    ped->state = PED_IN_CAR;

    librg_send_instream_except(MOD_CAR_ENTER, entped, msg->peer, data, {
        mod_log("sending enter message to EVERYYYYBODY\n");
        librg_data_wu32(&data, entped);
        librg_data_wu32(&data, entcar);
    });
}

void module_car_exit(librg_message_t *msg) {
    librg_streamer_client_remove(lastcar);
}



inline void module_car_init() {
    librg_event_add(LIBRG_ENTITY_CREATE, module_car_callback_create);
    librg_event_add(LIBRG_ENTITY_UPDATE, module_car_callback_update);
    //librg_event_add(LIBRG_ENTITY_REMOVE, module_car_callback_remove);
    librg_event_add(LIBRG_CLIENT_STREAMER_UPDATE, module_car_callback_clientstream);

    librg_network_add(MOD_CAR_CREATE, module_car_create);
    librg_network_add(MOD_CAR_ENTER, module_car_enter);
}

inline void module_car_tick() {

}
