/** @file CVehicle.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

#include "CActorVehicle.hpp"
#include "CVehicleEffectManager.hpp"

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
    public:
        pad(ICVehicle, pad0, 0x3E8);
        float                   m_fSteer;           // 03E8 - 03EC
        pad(ICVehicle, pad1, 0x4);                  // 03EC - 03F0
        float                   m_fMaxSteer;        // 03F0 - 03F4
        float                   m_fAddedSteer;      // 03F4 - 03F8
        pad(ICVehicle, pad2, 0x454);                // 03F8 - 084C
        C_VehicleEffectManager  *m_pEffectManager;  // 084C - 0850
	};

	class C_Vehicle : public ICVehicle
	{
	public:
		void AddVehicleFlags(unsigned int unk, int unk2)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x12637E0, this, unk, unk2);
		}

		void ClearVehicleFlags(unsigned int unk, int unk2)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x1217180, this, unk, unk2);
		}

		void Explode(bool unk)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x126EF40, this, unk);
		}

        void GetVehicleColor(vec3_t *const primary, vec3_t *const secondary)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x11F7880, this, primary, secondary);
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

        //CRASHY
        void Lock()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xD7CDC0, this);
        }

        void OpenDoor(int unk, int door, bool open)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x12618F0, this, unk, door, open);
        }

        void SetAddSteer(float steer)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x1204460, this, steer);
        }

		void SetBeaconLightOn(bool enable)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x120CC10, this, enable);
		}

		void SetBrake(float brake)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x1217610, this, brake);
		}

        void SetDoorFree(int door, bool free)
        {
            Mem::InvokeFunction<Mem::call_this, bool>(0x1261ED0, this, door, free);
        }

        void SetDynamic(bool dynamic, int unk)
        {
            Mem::InvokeFunction<Mem::call_this, bool>(0x12169F0, this, dynamic, unk);
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

		void SetOffPart(int part)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x1263DD0, this, part);
		}

		void SetOnPart(int part)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x1263EA0, this, part);
		}

		void SetPower(float power)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x1217560, this, power);
		}

		void SetReflectorLightsOn(bool toggle, E_VehicleLights light)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x11F7B80, this, toggle, light);
		}

        void SetSpeedFloat(float speed)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x9A01A0, this, speed);
        }

		void SetSPZText(const char *text)
		{
			if (strlen(text) > 8)
				return;

			Mem::InvokeFunction<Mem::call_this, int>(0x11F75D0, this, text, 0);
		}

        void SetSteer(float steer)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x1204360, this, steer);
        }

		void SetTaxiLightsOn(bool enable)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x120CC30, this, enable);
		}

        void SetVehicleColor(const vec3_t primary, const vec3_t secondary)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x11F7820, this, &primary, &secondary);
        }

		void SetVehicleFlags(int unk, int unk2)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x1263620, this, unk, unk2);
		}

        void StopAllSounds()
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x1234530, this);
        }

        //CRASHY
        void Unlock()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xD6AEA0, this);
        }
	};
};
