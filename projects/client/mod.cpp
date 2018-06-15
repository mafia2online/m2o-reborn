#define M2O_CLIENT
#include "m2o_config.h"

#include <string>
#include <future>
#include <functional>

#define LIBRG_IMPLEMENTATION
#include "librg.h"
#include "librg_ext.h"
#include "m2sdk.h"

#include "m2o_api.h"
#include "m2o_types.h"

#include "core/mod.h"
#include "core/vehicle.h"
#include "core/pedestrian.h"

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
                librg_event_trigger(ctx, MOD_CAR_EXIT_START, &event);
            } break;
        }
    });

    M2::AttachHandler(M2_EVENT_GAME_MESSAGE, [](m2sdk_event *data) {
        auto message = (M2::C_EntityMessage *)data->arg1;

        switch (message->m_dwMessage) {
            case M2::E_HumanMessage::MESSAGE_GAME_ENTER_EXIT_VEHICLE_DONE: {
                mod_log("[game-event] Enter/Exit Vehicle done\n");

                librg_event_t event = { 0 }; event.entity = mod.player;
                librg_event_trigger(ctx, MOD_CAR_INTERACTION_FINISH, &event);
            } break;

            default: {
                mod_log("[game-event] unknown event %d\n", (int)message->m_dwMessage);
            } break;
        }
    });

    M2::AttachHandler(M2_EVENT_CAR_ENTER_REQUEST, [](m2sdk_event *data) {
        data->arg5 = (void *)true;
        // data->arg5 = (void *)false; // to block entering the car

        mod_log("[game-event] ped request vehicle enter (%s)\n", ((bool)data->arg5) ? "granted" : "denied");
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

        librg_event_trigger(ctx, MOD_CAR_ENTER_START, &event);
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
    mod_message_send(ctx, MOD_USER_SET_NAME, [&](librg_data_t *data) {
        librg_data_wu8(data, zpl_strlen(username));
        librg_data_wptr(data, (void *)username, zpl_strlen(username));
    });
}

void mod_respawn() {
    M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetTime(0.5); /* 0.0 .. 1.0 - time of the day */

    auto ped = get_ped(mod.player);

    if (M2::C_SDSLoadingTable::Get()) {
        M2::C_SDSLoadingTable::Get()->ProcessLine("free_joe_load");
        M2::C_SDSLoadingTable::Get()->ProcessLine("free_summer_load");

        M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetDayTemplate("DT_RTRclear_day_late_afternoon");
        mod_log("[info] setting day template: %s\n", "DT_RTRclear_day_late_afternoon");
    }

    /* Disable ambiant peds */
    M2::Wrappers::SwitchFarAmbiants(false);
    M2::Wrappers::SwitchGenerators(false);

    /* Lock to prevent actions while respawning */
    ped->CPlayer->LockControls(true);

    /* Resetting player */
    ped->CHuman->GetScript()->SetHealth(720.0f);
    ped->CEntity->SetPosition(vec3(-421.75f, 479.31f, 0.05f));

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

    // call late (post window create)
    // platform-related code
    platform_init();

    // therotically we shouldn't call it here but because it's a
    // sync object it's fine itll work but the local player isn't created just yet.
    M2::C_GameGuiModule::Get()->FaderFadeIn(1);

    ctx = new librg_ctx_t;
    zpl_zero_item(ctx);

    // setup manual client mode
    ctx->tick_delay     = 32;
    ctx->mode           = LIBRG_MODE_CLIENT;
    ctx->world_size     = zplm_vec3f(5000.0f, 5000.0f, 0);
    ctx->max_entities   = M2O_ENTITY_LIMIT;

    librg_option_set(LIBRG_PLATFORM_ID, M2O_PLATFORM_ID);
    librg_option_set(LIBRG_PLATFORM_BUILD, M2O_VERSION_NUMBER);
    librg_option_set(LIBRG_PLATFORM_PROTOCOL, M2O_VERSION_PROTOCOL);

    librg_init(ctx);

    // setup callbacks
    librg_event_add(ctx, LIBRG_CONNECTION_REQUEST, [](librg_event_t *event) {
        // TODO: password sending
    });

    librg_event_add(ctx, LIBRG_CONNECTION_ACCEPT, [](librg_event_t *event) {
        mod_log("[info] connected to the server\n");
        auto ped = new ped_t((M2::C_Entity *)M2::C_Game::Get()->GetLocalPed());

        // // #if _DEBUG
        // mod.state = MOD_STUFF_STATE;
        // // #else
        // // mod.state = MOD_CONNECTED_STATE;
        // // #endif

        mod.player = event->entity;
        mod.player->user_data = ped;

        mod_nickname_set("the playah");
        mod_respawn();
    });

    librg_event_add(ctx, LIBRG_CONNECTION_REFUSE, [](librg_event_t *event) {
        mod_log("[info] connection refused\n");
    });

    librg_event_add(ctx, LIBRG_CONNECTION_DISCONNECT, [](librg_event_t *event) {
        mod_log("[info] disconnected form the server\n");
        // mod_connecting = 0;

        auto CEntity = (M2::C_Entity *)M2::C_Game::Get()->GetLocalPed();

        ((M2::C_Player2*)CEntity)->LockControls(true);
        // object->SetPosition(zplm_vec3_zero()); // creates black textures :O

        // if we have been disconnected before being connected
        if (mod.player) {
            delete mod.player->user_data;
        }

        // mod.state = MOD_TITLE_STATE;
        // mod.player = nullptr;
    });

    librg_event_add(ctx, LIBRG_ENTITY_CREATE, [](librg_event_t *event) {
        switch (event->entity->type) {
            case TYPE_PED: { module_ped_callback_create(event); } break;
            case TYPE_CAR: { module_car_callback_create(event); } break;
        }
    });

    librg_event_add(ctx, LIBRG_ENTITY_UPDATE, [](librg_event_t *event) {
        switch (event->entity->type) {
            case TYPE_PED: { module_ped_callback_update(event); } break;
            case TYPE_CAR: { module_car_callback_update(event); } break;
        }
    });

    librg_event_add(ctx, LIBRG_ENTITY_REMOVE, [](librg_event_t *event) {
        switch (event->entity->type) {
            case TYPE_PED: { module_ped_callback_remove(event); } break;
            case TYPE_CAR: { module_car_callback_remove(event); } break;
        }
    });

    librg_event_add(ctx, LIBRG_CLIENT_STREAMER_UPDATE, [](librg_event_t *event) {
        switch (event->entity->type) {
            case TYPE_PED: { module_ped_callback_clientstream(event); } break;
            case TYPE_CAR: { module_car_callback_clientstream(event); } break;
        }
    });

    librg_event_add(ctx, LIBRG_CLIENT_STREAMER_ADD, [](librg_event_t *event) {
        mod_log("[info] adding an entity %d to clientstream\n", event->entity->id);
        mod.stats.streamed_entities++;

        event->entity->flags &= ~MOD_ENTITY_INTERPOLATED;
    });

    librg_event_add(ctx, LIBRG_CLIENT_STREAMER_REMOVE, [](librg_event_t *event) {
        mod_log("[info] removing an entity %d from clientstream\n", event->entity->id);
        mod.stats.streamed_entities--;

        switch (event->entity->type) {
            case TYPE_PED:
            case TYPE_CAR: event->entity->flags |= MOD_ENTITY_INTERPOLATED; break;
        }
    });

    librg_network_add(ctx, MOD_USER_SET_NAME, [](librg_message_t *msg) {
        auto entity = librg_entity_fetch(msg->ctx, librg_data_ru32(msg->data));
        mod_assert(entity);

        u8 strsize = librg_data_ru8(msg->data);
        auto ped   = get_ped(entity);
        librg_data_rptr(msg->data, ped->name, strsize);

        // client-specific
        zpl_utf8_to_ucs2((u16 *)ped->cached_name, strsize, (const u8 *)ped->name);

        mod_log("set new name for client %u: %s\n", entity->id, ped->name);
    });

    // librg_network_add(ctx, MOD_USER_MESSAGE, mod_on_user_message);

    // call inits for modules
    module_ped_init();
    module_car_init();

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
    // tick networking
    librg_tick(ctx);

    // interpolate all entities
    librg_entity_iterate(ctx, (LIBRG_ENTITY_ALIVE | MOD_ENTITY_INTERPOLATED), [](librg_ctx_t *ctx, librg_entity_t *entity) {
        switch (entity->type) {
            case TYPE_PED: { module_ped_callback_interpolate(entity); } break;
            case TYPE_CAR: { module_car_callback_interpolate(entity); } break;
        }
    });

    /* show/hide mouse */
    if (input_key_down(VK_F1)) {
        input_block_set(!input_block_get());
    }

    /* show/hide mouse */
    if (input_key_down(VK_F2)) {
        mod_message_send(ctx, MOD_CAR_CREATE, nullptr);
    }

    /* connect to the server */
    if (input_key_down(VK_F5) && !mod.spawned) {
        mod_connect("localhost", 27010);
        mod.spawned = true;
    }

    static M2::C_Entity *ent;
    if (input_key_down(VK_F3) && mod.spawned) {
        ent = M2::Wrappers::CreateEntity(M2::eEntityType::MOD_ENTITY_PED, 0);
        auto pos = reinterpret_cast<M2::C_Human2*>(M2::C_Game::Get()->GetLocalPed())->GetPos();
        ent->SetPosition(pos);
        mod_log("Ped created\n");
    }


    if (input_key_down(VK_F4) && mod.spawned && ent) {
        vec3_t dir = reinterpret_cast<M2::C_Human2*>(M2::C_Game::Get()->GetLocalPed())->GetDir();
        M2::S_HumanCommandMoveDir *moveCMD = new M2::S_HumanCommandMoveDir;
        moveCMD->x = dir.x;
        moveCMD->y = dir.y;
        moveCMD->z = dir.z;
        reinterpret_cast<M2::C_Human2*>(ent)->AddCommand(M2::E_Command::COMMAND_MOVEDIR, moveCMD);

        mod_log("Command added\n");
    }

    if (input_key_down(VK_F6) && mod.spawned) {
        void *command;
        void *command2;
        command2 =  reinterpret_cast<M2::C_Human2*>(M2::C_Game::Get()->GetLocalPed())->GetCurrentMoveCommand(&command);
        mod_log("0x%p\n0x%p\n", command, command2);
    }
}
