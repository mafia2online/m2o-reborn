/** @file CPlayerRadio.cpp
*  @brief Game's radio wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
	class ICPlayerRadio
	{
	public:
	};

	class C_PlayerRadio : public GameClassWrapper<C_PlayerRadio, ICPlayerRadio, 0x1AB5A54>
	{
	public:

		void Activate(const void *unk)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x049DF60, this, unk);
		}

		void Deactivate(const void *unk)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x049DF60, this, unk);
		}

		void NextStation(bool forward)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x0453190, this, forward);
		}

		void SwitchStation(const char *station)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x04C2730, this, station);
		}

		void TurnOff()
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x048E300, this);
		}

		void TurnOn()
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x04C25D0, this);
		}
	};
};
