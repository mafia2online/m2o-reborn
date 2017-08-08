/** @file CActor.hpp
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
	class ICActor
	{
	public:
	};

	class C_Actor : public ICActor
	{
	public:
		void SetPos(Vector3 *pos)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x1164D40, this, &pos);
		}

		void SetRot(Vector3 *rot)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x046A6B0, this, &rot);
		}
	};
};