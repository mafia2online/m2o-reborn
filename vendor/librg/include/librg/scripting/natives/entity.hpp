// Copyright ReGuider Team, 2016-2017
//
#ifndef _scripting_entity
#define _scripting_entity

#include <librg/entities.h>

namespace librg
{
    namespace scripting
    {
        /**
         * Public API method for creating an entity.
         */
        inline static uint64_t entity_create()
        {
            entityx::Entity entity = librg::entities->create();
            return entity.id().id();
        }

        /**
         * Public API method for destroying an event.
         * @param index   An ID of the entity.
         */
        inline static void entity_destroy(uint64_t index)
        {
            librg::entities->get((entityx::Entity::Id)index).destroy();
        }

        /**
         * Registry method
         * @param table
         */
        inline static void entity_install(Sqrat::Table& table)
        {
            table.Func("entityCreate",  &entity_create);
            table.Func("entityDestroy", &entity_destroy);
        }
    }
}

#endif // _scripting_entity
