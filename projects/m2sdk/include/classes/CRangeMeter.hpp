/** @file CCameraModule.cpp
*  @brief Game's Env wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CCamera.hpp"

namespace M2
{
    class ICRangeMeter
    {
    public:
    };

    class C_RangeMeter : public GameClassWrapper<C_RangeMeter, ICRangeMeter, 0x1CBFF00>
    {
    public:
        void CloseSeason()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xAAB9A0, this);
        }

        int DespawnAllElements()
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0xAA2920, this);
        }

        int FreeUnrelatedElements()
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0xA6BAD0, this);
        }

        void OpenSeason(int season)
        {
            Mem::InvokeFunction<Mem::call_this, bool>(0xAB0800, this, season);
        }

        bool SwitchGenerators(bool trigger)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0xAAB870, this, trigger);
        }
    };
};
