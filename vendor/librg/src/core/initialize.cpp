// Copyright ReGuider Team, 2016-2017
//
#include <uv.h>

#ifdef WIN32
#include <clocale>
#endif

#include <cstring>

#include <librg/core.h>
#include <librg/events.h>
#include <librg/entities.h>
#include <librg/network.h>
#include <librg/resources.h>
#include <librg/streamer.h>
#include <librg/utils/timing.hpp>

using namespace librg;

uv_timer_t librg_poll_loop;
uv_tty_t librg_tty;

double librg_lasttime;
config_t core::config;

/**
 * Main polling method
 */
void on_poll_loop(uv_timer_t* req)
{
    network::poll();

    if (core::is_client()) {
        double newtime = get_time();
        librg::entities_interpolate((newtime - librg_lasttime) * 100.0);
        librg_lasttime = newtime;
    }
}

/**
 * Alloc callback for allocating input memory
 * @param handle         tty handle
 * @param suggested_size suggensted size by uv (65536 in most cases)
 * @param buf            buffer, where data will be written to, and read from by us
 */
static void tty_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
    buf->base = new char[1024];
    buf->len = 1024;
}

/**
 * On user console message
 * @param stream tty handle
 * @param nread  size of string
 * @param buf    buffer with data
 */
void on_console(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
    buf->base[nread] = '\0';
    // librg::core::onInput(buf->base);
    // add console event
    // core::log(buf->base);

    if (strncmp(buf->base, "exit", 4) == 0) {
        uv_stop(uv_default_loop());
    }
}

void librg::core_initialize(librg::mode_e mode)
{
    librg::core::set_mode(mode);

    librg::entities_initialize();
    librg::network_initialize();
    librg::resources_initialize();
    librg::streamer_initialize();

#ifdef WIN32
    // set our locale to the C locale
    // as unicode output only functions in this locale
    std::setlocale(LC_ALL, "C");
    SetConsoleOutputCP(CP_UTF8);
#endif

    // start loops
    uv_timer_init(uv_default_loop(), &librg_poll_loop);
    uv_timer_start(&librg_poll_loop, on_poll_loop, 0, 1);

    // terminal window
    uv_tty_init(uv_default_loop(), &librg_tty, 0, 1);
    uv_tty_set_mode(&librg_tty, UV_TTY_MODE_NORMAL);

    // setup reading callback
    uv_read_start((uv_stream_t*)&librg_tty, tty_alloc, on_console);
}
