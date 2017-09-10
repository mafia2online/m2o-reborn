/** @file CVehicle.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

#include "CActorVehicle.hpp"

namespace M2
{
	enum E_VehGearBoxAutomat
	{
		GEARBOX_HALF_AUTO = 1,
		GEARBOX_AUTO = 2
	};
    enum E_VehicleSeat
    {
        E_SEAT_DRIVER = 1,
        E_SEAT_PASSENGER_FRONT,
        E_SEAT_PASSENGER_BACK_1,
        E_SEAT_PASSENGER_BACK_2
    };

    enum E_VehicleLights
    {
        E_FRONT_LIGHT = 1,
        E_PLATE_LIGHT = 0
    };

    enum E_IndicatorLights
    {
        E_INDICATOR_RIGHT = 0,
        E_INDICATOR_LEFT = 1
    };

	class ICVehicle
	{

	};

	class C_Vehicle : public ICVehicle
	{
	public:
		void AddVehicleFlags(int unk, int unk2)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x12637E0, this, unk, unk2);
		}

		void ClearVehicleFlags(int unk, int unk2)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x1217180, this, unk, unk2);
		}

		void Explode(bool unk)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x126EF40, this, unk);
		}

        bool IsBeaconLightOn()
        {
            DWORD retn = (*(DWORD *)((DWORD)this + 0x6F0 ^ 0) & 0x40);
            return retn;
        }

        bool IsIndicatorLightsOn(E_IndicatorLights indicator)
        {
            int result = 8 * (indicator == 0) + 8;
            DWORD retn = (*(DWORD *)((DWORD)this + 0x6F0) & result);
        }

        bool IsReflectorLightOn(E_VehicleLights light)
        {
            DWORD retn;
            if (light == E_FRONT_LIGHT)
            {
                retn = (*(DWORD *)((DWORD)this + 0x6F0) & 1u);
            }
            else if (light == E_PLATE_LIGHT)
            {
                retn = (*(DWORD *)((DWORD)this + 0x6F0) & 2u);
            }
            return retn;
        }

        bool IsTaxiLightOn()
        {
            DWORD retn = (*(DWORD *)((DWORD)this + 0x6F0) & 0x80);
            return retn;
        }

		void SetBeaconLightOn(bool enable)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x120CC10, this, enable);
		}

		void SetBrake(float brake)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x1217610, this, brake);
		}

		void SetEngineOn(bool unk, bool unk2)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x1263850, this, unk, unk2);
		}

		void SetGear(int gear)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x11F3450, this, gear);
		}

		void SetGearBoxAutomat(E_VehGearBoxAutomat state)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x1263A20, this, state);
		}

		void SetHandbrake(float brake, bool unk)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x1204540, this, brake, unk);
		}

		void SetIndicatorLightsOn(int toggle, E_IndicatorLights indicator)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x120CBC0, this, toggle, indicator);
		}

		void SetOffPart(int part, bool unk)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x119C5B0, this, part, unk);
		}

		void SetOnPart(int part, bool unk)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x119C6C0, this, part, unk);
		}

		void SetPower(float power)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x1217560, this, power);
		}

		void SetReflectorLightsOn(bool toggle, E_VehicleLights light)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x11F7B80, this, toggle, light);
		}

		void SetSPZText(const char *text)
		{
			if (strlen(text) > 8)
				return;

			Mem::InvokeFunction<Mem::call_this, int>(0x11F75D0, this, text, 0);
		}

		void SetTaxiLightsOn(bool enable)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x120CC30, this, enable);
		}

		void SetVehicleFlags(int unk, int unk2)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x1263620, this, unk, unk2);
		}
	};
};
