#ifndef librg_events_default_h
#define librg_events_default_h

#include <librg/network.h>
#include <librg/entities.h>
#include <librg/components/transform.h>

namespace librg
{
    namespace events
    {
        /**
         * Default event
         */
        struct event_t {
            void* data;
        };

        /**
         * Tick event
         */
        struct event_tick_t {
            void* data;
            uint64_t tick;
            double dt;
        };

        /**
         * Log event
         */
        struct event_log_t {
            event_log_t(std::string s) : output(s) {}

            void* data;
            std::string output;
        };

        /**
         * Entity event
         */
        struct event_entity_t {
            event_entity_t() {}
            event_entity_t(entity_t e) : entity(e) {}
            event_entity_t(entity_t e, uint64_t g) : entity(e), guid(g) {}
            event_entity_t(entity_t e, uint64_t g, uint8_t t) : entity(e), guid(g), type(t) {}
            event_entity_t(void* d, entity_t e, uint64_t g, uint8_t t) : data(d), entity(e), guid(g), type(t) {}

            void* data;
            entity_t entity;
            uint64_t guid;
            uint8_t type;
        };

        /**
         * Bistreamed entity event
         */
        struct event_bs_entity_t {
            event_bs_entity_t(network::bitstream_t* d, entity_t e, uint64_t g, uint8_t t) : data(d), entity(e), guid(g), type(t) {}

            network::bitstream_t* data;
            entity_t entity;
            uint64_t guid;
            uint8_t type;
        };

        /**
         * Backwards commpability
         */
        using event_create_t = event_bs_entity_t;
        using event_update_t = event_bs_entity_t;
        using event_remove_t = event_bs_entity_t;
        using event_connect_t = event_bs_entity_t;
        using event_disconnect_t = event_bs_entity_t;
    }
}

#endif // librg_events_default_h
