// Copyright ReGuider Team, 2016-2017
//
#include <librg/network.h>

using namespace librg;

void librg::network_terminate()
{
    // for (auto pair : network::clients) {
    //     pair.second.destroy();
    //     // network::clients.erase(packet->guid);
    // }
    core::log("stopping network...");

    if (network::peer) {
        core::log("disconnecting from server...");

        ENetEvent event;

        enet_peer_disconnect(network::peer, 0);
        enet_host_service(network::host, &event, 100);
        enet_peer_reset(network::peer);
    }

    enet_deinitialize();
}
