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
    vec3_t lvelocity;
    quat_t lrotation;
    vec3_t tposition;
    vec3_t tvelocity;
    quat_t trotation;
} librg_component(interpolate);
