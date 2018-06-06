DWORD GameStartDrive__Return;
DWORD GameStartDrive_2__Return;
DWORD GameStartDrive_3__Return;
DWORD GameEndDrive__Return;
DWORD _callDrive = 0x042CAC0;
DWORD _callEnd = 0x99CE70;

/**
 * Hooking vehicle methods
 */
void __declspec(naked) GameStartDriveHook__1() {
    __asm call[_callDrive];
    __asm pushad;
    {
        m2sdk_event event = { 0 }; event.arg1 = (void *)M2::E_PlayerMessage::MESSAGE_MOD_BREAKIN_CAR;
        M2::TriggerHandler(M2_EVENT_MOD_MESSAGE, &event);
    }
    __asm popad;
    __asm jmp[GameStartDrive__Return];
}

void __declspec(naked) GameStartDriveHook__2() {
    __asm call[_callDrive];
    __asm pushad;
    {
        m2sdk_event event = { 0 }; event.arg1 = (void *)M2::E_PlayerMessage::MESSAGE_MOD_BREAKIN_CAR;
        M2::TriggerHandler(M2_EVENT_MOD_MESSAGE, &event);
    }
    __asm popad;
    __asm jmp[GameStartDrive_2__Return];
}

static M2::C_Car *car = nullptr;
void __declspec(naked) GameStartDriveHook__3() {
    __asm call[_callDrive];
    __asm pushad
    {
        m2sdk_event event = { 0 }; event.arg1 = (void *)M2::E_PlayerMessage::MESSAGE_MOD_ENTER_CAR;
        M2::TriggerHandler(M2_EVENT_MOD_MESSAGE, &event);
    }
    __asm popad;
    __asm jmp[GameStartDrive_3__Return];
}

void __declspec(naked) GameEndDriveHook() {
    __asm call[_callEnd];
    __asm pushad;
    {
        m2sdk_event event = { 0 }; event.arg1 = (void *)M2::E_PlayerMessage::MESSAGE_MOD_LEAVE_CAR;
        M2::TriggerHandler(M2_EVENT_MOD_MESSAGE, &event);
    }
    __asm popad;
    __asm jmp[GameEndDrive__Return];
}

void game_player_enter_car(M2::C_Player2 *player, M2::C_Actor *car, u8 seat) {
    m2sdk_event event = { 0 }; {
        event.arg1 = (void *)player;
        event.arg2 = (void *)car;
        event.arg3 = (void *)seat;
    }

    M2::TriggerHandler(M2_EVENT_CAR_ENTER, &event);
}

DWORD CPlayer__EnterCar__Call = 0x42CAC0;
DWORD CPlayer__EnterCar_JumpBack = 0x437945;
void __declspec(naked) CPlayer2__EnterCar() {
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

bool player_request_vehicle_enter(M2::C_Car *car) {
    m2sdk_event event = { 0 }; {
        event.arg1 = (void *)car;
    }

    M2::TriggerHandler(M2_EVENT_CAR_ENTER_REQUEST, &event);
    return (bool)event.arg5;
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
                        m2sdk_event event = { 0 }; event.arg1 = (void *)pMessage;
                        M2::TriggerHandler(M2_EVENT_GAME_MESSAGE, &event);
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
        m2sdk_log("The player just died\n");
    }
    else {
        m2sdk_log("An human just died\n");
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

    m2sdk_log("CHuman2__AddCommand: type %d humancmdptr %x", cmdtype, cmd);
    __asm popad;


    __asm jmp[_CHuman2__AddCommand];
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
    g_gamemodule_callback();
    __asm popad;
    __asm jmp[__GameModuleInstall];
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
    m2sdk_log("load city part\n");
    __asm popad;
    __asm jmp[__LoadCityPart];
}
