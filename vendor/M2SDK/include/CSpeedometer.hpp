/** @file CSpeedometer.cpp
*  @brief Game's CHud wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
	class ICSpeedometer
	{
	public:
		pad(ICSpeedometer, pad0, 0x54);		//0000 - 0054
		int m_speedLimit;					//0054 - 0058
	};

	class C_Speedometer : public ICSpeedometer
	{
	public:

		void SetFuel(float fuel, bool unk)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x0843140, this, fuel, unk);
		}

		void SetSpeedLimit(int limit)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x08431B0, this, limit);
		}

		void Show(bool show)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x08929F0, this, show);
		}

		void ShowSpeedLimiter(bool show)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x088BB20, this, show);
		}
	};
};