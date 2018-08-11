/** @file CGameVehicleModule.cpp
*  @brief Game's module wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
    class ICGameVehicleModule
    {
    public:
    };

    class C_GameVehicleModule : public GameClassWrapper<C_GameVehicleModule, ICGameVehicleModule, 0x1CC7F08>
    {
    public:
        double GetArcadeDriving()
        {
            return Mem::InvokeFunction<Mem::call_this, double>(0x847B00, this);
        }

        void SetArcadeDriving(float coeff)
        {
            Mem::InvokeFunction<Mem::call_this, bool>(0xB502F0, this, coeff);
        }

        void ResetArcadeDrivingToDefault()
        {
            Mem::InvokeFunction<Mem::call_this, bool>(0xB50340, this);
        }
    };
};
