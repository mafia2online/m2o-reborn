#define MOD_SERVER

#include "includes.h"

typedef struct {
    std::string hostname;
    std::string password;
} mod_settings_t;

librg_ctx_t ctx;
mod_settings_t mod_settings;

// shared stuff
#include "components.h"
#include "messages.h"

// server modules
#include "callbacks.h"
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
    auto entity = event->entity;

    mod_log("spawning player %u at: %f %f %f\n",
        entity->id,
        entity->position.x,
        entity->position.y,
        entity->position.z
    );

    //auto object = new ped_t;
    //zpl_zero_item(object); // fill object with 0s
    entity->user_data = new ped_t();

    // TODO: allocate stuff
    librg_entity_control_set(event->ctx, event->entity->id, event->entity->client_peer);
}


int main() {
    ctx.tick_delay = 32;
    ctx.mode = LIBRG_MODE_SERVER;
    ctx.world_size = zplm_vec3(5000.0f, 5000.0f, 5000.f);
    ctx.max_entities = 16000;
    ctx.max_connections = 100;


    librg_address_t address = {0};
    address.port = 27010;

    settings_read(&ctx, &address, &mod_settings);

    librg_log("starting on port: %u with conn: %u\n", address.port, ctx.max_connections);
    librg_log("my hostname: %s, my password: %s\n", mod_settings.hostname.c_str(), mod_settings.password.c_str());

    librg_init(&ctx);

    librg_event_add(&ctx, LIBRG_CONNECTION_REQUEST, on_connection_request);
    librg_event_add(&ctx, LIBRG_CONNECTION_ACCEPT, on_connect_accepted);

    librg_event_add(&ctx, LIBRG_ENTITY_CREATE, entity_on_create);
    librg_event_add(&ctx, LIBRG_ENTITY_UPDATE, entity_on_update);
    librg_event_add(&ctx, LIBRG_ENTITY_REMOVE, entity_on_remove);
    librg_event_add(&ctx, LIBRG_CLIENT_STREAMER_UPDATE, entity_on_csupdate);

    librg_network_start(&ctx, address);

    while (true) {
        librg_tick(&ctx);
        zpl_sleep_ms(5);
    }

    librg_network_stop(&ctx);
    librg_free(&ctx);

    return 0;
}
