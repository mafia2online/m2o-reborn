/** @file CRtrGameModule.cpp
*  @brief Game's Icons wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CRtR3DIconManager.hpp"

namespace M2
{
    class ICRtRGameModule
    {
    public:
        C_RtR3DIconManager      *m_pIconManager;
    };

    class C_RtRGameModule : public GameClassWrapper<C_RtRGameModule, ICRtRGameModule, 0x1CC6400>
    {
    public:
        bool IsRtR(int unk)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x476910, this, unk);
        }
    };
};
