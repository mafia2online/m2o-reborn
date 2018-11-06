
#include <Net/Entity/EntityManager.h>

#include <librg.h>
#include "m2o_types.h"

namespace nmd::net 
{
    EntityManager::EntityManager(librg_ctx_t *ctx) : ctx(ctx)
    {
        
    }

    void EntityManager::Init()
    {
        // entity managenment
        librg_event_add(ctx, LIBRG_ENTITY_CREATE, [](librg_event_t *event) 
        {
            switch (event->entity->type) 
            {
            case M2O_ENTITY_PLAYER_PED:
            case M2O_ENTITY_DUMMY_PED: {
              //  m2o_callback_ped_create(event);
            } break;
            case M2O_ENTITY_CAR: {
                //m2o_callback_car_create(event);
            } break;
            }
        });

        librg_event_add(ctx, LIBRG_ENTITY_UPDATE, [](librg_event_t *event) {
            switch (event->entity->type) {
            case M2O_ENTITY_PLAYER_PED:
            case M2O_ENTITY_DUMMY_PED: {
                //m2o_callback_ped_update(event);
            } break;
            case M2O_ENTITY_CAR: {
                //m2o_callback_car_update(event);
            } break;
            }
        });

        librg_event_add(ctx, LIBRG_ENTITY_REMOVE, [](librg_event_t *event) {
            switch (event->entity->type) 
            {
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

    void EntityManager::CreateLocalNetPlayer(librg_entity_t *p, C_Player2* game_player)
    {
        // for now
        local_player->user_data = m2o_ped_alloc(game_player);
    }

} // namespace nmd::net
