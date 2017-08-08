// Copyright ReGuider Team, 2016-2017
//
#include <librg/events.h>

using namespace librg;

void events::trigger(uint16_t id, events::event_t* event)
{
    for (auto & event_handler : events::event_handlers[id]) {
        event_handler(event);
    }

    if (event) {
        delete event;
    }
}
