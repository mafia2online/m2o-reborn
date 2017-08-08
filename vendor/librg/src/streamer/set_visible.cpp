// Copyright ReGuider Team, 2016-2017
//
#include <librg/streamer.h>

bool _set_visible_for(std::vector<librg::entity_t> &entities, librg::entity_t entity, bool state)
{
    auto &b = entities;
    auto query = std::find(b.begin(), b.end(), entity);
    if (query == b.end()) {
        if (!state) {
            b.push_back(entity);
            return true;
        } else {
            return false;
        }
    } else {
        if (state) {
            b.erase(query);
            return true;
        } else {
            return false;
        }
    }
}

bool librg::streamer::set_visible(librg::entity_t entity, bool state)
{
    return _set_visible_for(_root.blacklistedEntities, entity, state);
}

bool librg::streamer::set_visible_for(librg::entity_t target, librg::entity_t entity, bool state)
{
    auto streamable = target.component<librg::streamable_t>();
    if (!streamable) return false;

    auto &ignoredEntities = streamable->ignored;
    return _set_visible_for(ignoredEntities, entity, state);
}
