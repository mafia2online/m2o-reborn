/** @file CScene.hpp
*  @brief Game's class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

#include "CFrame.hpp"

namespace M2
{
    class ICScene
    {
    public:
    };

    class C_Scene : public ICScene
    {
    public:
        bool AddToScene(C_Frame *frame, bool unk)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x14BEBF0, this, frame, unk);
        }

        bool RemoveFromScene(C_Frame *frame, bool unk)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x1497C60, this, frame, unk);
        }
    };
};
