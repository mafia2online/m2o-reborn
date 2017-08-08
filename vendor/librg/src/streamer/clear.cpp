// Copyright ReGuider Team, 2016-2017
//
#include <librg/streamer.h>

void librg::streamer::qtree_t::clear()
{
    entities.clear();
    blacklistedEntities.clear();

    for (auto &tree : trees) {
        tree.clear();
    }

    trees.clear();
}

void librg::streamer::clear()
{
    _root.clear();

    for (auto entity : remove_queue) {
        entity.destroy();
    }

    remove_queue.clear();
    entity_cache.clear();
}

void librg::streamer::clear(aabb_t bounds)
{
    librg::streamer::clear();
    _root.boundary = bounds;
}
