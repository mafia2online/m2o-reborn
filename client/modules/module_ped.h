#include "callbacks_ped/create.h"
#include "callbacks_ped/update.h"
#include "callbacks_ped/remove.h"
#include "callbacks_ped/interpolate.h"

inline void module_ped_init() {
    librg_event_add(LIBRG_ENTITY_CREATE, module_ped_callback_create);
    librg_event_add(LIBRG_ENTITY_UPDATE, module_ped_callback_update);
    librg_event_add(LIBRG_ENTITY_REMOVE, module_ped_callback_remove);
    librg_event_add(LIBRG_CLIENT_STREAMER_UPDATE, module_ped_callback_clientstream);
}

inline void module_ped_tick() {
    librg_entity_filter_t filter = {0};
    if (librg_index_interpolate() == 0) return;

    filter.contains1 = librg_index_interpolate();
    filter.excludes1 = librg_index_clientstream();

    librg_entity_each(filter, module_ped_callback_interpolate);
}
