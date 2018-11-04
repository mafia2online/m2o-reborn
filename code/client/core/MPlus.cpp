#include <MPlus.h>

namespace mplus
{
    MPlusModule *GMPlusModule;

    MPlusModule::MPlusModule()
    {
        Intialize(this);

        // create renderinterface etc
    }

    void MPlusModule::Intialize(MPlusModule* inst)
    {
        printf(__FUNCTION__ "\n");

        auto mgr = C_TickedModuleManager::GetInstance();

        mgr->AddAction(C_TickedModuleManager::E_TickedModuleEvent::GAME_INIT, 500, inst,
                       (C_TickedModuleManager::TickedModuleCallback)(&OnGameInitialize), -1.0f, 0, 0);
        mgr->AddAction(C_TickedModuleManager::E_TickedModuleEvent::LOADING_STARTED, 1720, inst,
                       (C_TickedModuleManager::TickedModuleCallback)(&OnLoadStart), -1.0f, 0, 0);
        mgr->AddAction(C_TickedModuleManager::E_TickedModuleEvent::LOADING_FINISHED, 1720, inst,
                       (C_TickedModuleManager::TickedModuleCallback)(&OnLoadFinish), -1.0f, 0, 0);
        mgr->AddAction(C_TickedModuleManager::E_TickedModuleEvent::TICK, 400, inst,
                       (C_TickedModuleManager::TickedModuleCallback)(&OnTick), -1.0f, 0, 0);
        mgr->AddAction(C_TickedModuleManager::E_TickedModuleEvent::RENDER, 400, inst,
                       (C_TickedModuleManager::TickedModuleCallback)(&OnRender), -1.0f, 0, 0);
    }

    void MPlusModule::OnGameInitialize(I_TickedModuleCallEventContext&)
    {
        //printf(__FUNCTION__"\n");
        //printf("wat!\n");
    }

    void MPlusModule::OnLoadStart(I_TickedModuleCallEventContext&)
    {
        //printf(__FUNCTION__"\n");
        //printf("wat!\n");
    }

    void MPlusModule::OnLoadFinish(I_TickedModuleCallEventContext&)
    {
        //printf(__FUNCTION__"\n");
        //printf("wat!\n");
    }

    void MPlusModule::OnTick(I_TickedModuleCallEventContext&)
    {
        //printf(__FUNCTION__"\n");
        //printf("wat!\n");
    }

    void MPlusModule::OnRender(I_TickedModuleCallEventContext&)
    {
        //printf(__FUNCTION__"\n");
        //printf("wat!\n");
    }
}
