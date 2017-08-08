// Basic framework test suite.

#ifndef librg_test
#define librg_test

#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <string>
#include <chrono>

using namespace std::chrono;

/**
 * Glorious testing suite
 * (ported from sq)
 */
static int __passed = 0;
static int __total  = 0;

constexpr int PERFORMANCE_TEST_DURATION = 5000;

using vald_t = std::function<void(bool)>;
using cscb_t = std::function<void(vald_t)>;
using cfcb_t = std::function<void()>;
using case_t = std::function<void(std::string, cscb_t)>;
using perc_t = std::function<void(std::string, cfcb_t)>;
using desc_t = std::function<void(case_t)>;
using perf_t = std::function<void(perc_t)>;

static inline void describe(std::string ent, desc_t descinner) {
    printf("\n  Testing %s:\n", ent.c_str());

    descinner([](std::string condition, cscb_t callback) {
        try {
            callback([condition](bool result) {
                if (result) {
                    printf("    \x1B[32m[✓]\x1B[0m It %s - passed\n", condition.c_str()); __passed++;
                }
                else {
                    printf("    \x1B[31m[✗]\x1B[0m It %s - failed\n", condition.c_str());
                }

                __total++;
            });

        }
        catch (std::exception) {
            printf("  \x1B[31m[✗]\x1B[0m It %s - failed (exception)\n", condition.c_str()); __total++;
        }
    });
}

static inline void benchmark(std::string ent, perf_t perfinner) {
    printf("\n  Testing %s:\n", ent.c_str());

    perfinner([](std::string condition, cfcb_t callback) {
        milliseconds startms = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
        );

        milliseconds totalms(0);
        int totalticks = 0;

        while (duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - startms < milliseconds(PERFORMANCE_TEST_DURATION)) {
            milliseconds beginms = duration_cast< milliseconds >(
                system_clock::now().time_since_epoch()
            );
            {
                callback();
            }
            milliseconds endms = duration_cast< milliseconds >(
                system_clock::now().time_since_epoch()
            );

            totalms += endms - beginms;
            ++totalticks;
        }

        auto durationms = totalms / (double)totalticks;
        printf("    \x1B[32m[✓]\x1B[0m Measure %s - took %f ms.\n", condition.c_str(), durationms.count());
    });
}

static inline void motd()
{
    puts("Starting librg unit testing...");
}

static inline int results()
{
    printf("\nTotal passed %d/%d, total failed %d.\n", __passed, __total, __total - __passed);
    return __total - __passed;
}

#endif // librg_test
