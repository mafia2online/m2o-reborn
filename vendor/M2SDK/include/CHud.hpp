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
#include "CSpeedometer.hpp"
#include "CHints.hpp"

namespace M2
{
	class ICHud
	{
	public:
		pad(ICHud, pad0, 0x50);
		C_Speedometer *m_pSpeedo;		//0050 - 0054
		C_Hints *m_pHints;				//0054 - 0058
		pad(ICHud, pad1, 0x14);			//0058 - 006C
		C_Timer *m_pTimer;				//006C - 0070
		pad(ICHud, pad2, 0x30);			//0070 - 00A0
		C_HudScriptW *m_pScript;		//00A0 - ????
	};

	class C_Hud : public GameClassWrapper<C_Hud, ICHud, 0x1CBA618>
	{
	public:
		C_Timer	*GetHudTimer() { return reinterpret_cast<ICHud *>(this)->m_pTimer; }
		C_HudScriptW	*GetHudScript() { return reinterpret_cast<ICHud *>(this)->m_pScript; }
		C_Speedometer	*GetSpeedometer() { return reinterpret_cast<ICHud *>(this)->m_pSpeedo; }
		C_Hints			*GetHints() { return reinterpret_cast<ICHud *>(this)->m_pHints; }
	};
};