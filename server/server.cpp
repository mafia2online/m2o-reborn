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

// server modules
#include "modules/module_ped.h"
#include "modules/module_car.h"

#include "settings.h"

/**
 * Place to decide should the client be allowed to connect
 */
void on_connection_request(librg_event_t *event) {
    if (mod_settings.password.size() == 0) {
        return;
    }

    // read password
    u32 size = librg_data_ru32(event->data);
    std::string password = "";
    for (usize i = 0; i < size; ++i) {
        password += librg_data_ru8(event->data);
    }

    // if not matches - reject
    if (password != mod_settings.password) {
        librg_event_reject(event);
    }
}

/**
 * On client connected
 */
void on_connect_accepted(librg_event_t *event) {
    auto transform = librg_fetch_transform(event->entity);
    auto client    = librg_fetch_client(event->entity);

    mod_log("spawning player %u at: %f %f %f\n",
        event->entity,
        transform->position.x,
        transform->position.y,
        transform->position.z
    );

    librg_attach_ped(event->entity, {0});
    librg_streamer_client_set(event->entity, client->peer);
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

    module_ped_init();
    module_car_init();

    librg_network_start(address);

    while (true) {
        librg_tick();

        module_ped_tick();
        module_car_tick();

        zpl_sleep_ms(1);
    }

    librg_network_stop();
    librg_free();

    return 0;
}
