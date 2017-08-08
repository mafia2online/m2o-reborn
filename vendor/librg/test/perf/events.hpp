#include <librg/events.h>
#include <string>

namespace performance
{
    int number = 5;

    // event data format
    struct test_event_t {
        int num;
        int add;
    };

    void* test_event_proxy(const void *data, Sqrat::Array *array)
    {
        if (array) {
            auto num = array->Pop().Cast<int>();
            auto add = array->Pop().Cast<int>();
            auto testData = new test_event_t { num, add };
            return (void *)testData;
        }
        return (void *)data;
    }

    void test_event_cb(const void* data, void* /* blob */)
    {
        auto testData = (test_event_t *)data;
        number += testData->add * testData->num;
    }

    void events_test()
    {
        benchmark("events.h", [](perc_t test) {

            librg::events_initialize();

            auto eventId = librg::events::add("testEvent", test_event_cb, test_event_proxy);

            test("event trigger by reference", [eventId]() {
                number = 5;
                int adder = 10;
                int mult  = 2;
                auto testData = new test_event_t { adder, mult };

                librg::events::trigger(eventId, EVENT_PARAM(testData, [=](Sqrat::Array *array) {
                    array->Append(adder);
                    array->Append(mult);
                }), true);
            });

            test("event trigger by string", [eventId]() {
                number = 5;
                int adder = 10;
                int mult  = 2;
                auto testData = new test_event_t { adder, mult };

                librg::events::trigger("testEvent", EVENT_PARAM(testData, [=](Sqrat::Array *array) {
                    array->Append(adder);
                    array->Append(mult);
                }), true);
            });

            librg::events_terminate();
        });
    }
}
