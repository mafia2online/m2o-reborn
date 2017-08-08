// Copyright ReGuider Team, 2016-2017
//
#ifndef librg_components_transform_h
#define librg_components_transform_h

#include <librg/utils/math.h>

namespace librg
{
    struct transform_t {
        transform_t() = default;

        transform_t(hmm_vec3 position)
            : position(position), rotation(), scale() {}

        transform_t(hmm_vec3 position, hmm_vec4 rotation, hmm_vec3 scale)
            : position(position), rotation(rotation), scale(scale) {}

        hmm_vec3 position;
        hmm_vec4 rotation;
        hmm_vec3 scale;
    };
}

#endif // librg_components_transform_h
