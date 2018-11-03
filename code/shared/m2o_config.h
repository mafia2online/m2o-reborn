#define M2O_SERVER_API_REGION
// =======================================================================//
// !
// ! Main macro definitions
// !
// =======================================================================//

    #define M2O_NAME "Mafia 2 Online - Reborn"

    #define M2O_VERSION_MAJOR 1
    #define M2O_VERSION_MINOR 0
    #define M2O_VERSION_PATCH 0

    #define M2O_QUOTE_X(t) #t
    #define M2O_QUOTE(t) M2O_QUOTE_X(t)
    #define M2O_VERSION_CREATE(major, minor, patch) (((major)<<16) | ((minor)<<8) | (patch))
    #define M2O_VERSION_GET_MAJOR(version) (((version)>>16)&0xFF)
    #define M2O_VERSION_GET_MINOR(version) (((version)>>8)&0xFF)
    #define M2O_VERSION_GET_PATCH(version) ((version)&0xFF)

    #define M2O_VERSION_STRING M2O_QUOTE(M2O_VERSION_MAJOR) "." M2O_QUOTE(M2O_VERSION_MINOR) "." M2O_QUOTE(M2O_VERSION_PATCH)
    #define M2O_VERSION_NUMBER M2O_VERSION_CREATE(M2O_VERSION_MAJOR, M2O_VERSION_MINOR, M2O_VERSION_PATCH)
    #define M2O_VERSION_PRETTY M2O_NAME ". v" M2O_VERSION_STRING

    #define M2O_ENTITY_LIMIT 16000
    #define M2O_INVALID_ENTITY M2O_ENTITY_LIMIT

#undef M2O_SERVER_API_REGION

#define M2O_PLATFORM_ID 218
#define M2O_VERSION_PROTOCOL 1 /* changing parameter, blocks old clients from connecting */

#define M2O_DEBUG 1
#define LIBRG_DEBUG 1

#define M2O_TICKRATE_SERVER 10 /* updates per second*/
#define M2O_TICKRATE_CLIENT 10 /* updates per second*/

// =======================================================================//
// !
// ! Other sutff used everywhere
// !
// =======================================================================//

#define Vector2 zplm_vec2
#define Vector3 zplm_vec3
#define Quaternion zplm_quat

// TODO: refactor
// proxy some stuff
#define vec2_t zplm_vec2
#define vec3_t zplm_vec3
#define quat_t zplm_quat
#define vec2 zplm_vec2
#define vec3 zplm_vec3
#define vec4 zplm_vec4
#define vec2f zplm_vec2f
#define vec3f zplm_vec3f
#define vec4f zplm_vec4f

// helpers
#define when(init, name) auto name = init; if (name)
#define when_assert(init, name) auto name = init; librg_assert(name); if (name)

#ifdef M2O_SERVER
#define mod_log zpl_printf
#endif

#define print_posm(vec, msg) mod_log("%s %f %f %f\n", msg, vec.x, vec.y, vec.z)
#define print_pos(vec) print_posm(vec, "print_pos: ")

#define m2o_assert librg_assert
#define m2o_assert_msg librg_assert_msg
#define mod_assert librg_assert
#define mod_assert_msg librg_assert_msg

// =======================================================================//
// !
// ! Stuff used by server api generator
// !
// =======================================================================//

#define M2O_FETCH_ENTITY(dest, id, returned) \
    auto dest = librg_entity_fetch(ctx, id); \
    if (!dest) { m2o_error_set(M2O_UNDEFINED_ENTITY); return returned; }
