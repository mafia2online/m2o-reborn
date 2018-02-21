enum {
    M2O_ERROR_NONE,
    M2O_UNDEFINED_ENTITY,
};

static u32 m2o_last_error = M2O_ERROR_NONE;

m2o_export void m2o_set_last_error(u32 error) {
    m2o_last_error = error;
}

m2o_export u32 m2o_get_last_error() {
    u32 last_error = m2o_last_error;
    m2o_last_error = M2O_ERROR_NONE;
    return last_error;
}

