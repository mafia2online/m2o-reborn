// Copyright ReGuider Team, 2016-2017
//
#ifndef _scripting_timer
#define _scripting_timer

#include <uv.h>
#include <cstdint>

namespace librg
{
    namespace scripting
    {
        /**
         * Private method for cleaning up timers(intervals)
         * after they are finished
         * @param req
         */
        inline static void _timer_cleanup(uv_async_t* req)
        {
            delete (Sqrat::Function*)req->data;
            uv_close((uv_handle_t*) req, NULL);
        }

        /**
         * Private method for calling up callback
         * when interval iterations is triggered
         * @param req
         */
        inline static void _timer_interval_callback(uv_timer_t* req)
        {
            ((Sqrat::Function*)req->data)->Execute();
        }

        /**
         * Public api method, for creating ticking timer with provided interval
         * @param  callback    Squirrel wrapped function object, which will be added as a handler
         * @param  miliseconds delay between each tick
         * @return pointer onto created timer casted to uint
         */
        inline static std::uintptr_t timer_create(Sqrat::Function callback, uint16_t miliseconds)
        {
            uv_timer_t* timer_req = new uv_timer_t;
            timer_req->data = new Sqrat::Function(callback);

            uv_timer_init(uv_default_loop(), timer_req);
            uv_timer_start(timer_req, _timer_interval_callback, miliseconds, miliseconds);

            return reinterpret_cast<std::uintptr_t>(timer_req);
        }

        /**
         * Public api method for cleaning up created timer
         * @param req uint pointer, which was returned with setInterval
         */
        inline static void timer_destroy(std::uintptr_t req)
        {
            uv_timer_t* timer_req = reinterpret_cast<uv_timer_t*>(req);
            uv_timer_stop(timer_req);
            uv_async_t* async = new uv_async_t;
            uv_async_init(uv_default_loop(), async, _timer_cleanup);
            async->data = timer_req->data;
            uv_async_send(async);
        }

        /**
         * Registry method
         * @param native
         */
        inline static void timer_install(Sqrat::Table& native)
        {
            native.Func("timerCreate",  &timer_create);
            native.Func("timerDestroy", &timer_destroy);
        }
    }
}

#endif // _scripting_timer
