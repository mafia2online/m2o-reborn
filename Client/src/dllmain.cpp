/** @file dllmain.cpp
*  @brief Our DLL entrypoint.
*
*  This our dll entrypoint nothing fancy.
*
*  @author MyU (myudev0@gmail.com)
*  @bug Being the windows entrypoint.
*/

#include <stdafx.h>
#include "CCore.h"

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CCore::Instance().OnAttach(hModule);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

