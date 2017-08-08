/** @file CTextDatabase.hpp
*  @brief Game's class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
	class ICTextDatabase
	{
	public:
	};

	class C_TextDatabase : public GameClassWrapper<C_TextDatabase, ICTextDatabase, 0x1AC0CC8>
	{
	public:
		void Clear()
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x11EE4C0, this);
		}

		//Not fully working
		char *GetString(char *number)
		{
			unsigned __int32 ulText = strtoul(number, 0, 10);
			char *retn;

			Mem::InvokeFunction<Mem::call_this, int>(0x11EF920, this, &retn, ulText);

			return retn;
		}
	};
};