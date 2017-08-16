/** @file CCore.cpp
*  @brief Game's CameraModule wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

#include "CModel.hpp"

namespace M2
{
    class ICCoreModule
    {
    public:
    };

    class C_Core : public GameClassWrapper<C_Core, ICCoreModule, 0x1AC2778>
    {
    public:
        C_Model *AllocateModel(int unk)
        {
            return Mem::InvokeFunction<Mem::call_this, C_Model *>(0x14EBFB0, this, unk);
        }
    };
};