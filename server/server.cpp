#define MOD_SERVER

#include "includes.h"

typedef struct {
    std::string hostname;
    std::string password;
} mod_settings_t;

mod_settings_t mod_settings;

// shared stuff
#include "components.h"
#include "messages.h"

#include "entities/ped.h"
#include "entities/vehicle.h"
#include "core/callbacks.h"
#include "settings.h"

void register_messages() {
    librg_network_add(MOD_VEHICLE_CREATE, mod_vehicle_create);
    librg_network_add(MOD_VEHICLE_ENTER, mod_vehicle_enter);
}

int main() {
    librg_config_t config = {0};
    config.tick_delay = 32;
    config.mode = LIBRG_MODE_SERVER;
    config.world_size = zplm_vec2(5000.0f, 5000.0f);
    config.max_entities = 16000;
    config.max_connections = 100;

    librg_address_t address;
    address.port = 27010;

    settings_read(&config, &address, &mod_settings);

    librg_log("starting on port: %u with conn: %u\n", address.port, config.max_connections);
    librg_log("my hostname: %s, my password: %s\n", mod_settings.hostname.c_str(), mod_settings.password.c_str());

    librg_init(config);

    librg_event_add(LIBRG_CONNECTION_REQUEST, on_connection_request);
    librg_event_add(LIBRG_CONNECTION_ACCEPT, on_connect_accepted);
    librg_event_add(LIBRG_ENTITY_CREATE, entity_create);
    librg_event_add(LIBRG_ENTITY_UPDATE, entity_update);
    librg_event_add(LIBRG_ENTITY_REMOVE, entity_remove);
    librg_event_add(LIBRG_CLIENT_STREAMER_UPDATE, clientstream_update);

    register_messages();

    librg_network_start(address);

    while (true) {
        librg_tick();
        zpl_sleep_ms(1);
    }

    librg_network_stop();
    librg_free();

    return 0;
}
