#include <string>
#include <Windows.h>

#include "Injector.h"
#include "SteamWrapper.h"

#include "Shared\String.hpp"

#ifdef _DEBUG
static const char *const CORE_DLL_NAME("Client_d.dll");
#else
static const char *const CORE_DLL_NAME("Client.dll");
#endif

static const char*	GAME_NAME("Mafia 2");

static const char*	GAME_EXE_NAME("mafia2.exe");

static unsigned		GAME_APPID(50130);

static const char*	MOD_NAME("Mafia 2 Online");	

namespace Launcher
{
	static void HandleError(const std::string &errorMessage)
	{
		MessageBox(NULL, errorMessage.c_str(), "Fatal Error", MB_ICONERROR);
	}

	int Main(int argc, char *argv[])
	{
		SteamWrapper	*wrap = new SteamWrapper;
		std::string		errorMessage;

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
		ISteamApps *const	steamApps = wrap->GetContext()->SteamApps();
		const AppId_t		mafia2appId = GAME_APPID;

		if (steamApps->BIsAppInstalled(mafia2appId) == false)
		{
			Launcher::HandleError("In order to play M2-Online, you need to have Mafia2 installed with Steam");
			return (0);
		}

		// Fetch game path ?
		char gamePath[MAX_PATH];
		steamApps->GetAppInstallDir(mafia2appId, gamePath, MAX_PATH);

		// Is everything present ?
		SuperString pathString;
		pathString.Format("%s\\pc\\%s", gamePath, GAME_EXE_NAME);
		if (GetFileAttributesA(pathString.GetCStr()) == INVALID_FILE_ATTRIBUTES)
		{
			SuperString error;
			error.Format("Unable to find .exe file\n\nGame File : %s\nErrno : %u", pathString.GetSTLString(), GetLastError());
			Launcher::HandleError(error.GetSTLString());
			return (0);
		}

		SuperString dllPath;
		dllPath.Format("%s%s", GetAppPath(), CORE_DLL_NAME);

		if (GetFileAttributesA(dllPath.GetCStr()) == INVALID_FILE_ATTRIBUTES)
		{
			SuperString error = "Unable to find the DLL file";
			Launcher::HandleError(error.GetSTLString());
			return (0);
		}

		std::string appIDfile;
		appIDfile.assign(GetAppPath()).append("steam_appid.txt");

		if (GetFileAttributesA(appIDfile.c_str()) == INVALID_FILE_ATTRIBUTES)
		{
			SuperString error = "Unable to find the steam_appid.txt file";
			Launcher::HandleError(error.GetSTLString());
			return (0);
		}

		STARTUPINFO siStartupInfo;
		PROCESS_INFORMATION piProcessInfo;
		memset(&siStartupInfo, 0, sizeof(siStartupInfo));
		memset(&piProcessInfo, 0, sizeof(piProcessInfo));
		siStartupInfo.cb = sizeof(siStartupInfo);

		if (!CreateProcess(pathString.GetCStr(), NULL, NULL, NULL, TRUE, CREATE_SUSPENDED, NULL, GetAppPath(), &siStartupInfo, &piProcessInfo))
		{
			SuperString error = "Failed to start mafia2.exe, can't launch.";
			Launcher::HandleError(error.GetSTLString());
			return 1;
		}

		const InjectLibraryResults moduleInjectResult = InjectLibraryIntoProcess(piProcessInfo.hProcess, dllPath.GetCStr());

		if (moduleInjectResult != INJECT_LIBRARY_RESULT_OK)
		{
			SuperString error;
			error.Format("Failed to inject module into game process\nCore : %s (%i)", InjectLibraryResultToString(moduleInjectResult), moduleInjectResult);
			Launcher::HandleError(error.GetSTLString());

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