// Copyright ReGuider Team, 2016-2017
//
#include <librg/network.h>

using namespace librg;

void network::poll()
{
    if (network::host == nullptr) {
        return;
    }

    ENetEvent event;

    while (enet_host_service(network::host, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                {
                    auto data = network::bitstream_t(event.packet->data, event.packet->dataLength);
                    auto puid = data.read_uint16();

                    if (network::message_handlers[puid]) {
                        network::message_handlers[puid](event.peer, event.packet, &data);
                    }
                    else {
                        core::error("network: unknown message: %d", puid);
                    }

                    enet_packet_destroy(event.packet);
                }
                break;
            case ENET_EVENT_TYPE_CONNECT:
                network::message_handlers[network::connection_init](event.peer, nullptr, nullptr);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                network::message_handlers[network::connection_disconnect](event.peer, nullptr, nullptr);
                break;
            case ENET_EVENT_TYPE_NONE: break;
        }
    }
}
