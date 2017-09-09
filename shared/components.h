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
