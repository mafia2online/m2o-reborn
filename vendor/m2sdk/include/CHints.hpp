/** @file CHints.hpp
*  @brief Game's class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
	class ICHints
	{
	public:
	};

	class C_Hints : public  ICHints
	{
	public:
		void RadioSetTextID(int id)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x0843400, this, id);
		}
	};
};