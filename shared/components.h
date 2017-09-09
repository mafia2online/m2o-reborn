enum {
    TYPE_PLAYER,
    TYPE_VEHICLE,
};

typedef struct {
    zplm_vec3_t direction;
} librg_component(ped);


typedef struct {
    f32 brake;
    i16 model;
    i8 gear;
} librg_component(car);

typedef struct {
    vec3_t lposition;
    quat_t lrotation;

    vec3_t tposition;
    quat_t trotation;
    // vec3_t velocity;

    f32 delta;
} librg_component(interpolate);
