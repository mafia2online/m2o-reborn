/** @file CGameTrafficModule.hpp
*  @brief Game's traffic class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CTrafficLightsMgr.hpp"

namespace M2
{
    class ICGameTrafficModule
    {
    public:
        pad(ICGameTrafficModule, pad0, 0x7);            //0000 - 0007
        C_TrafficLightsMgr          *m_pTrafficLights;  //0007 - 000B
    };
    class C_GameTrafficModule : public GameClassWrapper<C_GameTrafficModule, ICGameTrafficModule, 0x1CC7CC0>
    {
    public:
    };
};
