#pragma once
#include "CommonHeaders.h"
#include "CPlayer2.hpp"

namespace M2
{
	class VCGame
	{
	public:
		DWORD sub_5BB900;
		DWORD sub_40B850;
		DWORD sub_5B5B20;
		DWORD nullsub_400;
		DWORD sub_40B860;
		DWORD Open;
		DWORD sub_5B5C00;
		DWORD sub_5B5B30;
		DWORD GameInit;
		DWORD OnUnload;
		DWORD sub_5B5B40;
		DWORD sub_5B5B50;
		DWORD nullsub_655;
		DWORD sub_5B5B60;
		DWORD sub_5B5B80;
		DWORD sub_5ACAE0;
		DWORD sub_58A780;
		DWORD sub_5B5BA0;
		DWORD sub_5B5BB0;
		DWORD sub_5B5B90;
		DWORD GetEntityFromIndex;
		DWORD sub_5B5BD0;
		DWORD sub_5B5BF0;
		DWORD sub_5C7780;
		DWORD sub_5C2430;
	};

	class ICGame
	{
	public:
		VCGame *m_pVFTable;
		DWORD m_dwFlags;
		DWORD m_dwRuntime;
		pad(ICGame, pad0, 0x24);
		void *unkClass;
		pad(ICGame, pad1, 0xE4);
		C_Player2 *m_pLocalPed;

	};

	class C_Game : public GameClassWrapper<C_Game, ICGame, 0x1ABFE14>
	{
	public:
		C_Player2	*GetLocalPed() { return reinterpret_cast<ICGame *>(GetInterface())->m_pLocalPed; }

		__forceinline DWORD GetEntityFromIndex(int index)
		{
			return VTBLCall(DWORD, GetEntityFromIndex, index);
		}

	};
};