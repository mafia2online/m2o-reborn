# ZPL - Event handler module
[![npm version](https://badge.fury.io/js/zpl_event.c.svg)](https://badge.fury.io/js/zpl_event.c)

Small event registration and callback management module. 
Handles simple event broadcasting using a small but yet simple to use API.

## Important note
Event handler uses hash table to store array of callbacks per
each valid event type.

Each event callback receives an anonymous pointer
which has to be casted to the proper base type.

### Example
1. Initialize event pool.
2. Declare your event callbacks and any data layout
  used by the events.
3. Add event callbacks to the pool. (Returns callback ID.)
4. Trigger arbitrary event in pool with specified dataset.
5. (Optional) Remove arbitrary event callback
  by refering to it through event type and its callback ID.


## Usage

```c
#define ZPL_IMPLEMENTATION
#define ZPLEV_IMPLEMENTATION
#include <zpl.h>
#include <zpl_event.h>

typedef struct {
    u64 a,b,c;
} test_e1_t;

enum {
    test_e1_ev,
};

ZPLEV(test_e1_v1) {
    ZPLEV_CAST(test_e1_t, data);

    zpl_printf("a: %lu b: %lu c: %lu\n", data->a, data->b, data->c);
};

ZPLEV(test_e1_v2) {
    ZPLEV_CAST(test_e1_t, data);

    zpl_printf("a -- %lu b -- %lu c -- %lu\n", data->a, data->b, data->c);
};
ZPLEV(test_e1_v3) {
    ZPLEV_CAST(test_e1_t, data);

    zpl_printf("a> %lu b> %lu c> %lu\n", data->a, data->b, data->c);
};

int
main(void) {

    zplev_pool events;
    zplev_init(&events, zpl_heap_allocator());

    zplev_add(&events, test_e1_ev, test_e1_v1); // 0
    zplev_add(&events, test_e1_ev, test_e1_v2); // 1
    zplev_add(&events, test_e1_ev, test_e1_v3); // 2


    test_e1_t cb_data = {1, 2, 3};
    zplev_trigger(&events, test_e1_ev, &cb_data);

    zplev_remove(&events, test_e1_ev, 1);
    zpl_printf("After deletion of ID 1\n");

// NOTE(ZaKlaus): Prints 0,2
    zplev_trigger(&events, test_e1_ev, &cb_data);

    zplev_destroy(&events);
    return 0;
}
```