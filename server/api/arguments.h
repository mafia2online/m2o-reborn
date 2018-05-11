//@ M2O_EXPORT_REGION_START
typedef enum m2o_arg_type {
    M2O_ARG_POINTER,
    M2O_ARG_STRING,
    M2O_ARG_INTEGER,
    M2O_ARG_REAL,
} m2o_arg_type;

typedef struct m2o_arg_value {
    m2o_arg_type type;
    usize size;

    union {
        void *pointer;
        char *string;
        i64 integer;
        f64 real;
    };
} m2o_arg_value;

typedef struct m2o_args {
    m2o_arg_value* values;
    usize size;
} m2o_args;
//@ M2O_EXPORT_REGION_END

m2o_export void m2o_args_init(m2o_args *arg) {
    zpl_array_init(arg->values, zpl_heap());
}

m2o_export void m2o_args_free(m2o_args *arg) {
    zpl_array_free(arg->values);
}

m2o_export usize m2o_args_size(m2o_args *arg) {
    return arg->size;
}

m2o_export m2o_arg_value *m2o_args_get(m2o_args *arg, usize i) {
    ZPL_ASSERT(arg && i < zpl_array_count(arg->values));
    return &arg->values[i];
}

m2o_export void m2o_args_push_string(m2o_args *arg, const char *string) {
    m2o_arg_value value = {M2O_ARG_STRING, 0}; {
        value.size = zpl_strlen(string);
        value.string = (char *)string;
    }

    zpl_array_append(arg->values, value);
    arg->size++;
}

m2o_export void m2o_args_push_integer(m2o_args *arg, i64 integer) {
    m2o_arg_value value = {M2O_ARG_INTEGER, 0}; {
        value.integer = integer;
    }

    zpl_array_append(arg->values, value);
    arg->size++;
}

m2o_export void m2o_args_push_real(m2o_args *arg, f64 real) {
    m2o_arg_value value = {M2O_ARG_REAL, 0}; {
        value.real = real;
    }

    zpl_array_append(arg->values, value);
    arg->size++;
}

m2o_export void m2o_args_push_pointer(m2o_args *arg, void *pointer) {
    m2o_arg_value value = {M2O_ARG_POINTER, 0}; {
        value.pointer = pointer;
    }

    zpl_array_append(arg->values, value);
    arg->size++;
}
