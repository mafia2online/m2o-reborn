/** @file CCameraModule.cpp
*  @brief Game's Env wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

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
        int DespawnAllElements()
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0xAA2920, this);
        }

        int FreeUnrelatedElements()
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0xA6BAD0, this);
        }

        bool SwitchGenerators(bool trigger)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0xAAB870, this, trigger);
        }
    };
};
