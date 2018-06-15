/** @file CHuman2.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

#include "CActor.hpp"
#include "CEntity.hpp"
#include "CHumanInventory.hpp"
#include "CHumanScript.hpp"
#include "CHumanWeaponController.hpp"
#include "CCar.hpp"

#include "CScene.hpp"

namespace M2
{
    enum E_Command : int
    {
        COMMAND_STAND = 0,
        COMMAND_MOVEDIR = 1,
        COMMAND_MOVETO = 2,
        COMMAND_OBSTACLE = 4,
        COMMAND_SPECIAL = 5,
        COMMAND_COVER = 6,
        COMMAND_ANIMPLAY = 7,
        COMMAND_ACTION = 8,
        COMMAND_CAR = 9,
        COMMAND_FIGHT = 10,
        COMMAND_DEATH = 11,
        COMMAND_USEOBJECT = 12,
        COMMAND_ANIMPLAYEFFECT = 13,
        COMMAND_ADDITACTION = 14
    };

    struct S_HumanCommandMoveDir
    {
        float x;
        float y;
        float z;
    };

    class unknow
    {
    public:
        pad(unknow, pad0, 0x1C);                        //0000 - 001C
        C_Scene             *m_pCurrentScene;
    };
	class ICHuman2
	{
	public:
        pad(ICHuman2, pad3, 0x18);                      //0000 - 0018
        unknow              *m_pUnk;                    //0018 - 001C
		pad(ICHuman2, pad0, 0x48);						//001C - 0064
		C_Car				*m_pCurrentCar;				//0064 - 0068
		pad(ICHuman2, pad1, 0x38);
		C_HumanInventory	*m_pInventory;				//00A0 - 00A4
		C_HumanScript		*m_pScript;					//00A4 - 00A8
		pad(ICHuman2, pad2, 0x8);						//00A8 - 00B0
		C_HumanWeaponController *m_pWeaponController;	//00B0 - 00B4
	};

	class C_Human2 : public ICHuman2
	{
	public:
		C_HumanInventory	*GetInventory() { return reinterpret_cast<ICHuman2 *>(this)->m_pInventory; }
		C_HumanScript	*GetScript() { return reinterpret_cast<ICHuman2 *>(this)->m_pScript; }
		C_HumanWeaponController	*GetWeaponController() { return reinterpret_cast<ICHuman2 *>(this)->m_pWeaponController; }

        void AddCommand(E_Command cmdtype, void *cmd)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x94D400, this, cmdtype, cmd);
        }

        void ChangeModel(C_Frame *model, int unk, bool unk2, bool unk3)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x94F7E0, this, model, unk, unk2, unk3);
        }

        void CleanCommands()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x94D6B0, this, 1, 1);
        }

        void *GetCurrentMoveCommand(void *unk)
        {
            return Mem::InvokeFunction<Mem::call_this, void *>(0x94D540, this, unk);
        }

        vec3_t GetDir()
        {
            vec3_t direction;
            Mem::InvokeFunction<Mem::call_this, int>(0x93A0E0, this, &direction);
            return direction;
        }

        vec3_t GetPos()
        {
            vec3_t position;
            Mem::InvokeFunction<Mem::call_this, int>(0x919E30, this, &position);
            return position;
        }

        quat_t GetRot()
        {
            quat_t rotation;
            Mem::InvokeFunction<Mem::call_this, int>(0x91A310, this, &rotation);
            return rotation;
        }

        double GetScale()
        {
            return Mem::InvokeFunction<Mem::call_this, double>(0x9382E0, this);
        }

        void SetColor(int color)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x9389B0, this, color);
        }

        void SetDir(vec3_t dir)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x94E9C0, this, &dir);
        }

        void SetPos(vec3_t pos)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x94E960, this, &pos);
        }

        void SetRot(quat_t rot)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x94EA80, this, &rot);
        }
	};
};
