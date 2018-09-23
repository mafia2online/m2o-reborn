#ifndef M2O_TYPES_H
#define M2O_TYPES_H

// =======================================================================//
// !
// ! Generic flags and constants
// !
// =======================================================================//

/* events/messages */
enum {
    /* first even exists only as a placeholder */
    M2O_FIRST_EVENT = LIBRG_EVENT_LAST,

    M2O_USER_SET_NAME,
    M2O_USER_MESSAGE,

    M2O_CAR_CREATE,
    M2O_CAR_ENTER,
    M2O_CAR_EXIT,

    M2O_PED_CREATE,
};

/* entity types */
typedef enum m2o_entity_type {
    M2O_ENTITY_PLAYER_PED,
    M2O_ENTITY_DUMMY_PED,
    M2O_ENTITY_CAR,
    M2O_ENTITY_OTHER,
} m2o_entity_type;

/* ped states */
typedef enum ped_state {
    PED_ON_GROUND,
    PED_ENTERING_CAR,
    PED_EXITING_CAR,
    PED_IN_CAR,
} ped_state;

/* car states */
typedef enum car_state {
    CAR_SPAWNED,
    CAR_DESTROYED,
} car_state;


/* entity flags */
enum {
    M2O_ENTITY_INTERPOLATED = (1 << 20),
    M2O_ENTITY_DRIVER       = (1 << 21),
};

// =======================================================================//
// !
// ! Generic data structs
// !
// =======================================================================//

typedef struct m2o_ped {
    u16 model;          /* entity model id */
    ped_state state;    /* current entity state */
    char name[128];     /* entity name */

    /* depending on the entity state it will have a vehicle it is seating in,
        or might have a target it is following/shooting at */
    union {
        librg_entity_id vehicle;
        librg_entity_id target;
    };

    u8 seat;  /* if ped is in the car, number represents a seat, (valid seat always >0) */
    u8 unsued;

    struct {
        u8 unused;
    } stream;

    /* a union representing a "autocastable" names for needed types */
    union {
        void *gameptr;

        #ifdef M2O_CLIENT
        M2::C_Player2 *CPlayer; /* watch out, not all peds are players */
        M2::C_Human2 *CHuman;
        M2::C_Entity *CEntity;
        #endif
    };
} m2o_ped;

typedef struct m2o_car {
    u16 model;          /* entity model id */
    car_state state;    /* current entity state */
    char name[128];     /* entity name */

    u8 gear;
    u8 unsued;

    struct {
        vec3 rotation;  /* vehicle euler? rotation */
        f32 steer;      /* vehicle steering wheel -1..0..1 */
    } stream;

    /* a union representing a "autocastable" names for needed types */
    union {
        void *gameptr;

        #ifdef M2O_CLIENT
        M2::C_Car *CCar;
        M2::C_Entity *CEntity;
        #endif
    };
} m2o_car;

// =======================================================================//
// !
// ! Generic methods for data structs
// !
// =======================================================================//

m2o_ped *m2o_ped_alloc(void *ptr) {
    m2o_ped _entity = {0};
    m2o_ped *entity = (m2o_ped *)zpl_alloc(zpl_heap(), sizeof(m2o_ped));

    /* copy default values */
    *entity = _entity;
    entity->gameptr = ptr;
    entity->target  = M2O_INVALID_ENTITY;

    return entity;
}

void m2o_ped_free(m2o_ped *ped) {
    zpl_free(zpl_heap(), ped);
}

m2o_ped *m2o_ped_get(librg_entity_t *entity) {
    mod_assert(entity && entity->user_data);
    return (m2o_ped *)(entity->user_data);
}

m2o_car *m2o_car_alloc(void *ptr) {
    m2o_car _entity = {0};
    m2o_car *entity = (m2o_car *)zpl_alloc(zpl_heap(), sizeof(m2o_car));

    /* copy default values */
    *entity = _entity;
    entity->gameptr = ptr;

    return entity;
}

void m2o_car_free(m2o_car *ped) {
    zpl_free(zpl_heap(), ped);
}

m2o_car *m2o_car_get(librg_entity_t *entity) {
    mod_assert(entity && entity->user_data);
    return (m2o_car *)(entity->user_data);
}

#endif // M2O_TYPES_H
