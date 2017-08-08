// Copyright ReGuider Team, 2016-2017
//
#include <librg/streamer.h>

void librg::streamer::qtree_t::create_child(aabb_t boundary)
{
    auto tree = qtree_t(boundary);
    trees.push_back(tree);
}
