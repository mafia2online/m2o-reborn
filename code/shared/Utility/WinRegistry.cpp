
#ifdef _WIN32

#include "WinRegistry.h"

bool Utility::ReadString(const HKEY hKeyLocation, const char * szSubKey, const char * szKey, const char * szDefault, char * szData, DWORD dwSize)
{
    HKEY hKey = nullptr;

    if (RegOpenKeyExA(hKeyLocation, szSubKey, NULL, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        DWORD dwType = REG_SZ;
        const auto lStatus = RegQueryValueExA(hKey, szKey, nullptr, &dwType, reinterpret_cast<BYTE *>(szData), &dwSize);
        RegCloseKey(hKey);
        return (lStatus == ERROR_SUCCESS);
    }

    if (szDefault)
        strncpy(szData, szDefault, dwSize);

    return false;
}

bool Utility::WriteString(const HKEY hKeyLocation, const char * szSubKey, const char * szKey, char * szData, const DWORD dwSize)
{
    HKEY hKey = nullptr;

    RegOpenKeyExA(hKeyLocation, szSubKey, NULL, KEY_WRITE, &hKey);

    if (!hKey)
        RegCreateKeyA(hKeyLocation, szSubKey, &hKey);

    if (hKey)
    {
        DWORD dwType = REG_SZ;
        RegSetValueExA(hKey, szKey, NULL, dwType, reinterpret_cast<BYTE *>(szData), dwSize);
        RegCloseKey(hKey);
        return true;
    }

    return false;
}

#endif
