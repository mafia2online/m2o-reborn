// Copyright ReGuider Team, 2016-2017
//
#include <librg/entities.h>

void librg::entities_terminate()
{
    delete librg::_systems;
    delete librg::entities;
    delete librg::_events;
}
