#define _SCL_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#define MAFIA_SDK_IMPLEMENTATION

#include <unordered_map>
#include <assert.h>

#include "m2sdk.h"

#define m2sdk_assert assert
m2sdk_callback g_gamemodule_callback = nullptr;

// include source code parts
#include "temp.hpp"
#include "memory.hpp"
#include "entity.hpp"
#include "gamehooks.hpp"
#include "sdp.hpp"

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

void M2::Initialize(m2sdk_callback callback) {
    g_gamemodule_callback = callback;

    Mem::Initialize();
    sdp_install(); // steam stuff

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
