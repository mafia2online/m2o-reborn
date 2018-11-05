#include "../../ue.h"
#include "C_Entity.h"

class C_EntityPos :
	public C_Entity
{
public:
	virtual void SetPos(const ue::sys::math::C_Vector&);
	virtual void SetDir(const ue::sys::math::C_Vector&);
	virtual void SetRot(const ue::sys::math::C_Quat&);
	virtual void SetScale(float);
	virtual void GetPos(ue::sys::math::C_Vector*) = 0;					// Index: 35
	virtual void GetDir(ue::sys::math::C_Vector*) = 0;
	virtual void GetRot(ue::sys::math::C_Quat*) = 0;
	virtual float GetScale() = 0;
	virtual void unk_Function39() = 0;
	virtual void unk_Function40() = 0;
	virtual void GameSavePRS(ue::sys::utils::C_BitStream*) = 0;
	virtual void GameLoadPRS(ue::sys::utils::C_BitStream*) = 0;
};