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
	class ICCar
	{
	public:
		pad(ICCar, pad0, 0xA8);
		C_Vehicle		m_pVehicle;
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

		void SetMotorDamage(float damage)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x09BADB0, this, damage);
		}

		void SetSiren(bool siren)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x0D6B500, this, siren);
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