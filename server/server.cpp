#include <librg/librg.h>
#include <librg/components/transform.h>
#include <librg/components/interpolable.h>
#include <librg/components/client.h>
#include <librg/components/client_streamable.h>

#include <librg/utils/fs.h>

#include <messages.h>

enum {
    TYPE_PLAYER,
    TYPE_VEHICLE,
};

struct movement_t {
    uint8_t type;
};

struct ped_t {
    float speed;
};

struct vehicle_t {
    int model;
};

using namespace librg;

/**
 * Entity add to streamer
 */
void entity_create_forplayer(events::event_t* evt)
{
    auto event = (events::event_create_t*) evt;
}

/**
 * Entity update in streamer
 */
void entity_update_forplayers(events::event_t* evt)
{
    auto event = (events::event_update_t*) evt;

    switch (event->type) {
        case TYPE_PLAYER: {
            auto object = event->entity.component<ped_t>();
            event->data->write_float(object->speed);
        } break;
    }
}

/**
 * Entity remove from streamer
 */
void entity_remove_forplayers(events::event_t* evt)
{
    auto event = (events::event_remove_t*) evt;
}

void clientstreamer_update(events::event_t* evt)
{
    auto event = (events::event_bs_entity_t*) evt;
    auto trans = event->entity.component<transform_t>();

    // core::log("received pos %f %f %f", 
    //     trans->position.x,
    //     trans->position.y,
    //     trans->position.z
    // );

    switch (event->type) {
        case TYPE_VEHICLE: {
            // auto speed = event->data->read_float();
        }
        break;
        case TYPE_PLAYER: {
            auto object = event->entity.component<ped_t>();
            // object->speed = event->data->read_float();
        }
        break;
    }
}

void ontick(events::event_t* evt)
{
    auto event = (events::event_tick_t*) evt;
}

void spawn_player(entity_t entity)
{
    auto client = entity.component<client_t>();

    // assiging streameable component will
    // auto-add this entity to the streamer
    entity.assign<streamable_t>();
    entity.assign<ped_t>();

    // add entity as client streamable
    streamer::client::set(entity, client->peer);
    core::log("spawned player!");
}

void unspawn_player(entity_t entity)
{
    streamer::remove(entity, true);
    streamer::client::remove(entity);

    entity.remove<streamable_t>();
    entity.remove<ped_t>();

    core::log("unspawned player!");
}

void client_connect(events::event_t* evt)
{
    auto event = (events::event_entity_t*)evt;
    auto entity = event->entity;
    auto client = entity.component<client_t>();

    core::log("connect: id: %ld name: %s serial: %s", client->peer->connectID, "nonono", "anananana");

    spawn_player(entity);
}

void client_disconnect(events::event_t* evt)
{
    auto event = (events::event_entity_t*)evt;
    auto entity = event->entity;
    auto client = entity.component<client_t>();

    core::log("disconnect: id: %ld name: %s serial: %s", client->peer->connectID, "nonono", "anananana");

    unspawn_player(entity);
}

int main(int argc, char** argv)
{
    std::string test = "";
    test.append("===============      SERVER      =================\n");
    test.append("==                                              ==\n");
    test.append("==                 ¯\\_(ツ)_/¯                   ==\n");
    test.append("==                                              ==\n");
    test.append("==================================================\n");
    printf("%s\n\n", test.c_str());

    librg::core_initialize(librg::mode_server);

    events::add(events::on_log, [](events::event_t* evt) {
        auto event = (events::event_log_t*) evt;
        std::cout << event->output;
    });

    events::set(events::on_tick, ontick);
    events::set(events::on_create, entity_create_forplayer);
    events::set(events::on_update, entity_update_forplayers);
    events::set(events::on_remove, entity_remove_forplayers);
    events::set(events::on_client_stream_entity, clientstreamer_update);
    events::set(events::on_connect, client_connect);
    events::set(events::on_disconnect, client_disconnect);

    librg::network::set(GAME_SPAWN_VEHICLE, [](network::peer_t* peer, network::packet_t* packet, network::bitstream_t* data) {
        auto transform = network::connected_peers[peer].component<transform_t>();
        core::log("spawning vehicle!");

        auto entity = entities->create();
        auto stream = entity.assign<streamable_t>(TYPE_VEHICLE);
        auto transf = entity.assign<transform_t>();

        transf->position = HMM_Vec3(transform->position.x, transform->position.y + 3.0f, transform->position.z);
    });

    network::set(GAME_PED_ENTER_VEHICLE, [](network::peer_t *peer, network::packet_t *packet, network::bitstream_t *data) {
        auto guid = data->read_uint64();
        auto id   = entity_t::Id(guid);

        core::log("player enters vehicle...");

        if (!entities->valid(id)) {
            core::log("invlid entity on vehicle enter");
            return;
        }

        // get vehicle
        auto entity = entities->get(id);

        // set current player as entity streamer
        streamer::client::set(entity, peer);
    });

    auto cfg = librg::config_t{};
    cfg.ip = "localhost";
    cfg.port = 27010;
    cfg.world_size = HMM_Vec3(5000, 5000, 5000);
    cfg.tick_delay = 32;
    cfg.max_connections = 32;
    cfg.platform_id = 1;
    cfg.proto_version = 1;
    cfg.build_version = 1;

    librg::core::start(cfg);
    librg::core_terminate();

    return 0;
}
