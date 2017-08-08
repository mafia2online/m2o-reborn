// Copyright ReGuider Team, 2016-2017
//
#ifndef librg_components_server_owned_h
#define librg_components_server_owned_h

#include <librg/entities.h>
#include <librg/components/transform.h>

namespace librg
{
    struct server_owned_t {
        server_owned_t() {}
        server_owned_t(uint64_t g) : guid(g) {}

        uint64_t guid;
    };
}

#endif // librg_components_server_owned_h
