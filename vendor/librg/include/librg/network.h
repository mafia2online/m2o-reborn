// Copyright ReGuider Team, 2016-2017
//
#ifndef librg_network_h
#define librg_network_h

#include <enet/enet.h>

#include <librg/core.h>
#include <librg/entities.h>
#include <librg/network/bitstream.h>

#ifndef LIBRG_NETWORK_MESSAGES
#define LIBRG_NETWORK_MESSAGES 1024
#endif

#ifndef LIBRG_NETWORK_CHANNELS
#define LIBRG_NETWORK_CHANNELS 4
#endif

namespace librg
{
    void network_initialize();
    void network_terminate();

    namespace network
    {
        using peer_t        = ENetPeer;
        using host_t        = ENetHost;
        using packet_t      = ENetPacket;
        using callback_t    = std::function<void(peer_t* peer, packet_t* packet, bitstream_t* data)>;
        using message_t     = std::function<void(bitstream_t* data)>;

        /**
         * Builtin network messages (events)
         */
        enum message_e {
            connection_init,
            connection_request,
            connection_refuse,
            connection_accept,
            connection_disconnect,

            entity_create,
            entity_update,
            client_streamer_add,
            client_streamer_remove,
            client_streamer_update,

            last_packet_number,
        };

        /**
         * Start the server
         * @param port
         */
        void start();

        /**
         * Calls a recieve update for network
         * (server receives new data from clients)
         * (client receives new data from server)
         */
        void poll();

        /**
         * Set callback for listening particular network event
         * @param id of the message, for custom messages id should be >= last_packet_number
         * @param callback method that will be called
         */
        void set(uint16_t id, callback_t callback);

        /**
         * Send message via "smart" method
         * it will try to detect who should be a receieve
         * depending on mode wea are running in
         * @param message id
         * @param message encoder
         */
        void msg(uint16_t id, message_t callback);

        /**
         * Send message to particular connected peer
         * @param messageid
         * @param address
         * @param message encoder
         */
        void msg(uint16_t id, peer_t* peer, message_t callback);

        /**
         * Send message to all peers except particular one
         * @param message id
         * @param peer to be ignored
         * @param message encoder
         */
        void msg_except(uint16_t id, peer_t* peer, message_t callback);

        /**
         * Send message for all peers inside stream zone for particular entity (or peer)
         * @param message id
         * @param entity which will be used as root for calculating nearby in-stream clients
         * @param message encoder
         */
        void msg_stream(uint16_t id, entity_t entity, message_t callback);

        /**
         * Send message to all connected peers
         * @param message id
         * @param message encoder
         */
        void msg_all(uint16_t id, message_t callback);

        /**
         * Our nifty data
         */
        extern host_t* host;
        extern peer_t* peer;

        /**
         * Data arrays
         * even more niftier
         */
        extern std::unordered_map<peer_t*, entity_t> connected_peers;
        extern std::array<callback_t, LIBRG_NETWORK_MESSAGES> message_handlers;
    }
}

#endif // librg_network_h
