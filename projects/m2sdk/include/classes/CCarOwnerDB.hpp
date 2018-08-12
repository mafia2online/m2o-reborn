/** @file CCarOwnerDB.cpp
*  @brief Game's Car Owner DB wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CCamera.hpp"

namespace M2
{
    class ICCarOwnerDB
    {
    public:
    };

    class C_CarOwnerDB : public GameClassWrapper<ICCarOwnerDB, ICRangeMeter, 0x1CCAC74>
    {
    public:
        bool IsLockpickedCar(C_Entity *car)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0xD45460, this, car);
        }

        void MakeOwnership(C_Entity *car, C_Entity *human)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xD65E00, this, car, human);
        }

        void SetLockpicked(C_Entity *car)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xD4EC60, this, car);
        }

        void RemoveCar(C_Entity *car)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xD53D30, this, car);
        }

        void RemoveOwner(C_Entity *car)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xD5BEC0, this, car);
        }
    };
}
