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
    * Hooking vehicle methods 
    */
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

    /**
    * Hooks for bugfixes
    */
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

    /* Human death */
    DWORD MineDeathHook_JumpBack = 0x00990CFF;
    DWORD _CHuman2__SetupDeath = 0x0098C160;

    void OnHumanSetupDeath(M2::C_Human2* human, M2::C_EntityMessageDamage* damage)
    {
        if (human == reinterpret_cast<M2::C_Human2*>(M2::C_Game::Get()->GetLocalPed())) {
            mod_log("The player just died\n");
        }
        else {
            mod_log("An human just died\n");
        }
    }

    __declspec(naked) void CHuman2__SetupDeath_Hook()

    {
        __asm
        {
            pushad
            push esi
            push ebp
            call OnHumanSetupDeath
            add esp, 0x8
            popad

            push    esi
            mov     ecx, ebp
            call    _CHuman2__SetupDeath

            jmp MineDeathHook_JumpBack
        }
    }

    DWORD _CHuman2__DoDamage = 0x09907D0;
    DWORD _DoDamage__JumpBack = 0x042FC6F;
    void OnHumanDoDamage(M2::C_Human2 *human, M2::C_EntityMessageDamage *message)
    {
        //Do things here
    }

    __declspec(naked) void CHuman2__DoDamage__Hook()
    {
        __asm
        {
            pushad;
            push esi;
            push edi;
            call OnHumanDoDamage;
            add esp, 0x8;
            popad;

            push edi;
            mov ecx, esi;
            call _CHuman2__DoDamage;

            jmp _DoDamage__JumpBack;
        }
    }

    /* Game Module Implementation */
    DWORD __GameModuleInstall = 0x4F2F0A;
    void __declspec(naked) GameModuleInstall()
    {
        __asm {
            mov eax, [edx + 1Ch];
            push 0Ah;
        }
        __asm pushad;
        gamemodule_install();
        __asm popad;
        __asm jmp[__GameModuleInstall];
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

    DWORD _CHuman2__AddCommand;
    void __declspec(naked) CHuman2__AddCommand()
    {
        __asm mov     eax, [esp + 4];
        __asm push    esi;



        __asm pushad;

        static M2::E_Command cmdtype;
        __asm mov cmdtype, eax;

        static char* cmd;
        __asm mov edi, [esp + 16];
        __asm mov cmd, edi;

        mod_log("CHuman2__AddCommand: type %d humancmdptr %x", cmdtype, cmd);
        __asm popad;


        __asm jmp[_CHuman2__AddCommand];
    }

    DWORD __LoadCityPart;
    void __declspec(naked) LoadCityPartsHook()
    {
        __asm {
            push ebx;
            push ebp;
            push esi;
            push edi;
            mov edi, [ecx + 16];
        }
        __asm pushad;
        mod_log("load city part\n");
        __asm popad;
        __asm jmp[__LoadCityPart];
    }
    /**
     * Game hooking calls
     */

    void gamehooks_install()
    {
        // Hooking game module registering
        Mem::Hooks::InstallJmpPatch(0x4F2F05, (DWORD)GameModuleInstall, 5);

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

        // Hooking human death
        Mem::Hooks::InstallJmpPatch(0x00990CF7, (DWORD)&CHuman2__SetupDeath_Hook);
        Mem::Hooks::InstallJmpPatch(0x042FC63, (DWORD)&CHuman2__DoDamage__Hook);

        //_CHuman2__AddCommand = (DWORD)Mem::Hooks::InstallNotDumbJMP(0x94D400, (DWORD)CHuman2__AddCommand, 5);
        __LoadCityPart = (DWORD)Mem::Hooks::InstallNotDumbJMP(0x4743C0, (DWORD)LoadCityPartsHook, 5);

        // Entity Messages hooks
        onReceiveMessage = (CScriptEntity__RecvMessage_t) Mem::Hooks::InstallJmpPatch(0x117BCA0, (DWORD)OnReceiveMessageHook);

        // Player input hook
        CPlayer2__UpdateInput__Return = Mem::Hooks::InstallNotDumbJMP(0x43BD42, (Address)CPlayer2__UpdateInput);

        // noop the CreateMutex, allow to run multiple instances
        Mem::Hooks::InstallJmpPatch(0x00401B89, 0x00401C16);

        // Always use vec3
        *(BYTE *)0x09513EB = 0x75;
        *(BYTE *)0x0950D61 = 0x75;

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
