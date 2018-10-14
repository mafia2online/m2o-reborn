/** @file CHuman2.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CActor.hpp"
#include "CEntity.hpp"
#include "CHumanInventory.hpp"
#include "CHumanScript.hpp"
#include "CHumanWeaponController.hpp"
#include "CHumanHeadController.hpp"
#include "CCar.hpp"
#include "CScene.hpp"

#include "CHuman2Commands.hpp"

#include <functional>

namespace M2
{
    enum E_Command : int
    {
        COMMAND_STAND = 0,//0x20
        COMMAND_MOVEDIR = 1,//0x58
        COMMAND_MOVETO = 2,//0x2C
        COMMAND_OBSTACLE = 4,//0x40
        COMMAND_SPECIAL = 5,//0x20
        COMMAND_COVER = 6,//0x78
        COMMAND_ANIMPLAY = 7,//0x94
        COMMAND_ACTION = 8,//0x68
        COMMAND_CAR = 9,//0x34
        COMMAND_FIGHT = 10,//0x78
        COMMAND_DEATH = 11,//0x24
        COMMAND_USEOBJECT = 12,//0x2C
        COMMAND_ANIMPLAYEFFECT = 13,//0x68
        COMMAND_ADDITACTION = 14//0x20
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
        C_AIController      *m_pAIController;           //00A8 - 00AC
        pad(ICHuman2, pad2, 0x4);
		C_HumanWeaponController *m_pWeaponController;	//00B0 - 00B4
        C_HumanHeadController   *m_pHeadController;     //00B4 - 00B8
        pad(ICHuman2, pad4, 0x7C);                      //00B8 - 0134
        C_CommandDescription     m_aCommandsArray[8];   //0134 - 0138
        pad(ICHuman2, pad5, 0x4);                       //0138 - 0178
        uint32_t                m_aCommandsQueue[5];    //0178 - 018C
        int32_t                 m_iNextCommand;         //018C - 0190
	};

	class C_Human2 : public ICHuman2
	{
	public:
		C_HumanInventory	*GetInventory() { return reinterpret_cast<ICHuman2 *>(this)->m_pInventory; }
		C_HumanScript	*GetScript() { return reinterpret_cast<ICHuman2 *>(this)->m_pScript; }
		C_HumanWeaponController	*GetWeaponController() { return reinterpret_cast<ICHuman2 *>(this)->m_pWeaponController; }

        void AddCommand(E_Command cmdtype, C_Command *cmd)
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

        void CleanMoveCommands()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x94D8D0, this);
        }

        void *GetCurrentMoveCommand(void *unk)
        {
            return Mem::InvokeFunction<Mem::call_this, void *>(0x94D540, this, &unk);
        }

        Vector3 GetDir()
        {
            Vector3 direction;
            Mem::InvokeFunction<Mem::call_this, int>(0x93A0E0, this, &direction);
            return direction;
        }

        Vector3 GetPos()
        {
            Vector3 position;
            Mem::InvokeFunction<Mem::call_this, int>(0x919E30, this, &position);
            return position;
        }

        Quaternion GetRot()
        {
            Quaternion rotation;
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

        void SetDir(Vector3 dir)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x94E9C0, this, &dir);
        }

        void SetPos(Vector3 pos)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x94E960, this, &pos);
        }

        void SetRot(Quaternion rot)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x94EA80, this, &rot);
        }
	};

#ifdef MAFIA_SDK_IMPLEMENTATION
    namespace C_Human2_Hooks
    {
        void HookSetupDeath(std::function<void(M2::C_Human2* human, M2::C_EntityMessageDamage* message)>);
        void HookDoDamage(std::function<void(M2::C_Human2 *human, M2::C_EntityMessageDamage *message)>);

        namespace FunctionPointers
        {
            std::function<void(M2::C_Human2* human, M2::C_EntityMessageDamage* message)> setupDeath;
            std::function<void(M2::C_Human2 *human, M2::C_EntityMessageDamage *message)> doDamage;
        };

        namespace Functions
        {
            inline void SetupDeath(M2::C_Human2* human, M2::C_EntityMessageDamage* message)
            {
                if (FunctionPointers::setupDeath != nullptr) {
                    FunctionPointers::setupDeath(human, message);
                }
            }

            inline void DoDamage(M2::C_Human2* human, M2::C_EntityMessageDamage* message)
            {
                if (FunctionPointers::doDamage != nullptr) {
                    FunctionPointers::doDamage(human, message);
                }
            }
        };

        namespace NakedFunctions
        {
            DWORD MineDeathHook_JumpBack = 0x00990CFF;
            DWORD _CHuman2__SetupDeath = 0x0098C160;
            void __declspec(naked) CHuman2__SetupDeath_Hook()
            {
                __asm {
                    pushad
                    push esi
                    push ebp
                    call Functions::SetupDeath
                    add esp, 0x8
                    popad

                    push    esi
                    mov     ecx, ebp
                    call    _CHuman2__SetupDeath

                    jmp MineDeathHook_JumpBack
                }
            }

            DWORD _CHuman2__DoDamage = 0x09907D0;
            DWORD _DoDamage__JumpBack = 0x042FC6F;
            __declspec(naked) void CHuman2__DoDamage__Hook()
            {
                __asm
                {
                    pushad;
                    push esi;
                    push edi;
                    call Functions::DoDamage;
                    add esp, 0x8;
                    popad;

                    push edi;
                    mov ecx, esi;
                    call _CHuman2__DoDamage;

                    jmp _DoDamage__JumpBack;
                }
            }
        };

        void HookSetupDeath(std::function<void(M2::C_Human2* human, M2::C_EntityMessageDamage* message)> ptr)
        {
            FunctionPointers::setupDeath = ptr;
            Mem::Hooks::InstallJmpPatch(0x00990CF7, (DWORD)NakedFunctions::CHuman2__SetupDeath_Hook);
        }

        void HookDoDamage(std::function<void(M2::C_Human2* human, M2::C_EntityMessageDamage* message)> ptr)
        {
            FunctionPointers::doDamage = ptr;
            Mem::Hooks::InstallJmpPatch(0x042FC63, (DWORD)NakedFunctions::CHuman2__DoDamage__Hook);
        }
    };
#endif
};
