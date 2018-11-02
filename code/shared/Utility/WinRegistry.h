#pragma once
#ifdef _WIN32

#include <Windows.h>

namespace Utility
{
	bool ReadString(HKEY hKeyLocation, const char * szSubKey, const char * szKey, const char * szDefault, char * szData, DWORD dwSize);
	bool WriteString(HKEY hKeyLocation, const char * szSubKey, const char * szKey, char * szData, DWORD dwSize);
}

#endif
