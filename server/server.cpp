#define LIBRG_DEBUG
#define LIBRG_IMPLEMENTATION
#include <librg.h>

void on_connect_accepted(librg_event_t *event) {
    librg_log("on_connect_accepted\n");

    librg_transform_t *transform = librg_fetch_transform(event->entity);
    librg_client_t *client = librg_fetch_client(event->entity);

    librg_log("spawning player %u at: %f %f %f\n",
        event->entity,
        transform->position.x,
        transform->position.y,
        transform->position.z
    );

    librg_streamer_client_set(event->entity, client->peer);
}

int main() {
    librg_config_t config;
    config.tick_delay = 32;
    config.mode = LIBRG_MODE_SERVER;
    config.world_size = zplm_vec2(5000.0f, 5000.0f);
    config.max_entities = 16000;
    librg_init(config);

    librg_event_add(LIBRG_CONNECTION_ACCEPT, on_connect_accepted);

    librg_address_t address;
    address.port = 27010;
    librg_network_start(address);

    while (true) {
        librg_tick();
        zpl_sleep_ms(1);
    }

    librg_network_stop();
    librg_free();

    return 0;
}
