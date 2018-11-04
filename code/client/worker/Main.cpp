
/*
*  This file is part of the NomadFX Project - https://nomad-group.net
*  More information regarding licensing can be found in LICENSE.md
*/

#include <Ui/UiApp.h>

#ifndef _WIN32
int main()

#else

#include <Windows.h>
#include <delayimp.h>

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
#endif
{
#ifdef _WIN32

    __HrLoadAllImportsForDll("libcef.dll");

    const CefMainArgs args(hInstance);
#else
    const CefMainArgs args();
#endif

    CefRefPtr<nfx::UiApp> ui_app = new nfx::UiApp;

    return CefExecuteProcess(args, ui_app, nullptr);
}
