/** @file entities.hpp
*  @brief Game's entities wrapper
*
*
*  @author Tyldar
*/
#pragma once
#include "CommonHeaders.h"

#include "m2sdk.h"

#include <string>
#include <Windows.h>

namespace M2
{
    enum eEntityType
    {
        MOD_ENTITY_CAR,
        MOD_ENTITY_PED
    };

    namespace Wrappers
    {
        C_Entity *CreateEntity(eEntityType type, int modelID)
        {
            M2::Wrappers::ModelManager *pModelManager = new M2::Wrappers::ModelManager();
            M2::Wrappers::GameModelManager *pPedModelManager = nullptr;
            mod_assert(pModelManager);

            M2::C_Entity *entity = nullptr;
            std::string dir;
            std::string model;
            switch (type)
            {
                case MOD_ENTITY_PED:
                {
                    M2::Models::GetPlayerModelFromID(modelID, &dir, &model);

                    pPedModelManager = pModelManager->Load(dir.c_str(), model.c_str());
                    mod_assert(pPedModelManager);

                    entity = reinterpret_cast<M2::C_Entity*>(M2::C_EntityFactory::Get()->CreateEntity<M2::C_Human2>(M2::EntityTypes::Entity_Human));
                    mod_assert(entity);
                }
                break;

                case MOD_ENTITY_CAR:
                {
                    M2::Models::GetVehicleModelFromID(modelID, &dir, &model);

                    pPedModelManager = pModelManager->Load(dir.c_str(), model.c_str());
                    mod_assert(pPedModelManager);

                    entity = reinterpret_cast<M2::C_Entity*>(M2::C_EntityFactory::Get()->CreateEntity<M2::C_Car>(M2::EntityTypes::Entity_Car));
                    mod_assert(entity);
                }
                break;
            }

            M2::C_Model *pModel = M2::C_Core::Get()->AllocateModel(2);
            mod_assert(pModel);

            pModel->CloneHierarchy(pPedModelManager->GetModelManager()->m_pModel);
            pModel->SetName("m2o_entity");
            pModel->MarkForNotify(2);

            entity->SetModel(pModel);

            switch (type)
            {
                case MOD_ENTITY_PED:
                {
                    entity->Setup();

                    DWORD flags = entity->m_dwFlags &= 0xFFFFB7BF | 0x4800;
                    entity->m_dwFlags = flags;
                }
                break;

                case MOD_ENTITY_CAR:
                {
                    if (reinterpret_cast<M2::C_Car*>(entity)->Init(NULL) == false) {
                        return nullptr;
                    }

                    reinterpret_cast<M2::C_Car*>(entity)->m_nSlotSDS = pPedModelManager->GetModelManager()->m_pSlot->m_iSlotNumber;
                    reinterpret_cast<M2::C_Car*>(entity)->Setup();

                    DWORD flags = entity->m_dwFlags & 0xFFFFFFBF | 0x4800;
                    entity->m_dwFlags = flags;
                }
                break;
            }

            if (entity->m_dwFlags & 0x20)
                mod_log("Flags set sucessfully!\n");
            else {
                entity->Release();
                return nullptr;
            }

            entity->Activate();
            return entity;
        }

        bool DestroyEntity(C_Entity *entity)
        {
            if (entity == nullptr) {
                mod_log("entity null\n");
                return false;
            }
            entity->Deactivate();
            if (entity->IsActive()) {
                return false;
            }
            entity->Release();
            entity->Destructor();
            Mem::InvokeFunction<Mem::call_cdecl, int>(0x4019F0, entity);
            entity = nullptr;

            return true;
        }
    };
};
