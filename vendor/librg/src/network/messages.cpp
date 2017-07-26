// Copyright ReGuider Team, 2016-2017
//
#include <librg/network.h>
#include <librg/streamer.h>
#include <librg/components/client.h>

using namespace librg;

#define LIBRG_NETWORK_MESSAGE_CHANNEL 2

/**
 * Set callback for listening particular network event
 * @param id       id of the message, for custom messages id should be >= last_packet_number
 * @param callback method that will be called
 */
void network::set(uint16_t id, network::callback_t callback)
{
    if (network::message_handlers[id]) {
        core::error("warning: you are redefining network message with id: %d", id);
    }

    network::message_handlers[id] = callback;
}

/**
 * Send message via "smart" method
 * it will try to detect who should be a receieve
 * depending on mode wea are running in
 * @param message id
 * @param message encoder
 */
void network::msg(uint16_t id, message_t callback)
{
    // TODO: make smart, now its stupid
    network::msg_all(id, callback);
}

/**
 * Send message to particular connected peer
 * @param messageid
 * @param address
 * @param message_t
 */
void network::msg(uint16_t id, network::peer_t* peer, network::message_t callback)
{
    network::bitstream_t message;

    message.write(id);
    if (callback) { callback(&message); }

    enet_peer_send(peer, LIBRG_NETWORK_MESSAGE_CHANNEL, enet_packet_create(
        message.raw(), message.raw_size(), ENET_PACKET_FLAG_RELIABLE
    ));
}

/**
 * Send message to all peers except particular one
 * @param message id
 * @param peer to be ignored
 * @param callback
 */
void network::msg_except(uint16_t id, network::peer_t* bad_peer, network::message_t callback)
{
    network::bitstream_t message;

    message.write(id);
    if (callback) { callback(&message); }

    for (auto pair : network::connected_peers) {
        if (pair.first == bad_peer) {
            continue;
        }

        enet_peer_send(pair.first, LIBRG_NETWORK_MESSAGE_CHANNEL, enet_packet_create(
            message.raw(), message.raw_size(), ENET_PACKET_FLAG_RELIABLE
        ));
    }
}

/**
 * Send message for all peers inside stream zone for particular entity (or peer)
 * @param message id
 * @param entity which will be used as root for calculating nearby in-stream clients
 * @param message_t
 */
void network::msg_stream(uint16_t id, entity_t entity, network::message_t callback)
{
    network::bitstream_t message;

    message.write(id);
    if (callback) { callback(&message); }

    auto entities = librg::streamer::query(entity);

    for (auto other : entities) {
        auto client = other.component<client_t>();
        if (!client || !client->active) continue;

        enet_peer_send(client->peer, LIBRG_NETWORK_MESSAGE_CHANNEL, enet_packet_create(
            message.raw(), message.raw_size(), ENET_PACKET_FLAG_RELIABLE
        ));
    }
}

/**
 * Send message to all connected peers
 * @param message id
 * @param message_t
 */
void network::msg_all(uint16_t id, network::message_t callback)
{
    network::msg_except(id, nullptr, callback);
}
