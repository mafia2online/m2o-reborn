// Copyright ReGuider Team, 2016-2017
//
#include <librg/core.h>

using namespace librg;

/**
 * Setup default mode (server)
 */
librg::mode_e current_mode = mode_server;

/**
 * Set mode of execution
 * @param mode
 */
void core::set_mode(librg::mode_e mode) {
    current_mode = mode;
}

/**
 * Get current mode of exectution
 * @return
 */
librg::mode_e core::get_mode() {
    return current_mode;
}
