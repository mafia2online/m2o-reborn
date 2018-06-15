/*

Copyright 2017-2018 Enguerrand Pinot, Vladyslav Hrytsenko

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

/* Utils */
#include "utils/Matrix.hpp"
#include "utils/TArray.hpp"

/* Classes */

#include "classes/CActor.hpp"
#include "classes/CActorVehicle.hpp"
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
#include "classes/CHints.hpp"
#include "classes/CHud.hpp"
#include "classes/CHudScriptW.h"
#include "classes/chuman2.hpp"
#include "classes/CHumanHeadController.hpp"
#include "classes/CHumanInventory.hpp"
#include "classes/CHumanScript.hpp"
#include "classes/CHumanWeaponController.hpp"
#include "classes/cmafiaframework.hpp"
#include "classes/CModel.hpp"
#include "classes/CMusic.hpp"
#include "classes/CNavigation.hpp"
#include "classes/CommonHeaders.h"
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
#include "classes/CTextDatabase.hpp"
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
#include "wrappers/lua.hpp"
#include "wrappers/models.hpp"
#include "wrappers/config.hpp"
