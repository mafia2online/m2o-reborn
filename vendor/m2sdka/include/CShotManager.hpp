/** @file CShotManager.cpp
*  @brief Game's GameGuiModule wrapper
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
	struct S_FireInit
	{

	};

	struct S_ExplosionInit
	{

	};

	class ICShotManager
	{
	public:
	};

	class C_ShotManager : public GameClassWrapper<C_ShotManager, ICShotManager, 0x1CC0A78>
	{
	public:
		void CreateExplosion(S_ExplosionInit const *exp)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x0ABEEE0, this, exp);
		}

		void CreateFire(S_FireInit const *fire)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x0ABDDF0, this, fire);
		}
	};
};