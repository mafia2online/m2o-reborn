/** @file CPlayer2.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

#include "CEntity.hpp"
#include "CHuman2.hpp"

namespace M2
{
	class ICPlayer2 : public C_Human2
	{
	public:
	};

	class C_Player2 : public ICPlayer2
	{
	public:
		void LockControls(bool lock)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x438230, this, lock);
		}
	};
};