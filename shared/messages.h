#include <librg/network.h>

enum {
    GAME_ON_SHOOT = librg::network::last_packet_number,
    GAME_SYNC_PACKET,
    GAME_SPAWN_VEHICLE,
    GAME_PED_ENTER_VEHICLE,
    GAME_PED_EXIT_VEHICLE,
};
