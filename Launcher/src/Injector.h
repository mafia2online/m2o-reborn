#pragma once

#include <Windows.h>
#include <sys/types.h>

enum InjectLibraryResults
{
	INJECT_LIBRARY_RESULT_OK,

	INJECT_LIBRARY_RESULT_WRITE_FAILED,
	INJECT_LIBRARY_GET_RETURN_CODE_FAILED,
	INJECT_LIBRARY_LOAD_LIBRARY_FAILED,
	INJECT_LIBRARY_THREAD_CREATION_FAILED,

	INJECT_LIBRARY_OPEN_PROCESS_FAIL

	// Add new values here to keep error numbers.
};

const char			*InjectLibraryResultToString(const InjectLibraryResults result);

InjectLibraryResults InjectLibraryIntoProcess(HANDLE hProcess, const char * szLibraryPath);
InjectLibraryResults InjectLibraryIntoProcess(DWORD dwProcessId, const char * szLibraryPath);

bool				StripPath1(char * szString);
char *				StripPath2(char * szString);

const char			* GetAppPath(void);
const char			* GetExePath(void);