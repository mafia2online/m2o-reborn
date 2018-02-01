/** @file CGameInputModule.cpp
*  @brief Game's InputModule wrapper
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
    class ICGameInputModule
    {
    public:
        void *m_pVFTable;
    };

    class C_GameInputModule : public GameClassWrapperStatic<C_GameInputModule, ICGameInputModule, 0x1CC65F0>
    {
    public:
        void DestroyEffect(void *vibrationEffect)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xB2E650, this, vibrationEffect);
        }

        void PlayFFVEffect(int unk, float unk2, float unk3)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xB2E5E0, this, unk, unk2, unk3);
        }

        void PlayerUserEffect(void *vibrationEffect, int eVibrationEffectSource)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xB2E660, this, vibrationEffect, eVibrationEffectSource);
        }

        void StopUserEffect(void *vibrationEffect)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xB2E6E0, this, vibrationEffect);
        }
    };
};
