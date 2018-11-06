#include "../../ue.h"
#include "C_EntityPos.h"

class C_Actor :
	public C_EntityPos // 42
{
public:
	virtual void GetCameraPoint() = 0;
	virtual void SetFrame(ue::sys::core::C_Frame*) = 0;
	virtual void SetOwner(C_Entity*) = 0;					// Index: 45
	virtual void unk_Function46() = 0;
	virtual void unk_Function47() = 0;
	virtual void EnableUnderwaterDetection(bool) = 0;
	virtual void GetUnderwaterStatus() = 0;
};