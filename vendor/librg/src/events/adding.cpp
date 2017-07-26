// Copyright ReGuider Team, 2016-2017
//
#include <librg/events.h>

using namespace librg;

std::array<std::vector<events::callback_t>, LIBRG_EVENT_HANDLERS> events::event_handlers;

/**
 * Adds a callback listener to a particular event
 * @param event id. for custom events, id should be >= last_event_number
 * @param callback
 */
void events::add(uint16_t id, events::callback_t callback) {
    events::event_handlers[id].push_back(callback);
}

/**
 * Sets event listener with a particular slot
 * @param event id same as in add method
 * @param slot on which we should insert the event
 * @param callback
 * @retval = true, if other event was rewritten
 * @retval = false, if event was just appended
 */
bool events::set(uint16_t id, size_t slot, events::callback_t callback) {
    if (events::event_handlers[id].size() > slot) {
        events::event_handlers[id][slot] = callback;
        return true;
    }
    else {
        events::add(id, callback);
        return false;
    }
}

/**
 * Shortcut for set method saving in slot 0
 * @param event id same as in add method
 * @param callback
 */
bool events::set(uint16_t id, events::callback_t callback) {
    return events::set(id, 0, callback);
}
