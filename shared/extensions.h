/**
 * This file contains various global shorcuts and extensions
 */

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
