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
            return(center.X - halfdim.X <= point.X
                && center.X + halfdim.X >= point.X
                && center.Y - halfdim.Y <= point.Y
                && center.Y + halfdim.Y >= point.Y);
        }

        bool contains_3d(hmm_vec3 point)
        {
            return(center.X - halfdim.X <= point.X
                && center.X + halfdim.X >= point.X
                && center.Y - halfdim.Y <= point.Y
                && center.Y + halfdim.Y >= point.Y
                && center.Z - halfdim.Z <= point.Z
                && center.Z + halfdim.Z >= point.Z);
        }

        bool intersects_2d(aabb_t rhs)
        {
            if (HMM_ABS(center.X - rhs.center.X) > (halfdim.X + rhs.halfdim.X)) return false;
            if (HMM_ABS(center.Y - rhs.center.Y) > (halfdim.Y + rhs.halfdim.Y)) return false;

            return true;
        }

        bool intersects_3d(aabb_t rhs)
        {
            if (HMM_ABS(center.X - rhs.center.X) > (halfdim.X + rhs.halfdim.X)) return false;
            if (HMM_ABS(center.Y - rhs.center.Y) > (halfdim.Y + rhs.halfdim.Y)) return false;
            if (HMM_ABS(center.Z - rhs.center.Z) > (halfdim.Z + rhs.halfdim.Z)) return false;

            return true;
        }

        hmm_vec3 halfdim;
        hmm_vec3 center;
    };
}

#endif // librg_utils_aabb_h
