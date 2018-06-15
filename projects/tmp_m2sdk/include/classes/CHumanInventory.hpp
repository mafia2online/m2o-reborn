/** @file CHumanInventory.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
	class ICHumanInventory
	{
	public:
	};

	class C_HumanInventory : public ICHumanInventory
	{
	public:
		void AddAmmo(int weapon, int ammo)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x0942B00, this, weapon, ammo);
		}

		void AddMoney(int amount, int unk)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x095B060, this, amount, unk);
		}

		void AddWeapon(int weapon, int ammo)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x0956BF0, this, weapon, ammo);
		}

		bool CanAddAmmo(int weapon, int ammo)
		{
			return Mem::InvokeFunction<Mem::call_this, int>(0x0942AE0, this, weapon, ammo);
		}

		bool CanAddWeapon(int weapon)
		{
			return Mem::InvokeFunction<Mem::call_this, int>(0x0942AB0, this, weapon);
		}

		bool CanFire()
		{
			return Mem::InvokeFunction<Mem::call_this, bool>(0x0420230, this);
		}

		bool CanReload()
		{
			return Mem::InvokeFunction<Mem::call_this, bool>(0x0420510, this);
		}

		void DoDrop()
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x0989F00, this);
		}

		void DoReload(int unk, int unk2)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x0969510, this, unk, unk2);
		}

		void DoShot(int unk)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x09898B0, this, unk);
		}

		int GetReloadAmount(int unk, int unk2)
		{
			return Mem::InvokeFunction<Mem::call_this, int>(0x09693F0, this, unk, unk2);
		}

		void Reset()
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x098CFB0, this);
		}

		int TellAmmoInSelectedWeapon()
		{
			return Mem::InvokeFunction<Mem::call_this, int>(0x0942D80, this);
		}
	};
};
