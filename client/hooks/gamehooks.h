#include "CEntity.hpp"
#include "CCar.hpp"
#include "CGame.hpp"
#include "CEntityMessage.hpp"

#include "messages.h"

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
        mod_game_tick();
        __asm popad;
        __asm jmp[GameLoopHook_1_Return];
    }

    DWORD GameLoopHook_2_Return;
    void __declspec(naked) GameLoopHook_2()
    {
        __asm fstp    dword ptr[esp + 0x10];
        __asm fld     dword ptr[esp + 0x10];
        __asm pushad;
        mod_game_tick();
        __asm popad;
        __asm jmp[GameLoopHook_2_Return];
    }

    DWORD GameInitHook_Return = 0x4ECFC0;
    DWORD _C_PreloadSDS__FinishPendingSlots = 0x4E2690;
    void __declspec(naked) GameInitHook()
    {
        __asm call[_C_PreloadSDS__FinishPendingSlots];
        __asm pushad;
        mod_game_init();
        __asm popad;
        __asm jmp[GameInitHook_Return];
    }

    void __declspec(naked) GameStartDriveHook__1()
    {
        __asm call[_callDrive];
        __asm pushad;
        player_mod_message(E_PlayerMessage::MESSAGE_MOD_BREAKIN_CAR);
        __asm popad;
        __asm jmp[GameStartDrive__Return];
    }

    void __declspec(naked) GameStartDriveHook__2()
    {
        __asm call[_callDrive];
        __asm pushad;
        player_mod_message(E_PlayerMessage::MESSAGE_MOD_BREAKIN_CAR);
        __asm popad;
        __asm jmp[GameStartDrive_2__Return];
    }

    static M2::C_Car *car = nullptr;
    void __declspec(naked) GameStartDriveHook__3()
    {
        __asm call[_callDrive];
        __asm pushad
        player_mod_message(E_PlayerMessage::MESSAGE_MOD_ENTER_CAR);
        __asm popad;
        __asm jmp[GameStartDrive_3__Return];
    }

    void __declspec(naked) GameEndDriveHook()
    {
        __asm call[_callEnd];
        __asm pushad;
        player_mod_message(E_PlayerMessage::MESSAGE_MOD_LEAVE_CAR);
        __asm popad;
        __asm jmp[GameEndDrive__Return];
    }

    void *_this;
    void _declspec(naked) FrameReleaseFix()
    {
        __asm
        {
            pushad;
            mov _this, esi;
        }

        //TODO: Check if _this != nullptr

        __asm
        {
            popad;
            retn;
        }
    }

    void _declspec(naked) FrameReleaseFix2()
    {
        //TODO: Check if _this != nullptr
        __asm
        {
            retn;
        }
    }

    /* Entity Messages */

    typedef bool(__cdecl * CScriptEntity__RecvMessage_t) (void *lua, void *a2, const char *function, M2::C_EntityMessage *message);
    CScriptEntity__RecvMessage_t onReceiveMessage;
    int OnReceiveMessageHook(void *lua, void *a2, const char *function, M2::C_EntityMessage *pMessage)
    {
        if (pMessage) {
            M2::C_Game *game = M2::C_Game::Get();
            if (game) {
                M2::C_Player2 *player = game->GetLocalPed();
                if (player) {
                    M2::C_Entity *entity = reinterpret_cast<M2::C_Entity*>(player);
                    if (entity) {
                        if (pMessage->m_dwReceiveGUID == entity->m_dwGUID) {
                            player_game_message(pMessage);
                        }
                    }
                }
            }
        }
        return onReceiveMessage(lua, a2, function, pMessage);
    }

    /* Player input process */
    DWORD CPlayer2__UpdateInput__Return;
    DWORD CPlayer2__UpdateInput__Call = 0x42ABE0;
    M2::C_Entity *player = nullptr;
    void __declspec(naked) CPlayer2__UpdateInput()
    {
        __asm call[CPlayer2__UpdateInput__Call];
        __asm mov player, ebx;
        __asm pushad;

        //TODO: Hook here

        __asm popad;
        __asm jmp[CPlayer2__UpdateInput__Return];
    }

    DWORD CPlayer__EnterCar__Call = 0x42CAC0;
    DWORD CPlayer__EnterCar_JumpBack = 0x437945;
    void __declspec(naked) CPlayer2__EnterCar()
    {
        __asm
        {
            mov eax, dword ptr ss : [esp + 0x10]
            mov ecx, dword ptr ds : [edi + 0x44]

            pushad
                push eax
                push ecx
                push esi
                call game_player_enter_car
                add esp, 0xC
            popad

            push eax
            push ecx
            mov ecx, esi
            call CPlayer__EnterCar__Call
            jmp CPlayer__EnterCar_JumpBack
        }
    }

    DWORD CHuman2CarWrapper__GetCar = 0x9235F0;
    DWORD CHuman2CarWrapper__GetDoor = 0x940C80;
    static M2::C_Car *tryToEnterCar = nullptr;
    void __declspec(naked) CHuman2CarWrapper__IsFreeToGetIn__Hook()
    {
        __asm
        {
            mov ecx, esi;
            call CHuman2CarWrapper__GetCar;
            mov tryToEnterCar, eax;
        }


        if (player_request_vehicle_enter(tryToEnterCar) == true) {
            __asm {
                mov     al, 1
                pop     esi
                retn    8
            }
        }
        else {
            __asm {
                mov     al, 0
                pop     esi
                retn    8
            }
        }
    }

    /* Actions patching */

    void __declspec(naked) CCarActionEnter__TestAction__Hook()
    {
        __asm {
            pop     edi;
            mov     al, 1;
            pop     esi;
            add     esp, 0Ch;
            retn    4;
        }
    }

    void __declspec(naked) CCarActionBreakIn__TestAction__Hook()
    {
        __asm {
            pop     edi
            pop     esi
            mov     al, 0
            pop     ebx
            retn    4
        }
    }
    /*
    DWORD _CHuman2__AddCommand;
    void __declspec(naked) CHuman2__AddCommand()
    {
        __asm mov     eax, [esp + 4];
        __asm push    esi;



        __asm pushad;

        static int cmdtype;
        __asm mov cmdtype, eax;

        static char* cmd;
        __asm mov edi, [esp + 16];
        __asm mov cmd, edi;

        mod_log("CHuman2__AddCommand: type %d humancmdptr %x\n", cmdtype, cmd);
        __asm popad;


        __asm jmp[_CHuman2__AddCommand];
    }*/

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
        Mem::Hooks::InstallJmpPatch(0x437935, (Address)CPlayer2__EnterCar);

        // Crash fix on C_Frame::Release
        Mem::Hooks::InstallJmpPatch(0x14E5BC0, (DWORD)FrameReleaseFix);
        Mem::Hooks::InstallJmpPatch(0x12F0DB0, (DWORD)FrameReleaseFix2);

        // Patchs for enter action testing
        Mem::Hooks::InstallJmpPatch(0xA3E8E1, (DWORD)CCarActionEnter__TestAction__Hook);
        Mem::Hooks::InstallJmpPatch(0xA3F0A6, (DWORD)CCarActionBreakIn__TestAction__Hook);
        Mem::Hooks::InstallJmpPatch(0x956143, (DWORD)CHuman2CarWrapper__IsFreeToGetIn__Hook);

        // Entity Messages hooks
        onReceiveMessage = (CScriptEntity__RecvMessage_t) Mem::Hooks::InstallJmpPatch(0x117BCA0, (DWORD)OnReceiveMessageHook);

        // Player input hook
        CPlayer2__UpdateInput__Return = Mem::Hooks::InstallNotDumbJMP(0x43BD42, (Address)CPlayer2__UpdateInput);

        // noop the CreateMutex, allow to run multiple instances
        Mem::Hooks::InstallJmpPatch(0x00401B89, 0x00401C16);

        // Always use vec3
        *(BYTE *)0x09513EB = 0x75;
        *(BYTE *)0x0950D61 = 0x75;

        // Disable game reloading after death
        *(BYTE *)0x1CC397D = 1;

        // Disable game controlling engine state and radio
        Mem::Hooks::InstallJmpPatch(0x956362, 0x9563B6); // When leaving car
        Mem::Hooks::InstallJmpPatch(0x95621A, 0x956333); // When entering car

        // Disable game pause when minimized or in background
        Mem::Hooks::InstallJmpPatch(0xAC6D2B, 0xAC6F79);
        Mem::Hooks::InstallJmpPatch(0xAC6E57, 0xAC6F79);

        // Disable game creating vehicle (common/police) map icons
        Mem::Hooks::InstallJmpPatch(0x9CC219, 0x9CC220);//C_Car::OnActivate
        Mem::Hooks::InstallJmpPatch(0x9CC53C, 0x9CC543);//C_Car::OnDeactivate
        Mem::Hooks::InstallJmpPatch(0x4DCABD, 0x4DCAC4);//C_Car::SetSeatOwner
        Mem::Hooks::InstallJmpPatch(0x4DCC7D, 0x4DCC8A);//C_Car::SetSeatOwner

        // Prevent game controlling wipers
        Mem::Hooks::InstallJmpPatch(0x4877F1, 0x487892);//C_Car::UpdateIdleFX
        Mem::Hooks::InstallJmpPatch(0xA151CB, 0xA151D4);//C_Car::InitTuning

        // Disable shop loading
        //Mem::Utilites::PatchAddress(0x4731A0, 0x0004C2);
        //Mem::Utilites::PatchAddress(0xAC4B80, 0x0004C2);

        // Disable garages
        //Mem::Utilites::PatchAddress(0xCD6E90, 0xC300B0);
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

    LRESULT __stdcall mod_wndproc_hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (!mod.window) {
            mod.window = hWnd;
        }

        switch (uMsg) {
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_CHAR:
                zpl_mutex_lock(&mod.mutexes.wnd_msg);
                mod.wnd_msg.push({ hWnd, uMsg, wParam, lParam });
                zpl_mutex_unlock(&mod.mutexes.wnd_msg);
                break;

            case WM_INPUTLANGCHANGE:
                // UpdateCurrentLanguage();
                mod_log("language changed\n");
                break;

            case WM_QUIT:
                mod_log("WM_QUIT\n");
                mod_exit("exit");
                break;
        }

        return CallWindowProc(mod_wndproc_original, hWnd, uMsg, wParam, lParam);
    }

    BOOL WINAPI mod_peekmsg_hook(mod_peekmsg_args) {
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
}
