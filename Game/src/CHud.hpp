/** @file CHud.cpp
*  @brief Game's CHud wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"
#include "CHudScriptW.h"

namespace M2
{
	class ICHud
	{
	public:
		pad(ICHud, pad0, 0xA0);
		C_HudScriptW *m_pScript;
	};

	class C_Hud : public GameClassWrapper<C_Hud, ICHud, 0x1CBA618>
	{
	public:
		C_HudScriptW	*GetHudScript() { return reinterpret_cast<ICHud *>(this)->m_pScript; }
	};
};