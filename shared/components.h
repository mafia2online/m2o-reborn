/**
 * Entity types
 */
enum {
    TYPE_PED,
    TYPE_CAR,
    TYPE_OTHER,
};

#define TYPE_PLAYER TYPE_PED

enum {
    PED_ON_GROUND,
    PED_IN_CAR,
};

struct interpolate_t {
    vec3_t lposition;
    quat_t lrotation;

    vec3_t tposition;
    quat_t trotation;

    f32 delta;
    i32 step;
    b32 enabled;
};

struct ped_t {
#pragma pack(push, 1)
    struct {
        // 128
        zplm_vec3_t direction;
        f32 speed;

        // 32
        u8 state;
        i8 move_state;
        b8 is_accelerating;
        //u8 unused;
    } stream;
#pragma pack(pop)

#ifdef MOD_CLIENT
    interpolate_t interpolate;

    M2::C_Entity *object;
    M2::Wrappers::GameModelManager *pGameModelManager;
#endif
 
    ped_t() {
        zpl_zero_item(this);
    }
};

struct car_t {
#pragma pack(push, 1)
    struct {
        u16 model;

        f32 wheel_turn;
        f32 brake;
        i8  gear;
    } stream;
#pragma pack(pop)

#ifdef MOD_CLIENT
    interpolate_t interpolate;

    M2::C_Entity *object;
    M2::Wrappers::GameModelManager *pGameModelManager;
#endif

    car_t() {
        zpl_zero_item(this);
    }
};



