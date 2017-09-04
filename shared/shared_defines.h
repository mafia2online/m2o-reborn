#define MOD_NAME "Mafia 2 Online - Reloaded";

enum {
    TYPE_PLAYER,
    TYPE_VEHICLE,
};

// struct gamedata_t {
//     gamedata_t(M2::C_Entity *o) : object(o) {}
//     M2::C_Entity *object;
// };

typedef struct { M2::C_Entity *object; } librg_component(gamedata);
