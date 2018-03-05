#define ZPL_IMPLEMENTATION
#include "zpl.h"

typedef enum evarg_type {
    EVARG_POINTER,
    EVARG_STRING,
    EVARG_INTEGER,
    EVARG_REAL,
} evarg_type;

typedef struct evarg_value {
    evarg_type type;
    usize size;

    union {
        void *pointer;
        char *string;
        i64 integer;
        f64 real;
    };
} evarg_value;

typedef struct evarg {
    evarg_value* values;
    usize size;
} evarg;

usize evarg_size(evarg *arg) {
    return arg->size;
}

evarg_value *evarg_get(evarg *arg, usize i) {
    ZPL_ASSERT(arg && i < zpl_array_count(arg->values));
    return &arg->values[i];
}

void trigger_event(const char *name, evarg *args) {
    zpl_printf("hello from my event: %s, with %d args\n", name, evarg_size(args));

    for (int i = 0; i < evarg_size(args); ++i) {
        evarg_value *value = evarg_get(args, i);

        switch (value->type) {
            case EVARG_REAL:    zpl_printf("got a real: %f\n", value->real); break;
            case EVARG_INTEGER: zpl_printf("got an integer: %d\n", value->integer); break;
            case EVARG_STRING:  zpl_printf("got a string: %s\n", value->string); break;
        }
    }

    zpl_printf("done\n\n");
}
