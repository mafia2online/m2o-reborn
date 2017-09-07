/** @file CHumanWeaponController.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"
#include "CHuman2.hpp"

namespace M2
{
	class ICHumanWeaponController
	{
	public:
	};

	class C_HumanWeaponController : public ICHumanWeaponController
	{
	public:
		bool CanReload(int unk)
		{
			return Mem::InvokeFunction<Mem::call_this, bool>(0x04205B0, this, unk);
		}

		void DoModelSelectByActor(int unk, int unk2, int unk3, int unk4)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x0431300, this, unk, unk2, unk3, unk4);
		}

		void DoShot(bool unk, vec3_t const *from, vec3_t const *to)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x0438790, this, unk, from, to);
		}

		void DoWeaponReload(int unk, bool unk2)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x04355F0, this, unk, unk2);
		}
	};
};
