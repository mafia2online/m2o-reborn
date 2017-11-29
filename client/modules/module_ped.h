#include "callbacks_ped/create.h"
#include "callbacks_ped/update.h"
#include "callbacks_ped/remove.h"
#include "callbacks_ped/interpolate.h"

inline void module_ped_init() {
    librg_event_add(ctx, LIBRG_ENTITY_CREATE, module_ped_callback_create);
    librg_event_add(ctx, LIBRG_ENTITY_UPDATE, module_ped_callback_update);
    librg_event_add(ctx, LIBRG_ENTITY_REMOVE, module_ped_callback_remove);
    librg_event_add(ctx, LIBRG_CLIENT_STREAMER_UPDATE, module_ped_callback_clientstream);
}

inline void module_ped_tick() {
    librg_entity_iterate(ctx, LIBRG_ENTITY_ALIVE, module_ped_callback_interpolate);
}
