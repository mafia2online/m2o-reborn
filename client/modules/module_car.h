#include "callbacks_car/create.h"
#include "callbacks_car/update.h"
#include "callbacks_car/remove.h"
#include "callbacks_car/interpolate.h"

void module_car_local_enter(void *custom) {
    auto car = (M2::C_Entity *)custom;
    auto ped = librg_fetch_ped(mod.player);

    mod_assert(car && ped);
    ped->state = PED_IN_CAR;

    // send vehicle create request onto server
    librg_entity_filter_t filter = { librg_index_car(), librg_index_gamedata() };
    librg_entity_eachx(filter, entity, {
        if (librg_fetch_gamedata(entity)->object == car) {
            librg_send(MOD_CAR_ENTER, data,{ librg_data_wu32(&data, entity); });
        }
    });
}

void module_car_remote_enter(librg_message_t *msg) {
    mod_log("received message for EVERYYYBODY\n");

    auto entped = librg_data_rentity(msg->data);
    auto entcar = librg_data_rentity(msg->data);
    auto gmped  = librg_fetch_gamedata(entped);
    auto gmcar  = librg_fetch_gamedata(entcar);

    mod_assert_msg(librg_entity_valid(entped) && gmped && gmped->object, "trying to put ped in invalid car");
    mod_assert_msg(librg_entity_valid(entcar) && gmcar && gmcar->object, "trying to put invalid ped in car");

    mod_log("im not putting anyone anywhere fuck u\n");
    return;

    mod_log("putting ped: %u in the car: %u\n", entped, entcar);

    M2::C_SyncObject *pSyncObject = nullptr;
    ((M2::C_Human2*)gmped->object)->GetScript()->UseAB(
        &pSyncObject,
        reinterpret_cast<M2::C_Vehicle *>(gmcar->object),
        true, M2::E_VehicleSeat::E_SEAT_DRIVER, false
    );
}

inline void module_car_init() {
    librg_event_add(LIBRG_ENTITY_CREATE, module_car_callback_create);
    librg_event_add(LIBRG_ENTITY_UPDATE, module_car_callback_update);
    librg_event_add(LIBRG_ENTITY_REMOVE, module_car_callback_remove);
    librg_event_add(LIBRG_CLIENT_STREAMER_UPDATE, module_car_callback_clientstream);
    
    // local events
    //librg_event_add(MOD_CAR_ENTER, module_car_local_enter);

    // remote events
    librg_network_add(MOD_CAR_ENTER, module_car_remote_enter);
}

inline void module_car_tick() {
    librg_entity_filter_t filter = {0};
    if (librg_index_interpolate() == 0) return;

    filter.contains1 = librg_index_interpolate();
    filter.excludes1 = librg_index_clientstream();

    librg_entity_each(filter, module_car_callback_interpolate);
}
