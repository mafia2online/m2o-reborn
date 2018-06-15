/** @file CPoliceWanted.cpp
*  @brief Game's CHud wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
	enum E_Wanted
	{
		UNWANTED = 0,
		UNK_1 = 1,
		UNK_2 = 2,
		UNK_3 = 3,
	};

	class ICPoliceWanted
	{
	public:
	};

	class C_PoliceWanted : public ICPoliceWanted
	{
	public:
		void Set(E_Wanted type, float a3, float a4)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x0894980, this, type, a3, a4);
		}
	};
};
