// Copyright ReGuider Team, 2016-2017
//
#include <librg/streamer.h>

bool librg::streamer::qtree_t::remove(entity_t entity)
{
    auto it = std::find(entities.begin(), entities.end(), entity);
    if (it != entities.end()) {
        entities.erase(it);
    } else {
        for (auto &tree : trees) {
            if (tree.remove(entity)) return true;
        }
        return false;
    }
    return true;
}

bool librg::streamer::remove(entity_t entity, bool soft_remove)
{
    if (!soft_remove) {
        librg::streamer::remove_queue.push_back(entity);
    }

    for (auto &cached : entity_cache) {
        auto it = std::find(cached.second.begin(), cached.second.end(), entity);

        if (it != cached.second.end()) cached.second.erase(it);
    }

    return _root.remove(entity);
}
