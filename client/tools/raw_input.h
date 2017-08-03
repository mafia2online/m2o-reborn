namespace rawinput
{
    typedef BOOL(WINAPI* fPeekMessage)(
        _Out_     LPMSG lpMsg,
        _In_opt_  HWND hWnd,
        _In_      UINT wMsgFilterMin,
        _In_      UINT wMsgFilterMax,
        _In_      UINT wRemoveMsg
        );

    BOOL WINAPI PeekMessage_Hook(
        _Out_     LPMSG lpMsg,
        _In_opt_  HWND hWnd,
        _In_      UINT wMsgFilterMin,
        _In_      UINT wMsgFilterMax,
        _In_      UINT wRemoveMsg
    );

    std::function<void(RAWMOUSE)> on_mousemove;
    std::function<void(RAWKEYBOARD)> on_keyboard;

    fPeekMessage mOriginalPeekMessage;

    inline void ProcessMessage(LPMSG message)
    {
        // foreign events
        game_on_wnd_proc(message->hwnd, message->message, message->wParam, message->lParam);

        POINT mpos;
        GetCursorPos(&mpos);
        ScreenToClient(global_window, &mpos);
        mouse_pos pos = {mpos.x, mpos.y};


        // mouse buttons

        if (GetAsyncKeyState(VK_LBUTTON)) {
            game_on_wnd_proc(message->hwnd, WM_LBUTTONDOWN, 0, (LPARAM)&pos);
        }
        else {
            game_on_wnd_proc(message->hwnd, WM_LBUTTONUP, 0, (LPARAM)&pos);
        }

        if (GetAsyncKeyState(VK_RBUTTON)) {
            game_on_wnd_proc(message->hwnd, WM_RBUTTONDOWN, 0, (LPARAM)&pos);
        }
        else {
            game_on_wnd_proc(message->hwnd, WM_RBUTTONUP, 0, (LPARAM)&pos);
        }

        // mouse move
        //game_on_wnd_proc(message->hwnd, WM_MOUSEMOVE, 0, (LPARAM)&pos);

    }

    BOOL WINAPI PeekMessage_Hook(
        _Out_     LPMSG lpMsg,
        _In_opt_  HWND hWnd,
        _In_      UINT wMsgFilterMin,
        _In_      UINT wMsgFilterMax,
        _In_      UINT wRemoveMsg
    )
    {
        bool bResult = mOriginalPeekMessage(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg) == 1;

        if (bResult)
        {
            switch (lpMsg->message)
            {
            case WM_INPUT:
            {
                ProcessMessage(lpMsg);
                break;
            }
            default:
                break;
            }
        }

        return bResult;
    }

    inline void hook()
    {
        mOriginalPeekMessage = reinterpret_cast <fPeekMessage> (DetourFunction(DetourFindFunction("USER32.DLL", "PeekMessageW"), reinterpret_cast < PBYTE > (PeekMessage_Hook)));
    }
};
