/** @file CSlotManager.cpp
*  @brief Game's slots manager wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
    class ICSlotManager
    {
    public:
    };

    class C_SlotManager : public GameClassWrapper<C_SlotManager, ICSlotManager, 0x1ABFE3C>
    {
    public:
        int Close(void *unk)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x5D12A0, this, unk);
        }

        int Open()
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x05D1DC0, this);
        }
    };
};
