#ifndef librg_unit_tests
#define librg_unit_tests

#include "utils/fs.hpp"
#include "utils/aabb.hpp"
#include "streamer.hpp"
//#include "events.hpp"

namespace unit
{
    void run_tests()
    {
        // utils
        fs_test();
        aabb_test();

        // main
        streamer_test();
        //events_test();
    }
}

#endif // librg_unit_tests
