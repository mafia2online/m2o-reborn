/**
 * This file contains various global shorcuts and extensions
 */

#include <functional>

using message_callback_t = std::function<void(librg_data_t *data)>;
using entity_callback_t  = std::function<void(librg_entity_t *entity)>;

inline void mod_message_send_all(librg_ctx_t *ctx, librg_message_id id, message_callback_t callback) {
    librg_data_t data; librg_data_init(&data); if (callback) callback(&data);
    librg_message_send_all(ctx, id, data.rawptr, librg_data_get_wpos(&data));
    librg_data_free(&data);
}

#define mod_message_send mod_message_send_all

inline void mod_message_send_to(librg_ctx_t *ctx, librg_message_id id, librg_peer_t *peer, message_callback_t callback) {
    librg_data_t data; librg_data_init(&data); if (callback) callback(&data);
    librg_message_send_to(ctx, id, peer, data.rawptr, librg_data_get_wpos(&data));
    librg_data_free(&data);
}

inline void mod_message_send_except(librg_ctx_t *ctx, librg_message_id id, librg_peer_t *peer, message_callback_t callback) {
    librg_data_t data; librg_data_init(&data); if (callback) callback(&data);
    librg_message_send_except(ctx, id, peer, data.rawptr, librg_data_get_wpos(&data));
    librg_data_free(&data);
}

inline void mod_message_send_instream(librg_ctx_t *ctx, librg_message_id id, librg_entity_id entity_id, message_callback_t callback) {
    librg_data_t data; librg_data_init(&data); if (callback) callback(&data);
    librg_message_send_instream(ctx, id, entity_id, data.rawptr, librg_data_get_wpos(&data));
    librg_data_free(&data);
}

inline void mod_message_send_instream_except(librg_ctx_t *ctx, librg_message_id id, librg_entity_id entity_id, librg_peer_t *peer, message_callback_t callback) {
    librg_data_t data; librg_data_init(&data); if (callback) callback(&data);
    librg_message_send_instream_except(ctx, id, entity_id, peer, data.rawptr, librg_data_get_wpos(&data));
    librg_data_free(&data);
}

inline void mod_entity_iterate(librg_ctx_t *ctx, u64 flags, entity_callback_t callback) {
    librg_entity_iteratex(ctx, flags, entity, { callback(librg_entity_fetch(ctx, entity)); } );
}
/*
inline void mod_message_add(librg_ctx_t *ctx, librg_message_id id, message_callback_t callback) {
    //librg_messsage_add(ctx, id, (librg_message_cb)callback.target);
    ctx->messages[id] = callback.target;
}
*/

#define librg_message_send librg_message_send_all


zplm_vec3 zplm_vec3_to_radians(zplm_vec3 value) {
    zplm_vec3 result = {0};
    result.x = zplm_to_radians(value.x);
    result.y = zplm_to_radians(value.y);
    result.z = zplm_to_radians(value.z);
    return result;
}

zplm_vec3 zplm_vec3_to_degrees(zplm_vec3 value) {
    zplm_vec3 result = {0};
    result.x = zplm_to_degrees(value.x);
    result.y = zplm_to_degrees(value.y);
    result.z = zplm_to_degrees(value.z);
    return result;
}

zplm_quat zplm_quat_from_eular(zplm_vec3 radians) {
    f32 cosZ = zplm_cos(0.5f * radians.z);
    f32 cosY = zplm_cos(0.5f * radians.y);
    f32 cosX = zplm_cos(0.5f * radians.x);

    // Calculate the sin angles
    f32 sinZ = zplm_sin(0.5f * radians.z);
    f32 sinY = zplm_sin(0.5f * radians.y);
    f32 sinX = zplm_sin(0.5f * radians.x);

    zplm_quat result = {0};

    // Compute the quaternion
    result.w = cosZ * cosY * cosX + sinZ * sinY * sinX;
    result.z = cosZ * cosY * sinX - sinZ * sinY * cosX;
    result.y = cosZ * sinY * cosX + sinZ * cosY * sinX;
    result.x = sinZ * cosY * cosX - cosZ * sinY * sinX;

    return result;
}

zplm_vec3 zplm_quat_to_eular(zplm_quat input) {
    f32 sqw = (input.w * input.w);
    f32 sqx = (input.x * input.x);
    f32 sqy = (input.y * input.y);
    f32 sqz = (input.z * input.z);

    zplm_vec3 result = {0};
    result.x = zplm_arctan2(2.f * (input.x * input.y + input.z * input.w), sqx - sqy - sqz + sqw);
    result.y = zplm_arcsin(-2.f * (input.x * input.z - input.y * input.w));
    result.z = zplm_arctan2(2.f * (input.y * input.z + input.x * input.w), -sqx - sqy + sqz + sqw);
    return result;
}

f32 zplm_offset_degrees(f32 a, f32 b) {
    f32 c = (b > a) ? b - a : 0.0f - (a - b);
    if (c > 180.0f) { c = 0.0f - (360.0f - c); }
    else if (c <= -180.0f) { c = (360.0f + c); }
    return c;
}

