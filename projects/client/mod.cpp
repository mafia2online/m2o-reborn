#define M2O_CLIENT
#include "m2o_config.h"

#include <string>
#include <future>
#include <functional>

#include "librg.h"
#include "librg_ext.h"
#include "m2sdk.h"

#include "m2o_api.h"
#include "m2o_types.h"

class m2o_module : public M2::C_TickedModule {
public:
    void init(M2::I_TickedModuleCallEventContext &);
    void tick(M2::I_TickedModuleCallEventContext &);
    void load_start(M2::I_TickedModuleCallEventContext &);
    void load_finish(M2::I_TickedModuleCallEventContext &);
};

static m2o_module m2o;
static librg_ctx_t *ctx;

struct {
    librg_entity_t *player;

    // game tick props
    f64 last_update;
    f64 last_delta;

    struct {
        i32 streamed_entities;
    } stats;
} mod;

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

void mod_respawn() {
    M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetTime(0.5); /* 0.0 .. 1.0 - time of the day */

    auto ped = get_ped(mod.player);

   /* if (M2::C_SDSLoadingTable::Get()) {
        M2::C_SDSLoadingTable::Get()->ActivateStreamMapLine("free_joe_load");
        M2::C_SDSLoadingTable::Get()->ActivateStreamMapLine("free_summer_load");

        M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetDayTemplate("DT_RTRclear_day_late_afternoon");
        mod_log("[info] setting day template: %s\n", "DT_RTRclear_day_late_afternoon");
    }*/

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

        // send our nickname
        // mod_request_username_change(event->entity->id, title_state_data.username_input);
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

    // librg_network_add(ctx, MOD_USER_SET_NAME, mod_user_name_set);
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
    std::thread([](){ std::this_thread::sleep_for(std::chrono::milliseconds(1500)); }).detach();
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

    int x, y; input_mouse_position(&x, &y);
    // mod_log("mouse pos: [%d, %d]\n", x, y);
static int foo = 0;
    if (x != 0 && !mod_connected() && !foo) {
        foo = 1;
        mod_connect("localhost", 27010);
    }

    // discord_update_presence();

    /* show/hide mouse */
    // if (input_key_down(VK_F1)) {
    //     mod.input_blocked = !mod.input_blocked;
    // }
}
