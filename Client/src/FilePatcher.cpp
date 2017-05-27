/** @file FilePatcher.cpp
*  @brief Doing patchy stuff on game files.
*
*
*  @author MyU (myudev0@gmail.com)
*/

#include <stdafx.h>
#include <CCore.h>
#include <Shared\Client\CPatcher.h>
#include <FilePatcher.h>

SString szPatchFiles[] = {
	/*"sdsconfig.bin",
	"StreamMapa.bin",
	"tables.sds",*/
	"gui.sds",
	"gui-main.sds"
};


typedef HANDLE(__stdcall* tCreateFileA)(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
tCreateFileA orig_CreateFileA = nullptr;
HANDLE WINAPI CreateFileAHooked(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	if (lpFileName == NULL)
		return INVALID_HANDLE_VALUE;

	HANDLE hFile = INVALID_HANDLE_VALUE;
	bool file_found = false;
	
	SString strFileName(lpFileName);
	std::string str = strFileName.GetSTLString();
	strFileName.Set(str.substr(str.find_last_of("/\\") + 1));

	for (auto szFileName : szPatchFiles)
	{
		if (strFileName == szFileName)
		{
			file_found = true;
		}
	}

	if (file_found)
	{
		SString strNewDir(CCore::Instance().GetModAppDir());
		strNewDir += "\\game_files\\";
		strNewDir += strFileName;
		CCore::Instance().GetLogger().Writeln("FilePatcher: Redirecting game file %s to %s", strFileName.GetCStr(), strNewDir.GetCStr());
		hFile = orig_CreateFileA(strNewDir.GetCStr(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	
		if (hFile == INVALID_HANDLE_VALUE)
		{
			CCore::Instance().ExitGame(SString("Couldn't patch file: ") + strFileName);
		}
	}
	else
	{
		hFile = orig_CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}

	return hFile;
}


void FilePatcher::Initialize()
{
	orig_CreateFileA = (tCreateFileA)Mem::Hooks::InstallDetourPatch("kernel32.dll", "CreateFileA", (Address)CreateFileAHooked);
}