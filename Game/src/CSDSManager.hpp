#pragma once
#include "CommonHeaders.h"

namespace M2
{
	class ICEnumator
	{
	public:
		void *m_pVFTable;
	};

	class ICSDSLoadingTable
	{
	public:
		void *m_pVFTable;
		ICEnumator *m_pEnumerator; // 0004
	};

	class C_SDSLoadingTable : public GameClassWrapper<C_SDSLoadingTable, 0x1ABFE4C>
	{
	public:
		ICSDSLoadingTable* GetInterface() { return reinterpret_cast<ICSDSLoadingTable*>(this); }

		/*
		void _declspec(naked) (int un)
		{
			_asm jmp off
		}*/

		void ActivateStreamMapLine(char * szName)
		{
			static uint32_t dwReturn;
			// method addr, class pointer | return name
			Mem::InvokeFunction<Mem::call_this, void>(0x5CFCD0, &GetInterface()->m_pEnumerator, &dwReturn, szName);
		}



	};
};