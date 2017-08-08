// Copyright ReGuider Team, 2016-2017
//
#ifndef librg_timing_hpp
#define librg_timing_hpp

#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
static double _librg_freq;
static bool   _librg_initialized = false;
#elif defined(__APPLE__)||defined(__linux__)
#include <sys/time.h>
static const long NANOSECONDS_PER_SECOND = 1000000000L;
#else
#include <chrono>
static std::chrono::system_clock::time_point m_epoch = std::chrono::high_resolution_clock::now();
#endif

namespace librg
{
    static inline double get_time()
    {
#if defined(WIN32)
        if (!_librg_initialized) {
            LARGE_INTEGER freq;

            QueryPerformanceFrequency(&freq);

            _librg_freq = (double)freq.QuadPart;
            _librg_initialized = true;
        }

        LARGE_INTEGER ticks;

        QueryPerformanceCounter(&ticks);

        return (double)(ticks.QuadPart / _librg_freq);
#elif defined(__APPLE__) || defined(__linux__)
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        return (double)(((long)ts.tv_sec * NANOSECONDS_PER_SECOND) + ts.tv_nsec) / ((double)(NANOSECONDS_PER_SECOND));
#else
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_epoch).count() / 1000000000.0;
#endif
    }
}

#endif // librg_timing_hpp