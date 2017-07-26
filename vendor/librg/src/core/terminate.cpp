// Copyright ReGuider Team, 2016-2017
//
#include <uv.h>

#include <librg/core.h>
#include <librg/entities.h>
#include <librg/network.h>
#include <librg/resources.h>
#include <librg/streamer.h>

void librg::core_terminate()
{
    librg::streamer_terminate();
    librg::entities_terminate();
    librg::network_terminate();
    librg::resources_terminate();

    // after work is done, closing loop
    uv_loop_close(uv_default_loop());
    uv_tty_reset_mode();
}
