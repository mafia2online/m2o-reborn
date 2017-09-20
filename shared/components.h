/**
 * Entity types
 */
enum {
    TYPE_PED,
    TYPE_CAR,
    TYPE_OTHER,
};

#define TYPE_PLAYER TYPE_PED


/**
 * Entity components
 * (shared)
 */
typedef struct {
    zplm_vec3_t direction;
    f32 speed;
    i8 move_state;
    b8 is_accelerating;
} librg_component(ped);

typedef struct {
    f32 wheel_turn;
    f32 brake;
    i16 model;
    i8 gear;
} librg_component(car);


/**
 * Entity components
 * (client)
 */
#ifdef MOD_CLIENT

    typedef struct {
        vec3_t lposition;
        quat_t lrotation;

        vec3_t tposition;
        quat_t trotation;
        // vec3_t velocity;

        f32 delta;
        i32 step;
    } librg_component(interpolate);

    typedef struct {
        M2::C_Entity *object;
        M2::Wrappers::GameModelManager *pGameModelManager;
    } librg_component(gamedata);

#endif
