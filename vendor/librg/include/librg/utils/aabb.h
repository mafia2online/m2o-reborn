// Copyright ReGuider Team, 2016-2017
//
#ifndef librg_utils_aabb_h
#define librg_utils_aabb_h

#include <librg/utils/math.h>

namespace librg
{
    struct aabb_t {
        aabb_t() {}
        aabb_t(hmm_vec3 halfdim) : halfdim(halfdim), center() {}
        aabb_t(hmm_vec3 center, hmm_vec3 halfdim)
            : center(center), halfdim(halfdim) {}

        bool contains_2d(hmm_vec3 point)
        {
            return(center.x - halfdim.x <= point.x
                && center.x + halfdim.x >= point.x
                && center.y - halfdim.y <= point.y
                && center.y + halfdim.y >= point.y);
        }

        bool contains_3d(hmm_vec3 point)
        {
            return(center.x - halfdim.x <= point.x
                && center.x + halfdim.x >= point.x
                && center.y - halfdim.y <= point.y
                && center.y + halfdim.y >= point.y
                && center.z - halfdim.z <= point.z
                && center.z + halfdim.z >= point.z);
        }

        bool intersects_2d(aabb_t rhs)
        {
            if (HMM_ABS(center.x - rhs.center.x) > (halfdim.x + rhs.halfdim.x)) return false;
            if (HMM_ABS(center.y - rhs.center.y) > (halfdim.y + rhs.halfdim.y)) return false;

            return true;
        }

        bool intersects_3d(aabb_t rhs)
        {
            if (HMM_ABS(center.x - rhs.center.x) > (halfdim.x + rhs.halfdim.x)) return false;
            if (HMM_ABS(center.y - rhs.center.y) > (halfdim.y + rhs.halfdim.y)) return false;
            if (HMM_ABS(center.z - rhs.center.z) > (halfdim.z + rhs.halfdim.z)) return false;

            return true;
        }

        hmm_vec3 halfdim;
        hmm_vec3 center;
    };
}

#endif // librg_utils_aabb_h
