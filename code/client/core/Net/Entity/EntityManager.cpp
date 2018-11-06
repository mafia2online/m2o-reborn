
#include <Net/Entity/EntityManager.h>

#include <Net/Entity/NetVehicle.h>
#include <Net/Entity/NetPed.h>

namespace nmd::net 
{
    EntityManager::EntityManager(librg_ctx_t *ctx) : ctx(ctx) {}

    void EntityManager::CreateCar(librg_event_t *event)
    {
        // entitymanager pool ?
        auto car = new NetVehicle;
        memset(car, 0, sizeof(NetVehicle));

        car->model = librg_data_ru16(event->data);
        car->state = (car_state)librg_data_ru8(event->data);
        librg_data_rptr(event->data, &car->stream, sizeof(car->stream));

        event->entity->flags |= M2O_ENTITY_INTERPOLATED;
        event->entity->user_data = car;

#if 0
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
#endif
    }

    void EntityManager::CreatePed(librg_event_t *event)
    {
        // pool
        auto ped = new NetPed;
        memset(ped, 0, sizeof(NetPed));

        ped->model = librg_data_ru16(event->data);
        ped->state = (ped_state)librg_data_ru8(event->data);

        auto namelen = librg_data_ru8(event->data);
        librg_data_rptr(event->data, ped->name, namelen);

        /* handle state specific casese*/
        switch (ped->state) {
            case PED_ON_GROUND:
                break;
            case PED_IN_CAR: {
                ped->vehicle = librg_data_ru32(event->data);
                ped->seat = librg_data_ru8(event->data);
            } break;
        }

        event->entity->flags |= M2O_ENTITY_INTERPOLATED;
        event->entity->user_data = ped;

        #if 0
       M2::C_Entity *entity = M2::Wrappers::CreateEntity(M2::eEntityType::MOD_ENTITY_PED, ped->model);

        if (entity->IsActive()) {
            print_posm(event->entity->position, "[info] creating ped at");
            ped->CEntity = entity;
            ped->CEntity->SetPosition(event->entity->position);

            /* remote peds need can be killed only from remote client side */
            ped->CHuman->GetScript()->SetDemigod(true);
        } else {
            mod_log("[warning] could not create a ped for entity: %d\n", event->entity->id);
        }

        { /* healthbar and nickname stuff */
            ped->nickname_value =
            gfx_create_text(0, M2O_PED_NICKNAME_FONT_SIZE, ped->name, vec4f(255, 255, 255, 255));
            ped->healthbar_base = gfx_create_rect(0, 0, M2O_PED_HEALTHBAR_WIDTH,
                                                  M2O_PED_HEALTHBAR_HEIGH, vec4f(50, 50, 50, 170));
            ped->healthbar_value =
            gfx_create_rect(M2O_PED_HEALTHBAR_PADDING, M2O_PED_HEALTHBAR_PADDING,
                            M2O_PED_HEALTHBAR_WIDTH - M2O_PED_HEALTHBAR_PADDING * 2,
                            M2O_PED_HEALTHBAR_HEIGH - M2O_PED_HEALTHBAR_PADDING * 2,
                            vec4f(255, 122, 122, 250));

            gfx_render_add(ped->nickname_value, 24);
            gfx_render_add(ped->healthbar_base, 22);
            gfx_render_add(ped->healthbar_value, 22);
        }
#endif
    }

    void EntityManager::Init() const 
    {
        // entitymanager pool ?
        librg_event_add(ctx, LIBRG_ENTITY_CREATE, [](librg_event_t *event) 
        {
            switch (event->entity->type) 
            {
                case M2O_ENTITY_PLAYER_PED:
                case M2O_ENTITY_DUMMY_PED: 
                {
                    CreatePed(event);
                } break;
                case M2O_ENTITY_CAR:
                {
                    CreateCar(event);
                } break;
            }
        });

        librg_event_add(ctx, LIBRG_ENTITY_UPDATE, [](librg_event_t *event) {
            switch (event->entity->type) {
                case M2O_ENTITY_PLAYER_PED:
                case M2O_ENTITY_DUMMY_PED: {
                    // m2o_callback_ped_update(event);
                } break;
                case M2O_ENTITY_CAR: {
                    // m2o_callback_car_update(event);
                } break;
            }
        });

        librg_event_add(ctx, LIBRG_ENTITY_REMOVE, [](librg_event_t *event) {
            switch (event->entity->type) {
                case M2O_ENTITY_PLAYER_PED:
                case M2O_ENTITY_DUMMY_PED: {
                    //  m2o_callback_ped_remove(event);
                } break;
                case M2O_ENTITY_CAR: {
                    //  m2o_callback_car_remove(event);
                } break;
            }
        });

        librg_event_add(ctx, LIBRG_CLIENT_STREAMER_UPDATE, [](librg_event_t *event) {
            switch (event->entity->type) {
                case M2O_ENTITY_PLAYER_PED:
                case M2O_ENTITY_DUMMY_PED: {
                    // m2o_callback_ped_clientstream(event);
                } break;
                case M2O_ENTITY_CAR: {
                    //  m2o_callback_car_clientstream(event);
                } break;
            }
        });

        // entity streaming managenment

        librg_event_add(ctx, LIBRG_CLIENT_STREAMER_ADD, [](librg_event_t *event)
        {
            printf("[info] adding an entity %d to clientstream\n", event->entity->id);

            /* increase stats info, and remove particlar entity from being interpolated */
            //  mod.stats.streamed_entities++;
            event->entity->flags &= ~M2O_ENTITY_INTERPOLATED;
        });

        librg_event_add(ctx, LIBRG_CLIENT_STREAMER_REMOVE, [](librg_event_t *event) 
        {
            printf("[info] removing an entity %d from clientstream\n", event->entity->id);

            /* decrease stats info, and add entity back to being interpolated */
            // mod.stats.streamed_entities--;
            switch (event->entity->type) {
                case M2O_ENTITY_CAR:
                case M2O_ENTITY_DUMMY_PED:
                case M2O_ENTITY_PLAYER_PED:
                    event->entity->flags |= M2O_ENTITY_INTERPOLATED;
                    break;
            }
        });
    }

    void EntityManager::CreateLocalNetPlayer(librg_entity_t *p, C_Player2 *game_player) {
        // for now
        local_player->user_data = m2o_ped_alloc(game_player);
    }

} // namespace nmd::net
