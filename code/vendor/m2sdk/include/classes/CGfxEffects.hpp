#pragma once
#include "CFrame.hpp"

namespace M2
{
    class ICGfxEffects
    {
    public:

    };

    class C_GfxEffects : public GameClassWrapper<C_GfxEffects, ICGfxEffects, 0x1AC18F4>
    {
    public:
        C_Frame * CreateEffect(unsigned int type, int unk)
        {
            return Mem::InvokeFunction<Mem::call_this, C_Frame*>(0x12B4B90, this, type, unk);
        }
    };
};
