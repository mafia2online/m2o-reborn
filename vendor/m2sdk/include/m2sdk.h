/*
* Swag SDK for M2O
*/

#include <string>
#include <vector>

#include <windows.h>
#include <detours.h>
#include <assert.h>
#include <unordered_map>

#define m2sdk_assert assert

#ifndef M2SDK_H
#define M2SDK_H

#define M2_Address unsigned long
#define M2_Pointer unsigned int
#define Byte unsigned char

#if !defined(m2sdk_log)
#define m2sdk_log(fmt, ...) 
#endif

#define pad(p,n,s) Byte p##__##n##[s]
#define VTBLCall(retn,name,...) Mem::InvokeFunction<Mem::call_this, retn>(GetInterface()->m_pVFTable->##name, this, __VA_ARGS__)

typedef enum {
    M2_EVENT_GAME_MESSAGE,
    M2_EVENT_MOD_MESSAGE,
    M2_EVENT_CITY_PARTS_LOAD,
    M2_EVENT_INPUT_UPDATE,
    M2_EVENT_DRIVE_START_1,
    M2_EVENT_DRIVE_START_2,
    M2_EVENT_DRIVE_START_3,
    M2_EVENT_DRIVE_END,
    M2_EVENT_CAR_ENTER_REQUEST,
    M2_EVENT_CAR_ENTER,
    M2_EVENT_CAR_HOOD_OPEN_REQUEST,
    M2_EVENT_CAR_HOOD_CLOSE_REQUEST,
    M2_EVENT_CAR_TRUNK_OPEN_REQUEST,
    M2_EVENT_CAR_TRUNK_CLOSE_REQUEST,
    M2_EVENT_CAR_FUELTANK_REQUEST
} m2sdk_event_id;

typedef union {
    struct {
        void *arg1;
        void *arg2;
        void *arg3;
        void *arg4;
        void *arg5;
    };

    void *args[5];
} m2sdk_event;

typedef void (*m2sdk_callback)(void);
typedef void (*m2sdk_callback_event)(m2sdk_event *);

namespace M2 {

    void Initialize(m2sdk_callback callback);
    void InitializeSDKHandlers();
    void Free();

    void AttachHandler(m2sdk_event_id id, m2sdk_callback_event callback);
    void DetachHandler(m2sdk_event_id id);
    void TriggerHandler(m2sdk_event_id id, m2sdk_event *data);

    template < typename T, typename I, M2_Address A > class GameClassWrapperStatic {
    public:
        inline I* GetInterface() { return reinterpret_cast<I*>(this); }
        static T *Get() { return reinterpret_cast<T*>(A); }
    };

    template < typename T, typename I, M2_Address A > class GameClassWrapper {
    public:
        inline I* GetInterface() { return reinterpret_cast<I*>(this); }
        static T *Get() { return reinterpret_cast<T*>(*(M2_Address*)A); }
    };

    template < typename T > class GameFakeClassWrapper {
    public:
        static T *Get() { static T classs; return &classs; }
    };

    enum E_PlayerMessage
    {
        MESSAGE_MOD_ENTER_CAR,
        MESSAGE_MOD_LEAVE_CAR,
        MESSAGE_MOD_BREAKIN_CAR
    };
}

/* Utils */
#include "utils/Matrix.hpp"
#include "utils/TArray.hpp"
#include "utils/Memory.hpp"

#ifndef Vector3
#include "utils/Vector3.hpp"
#endif

#ifndef Quaternion
#include "utils/Quaternion.hpp"
#endif

/* Classes */
#include "classes/CActor.hpp"
#include "classes/CActorVehicle.hpp"
#include "classes/CAIController.hpp"
#include "classes/CApplication.hpp"
#include "classes/CAvailableStations.hpp"
#include "classes/ccamera.hpp"
#include "classes/CGameCamera.hpp"
#include "classes/CCar.hpp"
#include "classes/CCarActionBreakIn.hpp"
#include "classes/CCarActionCloseHood.hpp"
#include "classes/CCarActionCloseTrunk.hpp"
#include "classes/CCarActionEnter.hpp"
#include "classes/CCarActionOpenHood.hpp"
#include "classes/CCarActionOpenTrunk.hpp"
#include "classes/CCarActionTankFuel.hpp"
#include "classes/CCarActionThrowFrom.hpp"
#include "classes/CCarManager.hpp"
#include "classes/CCarOwnerDB.hpp"
#include "classes/CCore.hpp"
#include "classes/CDoor.hpp"
#include "classes/CEntity.hpp"
#include "classes/CEntityFactory.hpp"
#include "classes/CEntityMessage.hpp"
#include "classes/CEntityMessageDamage.hpp"
#include "classes/CEntityPos.hpp"
#include "classes/CFrame.hpp"
#include "classes/CGame.hpp"
#include "classes/CGameCameraMafia.hpp"
#include "classes/CGameGuiModule.hpp"
#include "classes/CGameInputModule.hpp"
#include "classes/CGameRenderingSystemModule.hpp"
#include "classes/CGameTrafficModule.hpp"
#include "classes/CGameVehicleModule.hpp"
#include "classes/CGfxEffects.hpp"
#include "classes/CGfxEnvironmentEffects.hpp"
#include "classes/CGuiGame.hpp"
#include "classes/CHashName.hpp"
#include "classes/CHints.hpp"
#include "classes/CHud.hpp"
#include "classes/CHudScriptW.hpp"
#include "classes/chuman2.hpp"
#include "classes/CHuman2Commands.hpp"
#include "classes/CHumanHeadController.hpp"
#include "classes/CHumanInventory.hpp"
#include "classes/CHumanScript.hpp"
#include "classes/CHumanWeaponController.hpp"
#include "classes/CLockpick.hpp"
#include "classes/cmafiaframework.hpp"
#include "classes/CModel.hpp"
#include "classes/CMusic.hpp"
#include "classes/CNavigation.hpp"
#include "classes/CPhoneBook.hpp"
#include "classes/CPlayer2.hpp"
#include "classes/CPlayerModelManager.hpp"
#include "classes/CPlayerRadio.hpp"
#include "classes/CPlaylistsDB.hpp"
#include "classes/CPoliceRadio.hpp"
#include "classes/CPoliceRootCoordinator.hpp"
#include "classes/CPoliceWanted.hpp"
#include "classes/CPoliceWrapper.hpp"
#include "classes/CProgramsDB.hpp"
#include "classes/CRadio.hpp"
#include "classes/CRangeMeter.hpp"
#include "classes/CRenderCamera.hpp"
#include "classes/CResources.hpp"
#include "classes/CReticle.hpp"
#include "classes/CRtR3DIconManager.hpp"
#include "classes/CRtRGameModule.hpp"
#include "classes/CScene.hpp"
#include "classes/CScriptMachine.hpp"
#include "classes/CScriptMachineManager.hpp"
#include "classes/CSDSManager.hpp"
#include "classes/CSDSModelManager.hpp"
#include "classes/CShotManager.hpp"
#include "classes/CSingleMesh.hpp"
#include "classes/CSlot.hpp"
#include "classes/CSlotManager.hpp"
#include "classes/CSlotWrapper.hpp"
#include "classes/CSpeedometer.hpp"
#include "classes/CSyncObject.hpp"
#include "classes/CTableData.hpp"
#include "classes/CTables.hpp"
#include "classes/CTelephone.hpp"
#include "classes/CTextDatabase.hpp"
#include "classes/CTickedModuleManager.hpp"
#include "classes/CTimer.hpp"
#include "classes/CTrafficLightsMgr.hpp"
#include "classes/CTrafficStreaming.hpp"
#include "classes/CVehDoor.hpp"
#include "classes/CVehicle.hpp"
#include "classes/CVehicleEffectManager.hpp"
#include "classes/CWeatherManager.hpp"
#include "classes/CWrappersList.hpp"
#include "classes/CHuman2CarWrapper.hpp"
#include "classes/IActorActionModule.hpp"
#include "classes/SAgentCommandAim.hpp"
#include "classes/SAgentCommandLookAt.hpp"
#include "classes/SLightManager.hpp"

/* Wrappers */
#include "wrappers/camera.hpp"
#include "wrappers/game.hpp"
#include "wrappers/lua.hpp"
#include "wrappers/models.hpp"
#include "wrappers/entity.hpp"
#include "wrappers/radio.hpp"
#include "wrappers/config.hpp"

#ifdef MAFIA_SDK_IMPLEMENTATION

#define _SCL_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

m2sdk_callback g_gamemodule_callback = nullptr;

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


// =======================================================================//
// !
// ! General mod implementation part
// !
// =======================================================================//

std::unordered_map<m2sdk_event_id, m2sdk_callback_event> g_events;

void M2::AttachHandler(m2sdk_event_id id, m2sdk_callback_event callback) {
    g_events[id] = callback;
}

void M2::DetachHandler(m2sdk_event_id id) {
    g_events[id] = nullptr;
}

void M2::TriggerHandler(m2sdk_event_id id, m2sdk_event *data) {
    auto pair = g_events.find(id);
    if (pair != g_events.end() && pair->second) {
        pair->second(data);
    }
}

extern void m2sdk_preinit();

void M2::Initialize(m2sdk_callback callback) {
    g_gamemodule_callback = callback;
    m2sdk_preinit();

    Mem::Initialize();

    // Hooking game module registering
    Mem::Hooks::InstallJmpPatch(0x4F2F05, (DWORD)GameModuleInstall, 5);

    // vehicle hooks
    GameStartDrive__Return = Mem::Hooks::InstallNotDumbJMP(0x043B305, (M2_Address)GameStartDriveHook__1);
    GameStartDrive_2__Return = Mem::Hooks::InstallNotDumbJMP(0x43B394, (M2_Address)GameStartDriveHook__2);
    GameStartDrive_3__Return = Mem::Hooks::InstallNotDumbJMP(0x437940, (M2_Address)GameStartDriveHook__3);
    GameEndDrive__Return = Mem::Hooks::InstallNotDumbJMP(0x43BAAD, (M2_Address)GameEndDriveHook);

    // Crash fix on C_Frame::Release
    Mem::Hooks::InstallJmpPatch(0x14E5BC0, (DWORD)FrameReleaseFix);
    Mem::Hooks::InstallJmpPatch(0x12F0DB0, (DWORD)FrameReleaseFix2);

    //_CHuman2__AddCommand = (DWORD)Mem::Hooks::InstallNotDumbJMP(0x94D400, (DWORD)CHuman2__AddCommand, 5);
    __LoadCityPart = (DWORD)Mem::Hooks::InstallNotDumbJMP(0x4743C0, (DWORD)LoadCityPartsHook, 5);

    // Entity Messages hooks
    onReceiveMessage = (CScriptEntity__RecvMessage_t) Mem::Hooks::InstallJmpPatch(0x117BCA0, (DWORD)OnReceiveMessageHook);

    // Player input hook
    CPlayer2__UpdateInput__Return = Mem::Hooks::InstallNotDumbJMP(0x43BD42, (M2_Address)CPlayer2__UpdateInput);

    // noop the CreateMutex, allow to run multiple instances
    Mem::Hooks::InstallJmpPatch(0x00401B89, 0x00401C16);

    // Always use vec3
    *(BYTE *)0x09513EB = 0x75;
    *(BYTE *)0x0950D61 = 0x75;

    // Disable game controlling engine state and radio
    //Mem::Hooks::InstallJmpPatch(0x956362, 0x9563B6); // When leaving car
    //Mem::Hooks::InstallJmpPatch(0x95621A, 0x956333); // When entering car

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

    // Disable loading screen
    Mem::Utilites::PatchAddress(0x08CA820, 0xC300B0); // mov al, 0; retn

    // Disable DLC loadings (NONO, WE NEED DLCs !)
    //Mem::Utilites::PatchAddress(0x11A62C0, 0xC300B0); // mov al, 0; retn
    InitializeSDKHandlers();
}

void M2::InitializeSDKHandlers()
{
    M2::C_Door_Hooks::HookSolveContact([&](C_Door *instance, S_ContactEventInfo const& ev, E_DoorContactType contactType) {
        //instance->Lock();
    });

    M2::C_CarActionOpenHood_Hooks::HookTestAction([&](C_Car * car) {
        m2sdk_event event = { 0 }; {
            event.arg1 = (void *)car;
        }

        M2::TriggerHandler(M2_EVENT_CAR_HOOD_OPEN_REQUEST, &event);
        return (bool)event.arg5;
    });

    M2::C_CarActionCloseHood_Hooks::HookTestAction([&](C_Car * car) {
        m2sdk_event event = { 0 }; {
            event.arg1 = (void *)car;
        }

        M2::TriggerHandler(M2_EVENT_CAR_HOOD_CLOSE_REQUEST, &event);
        return (bool)event.arg5;
    });

    M2::C_CarActionOpenTrunk_Hooks::HookTestAction([&](C_Car * car) {
        m2sdk_event event = { 0 }; {
            event.arg1 = (void *)car;
        }

        M2::TriggerHandler(M2_EVENT_CAR_TRUNK_OPEN_REQUEST, &event);
        return (bool)event.arg5;
    });

    M2::C_CarActionCloseTrunk_Hooks::HookTestAction([&](C_Car * car) {
        m2sdk_event event = { 0 }; {
            event.arg1 = (void *)car;
        }

        M2::TriggerHandler(M2_EVENT_CAR_TRUNK_CLOSE_REQUEST, &event);
        return (bool)event.arg5;
    });

    M2::C_CarActionEnter_Hooks::HookTestAction([&](C_Car * car) {
        return true;
    });

    M2::C_CarActionBreakIn_Hooks::HookTestAction([&](C_Car * car) {
        return true;
    });

    M2::C_CarActionTankFuel_Hooks::HookTestAction([&](C_Car * car) {
        m2sdk_event event = { 0 }; {
            event.arg1 = (void *)car;
        }

        M2::TriggerHandler(M2_EVENT_CAR_FUELTANK_REQUEST, &event);
        return (bool)event.arg5;
    });

    M2::C_CarActionThrowFrom_Hooks::HookTestAction([&](C_Car * car) {
        return true;
    });

    M2::C_Human2CarWrapper_Hooks::HookIsFreeToGetIn([&](C_Car * car) {
        m2sdk_event event = { 0 }; {
            event.arg1 = (void *)car;
        }

        M2::TriggerHandler(M2_EVENT_CAR_ENTER_REQUEST, &event);
        return (bool)event.arg5;
    });

    M2::C_Player2_Hooks::HookEnterCar([&](C_Player2 *player, C_Actor *car, char seat) {
        m2sdk_event event = { 0 }; {
            event.arg1 = (void *)player;
            event.arg2 = (void *)car;
            event.arg3 = (void *)seat;
        }

        M2::TriggerHandler(M2_EVENT_CAR_ENTER, &event);
    });

    M2::C_Human2_Hooks::HookSetupDeath([&](C_Human2 *human, C_EntityMessageDamage *message) {
        if (human == reinterpret_cast<C_Human2*>(C_Game::Get()->GetLocalPed())) {
            m2sdk_log("The player just died\n");
        }
        else {
            m2sdk_log("An human just died\n");
        }
    });

    M2::C_Human2_Hooks::HookDoDamage([&](C_Human2 *human, C_EntityMessageDamage *message) {
        printf("damage lol\n");
    });
}

void M2::Free() {}

#endif // MAFIA_SDK_IMPLEMENTATION

#endif // M2SDK_H
