/** @file CHudScriptW.cpp
*  @brief Game's CHud wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
	enum eHintPosition
	{
		POS_TOPLEFT,
		POS_BOTTOMLEFT,
		POS_RADAR,
		POS_CENTER
	};

	enum eHintMode
	{
		MODE_GREY,
		MODE_REDYEL,
		MODE_BLUE,
		MODE_FLUO
	};

	class ICHudScriptW
	{
	public:
	};

	class C_HudScriptW : public ICHudScriptW
	{
	public:
		void CreateHint(eHintPosition pos, eHintMode mode, const char *text, float delay)
		{
			Mem::InvokeFunction<Mem::call_std, int>(0x08FC200, pos, text, 0, 0, 0, 0, mode, delay, 0);
		}

		void HelpHintShowQuick(float unk)
		{
			Mem::InvokeFunction<Mem::call_std, int>(0x09000C0, unk);
		}

		void InternalCreateHint(eHintPosition pos, wchar_t *unk, eHintMode mode, float delay, unsigned __int32 text)
		{
			/* How to call this func to display custom text */
			//wchar_t *test = L"Je fais un test";
			//M2::C_Hud::Get()->GetHudScript()->InternalCreateHint(M2::eHintPosition::POS_RADAR, test, M2::eHintMode::MODE_REDYEL, 1000, 0);
			Mem::InvokeFunction<Mem::call_std, int>(0x08DDE10, pos, unk, mode, delay, 0, text);
		}

		void Show(bool show)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x08A4CE0, this, show);
		}
	};
};