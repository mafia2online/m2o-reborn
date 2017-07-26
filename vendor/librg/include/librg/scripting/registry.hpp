// Copyright ReGuider Team, 2016-2017
//
#ifndef _scripting_registry
#define _scripting_registry

#include <librg/scripting/natives/event.hpp>
#include <librg/scripting/natives/entity.hpp>
#include <librg/scripting/natives/system.hpp>
#include <librg/scripting/natives/timer.hpp>
#include <librg/scripting/natives/resource.hpp>
#include <librg/scripting/natives/streamer.hpp>
//#include "natives/transform.hpp"

namespace librg
{
    namespace scripting
    {
        /**
         * All our bingings are going in there
         */
        inline static void registry_install(Sqrat::Table& native)
        {
            event_install(native);
            entity_install(native);
            system_install(native);
            timer_install(native);
            resource_install(native);
            streamer_install(native);
        }
    }
}

#endif // _scripting_registry
