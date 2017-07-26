// Copyright ReGuider Team, 2016-2017
//
#include <librg/streamer.h>
#include <librg/utils/math.h>

void librg::streamer::qtree_t::subdivide()
{
    hmm_vec3 hd = HMM_DivideVec3f(boundary.halfdim, 2.f);

    auto nwc = hmm_vec3{boundary.center.X - hd.X, boundary.center.Y - hd.Y, .0f};
    auto nwb = aabb_t(nwc, hd);
    auto nec = hmm_vec3{boundary.center.X + hd.X, boundary.center.Y - hd.Y, .0f};
    auto neb = aabb_t(nec, hd);
    auto swc = hmm_vec3{boundary.center.X - hd.X, boundary.center.Y + hd.Y, .0f};
    auto swb = aabb_t(swc, hd);
    auto sec = hmm_vec3{boundary.center.X + hd.X, boundary.center.Y + hd.Y, .0f};
    auto seb = aabb_t(sec, hd);

    create_child(nwb);
    create_child(neb);
    create_child(swb);
    create_child(seb);
}
