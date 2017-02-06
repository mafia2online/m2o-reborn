#include "CSDSManager.h"

using namespace M2; // exception

#define C_SDSManager_Ptr 0x1ABFE4C

C_SDSLoadingTable::C_SDSLoadingTable() : m_interface(*(ICSDSLoadingTable**)C_SDSManager_Ptr)
{

}

void M2::C_SDSLoadingTable::ActivateStreamMapLine(char * szName)
{
	static uint32_t dwReturn;
											 // method addr, class pointer | return name
	Mem::InvokeFunction<Mem::call_this, void>(0x5CFCD0, &m_interface->m_pEnumerator, &dwReturn, szName);
}
