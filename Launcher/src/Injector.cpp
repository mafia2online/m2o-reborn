#include <assert.h>
#include <Windows.h>
#include <psapi.h>
#include <fstream>
#include <sstream>
#include <sys/types.h>

#pragma comment(lib, "Psapi.lib")

#include "Injector.h"

const char *InjectLibraryResultToString(const InjectLibraryResults result)
{
	switch (result) {
	case INJECT_LIBRARY_RESULT_OK:				return "Ok";
	case INJECT_LIBRARY_RESULT_WRITE_FAILED:	return "Failed to write memory into process";
	case INJECT_LIBRARY_GET_RETURN_CODE_FAILED: return "Failed to get return code of the load call";
	case INJECT_LIBRARY_LOAD_LIBRARY_FAILED:	return "Failed to load library";
	case INJECT_LIBRARY_THREAD_CREATION_FAILED:	return "Failed to create remote thread";

	case INJECT_LIBRARY_OPEN_PROCESS_FAIL:		return "Open of the process failed";
	default:									return "Unknown error";
	}
}

InjectLibraryResults InjectLibraryIntoProcess(HANDLE hProcess, const char * szLibraryPath)
{
	InjectLibraryResults result = INJECT_LIBRARY_RESULT_OK;

	// Get the length of the library path
	size_t sLibraryPathLen = (strlen(szLibraryPath) + 1);

	// Allocate the a block of memory in our target process for the library path
	void * pRemoteLibraryPath = VirtualAllocEx(hProcess, NULL, sLibraryPathLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	// Write our library path to the allocated block of memory
	SIZE_T sBytesWritten = 0;
	WriteProcessMemory(hProcess, pRemoteLibraryPath, (void *)szLibraryPath, sLibraryPathLen, &sBytesWritten);

	if (sBytesWritten != sLibraryPathLen)
	{
		result = INJECT_LIBRARY_RESULT_WRITE_FAILED;
	}
	else
	{
		// Get the handle of Kernel32.dll
		HMODULE hKernel32 = GetModuleHandle("Kernel32");

		// Get the address of the LoadLibraryA function from Kernel32.dll
		FARPROC pfnLoadLibraryA = GetProcAddress(hKernel32, "LoadLibraryA");

		// Create a thread inside the target process to load our library
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pfnLoadLibraryA, pRemoteLibraryPath, 0, NULL);

		if (hThread)
		{
			// Wait for the created thread to end
			WaitForSingleObject(hThread, INFINITE);

			DWORD dwExitCode = 0;
			if (GetExitCodeThread(hThread, &dwExitCode))
			{
				// Should never happen as we wait for the thread to be finished.
				assert(dwExitCode != STILL_ACTIVE);
			}
			else {
				result = INJECT_LIBRARY_GET_RETURN_CODE_FAILED;
			}

			// In case LoadLibrary returns handle equal to zero there was some problem.
			if (dwExitCode == 0)
			{
				result = INJECT_LIBRARY_LOAD_LIBRARY_FAILED;
			}

			// Close our thread handle
			CloseHandle(hThread);
		}
		else
		{
			// Thread creation failed
			result = INJECT_LIBRARY_THREAD_CREATION_FAILED;
		}
	}

	// Free the allocated block of memory inside the target process
	VirtualFreeEx(hProcess, pRemoteLibraryPath, 0, MEM_RELEASE);
	return result;
}

InjectLibraryResults InjectLibraryIntoProcess(DWORD dwProcessId, const char * szLibraryPath)
{
	// Open our target process
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);

	if (!hProcess)
	{
		// Failed to open the process
		return INJECT_LIBRARY_OPEN_PROCESS_FAIL;
	}

	// Inject the library into the process
	InjectLibraryResults result = InjectLibraryIntoProcess(hProcess, szLibraryPath);

	// Close the process handle
	CloseHandle(hProcess);
	return result;
}

bool StripPath1(char * szString)
{
	for (size_t i = strlen(szString); i > 0; --i)
	{
#ifdef _WIN32
		if (szString[i] == '\\')
#else
		if (szString[i] == '/')
#endif
		{
			szString[i + 1] = '\0';
			return true;
		}
	}

	return false;
}

char * StripPath2(char * szString)
{
	char * szPath = szString;
	char * szStrippedPath = (szString + strlen(szString));

	while (szStrippedPath != szPath)
	{
#ifdef _WIN32
		if (*szStrippedPath == '\\')
#else
		if (*szStrippedPath == '/')
#endif
		{
			return (szStrippedPath + 1);
		}

		szStrippedPath--;
	}

	return szPath;
}

const char * GetAppPath(void)
{
#ifdef _WIN32
	static unsigned int uiDummy;
#endif
	static char szAppPath[MAX_PATH];

#ifdef _WIN32
	HMODULE hModuleHandle;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&uiDummy, &hModuleHandle);
	GetModuleFileName(hModuleHandle, szAppPath, MAX_PATH);
#else
	readlink("/proc/self/exe", szAppPath, MAX_PATH);
#endif

	StripPath1(szAppPath);
	return szAppPath;
}

const char * GetExePath(void)
{
	static char szExePath[MAX_PATH];

#ifdef _WIN32
	GetModuleFileName(GetModuleHandle(NULL), szExePath, MAX_PATH);
#else
	readlink("/proc/self/exe", szExePath, MAX_PATH);
#endif

	StripPath1(szExePath);
	return szExePath;
}