#include <string>
#include <windows.h>
#include <assert.h>
#include <psapi.h>
#include <fstream>
#include <sstream>
#include <sys/types.h>

#define VERSION_SAFE_STEAM_API_INTERFACES
#include <steam_api.h>

#ifdef _DEBUG
static const char *const CORE_DLL_NAME("Client_d.dll");
#else
static const char *const CORE_DLL_NAME("Client.dll");
#endif

static const char*  GAME_NAME("Mafia 2");
static const char*  GAME_EXE_NAME("mafia2.exe");
static unsigned     GAME_APPID(50130);
static const char*  MOD_NAME("Mafia 2 Online");

std::string Format(const char *szFormat, ...)
{
    va_list args;
    va_start(args, szFormat);
    const size_t len = static_cast<size_t>(vsnprintf(NULL, 0, szFormat, args));
    char *pszBuffer = new char[len + 1];
    vsprintf_s(pszBuffer, len+1, szFormat, args);
    va_end(args);
    std::string a = std::string(pszBuffer);
    delete[] pszBuffer;
    return a;
}

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

enum SteamWrapperError
{
    STEAM_WRAPPER_ERROR_STEAM_NOT_RUNNING = 1,
    STEAM_WRAPPER_ERROR_INIT_STEAM,
    STEAM_WRAPPER_ERROR_CONTEXT_CREATE,
    STEAM_WRAPPER_ERROR_CONTEXT_INIT
};

class SteamWrapper
{
private:
    CSteamAPIContext    *m_ctx;
    SteamWrapperError   m_lastError;

public:
    SteamWrapper();
    ~SteamWrapper() = default;

    bool    init();

    const   CSteamAPIContext    *GetContext() const;
    SteamWrapperError           GetLastErrorCode() const;
};


SteamWrapper::SteamWrapper() :
    m_ctx()
{
}

bool    SteamWrapper::init()
{
    if (!SteamAPI_IsSteamRunning()) {
        m_lastError = STEAM_WRAPPER_ERROR_STEAM_NOT_RUNNING;
        return false;
    }

    if (!SteamAPI_InitSafe()) {
        m_lastError = STEAM_WRAPPER_ERROR_INIT_STEAM;
        return false;
    }

    CSteamAPIContext *ctx = new CSteamAPIContext();
    m_ctx = ctx;

    if (!m_ctx) {
        m_lastError = STEAM_WRAPPER_ERROR_CONTEXT_CREATE;
        return false;
    }

    if (!m_ctx->Init()) {
        m_lastError = STEAM_WRAPPER_ERROR_CONTEXT_INIT;
        return false;
    }

    return true;
}

const CSteamAPIContext *SteamWrapper::GetContext() const
{
    return m_ctx;
}

SteamWrapperError SteamWrapper::GetLastErrorCode() const
{
    return m_lastError;
}

// const char          *InjectLibraryResultToString(const InjectLibraryResults result);

// InjectLibraryResults InjectLibraryIntoProcess(HANDLE hProcess, const char * szLibraryPath);
// InjectLibraryResults InjectLibraryIntoProcess(DWORD dwProcessId, const char * szLibraryPath);

// bool                StripPath1(char * szString);
// char *              StripPath2(char * szString);

// const char          * GetAppPath(void);
// const char          * GetExePath(void);

const char *InjectLibraryResultToString(const InjectLibraryResults result)
{
    switch (result) {
    case INJECT_LIBRARY_RESULT_OK:              return "Ok";
    case INJECT_LIBRARY_RESULT_WRITE_FAILED:    return "Failed to write memory into process";
    case INJECT_LIBRARY_GET_RETURN_CODE_FAILED: return "Failed to get return code of the load call";
    case INJECT_LIBRARY_LOAD_LIBRARY_FAILED:    return "Failed to load library";
    case INJECT_LIBRARY_THREAD_CREATION_FAILED: return "Failed to create remote thread";

    case INJECT_LIBRARY_OPEN_PROCESS_FAIL:      return "Open of the process failed";
    default:                                    return "Unknown error";
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





namespace Launcher
{
    static void HandleError(const std::string &errorMessage)
    {
        MessageBox(NULL, errorMessage.c_str(), "Fatal Error", MB_ICONERROR);
    }

    int Main(int argc, char *argv[])
    {
        SteamWrapper    *wrap = new SteamWrapper;
        std::string     errorMessage;

        // Process steam verifications
        if (wrap->init() == false)
        {
            switch (wrap->GetLastErrorCode())
            {
            case STEAM_WRAPPER_ERROR_STEAM_NOT_RUNNING:
                errorMessage = "Steam client is not running, please start it and retry to start the mod.";
                break;

            case STEAM_WRAPPER_ERROR_INIT_STEAM:
                errorMessage = "Unable to init the bridge between Mafia2Online and Steam. Not connected ?";
                break;

            case STEAM_WRAPPER_ERROR_CONTEXT_CREATE:
                errorMessage = "Unable to create the bridge between Mafia3Online and Steam, please be sure you have steam_appid.txt file.";
                break;

            case STEAM_WRAPPER_ERROR_CONTEXT_INIT:
                errorMessage = "Unknow error happened with SteamClient.";
                break;
            }

            Launcher::HandleError(errorMessage);
            return (0);
        }

        // Player has the game ?
        ISteamApps *const   steamApps = wrap->GetContext()->SteamApps();
        const AppId_t       mafia2appId = GAME_APPID;

        if (steamApps->BIsAppInstalled(mafia2appId) == false)
        {
            Launcher::HandleError("In order to play M2-Online, you need to have Mafia2 installed with Steam");
            return (0);
        }

        // Fetch game path ?
        char gamePath[MAX_PATH];
        steamApps->GetAppInstallDir(mafia2appId, gamePath, MAX_PATH);

        // Is everything present ?
        std::string pathString = Format("%s\\pc\\%s", gamePath, GAME_EXE_NAME);

        if (GetFileAttributesA(pathString.c_str()) == INVALID_FILE_ATTRIBUTES) {
            Launcher::HandleError(Format("Unable to find .exe file\n\nGame File : %s\nErrno : %u", pathString, GetLastError()));
            return (0);
        }

        std::string dllPath = Format("%s%s", GetAppPath(), CORE_DLL_NAME);

        if (GetFileAttributesA(dllPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
            std::string error = "Unable to find the DLL file";
            Launcher::HandleError(error);
            return (0);
        }

        std::string appIDfile;
        appIDfile.assign(GetAppPath()).append("steam_appid.txt");

        if (GetFileAttributesA(appIDfile.c_str()) == INVALID_FILE_ATTRIBUTES)
        {
            std::string error = "Unable to find the steam_appid.txt file";
            Launcher::HandleError(error);
            return (0);
        }

        STARTUPINFO siStartupInfo;
        PROCESS_INFORMATION piProcessInfo;
        memset(&siStartupInfo, 0, sizeof(siStartupInfo));
        memset(&piProcessInfo, 0, sizeof(piProcessInfo));
        siStartupInfo.cb = sizeof(siStartupInfo);

        if (!CreateProcess(pathString.c_str(), NULL, NULL, NULL, TRUE, CREATE_SUSPENDED, NULL, GetAppPath(), &siStartupInfo, &piProcessInfo))
        {
            std::string error = "Failed to start mafia2.exe, can't launch.";
            Launcher::HandleError(error);
            return 1;
        }

        const InjectLibraryResults moduleInjectResult = InjectLibraryIntoProcess(piProcessInfo.hProcess, dllPath.c_str());

        if (moduleInjectResult != INJECT_LIBRARY_RESULT_OK)
        {
            std::string error = Format("Failed to inject module into game process\nCore : %s (%i)", InjectLibraryResultToString(moduleInjectResult), moduleInjectResult);
            Launcher::HandleError(error);

            TerminateProcess(piProcessInfo.hProcess, 0);
            return 1;
        }

        ResumeThread(piProcessInfo.hThread);

        return 0;
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int appReturn = Launcher::Main(__argc, __argv);
    return appReturn;
}
