// =======================================================================//
// !
// ! Generic flags and constants
// !
// =======================================================================//

/* entity types */
enum {
    TYPE_PED,
    TYPE_PLAYER = TYPE_PED,
    TYPE_CAR,
    TYPE_OTHER,
};

/* ped states */
enum {
    PED_ON_GROUND,
    PED_ENTERING_CAR,
    PED_EXITING_CAR,
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
    i32 step;
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
        vec3_t look_at;
        f32 speed;

        // 32
        u8 state;
        i8 move_state;
        b8 is_accelerating;
        //u8 unused;
    } stream;
    #pragma pack(pop)

    librg_entity_t *vehicle;
    u8 seat;

#ifdef MOD_CLIENT
    interpolate_t interpolate;

    /* game entity */
    M2::C_Entity *object;
    M2::Wrappers::GameModelManager *pGameModelManager;
    M2::C_SyncObject *sync;

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
        vec3_t speed;
        f32 steer;

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



