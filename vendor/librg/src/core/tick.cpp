// Copyright ReGuider Team, 2016-2017
//
#include <uv.h>
#include <librg/core.h>

using namespace librg;

/**
 * Main ticker method
 */
void core::tick() {
    if (core::is_manual()) {
        uv_run(uv_default_loop(), UV_RUN_NOWAIT);
    }
}
