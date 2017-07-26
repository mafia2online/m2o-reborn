#ifndef ump_callbacks_h
#define ump_callbacks_h

#include <librg/librg.h>

#define LIBRG_EVENT librg::events::event_t* evt

void ontick(LIBRG_EVENT);
void entity_create(LIBRG_EVENT);
void entity_update(LIBRG_EVENT);
void entity_remove(LIBRG_EVENT);
void entity_inter(LIBRG_EVENT);
void clientstream_update(LIBRG_EVENT);

#undef LIBRG_EVENT

#endif // ump_callbacks_h