/** @file CActor.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"
#include "CEntityPos.hpp"

namespace M2
{
	class ICActor : public C_EntityPos
	{
	public:
	};

	class C_Actor : public ICActor
	{
	public:
		void SetPos(vec3_t *pos)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x1164D40, this, &pos);
		}

		void SetRot(vec3_t *rot)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x046A6B0, this, &rot);
		}
	};
};
