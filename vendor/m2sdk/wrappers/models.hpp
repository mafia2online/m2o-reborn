#pragma once

#include <windows.h>
#include <list>

#include "m2sdk.h"

#define MODELMGR_MAX			1000
#define SDS_LOAD_DIR_ADDR		0x190ABDC
#define SDS_LOAD_DIR_CARS		"/sds/cars/"
#define SDS_LOAD_DIR_HCHAR		"/sds/hchar/"
#define SDS_LOAD_DIR_PLAYER		"/sds/player/"
#define SDS_LOAD_DIR_TRAFFIC    "/sds/traffic/"

namespace M2
{
    namespace Wrappers
    {
        static char *GetDir()
        {
            return (char *)(SDS_LOAD_DIR_ADDR);
        }

        static void SetDir(const char *directory)
        {
            char *loadPath = (char *)(SDS_LOAD_DIR_ADDR);

            DWORD dwOldProtect = 0;
            VirtualProtect(loadPath, 0x20, PAGE_READWRITE, &dwOldProtect);

            sprintf(loadPath, "%s%%s.sds", directory);

            VirtualProtect(loadPath, 0x20, dwOldProtect, &dwOldProtect);
        }

        class GameModelManager
        {
        private:
            ICPlayerModelManager    *m_pModelManager;

        public:
            ICPlayerModelManager    *GetModelManager() { return m_pModelManager; }

        public:
            GameModelManager(): m_pModelManager(nullptr)
            {
                ICPlayerModelManager *pModelManager = (ICPlayerModelManager *)M2::Alloc(0x50);
                if (pModelManager)
                {
                    DWORD address = 0x5B6680;
                    __asm
                    {
                        mov ecx, pModelManager;
                        call address;
                    }
                    m_pModelManager = pModelManager;
                }
            }

            GameModelManager(ICPlayerModelManager * pModelManager)
            { 
                m_pModelManager = pModelManager; 
            }

            ~GameModelManager()
            {
                if (m_pModelManager) {
                    Free();
                    m_pModelManager = nullptr;
                }
            }

            bool Load(const char *directory, const char *model)
            {
                if (!m_pModelManager)
                    return false;

                bool result = false;
                ICPlayerModelManager *pModelManager = m_pModelManager;

                SetDir(directory);

                DWORD address = 0x5CBBE0;
                __asm
                {
                    push model;
                    mov ecx, pModelManager;
                    call address;
                    mov result, al;
                }

                strcpy(pModelManager->m_sModelName, model);

                SetDir(SDS_LOAD_DIR_PLAYER);
                
                return result;
            }

            void Free()
            {
                if (!m_pModelManager)
                    return;

                ICPlayerModelManager *pModelManager = m_pModelManager;

                DWORD address = 0x5AF310;

                __asm
                {
                    mov ecx, pModelManager;
                    call address;
                }
            }
        };
        class ModelManager
        {
            friend class GameModelManager;

        private:
            std::list< GameModelManager *>  m_pModelManagers;

        public:
            ModelManager() : m_pModelManagers()
            {

            }

            ~ModelManager()
            {
                m_pModelManagers.clear();
            }

            GameModelManager    *Load(const char *directory, const char *model)
            {
                if (m_pModelManagers.size() >= MODELMGR_MAX) {
                    return nullptr;
                }

                GameModelManager *pModelManager = new GameModelManager();
                if (!pModelManager)
                    return nullptr;

                if (!pModelManager->Load(directory, model)) {
                    delete(pModelManager);
                    return nullptr;
                }

                m_pModelManagers.push_back(pModelManager);
                return pModelManager;
            }

            bool    Free(const char *model)
            {
                for (std::list<GameModelManager *>::iterator iter = m_pModelManagers.begin(); iter != m_pModelManagers.end(); iter++)
                {
                    GameModelManager *manager = (*iter);
                    if (!manager)
                        continue;

                    if (!manager->GetModelManager()->m_pSlot)
                        continue;

                    if (manager->GetModelManager()->m_pSlot->GetModelByFileName(model))
                    {
                        m_pModelManagers.remove(manager);
                        delete(manager);
                        return true;
                    }
                }
                return false;
            }

            bool Free(GameModelManager * pModelManager)
            {
                if (!pModelManager || !pModelManager->GetModelManager())
                    return false;

                return Free(pModelManager->GetModelManager()->m_sModelName);
            }

            void Clear()
            {
                for (std::list < GameModelManager * >::iterator iter = m_pModelManagers.begin(); iter != m_pModelManagers.end(); iter++)
                {
                    delete(*iter);
                }

                m_pModelManagers.clear();
            }

            GameModelManager *GetModelManagerByName(const char * szModelName)
            {
                for (std::list < GameModelManager * >::iterator iter = m_pModelManagers.begin(); iter != m_pModelManagers.end(); iter++)
                {
                    GameModelManager * manager = (*iter);

                    if (!manager->GetModelManager() || !manager->GetModelManager()->m_pSlot)
                        continue;

                    if (!strcmp(szModelName, manager->GetModelManager()->m_sModelName))
                        return manager;
                }

                return nullptr;
            }
        };
    };
};
