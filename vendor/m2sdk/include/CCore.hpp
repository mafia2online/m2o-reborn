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
    void *Alloc(int iSize)
    {
        void *retn = nullptr;

        DWORD address = 0x401830;

        __asm
        {
            push iSize;
            call address;
            add esp, 4;
            mov retn, eax;
        }

        return retn;
    }

    void Free(void *pointer)
    {
        DWORD address = 0x4019F0;

        __asm
        {
            push pointer;
            call address;
            add esp, 4;
        }
        pointer = nullptr;
    }


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
