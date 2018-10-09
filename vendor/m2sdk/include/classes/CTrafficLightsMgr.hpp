/** @file CTrafficLightsMgr.hpp
*  @brief Game's traffic lights class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "SLightManager.hpp"

namespace M2
{
    class ICTrafficLightsMgr
    {
    public:
        pad(ICTrafficLightsMgr, pad0, 0x18);    //0000 - 0018
        S_LightManager  *m_pLightManager;       //0018 - 001C
    };

    class C_TrafficLightsMgr : public ICTrafficLightsMgr
    {
    public:
        void AcquireXRDSemaphore(int unk)
        {
            Mem::InvokeFunction<Mem::call_this, bool>(0xA10FA0, this, unk);
        }

        int CheckAPValidity()
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0xA16E40, this);
        }

        int Done()
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0xA193D0, this);
        }

        int GetCurrentState(int unk)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x9EAF30, this);
        }

        int GetLightState(int unk, int unk2)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x9EAFC0, this, unk, unk2);
        }

        void GetView()
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x9BFAC0, this);
        }

        int GetXRoadState(int unk)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x9EAEC0, this, unk);
        }

        void ReleaseGroupAndEverything()
        {
            Mem::InvokeFunction<Mem::call_this, void>(0xA17000, this);
        }

        void SpawnTL(C_Entity *ent)
        {
            Mem::InvokeFunction<Mem::call_this, char *>(0xA169E0, this, ent);
        }

        int UpdateWA()
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0xA18EA0, this);
        }

        void Update(unsigned long unk, float unk2)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0xA1CBF0, this, unk, unk2);
        }
    };
};
