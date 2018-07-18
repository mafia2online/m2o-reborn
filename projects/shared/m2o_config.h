// =======================================================================//
// !
// ! Main configuration macro definitions
// !
// =======================================================================//

#define M2O_NAME "Mafia 2 Online - Reborn"

#define M2O_VERSION_MAJOR 1
#define M2O_VERSION_MINOR 0
#define M2O_VERSION_PATCH 0

#define M2O_PLATFORM_ID 218
#define M2O_VERSION_PROTOCOL 1 /* changing parameter, blocks old clients from connecting */

#define M2O_QUOTE_X(t) #t
#define M2O_QUOTE(t) M2O_QUOTE_X(t)

#define M2O_VERSION_STRING M2O_QUOTE(M2O_VERSION_MAJOR) "." M2O_QUOTE(M2O_VERSION_MINOR) "." M2O_QUOTE(M2O_VERSION_PATCH)
#define M2O_VERSION_NUMBER LIBRG_VERSION_CREATE(M2O_VERSION_MAJOR, M2O_VERSION_MINOR, M2O_VERSION_PATCH)

#define M2O_VERSION_PRETTY M2O_NAME ". v" M2O_VERSION_STRING
#define M2O_ENTITY_LIMIT 16000
#define M2O_INVALID_ENTITY M2O_ENTITY_LIMIT

#define M2O_DEBUG 1
#define LIBRG_DEBUG 1

// =======================================================================//
// !
// ! Other sutff used everywhere
// !
// =======================================================================//

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define Vector2 zplm_vec2
#define Vector3 zplm_vec3
#define Quaternion zplm_quat

// TODO: refactor
// proxy some stuff
#define vec2_t zplm_vec2
#define vec3_t zplm_vec3
#define quat_t zplm_quat
#define vec3f zplm_vec3f
#define vec3 zplm_vec3

// helpers
#define when(init, name) auto name = init; if (name)
#define when_assert(init, name) auto name = init; librg_assert(name); if (name)

#ifdef M2O_SERVER
#define mod_log librg_log
#endif

#define print_posm(vec, msg) mod_log("%s %f %f %f\n", msg, vec.x, vec.y, vec.z)
#define print_pos(vec) print_posm(vec, "print_pos: ")

#define m2o_assert librg_assert
#define m2o_assert_msg librg_assert_msg
#define mod_assert librg_assert
#define mod_assert_msg librg_assert_msg

#define m2o_export
#define m2o_import

#define M2O_FETCH_ENTITY(dest, id, returned) \
    auto dest = librg_entity_fetch(ctx, id); \
    if (!dest) { m2o_set_last_error(M2O_UNDEFINED_ENTITY); return returned; }
