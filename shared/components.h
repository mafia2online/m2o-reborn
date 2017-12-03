// =======================================================================//
// !
// ! Generic flags and constants
// !
// =======================================================================//

/* entity types */
enum {
    TYPE_PED,
    TYPE_CAR,
    TYPE_OTHER,
    TYPE_PLAYER = TYPE_PED,
};

/* ped states */
enum {
    PED_ON_GROUND,
    PED_IN_CAR,
};

/* entity flags */
enum {
    MOD_ENTITY_INTERPOLATED = (1 << 20),
    MOD_ENTITY_DRIVER       = (1 << 21),
};

// =======================================================================//
// !
// ! Generic data structs
// !
// =======================================================================//

/**
 * Struct for storing the interpolation stuff
 */
struct interpolate_t {
    vec3_t lposition;
    vec3_t tposition;

    quat_t lrotation;
    quat_t trotation;

    f32 delta;
};

/**
 * Struct for storing the
 * pedestrian type of entity data
 */
struct ped_t {

    /**
     * Internal packed struct
     * for continious data sync
     */
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

    librg_entity_t *vehicle;

#ifdef MOD_CLIENT
    interpolate_t interpolate;

    /* game entity */
    M2::C_Entity *object;
    M2::Wrappers::GameModelManager *pGameModelManager;

    ped_t(M2::C_Entity *ent) {
        zpl_zero_item(this);
        object = ent;
    };
#endif

    ped_t() { zpl_zero_item(this); }
};

/**
 * Struct for storing the
 * vehicle type of entity data
 */
struct car_t {

    /**
     * Internal packed struct
     * for continious data sync
     */
    #pragma pack(push, 1)
    struct {
        quat_t rotation;
        f32 speed;

        f32 wheel_turn;
        f32 brake;
        i8  gear;
        u16 model;
    } stream;
    #pragma pack(pop)

#ifdef MOD_CLIENT
    interpolate_t interpolate;

    /* game entity */
    M2::C_Entity *object;
    M2::Wrappers::GameModelManager *pGameModelManager;
#endif

    car_t() {
        zpl_zero_item(this);
    }
};



