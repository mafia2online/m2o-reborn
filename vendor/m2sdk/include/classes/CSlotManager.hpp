/** @file CSlotManager.cpp
*  @brief Game's slots manager wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

#include "CPlayerModelManager.hpp"

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

        bool ConnectToFreeSlotByType(int type, void *mgr)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x5C3D10, this, type, mgr);
        }

        int Open()
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x05D1DC0, this);
        }
    };
};
