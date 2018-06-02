#include "cef_app.h"
#include <windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    CefMainArgs args(GetModuleHandle(NULL));
    return CefExecuteProcess(args, nullptr, nullptr);
}
