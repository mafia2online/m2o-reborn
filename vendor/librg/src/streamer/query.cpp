// Copyright ReGuider Team, 2016-2017
//
#include <librg/streamer.h>

void librg::streamer::qtree_t::query(std::vector<librg::entity_t> &visible, librg::aabb_t range, entityx::ComponentHandle<librg::streamable_t> streamable, librg::entity_t caller)
{
    if (!boundary.intersects_2d(range)) return;
    std::vector<librg::entity_t> ignoredEntities = streamable->ignored;

    for (auto &entity : entities) {
        auto position = entity.component<librg::transform_t>()->position;

        if (range.contains_2d(position)
            && std::find(ignoredEntities.begin(), ignoredEntities.end(), entity) == ignoredEntities.end()
            && std::find(blacklistedEntities.begin(), blacklistedEntities.end(), entity) == blacklistedEntities.end()
            && entity != caller
        ) {
            visible.push_back(entity);
        }
    }

    if (trees.size() == 0) return;

    for (auto &tree : trees) {
        tree.query(visible, range, streamable, caller);
    }
}

std::vector<librg::entity_t> librg::streamer::query(librg::entity_t entity)
{
    if (entity_cache.find(entity) != entity_cache.end()) {
        return entity_cache[entity];
    }

    auto visible = std::vector<librg::entity_t>();

    auto streamable = entity.component<librg::streamable_t>();
    if (!streamable) return visible;

    auto transform  = entity.component<librg::transform_t>();
    if (!transform) return visible;

    auto qrange   = streamable->query_range;
    auto position = transform->position;
    auto range    = librg::aabb_t(position, qrange);

    _root.query(visible, range, streamable, entity);

    entity_cache[entity] = visible;

    return visible;
}
