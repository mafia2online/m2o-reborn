/** @file CRtR3DIconManager.cpp
*  @brief Game's Icons wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

#include "CActor.hpp"

namespace M2
{
    class ICRtR3DIconManager
    {
    public:
    };

    class C_RtR3DIconManager : public ICRtR3DIconManager
    {
    public:
        void ClearIcons()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x8A63F0, this);
        }

        void RegisterIcon(void *framewrapper, C_Actor *actor)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x8C8730, this, framewrapper, actor);
        }

        void UnregisterIcon(C_Actor *actor)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x8A6490, this, actor);
        }
    };
};
