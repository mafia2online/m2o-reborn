namespace tools {
    // maximum mumber of lines the output console should have
    static const WORD MAX_CONSOLE_LINES = 500;

    void console_attach() {
        // Attach
        AllocConsole();
        AttachConsole(GetCurrentProcessId());

        // Relay Input/Output
        FILE* x;
        freopen_s(&x, "CONOUT$", "w", stdout);
        freopen_s(&x, "CONIN$", "r", stdin);

        // Title
        HWND hConsole = GetConsoleWindow();
        std::string title = TEXT("m2o-reborn");
        SetConsoleTitle(title.c_str());

        // Position
        RECT rect;
        GetWindowRect(hConsole, &rect);
        SetWindowPos(hConsole, NULL, 20, 20, 800, 600, 0);
    }

    void console_color_fg(int textColor) {
        HANDLE cOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO cBufferInfo;
        GetConsoleScreenBufferInfo(cOutHandle, &cBufferInfo);
        WORD attributes = cBufferInfo.wAttributes & ~FOREGROUND_RED & ~FOREGROUND_GREEN & ~FOREGROUND_BLUE & ~FOREGROUND_INTENSITY;
        attributes |= textColor;
        SetConsoleTextAttribute(cOutHandle, attributes);
    }

    void console_color_bg(int textColor) {
        HANDLE cOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO cBufferInfo;
        GetConsoleScreenBufferInfo(cOutHandle, &cBufferInfo);
        WORD attributes = cBufferInfo.wAttributes & ~BACKGROUND_RED & ~BACKGROUND_GREEN & ~BACKGROUND_BLUE & ~BACKGROUND_INTENSITY;
        attributes |= textColor;
        SetConsoleTextAttribute(cOutHandle, attributes);
    }

    void console_cursor_pos(int CursorX, int CursorY) {
        HANDLE cOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD cCursorPosition;
        cCursorPosition.X = CursorX;
        cCursorPosition.Y = CursorY;
        SetConsoleCursorPosition(cOutHandle, cCursorPosition);
    }

    void console_clear(void) {
        system("cls");
    }
}
