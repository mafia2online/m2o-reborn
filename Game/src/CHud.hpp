/** @file CHud.cpp
*  @brief Game's CHud wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"
#include "CHudScriptW.h"
#include "CTimer.hpp"

namespace M2
{
	class ICHud
	{
	public:
		pad(ICHud, pad0, 0x6C);			//0000 - 006C
		C_Timer *m_pTimer;				//006C - 0070
		pad(ICHud, pad1, 0x30);			//0070 - 00A0
		C_HudScriptW *m_pScript;
	};

	class C_Hud : public GameClassWrapper<C_Hud, ICHud, 0x1CBA618>
	{
	public:
		C_Timer	*GetHudTimer() { return reinterpret_cast<ICHud *>(this)->m_pTimer; }
		C_HudScriptW	*GetHudScript() { return reinterpret_cast<ICHud *>(this)->m_pScript; }
	};
};