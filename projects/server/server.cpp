#define M2O_SERVER_API
#define M2O_SERVER
#include "m2o_config.h"

#include <string>
#include <functional>

#define LIBRG_IMPLEMENTATION
#include "librg.h"
#include "librg_ext.h"

#define HTTP_IMPLEMENTATION
#include "http.h"

#if defined(ZPL_SYSTEM_WINDOWS)
#include <clocale>
#endif

// shared
#include "m2o_types.h"

struct m2o_api_vtable;
struct mod_t {
    bool running;
    m2o_api_vtable *api;

    u16 connected_amount;

    struct {
        std::string hostname;
        std::string password;

        std::string masterlist_login;
        std::string masterlist_secret;

        u16 port;
        u16 max_connections;
    } settings;
};

static mod_t mod;
librg_ctx_t *ctx;

#define M2O_API_DISABLE_VEC3

#include "api/general.h"
#include "api/vehicle.h"
#include "api/pedestrian.h"
#include "api/player.h"
#include "api/vtable.h" /* auto-generated file using $ node misc/generate-api.js */

// server modules
#include "core/settings.h"
#include "core/masterlist.h"
#include "core/plugin_manager.h"
#include "core/vehicle.h"
#include "core/pedestrian.h"
#include "core/router.h"

/* simple bandwidth measurer */
void mod_measure(void *user_data) {
    librg_ctx_t *ctx = (librg_ctx_t *)user_data;
    if (!ctx || !ctx->network.host) return;

    static u32 lastdl = 0;
    static u32 lastup = 0;

    f32 dl = (ctx->network.host->totalReceivedData - lastdl) * 8.0f / (1000.0f * 1000); // mbps
    f32 up = (ctx->network.host->totalSentData - lastup)     * 8.0f / (1000.0f * 1000); // mbps

    lastdl = ctx->network.host->totalReceivedData;
    lastup = ctx->network.host->totalSentData;

    mod_log("[info] update took %.3f ms. Current used bandwidth D/U: (%.3f / %.3f) mbps. \r", ctx->last_update, dl, up);
}

int main() {
    #if defined(ZPL_SYSTEM_WINDOWS)
    // Set our locale to the C locale, as Unicode output only functions in this locale
    std::setlocale(LC_ALL, "C");
    SetConsoleOutputCP(CP_UTF8);
    #endif

    const char *test = \
        "===============    M2O-SERVER    =================\n" \
        "==                                              ==\n" \
        "==                 ¯\\_(ツ)_/¯                   ==\n" \
        "==                                              ==\n" \
        "==================================================\n";
    mod_log("%s", test);
    mod_log("[info] server version: %s\n", M2O_VERSION_STRING);

    librg_option_set(LIBRG_PLATFORM_ID, M2O_PLATFORM_ID);
    librg_option_set(LIBRG_PLATFORM_BUILD, M2O_VERSION_NUMBER);
    librg_option_set(LIBRG_PLATFORM_PROTOCOL, M2O_VERSION_PROTOCOL);

    // allocate ctx
    ctx = new librg_ctx_t;
    zpl_zero_item(ctx);

    /* fill up default settings */
    ctx->mode            = LIBRG_MODE_SERVER;
    ctx->tick_delay      = 1000.0f / M2O_TICKRATE_SERVER;
    ctx->world_size      = zplm_vec3f(5000.0f, 5000.0f, 0.0f);
    ctx->max_entities    = M2O_ENTITY_LIMIT;
    ctx->max_connections = 1000;

    librg_address_t address = { 27010, NULL };
    settings_read(ctx, &address, &mod);

    mod_log("[info] settings:\n\n");

    mod_log("\thostname: %s\n",     mod.settings.hostname.c_str());
    mod_log("\tpassword: (%s)\n",   mod.settings.password.size() ? "yes" : "no");
    mod_log("\tport: %d\n\n",       address.port);

    mod_log("\tmax_connections: %d\n",  ctx->max_connections);
    mod_log("\tstream_range: %f\n",     (f32)librg_option_get(LIBRG_DEFAULT_STREAM_RANGE));
    mod_log("\ttick_delay: %f\n\n",     ctx->tick_delay);

    mod_log("[info] starting the server...\n");

    librg_init(ctx);
    mod_register_routes(ctx);

    zpl_timer_t *tick_timer = zpl_timer_add(ctx->timers);
    tick_timer->user_data = (void *)ctx; /* provide ctx as a argument to timer */
    zpl_timer_set(tick_timer, 1.0, -1, mod_measure);
    zpl_timer_start(tick_timer, 0);

    zpl_timer_t *masterlist_timer = zpl_timer_add(ctx->timers);
    zpl_timer_set(masterlist_timer, MASTERLIST_SECONDS, -1, masterlist_update);
    zpl_timer_start(masterlist_timer, 0);

    librg_network_start(ctx, address);

    mod.api = new m2o_api_vtable;
    mod.running = true;

    m2o_api_init(mod.api);
    m2o_plugins_init(ctx, &mod);

    while (mod.running) {
        librg_tick(ctx);
        masterlist_tick();
        m2o_plugins_tick(ctx, &mod);
        zpl_sleep_ms(5);
    }

    m2o_plugins_stop(ctx, &mod);
    librg_network_stop(ctx);
    librg_free(ctx);
    delete ctx;
    delete mod.api;

    return 0;
}
