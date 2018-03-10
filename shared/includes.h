// =======================================================================//
// !
// ! Main configuration macro definitions
// !
// =======================================================================//

#define MOD_NAME "Mafia 2 Online - Reborn"

#define MOD_VERSION_MAJOR 1
#define MOD_VERSION_MINOR 0
#define MOD_VERSION_PATCH 0

#define MOD_VERSION_PROTOCOL 1 /* changing parameter, blocks old clients from connecting */

#define MOD_QUOTE_X(t) #t
#define MOD_QUOTE(t) MOD_QUOTE_X(t)

#define MOD_VERSION_STRING MOD_QUOTE(MOD_VERSION_MAJOR) "." MOD_QUOTE(MOD_VERSION_MINOR) "." MOD_QUOTE(MOD_VERSION_PATCH)
#define MOD_VERSION_NUMBER LIBRG_VERSION_CREATE(MOD_VERSION_MAJOR, MOD_VERSION_MINOR, MOD_VERSION_PATCH)

#define MOD_VERSION_PRETTY MOD_NAME ". v" MOD_VERSION_STRING
#define MOD_ENTITY_LIMIT 16000
#define MOD_INVALID_ENTITY MOD_ENTITY_LIMIT

// =======================================================================//
// !
// ! Other sutff used everywhere
// !
// =======================================================================//

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// #ifdef _DEBUG
#define LIBRG_DEBUG
// #endif

#define LIBRG_IMPLEMENTATION
#include <librg.h>

// third party headers
#include <string>
#include <functional>

// proxy some stuff
typedef zplm_vec2_t vec2_t;
typedef zplm_vec3_t vec3_t;
typedef zplm_quat_t quat_t;

#define vec3 zplm_vec3f

// helpers
#define when(init, name) auto name = init; if (name)
#define when_assert(init, name) auto name = init; librg_assert(name); if (name)

#ifdef MOD_SERVER
#define mod_log librg_log
#endif

#define print_posm(vec, msg) mod_log("%s %f %f %f\n", msg, vec.x, vec.y, vec.z)
#define print_pos(vec) print_posm(vec, "print_pos: ")

#define mod_assert librg_assert
#define mod_assert_msg librg_assert_msg

// TODO: move to components.h
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

#define m2o_export
#define m2o_import

#define M2O_FETCH_ENTITY(dest, id, returned) \
    auto dest = librg_entity_fetch(ctx, id); \
    if (!dest) { m2o_set_last_error(M2O_UNDEFINED_ENTITY); return returned; }
