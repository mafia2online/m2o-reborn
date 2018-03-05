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

void evarg_init(evarg *arg) {
    zpl_array_init(arg->values, zpl_heap());
}

void evarg_free(evarg *arg) {
    zpl_array_free(arg->values);
}

void evarg_push_string(evarg *arg, const char *string) {
    evarg_value value = {0}; {
        value.type = EVARG_STRING;
        value.size = zpl_strlen(string);
        value.string = (char *)string;
    }

    zpl_array_append(arg->values, value);
    arg->size++;
}

void evarg_push_integer(evarg *arg, i64 integer) {
    evarg_value value = {0}; {
        value.type = EVARG_INTEGER;
        value.integer = integer;
    }

    zpl_array_append(arg->values, value);
    arg->size++;
}

void evarg_push_real(evarg *arg, f64 real) {
    evarg_value value = {0}; {
        value.type = EVARG_REAL;
        value.real = real;
    }

    zpl_array_append(arg->values, value);
    arg->size++;
}

void evarg_push_pointer(evarg *arg, void *pointer) {
    evarg_value value = {0}; {
        value.type = EVARG_POINTER;
        value.pointer = pointer;
    }

    zpl_array_append(arg->values, value);
    arg->size++;
}

typedef void (callback)(const char *, evarg *);

int main() {
    zpl_dll_handle dll = zpl_dll_load("plugin.dylib");
    ZPL_ASSERT(dll);

    callback *trigger_event = (callback *)zpl_dll_proc_address(dll, "trigger_event");
    ZPL_ASSERT(trigger_event);

    evarg args = {0};
    evarg_init(&args);

    evarg_push_string(&args, "hello world");
    evarg_push_integer(&args, 125);
    evarg_push_real(&args, 892323232.29999994);

    trigger_event("foobar", &args);

    evarg_free(&args);
    return 0;
}
