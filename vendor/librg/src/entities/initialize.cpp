// Copyright ReGuider Team, 2016-2017
//
#include <librg/entities.h>

entityx::EventManager*  librg::_events;
entityx::EntityManager* librg::entities;
entityx::SystemManager* librg::_systems;

void librg::entities_initialize()
{
    librg::_events  = new entityx::EventManager();
    librg::entities = new entityx::EntityManager(*librg::_events);
    librg::_systems = new entityx::SystemManager(*librg::entities, *librg::_events);
}
