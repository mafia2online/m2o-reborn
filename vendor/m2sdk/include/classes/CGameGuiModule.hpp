/** @file CGameGuiModule.cpp
*  @brief Game's GameGuiModule wrapper
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

#include "CGameGuiScriptW.hpp"

namespace M2
{
	class ICGameGuiModule
	{
	public:
        pad(ICGameGuiModule, pad0, 0x1758);     //0000 - 1758
        C_GameGuiScriptW    *m_pGuiScriptW;     //1758 - 175C
	};

	class C_GameGuiModule : public GameClassWrapperStatic<C_GameGuiModule, ICGameGuiModule, 0x1AB64F0>
	{
	public:
		void FaderFadeIn(float time)
		{
			static uint32_t dwReturn;
			Mem::InvokeFunction<Mem::call_this, void>(0x472300, this, &dwReturn, time, 0);
		}

		void FaderFadeOut(float time)
		{
			static uint32_t dwReturn;
			Mem::InvokeFunction<Mem::call_this, void>(0x472330, this, &dwReturn, time, 0);
		}

		bool IsMainMenuOpened()
		{
			return Mem::InvokeFunction<Mem::call_cdecl, bool>(0x048C9C0);
		}

		void OpenPauseMenuOverIngame()
		{
			Mem::InvokeFunction<Mem::call_cdecl, int>(0x04E0540);
		}
	};
};
