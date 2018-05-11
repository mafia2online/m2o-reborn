/** @file CDoor.cpp
*  @brief Game's doors wrapper
*
*
*  @author DavoSK
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
    class C_Door
    {
    public:

        void AILock()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x44A700, this);
        }

        void AIUnlock()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x44A740, this);
        }

        void Close()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4EAD30, this);
        }

        void DisableAction()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x44A850, this);
        }

        void EnableAction()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x44A7E0, this);
        }

        bool IsLocked()
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x44A780, this);
        }

        void Kick(const vec3_t & position)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4F5110, this, position);
        }

        void Lock()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4F10C0, this);
        }

        void Open(const vec3_t & position, bool unk1, unsigned int unk2)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4EAC80, this, &position, unk1, unk2);
        }

        void StartLockpick()
        {
            Mem::InvokeFunction<Mem::call_this, bool>(0x4F1140, this);
        }

        void StopLockpick()
        {
            Mem::InvokeFunction<Mem::call_this, bool>(0x4F1160, this, true);
        }

        void ToggleOpen(const vec3_t & position, bool unk1)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4EAD70, this, position, unk1);
        }

        void Unlock()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4F10F0, this);
        }

        /*
        NOTE(DavoSK): Not important functions that can be reversed later
        PlayAnim				- LUA sub_D87100
        PlayAnimNoResistance	- LUA sub_D87190
        StopAnim				- LUA sub_D76580
        */
    };

};
