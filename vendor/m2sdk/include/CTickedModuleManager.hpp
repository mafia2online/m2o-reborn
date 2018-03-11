/** @file CTickedModuleManager.hpp
*  @brief Game's module manager
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
    enum TickedModuleEvent
    {
        EVENT_SYSTEM_INIT = 1,
        EVENT_SYSTEM_DONE = 2,
        EVENT_TICK = 3,
        EVENT_RENDER = 7,
        EVENT_GAME_QUIT = 8,
        EVENT_MISSION_BEFORE_OPEN = 9,
        EVENT_MISSION_OPEN = 10,
        EVENT_MISSION_CLOSE = 11,
        EVENT_GAME_INIT = 13,
        EVENT_GAME_DONE = 14,
        EVENT_GAMELOOP_AFTER = 24,
        EVENT_GAME_SAVE = 27,
        EVENT_GAMEPARAMS_RELOAD = 28,
        EVENT_APP_ACTIVATE = 31,
        EVENT_LOADING_STARTED = 32,
        EVENT_LOADING_FINISHED = 33,
        EVENT_GAME_PAUSED = 34,
        EVENT_GAME_UNPAUSED = 35,

    };

    struct I_TickedModuleCallEventContext {};
    class C_TickedModule 
    {
    public:
        virtual void Tick(I_TickedModuleCallEventContext &) {};
        virtual void GameDone(I_TickedModuleCallEventContext &) {};
        virtual void GameInit(I_TickedModuleCallEventContext &) {};
        virtual void OpenMission(I_TickedModuleCallEventContext &) {};
        virtual void CloseMission(I_TickedModuleCallEventContext &) {};
    };

    class ICTickedModuleManager
    {

    };

    typedef 

    class C_TickedModuleManager2 : public GameClassWrapper<C_TickedModuleManager2, ICTickedModuleManager, 0x1ABE8BC>
    {
    public:
        void AddAction(unsigned int action, int timer, C_TickedModule *module, void (C_TickedModule::*callback)(I_TickedModuleCallEventContext&), float unk, unsigned int unk2, unsigned int unk3)
        {
            Mem::InvokeFunction<Mem::call_this, char>(0x11A0C10, this, action, timer, module, callback, unk, unk2, unk3);
        }

        void AddEvent(unsigned int unk, const char *name)
        {
            Mem::InvokeFunction<Mem::call_this, char>(0x11A58A0, this, unk, name);
        }

        void DelAction(unsigned int, C_TickedModule *module, void (C_TickedModule::*callback)(I_TickedModuleCallEventContext &))
        {
            Mem::InvokeFunction<Mem::call_this, char>(0x11A0DC0, this, module, callback);
        }

        void DelModuleActions(C_TickedModule *module)
        {
            Mem::InvokeFunction<Mem::call_this, char>(0x11A0F40, this, module);
        }

        int GetEventidFromName(const char *name)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x1176FF0, this, name);
        }

        void RegisterStaticPlugins()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x11672E0, this);
        }
    };
};
