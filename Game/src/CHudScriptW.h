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

		void Show(bool show)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x08A4CE0, this, show);
		}
	};
};