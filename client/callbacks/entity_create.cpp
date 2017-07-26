#include "../client.h"

/**
* Entity add to streamer
*/
void entity_create(librg::events::event_t* evt)
{
    librg::core::log("creating entity");
    auto event      = (librg::events::event_create_t*) evt;
    auto transform  = event->entity.component<librg::transform_t>();
    auto inter      = event->entity.assign<librg::interpolable_t>(*transform);

    switch (event->type)
    {
        case TYPE_PLAYER: {
            DWORD model = GAMEPLAY::GET_HASH_KEY((char *)"mp_m_freemode_01");

            if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
            {
                STREAMING::REQUEST_MODEL(model);
                while (!STREAMING::HAS_MODEL_LOADED(model))	WAIT(0);

                event->entity.assign<ped_t>();
                event->entity.assign<gamedata_t>(
                    PED::CREATE_PED(26, model, transform->position.X, transform->position.Y, transform->position.Z, transform->rotation.X, false, false)
                );

                // todo: add invincibility
            }
        } break;
        case TYPE_VEHICLE: {
            librg::core::log("creating vehicle");

            STREAMING::SET_VEHICLE_POPULATION_BUDGET(3000);

            Hash model = GAMEPLAY::GET_HASH_KEY((char*)"BULLET");
            /*
            if (!STREAMING::IS_MODEL_IN_CDIMAGE(model) || !STREAMING::IS_MODEL_VALID(model))
            {
                std::cout << "[CVehicleEntity::CreateVehicle] invalid vehicle for " << Information.Id << " reverting to dilettante." << std::endl;
                model = GAMEPLAY::GET_HASH_KEY("dilettante");
                Data.Model = "dilettante";
            }
            */

            STREAMING::REQUEST_MODEL(model);

            while (!STREAMING::HAS_MODEL_LOADED(model)) {
                WAIT(100);
            }

            auto game_entity = VEHICLE::CREATE_VEHICLE(model, transform->position.X, transform->position.Y, transform->position.Z, transform->rotation.X, FALSE, TRUE);

            
            STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

            ENTITY::FREEZE_ENTITY_POSITION(game_entity, true);
            STREAMING::REQUEST_COLLISION_AT_COORD(transform->position.X, transform->position.Y, transform->position.Z);
            ENTITY::SET_ENTITY_COORDS_NO_OFFSET(game_entity, transform->position.X, transform->position.Y, transform->position.Z, false, false, false);

            ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(game_entity, true);
            ENTITY::SET_ENTITY_COLLISION(game_entity, true, false);
            //ENTITY::SET_ENTITY_ROTATION(game_entity, Data.Rotation.x, Data.Rotation.y, Data.Rotation.z, 2, true);
            ENTITY::SET_ENTITY_QUATERNION(game_entity, 
                transform->rotation.X,
                transform->rotation.Y,
                transform->rotation.Z,
                transform->rotation.W
            );

            VEHICLE::SET_TAXI_LIGHTS(game_entity, true);

            // Vehicle Number Plate
            VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(game_entity, (char *)"lol");

            const int Class = VEHICLE::GET_VEHICLE_CLASS(game_entity);
            if (Class == 18 || Class == 17 || Class == 15 || Class == 16 || Class == 20 || Class == 14)
            {
                VEHICLE::SET_VEHICLE_MOD(game_entity, 48, 0, 0);
                VEHICLE::SET_VEHICLE_LIVERY(game_entity, 0);
            }

            /*
            // Vehicle Mods
            for (int i = 0; i < SizeOfArray(Data.Mods); i++)
            {
                VEHICLE::SET_VEHICLE_MOD(Game.Vehicle, i, Data.Mods[i].index, false);
                if (i = 48)
                    VEHICLE::SET_VEHICLE_LIVERY(Game.Vehicle, Data.Mods[i].index);
            }

            // Vehicle Colors
            if (!Data.Colors[0].custom)
            {
                VEHICLE::SET_VEHICLE_MOD_COLOR_1(Game.Vehicle, Data.Colors[0].type, Data.Colors[0].color, 0);
            }
            else
            {
                VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(Game.Vehicle, Data.Colors[0].customCol.Red, Data.Colors[0].customCol.Green, Data.Colors[0].customCol.Blue);
            }

            if (!Data.Colors[1].custom)
            {
                VEHICLE::SET_VEHICLE_MOD_COLOR_2(Game.Vehicle, Data.Colors[1].type, Data.Colors[1].color);
            }
            else
            {
                VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(Game.Vehicle, Data.Colors[1].customCol.Red, Data.Colors[1].customCol.Green, Data.Colors[1].customCol.Blue);
            }

            // Engine State (Forces Non-Vanilla engine state)
            if (Data.ForceEngineState > -1)
            {
                VEHICLE::SET_VEHICLE_ENGINE_ON(Game.Vehicle, Data.ForceEngineState, true, true);
                VEHICLE::SET_VEHICLE_UNDRIVEABLE(Game.Vehicle, !Data.ForceEngineState);
            }

            VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(Game.Vehicle, Data.PlateStyle);

            for (int i = 1; i < SizeOfArray(Data.Extras); i++)
            {
                VEHICLE::SET_VEHICLE_EXTRA(Game.Vehicle, i, Data.Extras[i].enabled);
            }

            VEHICLE::SET_VEHICLE_DOORS_LOCKED(Game.Vehicle, Data.DoorsLockState);

            VEHICLE::SET_VEHICLE_ENGINE_HEALTH(Game.Vehicle, Data.EngineHealth);

            VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(Game.Vehicle, Data.FuelTankHealth);

            VEHICLE::SET_TAXI_LIGHTS(Game.Vehicle, Data.TaxiLight);

            VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(Game.Vehicle, 0, Data.RightIndicator);
            VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(Game.Vehicle, 1, Data.LeftIndicator);

            VEHICLE::SET_VEHICLE_INTERIORLIGHT(Game.Vehicle, Data.InteriorLight);

            //VEHICLE::SET_VEHICLE_SIREN_SOUND(Game.Vehicle, Data.SirenSound);
            */
            ENTITY::FREEZE_ENTITY_POSITION(game_entity, false);
            ENTITY::SET_ENTITY_DYNAMIC(game_entity, true);

            event->entity.assign<gamedata_t>(game_entity);
            event->entity.assign<vehicle_t>();
        }break;
    }

    librg::core::log("entity succeessfully created");
}

