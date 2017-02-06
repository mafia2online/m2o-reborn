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

	class C_SDSLoadingTable : public ClassWrapper<C_SDSLoadingTable>
	{
	public:
		C_SDSLoadingTable();

		void ActivateStreamMapLine(char *szName);

		inline ICSDSLoadingTable* GetInterface() { return m_interface; }
	private:
		ICSDSLoadingTable* m_interface;
	};
};