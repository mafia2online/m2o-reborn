#include "../../ue.h"
#include "S_EntityInitProps.h"
#include "C_EntityMessage.h"

/*
 * VTable 1: 0x1909FB8
 * VTable 2: 0x190A038
 */
class C_Entity
{
public:
	virtual ~C_Entity() = 0;												// Index: 0
	virtual int unk_retn5() = 0;
	virtual void Init(const S_EntityInitProps*) = 0;
	virtual void GameInit() = 0;
	virtual void GameDone() = 0;
	virtual void GameRestore() = 0;											// Index: 5
	virtual void OnActivate() = 0;
	virtual void OnDeactivate() = 0;
	virtual bool IsActive() = 0;
	virtual int unk_retn0() = 0;
	virtual void GameSavePrerequisite(ue::sys::utils::C_BitStream*) = 0;	// Index: 10
	virtual void GameSave(ue::sys::utils::C_BitStream*) = 0;
	virtual void GameSaveDependencies(ue::sys::utils::C_BitStream*) = 0;
	virtual void GameLoadPrerequisite(ue::sys::utils::C_BitStream*) = 0;
	virtual void GameLoad(ue::sys::utils::C_BitStream*) = 0;
	virtual void GameLoadDependencies(ue::sys::utils::C_BitStream*) = 0;	// Index: 15	
	virtual void EntityInvalidate(C_Entity*) = 0;
	virtual void AddOnEvent(uint32_t, uint32_t, uint32_t) = 0;
	virtual int unk_retn1() = 0;
	virtual void GetLineIntersection(/* ... */) = 0;
	virtual void HideAreaAction(bool) = 0;									// Index: 20
	virtual void RecvMessage(C_EntityMessage*) = 0;
	virtual void InvalidateRelation(C_Entity*) = 0;
	virtual void GetCsInterface() = 0;
	virtual void GetCreateCsInterface() = 0;
	virtual void unk_Function25() = 0;
	virtual void unk_Function26() = 0;
	virtual void unk_Function27() = 0;
	virtual void unk_Function28() = 0;
	virtual void unk_Function29() = 0;
	virtual bool Update(float) = 0;											// Index: 30
};