#include <string>
#include <Windows.h>

namespace Launcher
{
	static void HandleError(const std::string &errorMessage)
	{
		MessageBox(NULL, errorMessage.c_str(), "Fatal Error", MB_ICONERROR);
	}

	int Main(int argc, char *argv[])
	{
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int appReturn = Launcher::Main(__argc, __argv);
	return appReturn;
}