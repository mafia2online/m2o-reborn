/** @file CHuman2.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

#include "CEntity.hpp"
#include "CHumanInventory.hpp"
#include "CHumanScript.hpp"

namespace M2
{
	class ICHuman2 : public C_Entity
	{
	public:
		pad(ICHuman2, pad0, 0xA0);
		C_HumanInventory	*m_pInventory;
		C_HumanScript		*m_pScript;
	};

	class C_Human2 : public ICHuman2
	{
	public:
		C_HumanInventory	*GetInventory() { return reinterpret_cast<ICHuman2 *>(this)->m_pInventory; }
		C_HumanScript		*GetScript() { return reinterpret_cast<ICHuman2 *>(this)->m_pScript; }
	};
};