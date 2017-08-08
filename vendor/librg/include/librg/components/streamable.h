// Copyright ReGuider Team, 2016-2017
//
#ifndef librg_components_streamable_h
#define librg_components_streamable_h

#include <librg/entities.h>
#include <librg/components/transform.h>

namespace librg
{
    struct streamable_t {
        streamable_t() : query_range{ 250, 250, 250 }, ignored(), type(0) {}
        streamable_t(uint8_t type) : query_range{ 250, 250, 250 }, ignored(), type(type) {}
        streamable_t(hmm_vec3 queryRange): query_range(queryRange), ignored(), type(0) {}
        streamable_t(float x, float y, float z) : query_range(hmm_vec3{ x,y,z }), ignored(), type(0) {}
        streamable_t(hmm_vec3 queryRange, uint8_t type): query_range(queryRange), ignored(), type(type) {}

        uint8_t type;
        hmm_vec3 query_range;
        std::vector<entity_t> ignored;
    };
}

#endif // librg_components_streamable_h
