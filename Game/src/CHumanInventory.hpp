/** @file CHumanInventory.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
	class ICHumanInventory
	{
	public:
	};

	class C_HumanInventory : public ICHumanInventory
	{
	public:
		void AddMoney(int amount, int unk)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x095B060, this, amount, unk);
		}

		void AddWeapon(int weapon, int ammo)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x0956BF0, this, weapon, ammo);
		}
	};
};
