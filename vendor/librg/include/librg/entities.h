// Copyright ReGuider Team, 2016-2017
//
#ifndef librg_entities_h
#define librg_entities_h

#include <entityx/entityx.h>
#include <entityx/Entity.h>
#include <entityx/System.h>
#include <entityx/deps/Dependencies.h>

namespace librg
{
    extern entityx::EventManager*   _events;
    extern entityx::EntityManager*  entities;
    extern entityx::SystemManager* _systems;

    using entity_t = entityx::Entity;

    void entities_initialize();
    void entities_terminate();

    // todo
    void entities_interpolate(double dt);
}

#endif // librg_entities_h
