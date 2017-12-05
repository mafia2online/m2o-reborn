#pragma once
#include "CommonHeaders.h"

namespace M2
{
    class ICVehicleEffectManager
    {
    public:
    };
    class C_VehicleEffectManager : public ICVehicleEffectManager
    {
    public:
        void AddExhaustSmoke()
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x1258460, this);
        }

        void EmitCarFire(bool emit)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x11F7170, this, emit);
        }

        void EmitExhaustSmoke(void* unk, int unk2)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x1234C50, this, unk, unk2);
        }

        void EmitMotorSmoke(void* unk, float unk2)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x1234D80, this, unk, unk2);
        }

        void EmitRain()
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x121CFE0, this,  NULL);
        }

        void StopEmitExhaustSmoke()
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x120A860, this);
        }
    };
};
