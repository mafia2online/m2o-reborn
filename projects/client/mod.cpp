#define M2O_CLIENT
#include "m2o_config.h"

#include <string>
#include <future>
#include <functional>

#define LIBRG_IMPLEMENTATION
#include "librg.h"
#include "librg_ext.h"

#define m2sdk_log zpl_printf
#define MAFIA_SDK_IMPLEMENTATION
#include "m2sdk.h"

void* CIE_Alloc(size_t siz) {
    DWORD functionAddress = 0x00401830;
    __asm {
        push siz
        call functionAddress
        add esp, 0x4
    };
}

#include "m2o_client.h"
#include "m2o_types.h"

#include "mod/mod.h"
#include "mod/vehicle.h"
#include "mod/pedestrian.h"

// =======================================================================//
// !
// ! General mod implementation part
// !
// =======================================================================//

void mod_install() {
    mod_log("[info] mod_init\n");

    auto tmm = M2::C_TickedModuleManager::Get();
    using tc = void (M2::C_TickedModule::*)(M2::I_TickedModuleCallEventContext &);

    tmm->AddAction(M2::TickedModuleEvent::EVENT_GAME_INIT,        500,  &m2o, (tc)(&m2o_module::init), -1.0f, 0, 0);
    tmm->AddAction(M2::TickedModuleEvent::EVENT_TICK,             400,  &m2o, (tc)(&m2o_module::tick), -1.0f, 0, 0);
    tmm->AddAction(M2::TickedModuleEvent::EVENT_LOADING_STARTED,  1720, &m2o, (tc)(&m2o_module::load_start), -1.0f, 0, 0);
    tmm->AddAction(M2::TickedModuleEvent::EVENT_LOADING_FINISHED, 1720, &m2o, (tc)(&m2o_module::load_finish), -1.0f, 0, 0);

    M2::AttachHandler(M2_EVENT_MOD_MESSAGE, [](m2sdk_event *data) {
        auto message = (int)data->arg1;

        switch (message) {
            case M2::E_PlayerMessage::MESSAGE_MOD_ENTER_CAR: {
                mod_log("[game-event] ped start to enter vehicle\n");
            } break;

            case M2::E_PlayerMessage::MESSAGE_MOD_BREAKIN_CAR: {
                mod_log("[game-event] Start to breakin vehicle\n");
            } break;

            case M2::E_PlayerMessage::MESSAGE_MOD_LEAVE_CAR: {
                mod_log("[game-event] Start to leave car\n");

                librg_event_t event = { 0 }; event.entity = mod.player;
                librg_event_trigger(ctx, M2O_CAR_EXIT, &event);
            } break;
        }
    });

    M2::AttachHandler(M2_EVENT_GAME_MESSAGE, [](m2sdk_event *data) {
        auto message = (M2::C_EntityMessage *)data->arg1;

        switch (message->m_dwMessage) {
            case M2::E_HumanMessage::MESSAGE_GAME_ENTER_EXIT_VEHICLE_DONE: {
                mod_log("[game-event] Enter/Exit Vehicle done\n");
            } break;

            default: {
                mod_log("[game-event] unknown event %d\n", (int)message->m_dwMessage);
            } break;
        }
    });

    M2::AttachHandler(M2_EVENT_CAR_ENTER_REQUEST, [](m2sdk_event *data) {
        data->arg5 = (void *)true;
    });

    M2::AttachHandler(M2_EVENT_CAR_HOOD_OPEN_REQUEST, [](m2sdk_event *data) {
        data->arg5 = (void *)true;
    });

    M2::AttachHandler(M2_EVENT_CAR_HOOD_CLOSE_REQUEST, [](m2sdk_event *data) {
        data->arg5 = (void *)true;
    });

    M2::AttachHandler(M2_EVENT_CAR_TRUNK_OPEN_REQUEST, [](m2sdk_event *data) {
        data->arg5 = (void *)true;
    });

    M2::AttachHandler(M2_EVENT_CAR_TRUNK_CLOSE_REQUEST, [](m2sdk_event *data) {
        data->arg5 = (void *)true;
    });

    M2::AttachHandler(M2_EVENT_CAR_FUELTANK_REQUEST, [](m2sdk_event *data) {
        data->arg5 = (void *)true;
    });

    M2::AttachHandler(M2_EVENT_CAR_ENTER, [](m2sdk_event *data) {
        auto player = (M2::C_Player2 *)data->arg1;
        auto car    = (M2::C_Car *)data->arg2;
        auto seat   = (int)data->arg3;

        mod_log("[game-event] ped entering the car on seat: %d\n", seat);

        librg_event_t event = { 0 };
        u8 iseat = seat + 1;

        event.entity = mod.player;
        event.user_data = (void *)car;
        event.data = (librg_data_t*)&iseat;

        librg_event_trigger(ctx, M2O_CAR_ENTER, &event);
    });
}

void mod_connect(const char *host, int port) {
    librg_address_t addr = { port, (char *)host };
    librg_network_start(ctx, addr);
}

void mod_disconnect() {
    librg_network_stop(ctx);
}

bool mod_connected() {
    return librg_is_connected(ctx);
}

void mod_nickname_set(const char *username) {
    mod_assert(username);
    mod_message_send(ctx, M2O_USER_SET_NAME, [&](librg_data_t *data) {
        librg_data_wu8(data, zpl_strlen(username));
        librg_data_wptr(data, (void *)username, zpl_strlen(username));
    });
}

void mod_respawn() {
    M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetTime(0.5); /* 0.0 .. 1.0 - time of the day */

    auto ped = m2o_ped_get(mod.player);

    if (M2::C_SDSLoadingTable::Get()) {
        M2::C_SDSLoadingTable::Get()->ProcessLine("free_joe_load");
        M2::C_SDSLoadingTable::Get()->ProcessLine("free_summer_load");

        M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetDayTemplate("DT_RTRrainy_day_evening");
        mod_log("[info] setting day template: %s\n", "DT_RTRclear_day_afternoon");
    }

    /* Disable ambiant peds */
    M2::Wrappers::SwitchFarAmbiants(false);
    M2::Wrappers::SwitchGenerators(false);

    /* Lock to prevent actions while respawning */
    ped->CPlayer->LockControls(true);

    /* Resetting player */
    ped->CHuman->GetScript()->SetHealth(720.0f);
    // ped->CEntity->SetPosition(vec3f(-421.75f, 479.31f, 0.05f));
    ped->CEntity->SetPosition(vec3f(240.641052f, 699.223755f, -24.153996f));

    /* Enabling controls */
    ped->CPlayer->LockControls(false);
}

// =======================================================================//
// !
// ! General mod events
// !
// =======================================================================//

void m2o_module::init(M2::I_TickedModuleCallEventContext &) {
    mod_log("[GameModule]: EventGameInit\n");

    // call late (post window create) platform-related code
    platform_init();

    // therotically we shouldn't call it here but because it's a
    // sync object it's fine itll work but the local player isn't created just yet.
    M2::C_GuiGame::Get()->StartGame(M2::E_iLoadType::TYPE_NEW_GAME, 0, 1, "freeride", false);
    M2::C_GameGuiModule::Get()->FaderFadeIn(1);

    ctx = new librg_ctx_t;
    zpl_zero_item(ctx);

    // setup manual client mode
    ctx->tick_delay     = 1000.0f / M2O_TICKRATE_CLIENT;
    ctx->mode           = LIBRG_MODE_CLIENT;
    ctx->world_size     = zplm_vec3f(5000.0f, 5000.0f, 0);
    ctx->max_entities   = M2O_ENTITY_LIMIT;

    librg_option_set(LIBRG_PLATFORM_ID, M2O_PLATFORM_ID);
    librg_option_set(LIBRG_PLATFORM_BUILD, M2O_VERSION_NUMBER);
    librg_option_set(LIBRG_PLATFORM_PROTOCOL, M2O_VERSION_PROTOCOL);

    librg_init(ctx);
    cef_init();

    /**
     * Connection handling callbacks
     */
    librg_event_add(ctx, LIBRG_CONNECTION_REQUEST, [](librg_event_t *event) {
        // TODO: password sending
    });

    librg_event_add(ctx, LIBRG_CONNECTION_ACCEPT, [](librg_event_t *event) {
        mod_log("[info] connected to the server\n");

        /* setup default timeout */
        enet_peer_timeout(event->peer, 10, 5000, 10000);

        mod.player = event->entity;
        mod.player->user_data = m2o_ped_alloc(M2::C_Game::Get()->GetLocalPed());

        mod_nickname_set("the playah");
        mod_respawn();
    });

    librg_event_add(ctx, LIBRG_CONNECTION_REFUSE, [](librg_event_t *event) {
        mod_log("[info] connection refused\n");
    });

    librg_event_add(ctx, LIBRG_CONNECTION_DISCONNECT, [](librg_event_t *event) {
        mod_log("[info] disconnected form the server\n");

        M2::C_Game::Get()->GetLocalPed()->LockControls(true);
        // object->SetPosition(zplm_vec3_zero()); // creates black textures :O

        // if we have been disconnected before being connected
        if (mod.player) {
            m2o_ped_free((m2o_ped *)mod.player->user_data);
        }
    });

    /**
     * General routing callbacks
     */
    librg_event_add(ctx, LIBRG_ENTITY_CREATE, [](librg_event_t *event) {
        switch (event->entity->type) {
            case M2O_ENTITY_PLAYER_PED:
            case M2O_ENTITY_DUMMY_PED: { m2o_callback_ped_create(event); } break;
            case M2O_ENTITY_CAR: { m2o_callback_car_create(event); } break;
        }
    });

    librg_event_add(ctx, LIBRG_ENTITY_UPDATE, [](librg_event_t *event) {
        switch (event->entity->type) {
            case M2O_ENTITY_PLAYER_PED:
            case M2O_ENTITY_DUMMY_PED: { m2o_callback_ped_update(event); } break;
            case M2O_ENTITY_CAR: { m2o_callback_car_update(event); } break;
        }
    });

    librg_event_add(ctx, LIBRG_ENTITY_REMOVE, [](librg_event_t *event) {
        switch (event->entity->type) {
            case M2O_ENTITY_PLAYER_PED:
            case M2O_ENTITY_DUMMY_PED: { m2o_callback_ped_remove(event); } break;
            case M2O_ENTITY_CAR: { m2o_callback_car_remove(event); } break;
        }
    });

    librg_event_add(ctx, LIBRG_CLIENT_STREAMER_UPDATE, [](librg_event_t *event) {
        switch (event->entity->type) {
            case M2O_ENTITY_PLAYER_PED:
            case M2O_ENTITY_DUMMY_PED: { m2o_callback_ped_clientstream(event); } break;
            case M2O_ENTITY_CAR: { m2o_callback_car_clientstream(event); } break;
        }
    });

    /**
     * Client streaming callbacks
     */
    librg_event_add(ctx, LIBRG_CLIENT_STREAMER_ADD, [](librg_event_t *event) {
        mod_log("[info] adding an entity %d to clientstream\n", event->entity->id);

        /* increase stats info, and remove particlar entity from being interpolated */
        mod.stats.streamed_entities++;
        event->entity->flags &= ~M2O_ENTITY_INTERPOLATED;
    });

    librg_event_add(ctx, LIBRG_CLIENT_STREAMER_REMOVE, [](librg_event_t *event) {
        mod_log("[info] removing an entity %d from clientstream\n", event->entity->id);

        /* decrease stats info, and add entity back to being interpolated */
        mod.stats.streamed_entities--;
        switch (event->entity->type) {
            case M2O_ENTITY_CAR:
            case M2O_ENTITY_DUMMY_PED:
            case M2O_ENTITY_PLAYER_PED:
                event->entity->flags |= M2O_ENTITY_INTERPOLATED;
                break;
        }
    });

    /**
     * Custom/specific callbacks
     */
    librg_network_add(ctx, M2O_USER_SET_NAME, [](librg_message_t *msg) {
        auto entity = librg_entity_fetch(msg->ctx, librg_data_ru32(msg->data));
        mod_assert(entity);

        u8 strsize = librg_data_ru8(msg->data);
        auto ped   = m2o_ped_get(entity);
        librg_data_rptr(msg->data, ped->name, strsize);

        mod_log("set new name for client %u: %s\n", entity->id, ped->name);
    });

    m2o_car_callbacks_init();

    // discord_init();
}

void m2o_module::load_start(M2::I_TickedModuleCallEventContext &) {
    mod_log("[GameModule]: EventLoadingStarted\n");
}

void m2o_module::load_finish(M2::I_TickedModuleCallEventContext &) {
    mod_log("[GameModule]: EventLoadingFinished\n");
    std::thread([]() {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(1500)
        );

        mod_respawn();
    }).detach();
}

void m2o_module::tick(M2::I_TickedModuleCallEventContext &) {
    mod.last_delta  = zpl_time_now() - mod.last_update;
    mod.last_update = zpl_time_now();

    // tick networking
    platform_tick();
    librg_tick(ctx);

    // interpolate all entities
    librg_entity_iterate(ctx, (LIBRG_ENTITY_ALIVE | M2O_ENTITY_INTERPOLATED), [](librg_ctx_t *ctx, librg_entity_t *entity) {
        switch (entity->type) {
            case M2O_ENTITY_PLAYER_PED:
            case M2O_ENTITY_DUMMY_PED: { m2o_callback_ped_interpolate(entity); } break;
            case M2O_ENTITY_CAR: { m2o_callback_car_interpolate(entity); } break;
        }
    });

    // constnaly set our health to top (DEBUG)
    if (mod.player) {
        ((M2::C_Human2 *)M2::C_Game::Get()->GetLocalPed())->GetScript()->SetHealth(720.0f);
    }

    /* show/hide mouse */
    if (input_key_down(VK_F1)) {
        input_block_set(!input_block_get());
    }

    /* create a car */
    if (input_key_down(VK_F2)) {
        mod_message_send(ctx, M2O_CAR_CREATE, nullptr);
    }

    /* connect to the server */
    if (input_key_down(VK_F5) && !mod.spawned) {
        mod_connect("localhost", 27010);
        mod.spawned = true;
    }


    static M2::C_Entity *ent;
    if (input_key_down(VK_F3)) {
        if (!ent) {
            ent = M2::Wrappers::CreateEntity(M2::eEntityType::MOD_ENTITY_PED, 10);
        }
        if (ent) {
            auto pos = reinterpret_cast<M2::C_Human2*>(M2::C_Game::Get()->GetLocalPed())->GetPos();
            if (ent) {
                ent->SetPosition(pos);
            }
            pos.x += 1.f;
            pos.y += 1.f;
            pos.z -= 2.f;
            ent->SetPosition(pos);
        }
    }

    if (input_key_down(VK_F4) && mod.spawned) {
        if (ent) {
            static void* moveCommand = nullptr;
            if (!moveCommand) {
                //moveCommand = zpl_malloc(0x58);
                // zpl_zero_size(moveCommand, 0x58);
                //mod_log("moveCommand address = 0x%x size: %d\n", ((uintptr_t)moveCommand), sizeof(moveCommand));
                moveCommand = CIE_Alloc(0x58);
                //moveCommand = new char[0x58];
                ((M2::C_Human2*)ent)->AddCommand(M2::E_Command::COMMAND_MOVEDIR, moveCommand);
            }


            mod_log("moveCommand address = 0x%x size: %d\n", ((uintptr_t)moveCommand), sizeof(char[0x58]));

            if (((M2::C_Command*)moveCommand)->m_iCommandID == 1) {
                M2::S_HumanCommandMoveDir* cmd = (M2::S_HumanCommandMoveDir*)moveCommand;
                cmd->moveSpeed = 0;
                cmd->speedMultiplier = 0.1f;
                cmd->potentialMoveVector = { 1.f, 1.f };
            }

            ((M2::C_Human2*)ent)->m_iCurrentCommand = 1;
            ((M2::C_Human2*)ent)->m_aCommandsArray[1].m_pCommand = moveCommand;
        }

        //M2::Wrappers::lua::Execute("game.sds:ActivateStreamMapLine(\"11_galante_load\")");
        //M2::Wrappers::lua::Execute("icon = game.entitywrapper:GetEntityByName(\"RTR_POUTA1_00\")");
        //M2::Wrappers::lua::Execute("icon:Activate()");
        //M2::Wrappers::lua::Executef("icon:SetPos(Math:newVector(%f, %f, %f))", pos.x, pos.y, pos.z);
    }

    if (GetAsyncKeyState(VK_F7) & 0x1) {
        M2::C_Entity *test = M2::C_WrappersList::Get()->GetEntityByName("RTR_POUTA1_00");
        if (test) {
            mod_log("entity : 0x%p\n", test);
        }
        else {
            mod_log("null ptr\n");
        }
    }

    if (GetAsyncKeyState(VK_F6) & 0x1) {
        char *fileName = M2::C_Tables::Get()->GetModelFileName(M2::EntityTypes::Entity_Car, 5);
        mod_log("Veh file : %s\n", fileName);

        M2::CModelManager::Get().OpenModel(M2::EntityTypes::Entity_Car, fileName);
        M2::C_Model* model = M2::CModelManager::Get().m_pModel;
        mod_log("Model ptr: 0x%X", model);
    }

    if (input_key_down(VK_F9)) {
        vec3_t curpos, tarpos;
        curpos = reinterpret_cast<M2::C_Entity*>(M2::C_Game::Get()->GetLocalPed())->GetDirection();
        tarpos = reinterpret_cast<M2::C_Entity*>(M2::C_Game::Get()->GetLocalPed())->GetPosition();

        static M2::C_SyncObject *sync = NULL;
        ((M2::C_Human2 *)ent)->GetScript()->ScrMoveV(&sync, tarpos, M2::HUMAN_MOVE_MODE_SPRINT, curpos, true);

        print_posm(curpos, "direction");
        mod_log("moving ped to your location");
    }
}
