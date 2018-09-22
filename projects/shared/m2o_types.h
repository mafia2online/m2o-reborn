// =======================================================================//
// !
// ! Generic flags and constants
// !
// =======================================================================//

/* events/messages */
enum {
    MOD_CAR_CREATE = LIBRG_EVENT_LAST,
    MOD_CAR_ENTER_START,
    MOD_CAR_ENTER_FINISH,
    MOD_CAR_EXIT_START,
    MOD_CAR_EXIT_FINISH,
    MOD_CAR_INTERACTION_FINISH,

    MOD_PED_CREATE,

    MOD_USER_SET_NAME,
    MOD_USER_MESSAGE,
};

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

#ifdef M2O_CLIENT
// t is a value that goes from 0 to 1 to interpolate in a C1 continuous way across uniformly sampled data points.
// when t is 0, this will return B.  When t is 1, this will return C.
inline f32 cubic_hermite_f32(float A, float B, float C, float D, float t)
{
    float a = -A / 2.0f + (3.0f*B) / 2.0f - (3.0f*C) / 2.0f + D / 2.0f;
    float b = A - (5.0f*B) / 2.0f + 2.0f*C - D / 2.0f;
    float c = -A / 2.0f + C / 2.0f;
    float d = B;

    return a*t*t*t + b*t*t + c*t + d;
}

struct interpolate3_hermite_t {
    vec3_t A;
    vec3_t B;
    vec3_t C;
    vec3_t D;
};

inline void cubic_hermite_v3_value(interpolate3_hermite_t *src, vec3_t value) {
    src->A = src->B;
    src->B = src->C;
    src->C = src->D;
    src->D = value;

    if (src->C == zplm_vec3f_zero()) {
        src->A = src->D;
        src->B = src->D;
        src->C = src->D;
    }
}

inline vec3_t cubic_hermite_v3(vec3_t A, vec3_t B, vec3_t C, vec3_t D, f32 t) {
    return vec3f(
        cubic_hermite_f32(A.x, B.x, C.x, D.x, t),
        cubic_hermite_f32(A.y, B.y, C.y, D.y, t),
        cubic_hermite_f32(A.z, B.z, C.z, D.z, t)
    );
}

inline vec3_t cubic_hermite_v3_interpolate(interpolate3_hermite_t *inter, f32 t) {
    return cubic_hermite_v3(inter->A, inter->B, inter->C, inter->D, t);
}



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
    char name[128];

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

#ifdef M2O_CLIENT
    wchar_t cached_name[64];
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
        target_entityid = M2O_INVALID_ENTITY;
    };
#endif

    ped_t() {
        zpl_zero_item(this);
        target_entityid = M2O_INVALID_ENTITY;
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

#ifdef M2O_CLIENT

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
