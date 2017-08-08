// Copyright ReGuider Team, 2016-2017
//
#ifndef _scripting_streamer
#define _scripting_streamer

#include <librg/streamer.h>

// TODO(zaklaus): ADD DOCS !!!

namespace librg
{
    namespace scripting
    {
        inline static void streamer_set_visible(uint64_t index, bool state)
        {
            auto entity = librg::entities->get((entity_t::Id)index);
            librg::streamer::set_visible(entity, state);
        }

        inline static void streamer_set_visible_for(uint64_t index, uint64_t targetIndex, bool state)
        {
            auto entity = librg::entities->get((entity_t::Id)index);
            auto target = librg::entities->get((entity_t::Id)targetIndex);
            librg::streamer::set_visible_for(target, entity, state);
        }

        inline static void streamer_set_query_range(uint64_t index, Sqrat::Array range)
        {
            auto entity = librg::entities->get((entity_t::Id)index);
            auto streamer = entity.component<streamable_t>();

            if (streamer) {
                auto x = range.GetValue<float>(0);
                auto y = range.GetValue<float>(1);
                auto z = range.GetValue<float>(2);

                streamer->query_range = hmm_vec3{ *x,*y,*z };
            }
        }

        // TODO: How to get VM handle here? It is required by Sqrat::Array...
        inline static int streamer_get_query_range(uint64_t index)
        {
            auto entity = librg::entities->get((entity_t::Id)index);
            auto streamer = entity.component<streamable_t>();

            if (streamer) {
                auto range = streamer->query_range;
            }

            return -1;
        }

        inline static void streamer_install(Sqrat::Table& table)
        {
            table.Func("streamerSetVisible", &streamer_set_visible);
            table.Func("streamerSetVisibleFor", &streamer_set_visible_for);
            table.Func("streamerSetQueryRange", &streamer_set_query_range);
            table.Func("streamerGetQueryRange", &streamer_get_query_range);
        }
    }
}

#endif // _scripting_streamer
