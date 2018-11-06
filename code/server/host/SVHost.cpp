
#include <thread>
#include <iostream>

#include <SVHost.h>
#include <utility/PathUtils.h>
#include <IModules.h>

#ifdef __linux__
#include <dlfcn.h>
#include <errno.h>
#elif _WIN32
#include <Windows.h>
#endif

SVHost::SVHost(int, char *[]) : should_terminate(false)
{
}

bool SVHost::Initialize()
{
#ifdef _WIN32

    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        auto handle = GetStdHandle(STD_OUTPUT_HANDLE);

        GetConsoleScreenBufferInfo(handle, &csbi);
        SetConsoleTextAttribute(handle, csbi.wAttributes | FOREGROUND_INTENSITY);
        SetConsoleTextAttribute(handle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        SetConsoleTitleW(FXNAME_WIDE L" Server");
    }

    // initialize server components
    {
        auto root = Utility::MakeAbsolutePathW(L"core-server.dll");

        core_server = LoadLibraryW(root.c_str());

        if (core_server)
        {
            auto init = reinterpret_cast<GameServerInterface*(*)()>(GetProcAddress((HMODULE)core_server, "RunServer"));

            if (init)
            {
                gs = init();
            }
        }
		else
		{
			printf("[Host] Unable to load server core\n");
			
			return false;
		}

    }

#else

    {
        auto root = Utility::MakeAbsolutePathA("core-server.so");

        core_server = dlopen(root.c_str(), RTLD_LAZY);

        if (core_server)
        {
            auto init = reinterpret_cast<GameServerInterface*(*)()>(dlsym(core_server, "RunServer"));

            if (init)
            {
                gs = init();
            }
        }
		else
		{
			printf("[Host] Unable to load server core\n");
			
			return false;
		}
    }

#endif

    printf("[Host] Init\n");

    gs->SetInputPoller([](std::string & str) 
    {
        std::getline(std::cin, str);
    });

    return true;
}

void SVHost::Run()
{
    if (gs->Initialize())
    {
        gs->Run();
    }
}

void SVHost::Shutdown()
{
    gs->Shutdown();

    delete gs;

#ifdef _WIN32
    FreeLibrary(static_cast<HMODULE>(core_server));
#else
    dlclose(core_server);
#endif
}
