#include "../ue.h"
#include "I_EntityListener.h"
#include <vector>

class C_Entity;
class C_Actor;

struct I_EntityList
{
	enum E_EntityEvents
	{};

	virtual ~I_EntityList() = 0;

	virtual void AddEntity(C_Entity*) = 0;
	virtual void RemoveEntity(C_Entity*) = 0;
	virtual void UpdateEntityInNamesTable(C_Entity*, ue::sys::utils::C_HashName, ue::sys::utils::C_HashName) = 0;
	virtual void RemoveEntityFromNamesTable(C_Entity*) = 0;
	virtual void GetEntityCount(void) = 0;
	virtual void GetEntityByIndex(int) = 0;
	virtual void GetEntityByGuid(uint32_t) = 0;
	virtual void GetEntityByName(ue::sys::utils::C_HashName) = 0;
	virtual void GetActorByGuid(uint32_t) = 0;
	virtual void ProcessEventToListeners(C_Entity*, E_EntityEvents) = 0;
	virtual void RegisterListener(I_EntityListener*) = 0;
	virtual void UnregisterListener(I_EntityListener*) = 0;
	virtual void AddActorWithActions(C_Actor*) = 0;
	virtual void RemoveActorWithActions(C_Actor*) = 0;
	virtual void EnumActorsWithActions(const ue::sys::math::C_Sphere&, std::vector<C_Actor*>&) = 0;
	virtual void DbgGetNameConflictEntities(void) = 0;
}