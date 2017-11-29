#define _CRT_SECURE_NO_WARNINGS
#define D_SCL_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifdef _DEBUG
#define LIBRG_DEBUG
#endif

#define MOD_NAME "Mafia 2 Online - Reloaded"

#define LIBRG_IMPLEMENTATION
#define ZPLJ_IMPLEMENTATION

#include <librg.h>
#include <zpl_json.h>

// third party headers
#include <string>

// proxy some stuff
typedef zplm_vec2_t vec2_t;
typedef zplm_vec3_t vec3_t;
typedef zplm_quat_t quat_t;

#define vec3 zplm_vec3

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

#define librg_message_send librg_message_send_all

enum {
    MOD_CAR_CREATE = LIBRG_EVENT_LAST,
    MOD_CAR_ENTER,
};
