#include "callbacks_car/create.h"
#include "callbacks_car/update.h"
#include "callbacks_car/remove.h"
#include "callbacks_car/interpolate.h"

void module_car_local_enter(void *custom) {
    auto expected = (M2::C_Entity *)custom;
    auto ped = (ped_t*)mod.player->user_data;

    mod_assert(expected && ped);
    ped->stream.state = PED_IN_CAR;

    // send vehicle create request onto server
    librg_entity_iteratex(ctx, LIBRG_ENTITY_ALIVE, id, {
        if (librg_entity_type(ctx, id) == TYPE_CAR) {
            auto entity = librg_entity_fetch(ctx, id);
            auto car = (car_t *)entity->user_data;

            if (car->object == expected) {
                librg_send(ctx, MOD_CAR_ENTER, data, { librg_data_wu32(&data, id); });
            }
        }
    });

    //librg_entity_iterate(ctx, LIBRG_ENTITY_ALIVE, );
}

void module_car_remote_enter(librg_message_t *msg) {
    mod_log("received message for EVERYYYBODY\n");

    auto entped = librg_data_rent(msg->data);
    auto entcar = librg_data_rent(msg->data);
    auto gmped  = librg_entity_fetch(ctx, entped);
    auto gmcar  = librg_entity_fetch(ctx, entcar);

    auto ped = (ped_t *)gmped->user_data;
    auto car = (car_t *)gmcar->user_data;

    mod_assert_msg(gmped && ped->object, "trying to put ped in invalid car");
    mod_assert_msg(gmcar && car->object, "trying to put invalid ped in car");

    mod_log("im not putting anyone anywhere fuck u\n");
    return;

    mod_log("putting ped: %u in the car: %u\n", entped, entcar);

    M2::C_SyncObject *pSyncObject = nullptr;
    ((M2::C_Human2*)ped->object)->GetScript()->ScrDoAction(
        &pSyncObject,
        reinterpret_cast<M2::C_Vehicle *>(car->object),
        true, M2::E_VehicleSeat::E_SEAT_DRIVER, false
    );
}

inline void module_car_init() {
    librg_event_add(ctx, LIBRG_ENTITY_CREATE, module_car_callback_create);
    librg_event_add(ctx, LIBRG_ENTITY_UPDATE, module_car_callback_update);
    librg_event_add(ctx, LIBRG_ENTITY_REMOVE, module_car_callback_remove);
    librg_event_add(ctx, LIBRG_CLIENT_STREAMER_UPDATE, module_car_callback_clientstream);
    
    // local events
    //librg_event_add(MOD_CAR_ENTER, module_car_local_enter);

    // remote events
    librg_network_add(ctx, MOD_CAR_ENTER, module_car_remote_enter);
}

inline void module_car_tick() {
    librg_entity_iterate(ctx, LIBRG_ENTITY_ALIVE, module_car_callback_interpolate);
}
