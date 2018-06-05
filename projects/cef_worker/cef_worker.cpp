#include <windows.h>
#include <cef_app.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    CefMainArgs args(GetModuleHandle(NULL));
    return CefExecuteProcess(args, nullptr, nullptr);
}
