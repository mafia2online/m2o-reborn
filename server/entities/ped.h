/**
 * On entity is created for player
 */
void ped_oncreate(librg_event_t *event)
{

}

void ped_onclient(librg_event_t *event)
{
    when_assert(librg_fetch_ped(event->entity), ped) {
        librg_data_rptr(&event->data, &ped->direction, sizeof(ped->direction));
    }
}

void ped_onupdate(librg_event_t *event)
{
    when_assert(librg_fetch_ped(event->entity), ped) {
        librg_data_wptr(&event->data, &ped->direction, sizeof(ped->direction));
    }
}
