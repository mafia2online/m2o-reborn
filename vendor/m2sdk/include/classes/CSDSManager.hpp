#pragma once

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

	class C_SDSLoadingTable : public GameClassWrapper<C_SDSLoadingTable, ICSDSLoadingTable, 0x1ABFE4C>
	{
	public:

        uint32_t ProcessLine(char * szName)
		{
            static uint32_t dwReturn;
            Mem::InvokeFunction<Mem::call_this, int *>(0x5CFCD0, &GetInterface()->m_pEnumerator, &dwReturn, szName);
            return dwReturn;
		}
	};
};
