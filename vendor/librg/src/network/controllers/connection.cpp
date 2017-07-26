// Copyright ReGuider Team, 2016-2017
//
#include <enet/enet.h>

#include <librg/core.h>
#include <librg/events.h>
#include <librg/streamer.h>
#include <librg/network.h>
#include <librg/network/controllers.h>
#include <librg/components/client.h>
#include <librg/components/streamable.h>
#include <librg/components/transform.h>
#include <librg/components/server_owned.h>

using namespace librg;
using namespace librg::network;

void connection_controller::init(peer_t* peer, packet_t* packet, bitstream_t* data)
{
    char my_host[16];

    enet_address_get_host_ip(&peer->address, my_host, 16);
    core::log("a new connection attempt at %s:%u.\n", my_host, peer->address.port);

    if (core::is_client()) {
        network::msg(connection_request, peer, [](bitstream_t* message) {
            message->write_uint8(core::config.platform_id);
            message->write_uint8(core::config.proto_version);
            message->write_uint8(core::config.build_version);

            message->write("Test Player");
            message->write("4555ASDASD4555ASDASD4555");
        });
    }
}

void connection_controller::request(peer_t* peer, packet_t* packet, bitstream_t* data)
{
    auto platform_id   = data->read_uint8();
    auto proto_version = data->read_uint8();
    auto build_version = data->read_uint8();

    // // incompatible protocol version - force immidiate disconnect
    // if (proto_version != core::config.proto_version || platform_id != core::config.platform_id) {
    //     output.Write(static_cast<RakNet::MessageID>(CONNECTION_REFUSED));
    //     output.Write("Incompatible game version.");

    //     data.peer->Send(&output, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

    //     core::log("server_connect: refsued ip: %s, reason: incompatible game version.", packet->systemAddress.ToString(true, ':'));
    //     return;
    // }

    // // let server owner to decide, to kick or not to kick
    // if (buildVersion != buildVersion) {
    //     // TODO(inlife): add check for server parameters to decide, should be connection refused or allowed
    //     output.Write(static_cast<RakNet::MessageID>(CONNECTION_REFUSED));
    //     output.Write("Incompatible build version.");

    //     data.peer->Send(&output, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

    //     core::log("server_connect: refsued ip: %s, reason: incompatible build version.", packet->systemAddress.ToString(true, ':'));
    //     return;
    // }

    // RakNet::RakString nickName;
    // input.read(nickName);

    // RakNet::RakString serial;
    // input.read(serial);

    auto entity = entities->create();

    // assign default compoenents
    entity.assign<transform_t>();
    entity.assign<client_t>(peer, "nonono", "anananana");

    // add client peer to storage
    connected_peers.insert(std::make_pair(peer, entity));

    // send and trigger
    network::msg(connection_accept, peer, [&entity](bitstream_t* message) {
        message->write_uint64(entity.id().id());
    });

    events::trigger(events::on_connect, new events::event_entity_t(entity));
}

void connection_controller::accept(peer_t* peer, packet_t* packet, bitstream_t* data)
{
    auto entity = entities->create();
    auto guid   = data->read_uint64();

    // assign default compoennets
    entity.assign<transform_t>();
    entity.assign<server_owned_t>(guid);

    // add server peer to storage
    connected_peers.insert(std::make_pair(peer, entity));
    streamer::entity_pool.insert(std::make_pair(guid, entity));
    events::trigger(events::on_connect, new events::event_entity_t(entity));
}

void connection_controller::refuse(peer_t* peer, packet_t* packet, bitstream_t* data)
{
    events::trigger(events::on_refuse, nullptr);
}

void connection_controller::disconnect(peer_t* peer, packet_t* packet, bitstream_t* data)
{
    if (connected_peers.find(peer) == connected_peers.end()) {
        return core::log("did not find the entity in connected_peers");
    }

    auto entity = connected_peers[peer];

    if (core::is_server()) {
        entity.component<client_t>()->active = false;
    }

    connected_peers.erase(peer);

    return events::trigger(events::on_disconnect, new events::event_entity_t(entity));
}
