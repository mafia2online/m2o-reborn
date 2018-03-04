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

#ifdef MOD_CLIENT

    struct interpolate1_t {
        float last;
        float targ;

        float last_speed;
        float targ_speed;
    };

    struct interpolate2_t {
        vec2_t last;
        vec2_t targ;

        vec2_t last_speed;
        vec2_t targ_speed;
    };

    struct interpolate3_t {
        vec3_t last;
        vec3_t targ;

        vec3_t last_speed;
        vec3_t targ_speed;
    };

    struct interpolate4_t {
        quat_t last;
        quat_t targ;

        quat_t last_speed;
        quat_t targ_speed;
    };

#endif

/**
 * Struct for storing the
 * pedestrian type of entity data
 */
struct ped_t {
    u16 model;
    u8  state;
    char name[64];

    u8 seat;
    librg_entity_t *vehicle;

    u32 target_entityid;

    /**
     * Internal packed struct
     * for continious data sync
     */
    #pragma pack(push, 1)
    struct {
        zplm_vec3_t direction;
        vec3_t look_at;
        vec3_t vspeed;
        f32 speed;

        // 32
        u8 state;
        i8 move_state;
        b8 is_accelerating;
        //u8 unused;
    } stream;
    #pragma pack(pop)

#ifdef MOD_CLIENT
    f32 inter_delta;
    interpolate3_hermite_t inter_pos;

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
        target_entityid = MOD_INVALID_ENTITY;
    };
#endif

    ped_t() {
        zpl_zero_item(this);
        target_entityid = MOD_INVALID_ENTITY;
    }
};




/**
 * Struct for storing the
 * vehicle type of entity data
 */
struct car_t {
    u16 model;
    u8 state;
    i8 gear;

    librg_entity_t *librg_entity;

#ifdef MOD_CLIENT

    /* game entity */
    union {
        M2::C_Car *CCar;
        M2::C_Entity *CEntity;
    };

    M2::C_SyncObject *sync;

    f32 inter_delta;
    interpolate3_hermite_t inter_pos;
    interpolate4_t inter_rot;
    interpolate1_t inter_steer;

    void setCEntity(M2::C_Entity *entity) {
        this->CEntity = entity;
    };

#endif

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

    car_t(librg_entity_t *entity) {
        zpl_zero_item(this);
        this->librg_entity = entity;
    }

    void dbg() {
        mod_log("[dbg]:\n" \
            "car_t {\n\t" \
                "model: %d,\n\t" \
                "position: [%.2f, %.2f, %.2f],\n\t" \
                "rotation: [%.2f, %.2f, %.2f, %.2f],\n\t" \
                "speed: [%.2f, %.2f, %.2f],\n\t" \
                "steer: %.2f,\n\t" \
                "brake: %.2f,\n"\
            "}\n",
            this->model,
            this->librg_entity->position.x,
            this->librg_entity->position.y,
            this->librg_entity->position.z,
            this->stream.rotation.x,
            this->stream.rotation.y,
            this->stream.rotation.z,
            this->stream.rotation.w,
            this->stream.speed.x,
            this->stream.speed.y,
            this->stream.speed.z,
            this->stream.steer,
            this->stream.brake
        );
    }
};

// =======================================================================//
// !
// ! Helper methods
// !
// =======================================================================//

inline ped_t *get_ped(librg_entity_t *entity) {
    mod_assert(entity && entity->user_data);
    return (ped_t *)(entity->user_data);
}

inline car_t *get_car(librg_entity_t *entity) {
    mod_assert(entity && entity->user_data);
    return (car_t *)(entity->user_data);
}
