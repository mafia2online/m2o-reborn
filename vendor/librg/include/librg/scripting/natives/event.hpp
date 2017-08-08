// Copyright ReGuider Team, 2016-2017
//
#ifndef _scripting_event
#define _scripting_event

namespace librg
{
    namespace scripting
    {
        /**
         * Public API method for adding event listener.
         * Multiple event handlers per event name are supported.
         *
         * @param eventName Event name under which the listener should be registered.
         * @param callback    Squirrel wrapped function object.
         */
        inline static void event_add_listener(const char *eventName, Sqrat::Function callback)
        {
            
        }

        /**
         * Public API method for dispatching an event.
         * This method passes handler arguments via Squirrel array.
         * This method calls all registered handlers under specified event name.
         *
         * @param eventName   Name of the event to call.
         * @param params      Array of values.
         */
        inline static void event_dispatch(const char *eventName, Sqrat::Array array)
        {

        }

        /**
         * Registry method
         * @param table
         */
        inline static void event_install(Sqrat::Table& table)
        {
            table.Func("eventAddListener",  &event_add_listener);
            table.Func("eventDispatch",     &event_dispatch);
        }
    }
}

#endif // _scripting_event
