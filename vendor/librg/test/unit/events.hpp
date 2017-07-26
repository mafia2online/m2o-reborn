#include <librg/events.h>
#include <string>

namespace unit
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
        describe("events.h", [](case_t it) {

            librg::events_initialize();

            auto eventId = librg::events::_events.end();

            it("should be able to register an event", [&eventId](vald_t validate) {
                eventId = librg::events::add("testEvent", test_event_cb, test_event_proxy);

                validate(eventId != librg::events::_events.end());
            });

            it("should be able to register a second event", [&eventId](vald_t validate) {
                librg::events::add("testEventNotUsed", test_event_cb, test_event_proxy);

                validate(eventId != librg::events::_events.end());
            });

            it("should be able to trigger an event using direct reference", [eventId](vald_t validate) {
                number = 5;
                int adder = 10;
                int mult  = 2;
                auto testData = new test_event_t { adder, mult };

                librg::events::trigger(eventId, EVENT_PARAM(testData, [=](Sqrat::Array *array) {
                    array->Append(adder);
                    array->Append(mult);
                }), true);

                validate(number == 25);
            });

            it("should be able to trigger an event using string lookup", [eventId](vald_t validate) {
                number = 5;
                int adder = 10;
                int mult  = 2;
                auto testData = new test_event_t { adder, mult };

                librg::events::trigger("testEvent", EVENT_PARAM(testData, [=](Sqrat::Array *array) {
                    array->Append(adder);
                    array->Append(mult);
                }), true);

                validate(number == 25);
            });

            librg::events_terminate();
        });
    }
}
