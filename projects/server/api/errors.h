//@ M2O_EXPORT_REGION_START
typedef enum {
    M2O_ERROR_NONE,
    M2O_UNDEFINED_ENTITY,
} m2o_error;
//@ M2O_EXPORT_REGION_END

static m2o_error m2o_last_error = M2O_ERROR_NONE;

m2o_export void m2o_set_last_error(m2o_error error) {
    m2o_last_error = error;
}

m2o_export m2o_error m2o_get_last_error() {
    m2o_error last_error = m2o_last_error;
    m2o_last_error = M2O_ERROR_NONE;
    return last_error;
}

