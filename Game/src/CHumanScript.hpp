/** @file CHumanScript.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
	enum ePhysState : BYTE
	{
		E_DYNAMIC = 0,
		E_ENABLED = 1,
		E_KINEMATIC = 2,
		E_DISABLED = 3
	};

	class ICHumanScript
	{
	public:
	};

	class C_HumanScript : public ICHumanScript
	{
	public:
		void SetPhysState(ePhysState state)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x093B8A0, this, state);
		}
	};
};
