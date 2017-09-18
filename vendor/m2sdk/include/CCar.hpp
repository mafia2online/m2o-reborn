/** @file CCar.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"
#include "CVehicle.hpp"

namespace M2
{
    class CCarVFTable
    {
    public:
        pad(CCarVFTable, pad0, 0x88);
        DWORD SetSPZText;               // 0088 - 008C
        DWORD GetSPZText;               // 008C - 0090
        DWORD SetDirtLevel;             // 0090 - 0094
        pad(CCarVFTable, pad1, 0xC);    // 0094 - 00A0
        DWORD GetWorldEntity;           // 00A0 - 00A4
        pad(CCarVFTable, pad2, 0x40);   // 00A4 - 00E4
        DWORD GetTotalSeats;            // 00E4 - 00E8
        pad(CCarVFTable, pad3, 0x4);    // 00E8 - 00EC
        DWORD GetSeatAtIndex;           // 00EC - 00F0
        pad(CCarVFTable, pad4, 0x30);   // 00F0 - 0120
        DWORD SetHornOn;                // 0120 - 0124
    };

	class ICCar
	{
	public:
        CCarVFTable     *m_pVFTable;        // 0000 - 0004
        pad(ICCar, pad1, 0x74);             // 0004 - 0078
        int             m_nSlotSDS;         // 0078 - 007C
		pad(ICCar, pad2, 0x2C);             // 007C - 00A8
		C_Vehicle		m_pVehicle;         // 00A8 - 00CC
        pad(ICCar, pad3, 0x1BC);            // 00CC - 0288
        vec3_t          m_vecMoveSpeed;     // 0288 - 0294
        float           m_fSpeed;           // 0294 - 0298
        float           m_fSpeedDir;        // 0298 - 029C
	};

	class C_Car : public ICCar
	{
	public:
		void CloseHood()
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x09BD030, this);
		}

		void CloseTrunk()
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x09BCF30, this);
		}

        void DoorChangeState(int door)
        {
            Mem::InvokeFunction<Mem::call_this, void *>(0x4DBE10, this, door);
        }

		int GetActualPlayerSeatIndex()
		{
			return Mem::InvokeFunction<Mem::call_this, int>(0x09A22D0, this);
		}

		long double GetDamage()
		{
			return Mem::InvokeFunction<Mem::call_this, long double>(0x09BA9D0, this);
		}

		long double GetFuelTankCapacity()
		{
			return Mem::InvokeFunction<Mem::call_this, long double>(0x09BD0B0, this);
		}

		long double GetMotorDamage()
		{
			return Mem::InvokeFunction<Mem::call_this, long double>(0x09BAD20, this);
		}

		double GetRepairPrice()
		{
			return Mem::InvokeFunction<Mem::call_this, double>(0x04476E0, this);
		}
        
        vec3_t GetWheelPos(int index)
        {
            if (index <= 0) {
                index = 1;
            }

            vec3_t pos;
            Mem::InvokeFunction<Mem::call_this, int>(0x9BCB90, this, &pos, index - 1);
            return pos;
        }

        void Init(int initProps)
        {
            Mem::InvokeFunction<Mem::call_this, bool>(0x9A1E40, this, initProps);
        }

		bool IsSeatWindowOpen(int seat)
		{
			return Mem::InvokeFunction<Mem::call_this, bool>(0x09A2280, this, seat);
		}

		void LockEntryPoints()
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x09CCA50, this);
		}

		void LockPlayerEntryPoints()
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x09CCAC0, this);
		}

		void LockTrunks()
		{
			Mem::InvokeFunction<Mem::call_this, unsigned int>(0x09CEF30, this);
		}

        void OpenDoors(int open)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x9BBC70, this, open);
        }

		void OpenHood()
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x09BCFB0, this);
		}

		void OpenSeatWindow(int window, bool unk, bool unk2)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x09A2230, window, unk, unk2);
		}

		void OpenTrunk()
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x09BCEB0, this);
		}

        void ResetRigidBody()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x46CCA0, this);
        }

        void RestoreCar()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4E9890, this, 1, 0, 0);
        }

		void SetMotorDamage(float damage)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x09BADB0, this, damage);
		}

		void SetSiren(bool siren)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x0D6B500, this, siren);
		}

        void SetSpeedFloat(float speed)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x9A01A0, this, speed);
        }

        void Setup()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x0A151F0, this);
        }

		void UnlockEntryPoints()
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x09A1D40, this);
		}

		void UnlockPlayerEntryPoints()
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x09CCB20, this);
		}

		void UnlockTrunks()
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x0D6B020, this);
		}
	};
};
