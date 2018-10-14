/** @file CHumanWeaponController.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
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
            //unk must be 1 or -1
            //unk3/4 are always 0
			Mem::InvokeFunction<Mem::call_this, void>(0x0431300, this, unk, unk2, unk3, unk4);
		}

        void DoModelSelectById(int hand, int model, int unk)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x91DCA0, this, hand, model, unk);
        }

        void DoMouthModelSelectById(int model)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x42DB60, this, model);
        }

		void DoShot(bool unk, Vector3 const *from, Vector3 const *to)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x0438790, this, unk, from, to);
		}

		void DoWeaponReload(int unk, bool unk2)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x04355F0, this, unk, unk2);
		}

        void DoWeaponDrop(int unk, int unk2)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x431F30, this, unk, unk2);
        }

        void DoWeaponDropBest()
        {
            Mem::InvokeFunction<Mem::call_this, unsigned int>(0x432020, this);
        }

        void SetAnimationMatrices(const void *matrix)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x42DDF0, this, matrix);
        }
	};
};
