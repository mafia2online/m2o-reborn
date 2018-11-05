#include "../../ue.h"
#include "C_Actor.h"
#include "I_Human2.h"
#include "C_EntityMessageDamage.h"

class C_Human2 :
	public C_Actor // 49
{
public:
	virtual void TickPrePhysics(float) = 0;												// Index: 50
	virtual void TickPostPhysics(float) = 0;
	virtual void unk_Function52() = 0;
	virtual void unk_Function53() = 0;
	virtual void unk_Function54() = 0;
	virtual void ChangeModelComplete(ue::sys::core::C_Frame*, bool) = 0;				// Index: 55
	virtual void ChangeModel(ue::sys::core::C_Frame*, int, bool, bool) = 0;
	virtual void SetBurnedModel(ue::sys::core::C_Frame*) = 0;
	virtual void RecoverFromBurnedModel() = 0;
	virtual void ForceUpdateVisual() = 0;
	virtual void GetModelNameByShopID(int) = 0;											// Index: 60
	virtual void GetCurrentModelShopID() = 0;
	virtual void EnableAnimFPSLod(bool) = 0;
	virtual void AnimFPSLodEnabled() = 0;
	virtual void GetPosRequest(I_Human2::E_PosRequestType) = 0;
	virtual void GetDirRequest(I_Human2::E_DirRequestType) = 0;							// Index: 65
	virtual void GetTMRequest(I_Human2::E_TMRequestType, ue::sys::math::C_Matrix&) = 0;
	virtual void GetVelocity() = 0;
	virtual void IsInCover() = 0;
	virtual void IsFighting() = 0;
	virtual void IsDraggingBodyV() = 0;													// Index: 70
	virtual void GetRigidBody() = 0;
	virtual void Spawn() = 0;
	virtual void unk_Function73();
	virtual void SetTransparency(float) = 0;
	virtual void GetTransparency() = 0;													// Index: 75
	virtual void SetTransparencyTarget(float) = 0;
	virtual void SetDirtBlend(float) = 0;
	virtual void CustomRequest(I_Human2::E_CustomRequest) = 0;
	virtual void SetColor(uint32_t) = 0;
	virtual void GetColor() = 0;														// Index: 80
	virtual void DoDamage(C_EntityMessageDamage*) = 0;
	virtual void AreControlsLocked() = 0;
	virtual void IsProcessedByRender() = 0;
	virtual void GetUnderwateDetectionEnabled() = 0;
	virtual void InitColours() = 0;														// Index: 85
	virtual void GetDuplicatedMaterialGUID(const ue::sys::utils::C_GUID&) = 0;
	virtual void LinkObject(const uint64_t&, ue::sys::core::C_Frame*) = 0;
	virtual void UnlinkObject(ue::sys::core::C_Frame*) = 0;
	virtual void LockControls(bool) = 0;
	virtual void IsControlLockFinished() = 0;											// Index: 90
	virtual void SetPlayerCtrlStyle(const char*) = 0;
	virtual void GetPlayerCtrlStyle() = 0;
	virtual void IsPlayerMovement() = 0;
	virtual void CanPlayerTakeCover() = 0;
	virtual void SetFightAbility(bool, int) = 0;										// Index: 95
	virtual void SetFightControlStyle(int) = 0;
	virtual void SetFightHint(bool, int) = 0;
	virtual void SetFightGrabTimeScale(bool) = 0;
	virtual void SetForcedDropWeapon(bool) = 0;
	virtual void GetGuiController() = 0;												// Index: 100
	virtual void IsCrouch() = 0;
	virtual void IsCrouchOrDrag() = 0;
	virtual void ProcessNotification(short, const ue::sys::utils::C_GUID&) = 0;
	virtual void UpdateAnimSetFromWeapon(int) = 0;
	virtual void ProcessNotificationCamera(short) = 0;									// Index: 105
	virtual void ProcessNotificationFFX(short) = 0;
	virtual void CoatEnabled() = 0;
};