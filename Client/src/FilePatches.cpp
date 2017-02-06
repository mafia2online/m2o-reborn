#include <stdafx.h>
#include <Shared\Client\CPatcher.h>
#include <FilePatcher.h>

SString szPatchFiles[] = {
	"sdsconfig.bin",
	"StreamMapa.bin",
	"tables.sds",
	"gui.sds",
	"gui-main.sds"
};

const char* GetFileNameFromPath(const char* szPath)
{
	const char* szFileName = szPath;

	if (szFileName != NULL)
	{
		int len = strlen(szPath);
		if (len > 0)
		{
			szFileName = szPath + len - 1;

			while (szFileName != szPath &&
				*szFileName != '\\' && *szFileName != '/')
				szFileName--;

			if (*szFileName == '\\' || *szFileName == '/')
				szFileName++;
		}
	}

	return szFileName;
}

typedef HANDLE(__stdcall* tCreateFileA)(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
tCreateFileA orig_CreateFileA = nullptr;
HANDLE WINAPI CreateFileAHooked(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	if (lpFileName == NULL)
		return INVALID_HANDLE_VALUE;

	HANDLE hFile = INVALID_HANDLE_VALUE;
	bool file_found = false;
	const char* szFileName = GetFileNameFromPath(lpFileName);

	for (auto szFileName : szPatchFiles)
	{
		if (szFileName == szFileName)
		{
			file_found = true;
		}
	}

	if (file_found)
	{
		char szCurDir[MAX_PATH] = { '\0' };
		GetCurrentDirectoryA(sizeof(szCurDir), szCurDir);
		sprintf_s(szCurDir, MAX_PATH, "%s\\game_files\\%s", szCurDir, szFileName);

		hFile = CreateFileA(szCurDir, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}
	else
	{
		hFile = CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}

	return hFile;
}


void FilePatcher::Initialize()
{
	orig_CreateFileA = (tCreateFileA)Mem::Hooks::InstallDetourPatch("kernel32.dll", "CreateFileA", (Address)CreateFileAHooked);
}