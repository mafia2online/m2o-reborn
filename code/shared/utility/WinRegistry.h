#pragma once
#ifdef _WIN32

#include <Windows.h>

namespace Utility
{
	bool ReadString(HKEY hKeyLocation, const char * szSubKey, const char * szKey, const char * szDefault, char * szData, DWORD dwSize);
	bool WriteString(HKEY hKeyLocation, const char * szSubKey, const char * szKey, char * szData, DWORD dwSize);

    bool ReadStringW(HKEY hKeyLocation, const wchar_t * szSubKey, const wchar_t  * szKey, const wchar_t * szDefault, wchar_t * szData, DWORD dwSize);
    bool WriteStringW(HKEY hKeyLocation, const wchar_t * szSubKey, const wchar_t  * szKey, wchar_t * szData, DWORD dwSize);
}

#endif
