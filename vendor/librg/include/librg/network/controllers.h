// Copyright ReGuider Team, 2016-2017
//
#ifndef librg_network_controllers_h
#define librg_network_controllers_h

#include <librg/network.h>

/**
 * MACRO
 * for defining controller action
 */
#define controller_action(name) name(peer_t* peer, packet_t* packet, bitstream_t* data)

namespace librg
{
    namespace network
    {
        namespace connection_controller
        {
            void controller_action(init);
            void controller_action(request);
            void controller_action(refuse);
            void controller_action(accept);
            void controller_action(disconnect);
        }

        namespace entity_controller
        {
            void controller_action(create);
            void controller_action(update);
            void controller_action(client_streamer_add);
            void controller_action(client_streamer_remove);
            void controller_action(client_streamer_update);
        }
    }
}

#undef controller_action

#endif // librg_network_controllers_h
