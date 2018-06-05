/** @file entities.hpp
*  @brief Game's entities wrapper
*
*
*  @author Tyldar
*/
#pragma once

namespace M2
{
    enum eEntityType
    {
        MOD_ENTITY_CAR,
        MOD_ENTITY_PED
    };

    namespace Wrappers
    {
        void PreloadModels();
        C_Entity *CreateEntity(eEntityType type, int modelID);
        bool DestroyEntity(C_Entity *entity, eEntityType type);
    };
};
