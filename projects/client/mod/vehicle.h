#define VEHICLE_THRESHOLD_FOR_SPEED 0.6f
#define VEHICLE_INTERPOLATION_THRESHOLD 15

#include "vehicle/interp.h"

// =======================================================================//
// !
// ! Generic entity events
// !
// =======================================================================//

void m2o_callback_car_create(librg_event_t *event) {
    auto car = m2o_car_alloc(NULL);

    car->model = librg_data_ru16(event->data);
    car->state = (car_state)librg_data_ru8(event->data);
    librg_data_rptr(event->data, &car->stream, sizeof(car->stream));

    event->entity->flags |= M2O_ENTITY_INTERPOLATED;
    event->entity->user_data = car;

    M2::C_Entity *entity = M2::Wrappers::CreateEntity(M2::eEntityType::MOD_ENTITY_CAR, car->model);

    if (entity->IsActive()) {
        print_posm(event->entity->position, "[info] creating car at");
        car->CEntity = entity;
        car->CCar->SetPos(event->entity->position);
        car->CCar->SetRot(zplm_quat_from_eular(zplm_vec3_to_radians(car->stream.rotation)));
        car->CCar->m_pVehicle.SetEngineOn(true, false);
    } else {
        mod_log("[warning] could not spawn a vehicle for entity: %d\n", event->entity->id);
    }
}

void m2o_callback_car_remove(librg_event_t *event) {
    auto car = m2o_car_get(event->entity); mod_assert(car && car->CEntity);
    mod_log("destroying vehicle entity %d\n", event->entity->id);

    // iterate and remove all peds that are in the car
    librg_entity_iteratex(ctx, LIBRG_ENTITY_ALIVE, librg_lambda(entityid), {
        auto entity = librg_entity_fetch(ctx, entityid);
        if (entity->type != M2O_ENTITY_DUMMY_PED && entity->type != M2O_ENTITY_PLAYER_PED) continue;
        auto ped = m2o_ped_get(entity);

        if (ped->vehicle == event->entity->id && ped->CEntity) {
            mod_log("[info] removing ped from the vehicle\n");

            // TODO: make it actually remove ped from the seat, for now just delete everyone
            M2::Wrappers::DestroyEntity(ped->CEntity, M2::eEntityType::MOD_ENTITY_PED);
            ped->CEntity = NULL;
        }
    });

    M2::Wrappers::DestroyEntity(car->CEntity, M2::eEntityType::MOD_ENTITY_CAR);
    m2o_car_free(car);
}

// =======================================================================//
// !
// ! Update/sync entity events
// !
// =======================================================================//

/**
 * The entity in our stream zone gets updated
 */
void m2o_callback_car_update(librg_event_t *event) {
    auto car = m2o_car_get(event->entity);

    // make sure we have all objects
    mod_assert(car && car->CEntity);
    librg_data_rptr(event->data, &car->stream, sizeof(car->stream));

    // car->CCar->SetPos(event->entity->position);
    // car->CCar->SetRot(car->stream.rotation);

    if (zplm_vec3_mag2(car->stream.speed) > VEHICLE_THRESHOLD_FOR_SPEED) {
        car->CCar->m_pVehicle.SetSpeed(car->stream.speed);
    } else {
        car->CCar->m_pVehicle.SetSpeed(zplm_vec3f_zero());
    }

    car->CCar->m_pVehicle.SetSteer(car->stream.steer);

    m2o_car_target_position_set(car, event->entity->position, 1.0f / M2O_TICKRATE_SERVER, true, car->stream.speed.z);
    m2o_car_target_rotation_set(car, car->stream.rotation, 1.0f / M2O_TICKRATE_SERVER);
}

/**
 * The entity is streamed by us, and we are sending an update with new data
 */
void m2o_callback_car_clientstream(librg_event_t *event) {
    auto car = m2o_car_get(event->entity);
    mod_assert(car && car->CEntity);

    event->entity->position = car->CEntity->GetPosition();
    car->stream.steer       = car->CCar->m_pVehicle.m_fSteer;
    car->stream.speed       = car->CCar->m_pVehicle.m_vSpeed;
    car->stream.rotation    = zplm_vec3_to_degrees(
        zplm_quat_to_eular(car->CEntity->GetRotation())
    );

    librg_data_wptr(event->data, &car->stream, sizeof(car->stream));
}

// =======================================================================//
// !
// ! Interpolation
// !
// =======================================================================//

void m2o_callback_car_interpolate(librg_entity_t *entity) {
    auto car = m2o_car_get(entity);

    if (!car || !car->gameptr) {
        mod_log("[warning] calling car interpolate w/o proper car object");
        return;
    }

    auto player_ped = m2o_ped_get(mod.player);

    if (!player_ped || !player_ped->gameptr) {
        mod_log("[warning] calling interpolation w/o player ped");
        return;
    }

    // if (player_ped->vehicle != entity->id) {
        m2o_car_target_position_update(car);
        m2o_car_target_rotation_update(car);
    // }
    // else {
    //     // Otherwize make sure we have no interpolation stuff stored
    //     car->interp.pos.finishTime = 0;
    //     car->interp.rot.finishTime = 0;
    // }
}

void m2o_car_callbacks_init() {

    // =======================================================================//
    // !
    // ! Local car events
    // !
    // =======================================================================//

    librg_event_add(ctx, M2O_CAR_ENTER, [](librg_event_t *event) {
        auto vehicle = (M2::C_Entity *)event->user_data;
        auto ped     = m2o_ped_get(mod.player);

        mod_assert(vehicle && ped);
        u8 seat = *(u8*)event->data;

        mod_entity_iterate(ctx, LIBRG_ENTITY_ALIVE, [&](librg_entity_t *entity) {
            if (entity->type != M2O_ENTITY_CAR) return;
            auto car = m2o_car_get(entity);

            if (car->CEntity == vehicle) {
                /* set the passanger data */
                ped->state      = PED_IN_CAR;
                ped->seat       = seat;
                ped->vehicle    = entity->id;

                /* if we are being a driver */
                if (seat == 1) {
                    mod.player->flags |= M2O_ENTITY_DRIVER;
                }

                mod_message_send(ctx, M2O_CAR_ENTER, [&](librg_data_t *data) {
                    librg_data_wu32(data, entity->id);
                    librg_data_wu8(data, seat);
                });
            }
        });
    });

    librg_event_add(ctx, M2O_CAR_EXIT, [](librg_event_t *event) {
        auto ped = m2o_ped_get(mod.player);

        /* force remove driving if we are */
        mod.player->flags &= ~M2O_ENTITY_DRIVER;

        ped->seat    = 0;
        ped->vehicle = M2O_INVALID_ENTITY;
        ped->state   = PED_ON_GROUND;

        mod_message_send(ctx, M2O_CAR_EXIT, nullptr);
    });

    // =======================================================================//
    // !
    // ! Remote car events
    // !
    // =======================================================================//

    librg_network_add(ctx, M2O_CAR_ENTER, [](librg_message_t *msg) {
        auto player  = librg_entity_fetch(ctx, librg_data_rent(msg->data));
        auto vehicle = librg_entity_fetch(ctx, librg_data_rent(msg->data));
        mod_assert(player && vehicle);

        auto seat = librg_data_ru8(msg->data);
        auto ped  = m2o_ped_get(player); mod_assert_msg(ped->CEntity, "trying to put ped in invalid car");
        auto car  = m2o_car_get(vehicle); mod_assert_msg(car->CEntity, "trying to put invalid ped in car");

        mod_log("[info] putting ped: %u in the car: %u\n", player->id, vehicle->id);

        ped->state   = PED_IN_CAR;
        ped->vehicle = vehicle->id;
        ped->seat    = seat;

        // TODO: fix crash on car enter
        // // TODO: refactor this stuff
        // M2::C_SyncObject *pSyncObject = nullptr;
        // ped->CHuman->GetScript()->ScrDoAction(
        //     &pSyncObject,
        //     (M2::C_Vehicle *)car->CEntity, /* weird cast there, casting incompatible C_Car to C_Vehicle, probably argument should be C_Car */
        //     true,
        //     (M2::E_VehicleSeat)seat,
        //     true
        // );
    });

    librg_network_add(ctx, M2O_CAR_EXIT, [](librg_message_t *msg) {
        auto player  = librg_entity_fetch(ctx, librg_data_rent(msg->data)); mod_assert(player);
        auto ped     = m2o_ped_get(player); mod_assert(ped && ped->CEntity);
        auto vehicle = librg_entity_fetch(ctx, ped->vehicle);
        auto car     = m2o_car_get(vehicle); mod_assert(car && car->CEntity);

        mod_log("[info] removing ped: %u from the car: %u\n", player->id, ped->vehicle);

        // M2::C_SyncObject *pSyncObject = nullptr;
        // ped->CHuman->GetScript()->ScrDoAction(
        //     &pSyncObject,
        //     (M2::C_Vehicle *)car->CEntity,
        //     true,
        //     (M2::E_VehicleSeat)ped->seat,
        //     true
        // );

        ped->state   = PED_ON_GROUND;
        ped->vehicle = M2O_INVALID_ENTITY;
        ped->seat    = 0;
    });
}
