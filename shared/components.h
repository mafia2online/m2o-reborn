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
    union {
        M2::C_Player2 *CPlayer; /* watch out, not all peds are players */
        M2::C_Human2 *CHuman;
        M2::C_Entity *CEntity;
    };

    M2::C_SyncObject *sync;

    ped_t(M2::C_Entity *ent) {
        zpl_zero_item(this);
        CEntity = ent;
    };
#endif

    ped_t() { zpl_zero_item(this); }
};

/**
 * Struct for storing the
 * vehicle type of entity data
 */
struct car_t {

    u16 model;
    i8  gear;

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
    } stream;
    #pragma pack(pop)

#ifdef MOD_CLIENT
    interpolate_t interpolate;

    /* game entity */
    union {
        M2::C_Car *CCar;
        M2::C_Entity *CEntity;
    };

    M2::C_SyncObject *sync;

    car_t(M2::C_Entity *ent) {
        zpl_zero_item(this);
        CEntity = ent;
    };

#endif

    car_t() {
        zpl_zero_item(this);
    }
};

inline ped_t *get_ped(librg_entity_t *entity) {
    mod_assert(entity && entity->user_data);
    return (ped_t *)(entity->user_data);
}

inline car_t *get_car(librg_entity_t *entity) {
    mod_assert(entity && entity->user_data);
    return (car_t *)(entity->user_data);
}
