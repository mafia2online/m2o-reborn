/** @file CEntityMessage.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"
#include "CEntityMessageDamage.hpp"

namespace M2
{
    enum E_HumanMessage
    {
        MESSAGE_GAME_ENTER_EXIT_VEHICLE_DONE = 851972
    };

	class ICEntityMessage
	{
	public:
		void *m_pVFTable;							// 0000 - 0004
		pad(ICEntityMessage, pad0, 0x4);			// 0004 - 0008
		DWORD m_dwSenderGUID;						// 0008 - 000C
		DWORD m_dwReceiveGUID;						// 000C - 0010
		DWORD m_dwMessage;							// 0010 - 0014
		DWORD m_dwUnknown1;							// 0014 - 0018
		DWORD m_dwUnknown2;							// 0018 - 001C
		DWORD m_dwEnemyGUID;						// 001C - 0020
		pad(ICEntityMessage, pad1, 0x8);			// 0020 - 0028
		DWORD m_dwKillerGUID;						// 0028 - 002C
	};

	class C_EntityMessage : public ICEntityMessage
	{
	};
};
