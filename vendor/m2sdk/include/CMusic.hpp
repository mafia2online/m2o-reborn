#pragma once
#include "CommonHeaders.h"

namespace M2
{
    class ICMusic
    {
    public:
    };

    class C_Music : public GameClassWrapper<C_Music, ICMusic, 0x1CC2C58>
    {
    public:
        void PlayGameMusic(const char *music)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xB27170, this, music, true, 1, 1, 1);
        }

        void StopGameMusic(float unk)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xB17FC0, this, unk);
        }
    };
};
