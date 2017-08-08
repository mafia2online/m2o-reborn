// Copyright ReGuider Team, 2016-2017
//
#ifndef librg_components_client_streamable_h
#define librg_components_client_streamable_h

#include <librg/network.h>

namespace librg
{
    struct client_streamable_t {
        client_streamable_t() : peer() {}
        client_streamable_t(network::peer_t* p) : peer(p) {}

        network::peer_t* peer;
    };
}

#endif // librg_components_client_streamable_h
