#ifndef librg_perf_tests
#define librg_perf_tests

#include "streamer.hpp"

namespace performance
{
    void run_tests()
    {
        printf("%s\n", "\nRunning performance tests.");
        //events_test();
        streamer_test();
    }
}
#endif // librg_perf_tests
