// Copyright ReGuider Team, 2016-2017
//
#ifndef librg_components_client_h
#define librg_components_client_h

#include <string>
#include <unordered_map>

#include <librg/network.h>

namespace librg
{
    struct client_t {
        client_t(network::peer_t* p, std::string n, std::string s)
            : peer(p), nickname(n), serial(s), active(true), last_snapshot() {}

        network::peer_t* peer;
        std::string nickname;
        std::string serial;
        bool active;

        std::unordered_map<uint64_t, bool> last_snapshot;
    };
}

#endif // librg_components_client_h
