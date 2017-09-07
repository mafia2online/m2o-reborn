namespace tools {

    DWORD GameStartDrive__Return;
    DWORD GameStartDrive_2__Return;
    DWORD GameStartDrive_3__Return;
    DWORD GameEndDrive__Return;
    DWORD _callDrive = 0x042CAC0;
    DWORD _callEnd = 0x99CE70;

    /**
     * Game hooking methods
     */

    DWORD GameLoopHook_1_Return;
    DWORD _call = 0x473D10;
    void __declspec(naked) GameLoopHook_1()
    {
        __asm call[_call];
        __asm pushad;
        game_tick();
        __asm popad;
        __asm jmp[GameLoopHook_1_Return];
    }

    DWORD GameLoopHook_2_Return;
    void __declspec(naked) GameLoopHook_2()
    {
        __asm fstp    dword ptr[esp + 0x10];
        __asm fld     dword ptr[esp + 0x10];
        __asm pushad;
        game_tick();
        __asm popad;
        __asm jmp[GameLoopHook_2_Return];
    }

    DWORD GameInitHook_Return = 0x4ECFC0;
    DWORD _C_PreloadSDS__FinishPendingSlots = 0x4E2690;
    void __declspec(naked) GameInitHook()
    {
        __asm call[_C_PreloadSDS__FinishPendingSlots];
        __asm pushad;
        game_init();
        __asm popad;
        __asm jmp[GameInitHook_Return];
    }


    //TODO Reverse properly to get the vehicle where the player enter in : waiting vehicle creation method
    void __declspec(naked) GameStartDriveHook__1()
    {
        __asm call[_callDrive];
        __asm pushad;
        mod_log("Break in car1\n");
        __asm popad;
        __asm jmp[GameStartDrive__Return];
    }

    void __declspec(naked) GameStartDriveHook__2()
    {
        __asm call[_callDrive];
        __asm pushad;
        mod_log("Break in car2\n");
        __asm popad;
        __asm jmp[GameStartDrive_2__Return];
    }

    void __declspec(naked) GameStartDriveHook__3()
    {
        __asm call[_callDrive];
        __asm pushad;
        mod_log("Entered car\n");
        __asm popad;
        __asm jmp[GameStartDrive_3__Return];
    }

    void __declspec(naked) GameEndDriveHook()
    {
        __asm call[_callEnd];
        __asm pushad;
        mod_log("Leaved car\n");
        __asm popad;
        __asm jmp[GameEndDrive__Return];
    }

    /**
     * Game hooking calls
     */

    void gamehooks_install()
    {
        // main game loops
        GameLoopHook_1_Return = Mem::Hooks::InstallNotDumbJMP(0x4ED614, (Address)GameLoopHook_1);
        GameLoopHook_2_Return = Mem::Hooks::InstallNotDumbJMP(0x4ED04D, (DWORD)GameLoopHook_2, 8);

        // game init
        GameInitHook_Return = Mem::Hooks::InstallNotDumbJMP(0x4ECFBB, (DWORD)GameInitHook);

        // vehicle hooks
        GameStartDrive__Return = Mem::Hooks::InstallNotDumbJMP(0x043B305, (Address)GameStartDriveHook__1);
        GameStartDrive_2__Return = Mem::Hooks::InstallNotDumbJMP(0x43B394, (Address)GameStartDriveHook__2);
        GameStartDrive_3__Return = Mem::Hooks::InstallNotDumbJMP(0x437940, (Address)GameStartDriveHook__3);
        GameEndDrive__Return = Mem::Hooks::InstallNotDumbJMP(0x43BAAD, (Address)GameEndDriveHook);

        // noop the CreateMutex, allow to run multiple instances
        Mem::Hooks::InstallJmpPatch(0x00401B89, 0x00401C16);

        // Disabled hooks (last edited by MyU)
        // AddEvent = (DWORD)Mem::Hooks::InstallJmpPatch(0x11A58A0, (DWORD)C_TickedModuleManager__AddEvent);
        // CallEvent = (DWORD)Mem::Hooks::InstallDetourPatch(0x1199B40, (DWORD)C_TickedModuleManager__CallEvent);
        // CallEvents = (DWORD)Mem::Hooks::InstallDetourPatch(0x1199BA0, (DWORD)C_TickedModuleManager__CallEvents);
        // CallEventByIndex = (DWORD)Mem::Hooks::InstallDetourPatch(0x1199960, (DWORD)C_TickedModuleManager__CallEventByIndex);
        // Mem::Hooks::InstallJmpPatch(0x5CFCD0, (DWORD)HOOK_C_SDSManager__ActivateStreamMapLine);
    }

    /**
     * Hooking os provided event loop
     * (to block mouse or keyboard events)
     */

    #define mod_peekmsg_args            \
        _Out_     LPMSG lpMsg,          \
        _In_opt_  HWND hWnd,            \
        _In_      UINT wMsgFilterMin,   \
        _In_      UINT wMsgFilterMax,   \
        _In_      UINT wRemoveMsg

    typedef BOOL(WINAPI *wnd_peekmsg_cb)(mod_peekmsg_args);
    typedef WNDPROC wnd_wndproc_cb;

    wnd_peekmsg_cb mod_peekmsg_original;
    wnd_wndproc_cb mod_wndproc_original;

    LRESULT __stdcall mod_wndproc_hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (!mod.window) {
            mod.window = hWnd;
            mod.input_blocked = false;
        }

        if (nk_ctx) {
            nk_input_begin(nk_ctx);
            nk_d3d9_handle_event(hWnd, uMsg, wParam, lParam);
            nk_input_end(nk_ctx);
        }

        return CallWindowProc(mod_wndproc_original, hWnd, uMsg, wParam, lParam);
    }

    BOOL WINAPI mod_peekmsg_hook(mod_peekmsg_args)
    {
        mod_wndproc_hook(hWnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);

        // blocking the input
        if (mod.input_blocked && lpMsg->message == WM_INPUT) {
            return false;
        }

        return mod_peekmsg_original(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
    }

    void gamehooks_install_late()
    {
        HWND hWnd = *(HWND *)((*(DWORD*)0x1ABFE30) + 28);
        SetWindowText(hWnd, MOD_NAME);

        // hook the wind proc
        mod_wndproc_original = (wnd_wndproc_cb)SetWindowLongPtr(hWnd, GWL_WNDPROC, (LONG_PTR)mod_wndproc_hook);
        SetWindowLongW(hWnd, GWL_WNDPROC, GetWindowLong(hWnd, GWL_WNDPROC));

        // hook the peek msg
        mod_peekmsg_original = (wnd_peekmsg_cb)DetourFunction(
            DetourFindFunction("USER32.DLL", "PeekMessageW"),
            reinterpret_cast<PBYTE>(mod_peekmsg_hook)
        );
    }


    /**
     * Archieve
     */


    /*
    std::unordered_map<int, SString> event_name_map;

    DWORD AddEvent;
    int __fastcall C_TickedModuleManager__AddEvent(DWORD _this, DWORD ebx, int a2, char *a3)
    {
    //CCore::Instance().GetLogger().Writeln("AddEvent(%d, %s) - Current Thread ID: %x", a2, a3, GetCurrentThreadId());
    event_name_map[a2] = SString(a3);
    return Mem::InvokeFunction<Mem::call_this, int>(AddEvent, _this, a2, a3);
    }

    DWORD CallEvent;
    int __fastcall C_TickedModuleManager__CallEvent(DWORD _this, DWORD ebx, int a2, int a3)
    {

    if (a2 != 36 && a2 != 37 && a2 != 5 && a2 != 22 && a2 != 23)
    CCore::Instance().GetLogger().Writeln("CallEvent \"%s\" with params %d from %x.", event_name_map[a2].GetCStr(), a3, GetCurrentThreadId());

    return Mem::InvokeFunction<Mem::call_this, int>(CallEvent, _this, a2, a3);
    }

    DWORD CallEvents;
    int __fastcall C_TickedModuleManager__CallEvents(DWORD _this, DWORD ebx, int a1, int a3, int *a4, int a5)
    {
    CCore::Instance().GetLogger().Writeln("CallEvents: %d %d %d %d", a1, a3, a4, a5);
    //CCore::Instance().GetLogger().Writeln("Call Events size: %d", vec.size());

    std::vector<int>* vec = reinterpret_cast<std::vector<int>*>(a1);
    CCore::Instance().GetLogger().Writeln("Vector size: %d %d", vec->size(), vec[0]);


    //CCore::Instance().GetLogger().Writeln("CallEvents %s with params %d from %x.", event_name_map[a2].GetCStr(), a3, GetCurrentThreadId());
    return Mem::InvokeFunction<Mem::call_this, int>(CallEvents, _this, a1, a3, a4, a5);
    }

    DWORD CallEventByIndex;
    int __fastcall C_TickedModuleManager__CallEventByIndex(DWORD _this, DWORD ebx, rsize_t DstSize, int a3, int a4)
    {
    CCore::Instance().GetLogger().Writeln("CallEventByIndex: %d %d %d", DstSize, a3, a4);
    //CCore::Instance().GetLogger().Writeln("CallEvents %s with params %d from %x.", event_name_map[a2].GetCStr(), a3, GetCurrentThreadId());
    return Mem::InvokeFunction<Mem::call_this, int>(CallEventByIndex, _this, DstSize, a3, a4);
    }



    const char * szStreamMapLine = "";
    DWORD C_SDSManager__ActivateStreamMapLine_JMP = 0x5CFCD7;
    DWORD C_SDSManager__ActivateStreamMapLine_END = 0x5CFFC1;

    void meh()
    {
    if (strcmp(szStreamMapLine, "free_summer_load") == 0) // free_area_state
    {

    }
    else if (strcmp(szStreamMapLine, "free_summer_load") == 0)
    {
    CCore::Instance().GetLogger().Writeln("FaderFadeIn %x!", M2::C_GameGuiModule::Get());
    }
    }
    DWORD _this_ebx;
    DWORD _ecx;
    void __declspec (naked) HOOK_C_SDSManager__ActivateStreamMapLine(void)
    {
    _asm push ebp;
    _asm mov ebp, esp;
    _asm mov eax, [ebp + 4];
    _asm mov _this_ebx, eax;
    _asm mov eax, [ebp + 12];
    _asm mov szStreamMapLine, eax;
    _asm mov _ecx, ecx;
    _asm pop ebp;
    _asm pushad;

    meh();
    CCore::Instance().GetLogger().Writeln("SDS::ActivateStreamMapLine Requesting: '%s' (%x)", szStreamMapLine, _ecx);

    _asm popad;
    _asm sub esp, 2Ch;
    _asm push ebx;
    _asm push ebp;
    _asm push esi;
    _asm push edi;
    _asm jmp C_SDSManager__ActivateStreamMapLine_JMP;
    }
    */
}
