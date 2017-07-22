/** @file CSyncObject.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
	class ICSyncObject
	{

	};

	class C_SyncObject : public ICSyncObject
	{
		void Done()
		{
			Mem::InvokeFunction<Mem::call_this, unsigned int>(0x11E7680, this);
		}

		void Reactivate()
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x11E7590, this);
		}
	};
};