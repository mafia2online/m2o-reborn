#include <string>
#include <vector>

#include <windows.h>
#include <detours.h>

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
#include "classes/CApplication.hpp"
#include "classes/ccamera.hpp"
#include "classes/CGameCamera.hpp"
#include "classes/CCar.hpp"
#include "classes/CCarManager.hpp"
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
#include "classes/CGfxEffects.hpp"
#include "classes/CGfxEnvironmentEffects.hpp"
#include "classes/CGuiGame.hpp"
#include "classes/CHints.hpp"
#include "classes/CHud.hpp"
#include "classes/CHudScriptW.hpp"
#include "classes/chuman2.hpp"
#include "classes/CHumanHeadController.hpp"
#include "classes/CHumanInventory.hpp"
#include "classes/CHumanScript.hpp"
#include "classes/CHumanWeaponController.hpp"
#include "classes/cmafiaframework.hpp"
#include "classes/CModel.hpp"
#include "classes/CMusic.hpp"
#include "classes/CNavigation.hpp"
#include "classes/CPhoneBook.hpp"
#include "classes/CPlayer2.hpp"
#include "classes/CPlayerModelManager.hpp"
#include "classes/CPlayerRadio.hpp"
#include "classes/CPoliceWanted.hpp"
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
#include "classes/CSpeedometer.hpp"
#include "classes/CSyncObject.hpp"
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

/* Wrappers */
#include "wrappers/camera.hpp"
#include "wrappers/entity.hpp"
#include "wrappers/game.hpp"
#include "wrappers/lua.hpp"
#include "wrappers/models.hpp"
#include "wrappers/config.hpp"

#endif // M2SDK_H
