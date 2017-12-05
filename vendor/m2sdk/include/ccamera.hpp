/** @file CCamera.cpp
*  @brief Game's CCamera wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

#include "CRenderCamera.hpp"
#include "CGameCameraMafia.hpp"

namespace M2
{
	struct ShakeCommandData
	{
		float speed;
		float strength;
		float duration;
	};

    struct SetPosCommandData
    {
        vec3_t start_position;
        vec3_t destination_position;
    };

	enum eCameraCommand
	{
		CAMCOMMAND_LOCKC = 1282368363, //Lock control
		CAMCOMMAND_GOVER = 1381191748, //Start game over sequence
		CAMCOMMAND_SHAKE = 1399349587, //Shake the screen
	};

	enum eCameraMode
	{
        CREATE_FRAME = 118,
		CAMMODE_FPV = 121,
	};

	class ICCamera
	{
	public:
		pad(ICCamera, pad0, 0x12C);								// 0000 - 012C
		C_GameCameraMafia	*m_pGameCamera;						// 012C - 0130

	};

	class C_Camera : public ICCamera
	{
	public:
		int BroadcastCommand(eCameraCommand command, void *data, void *unknow)
		{
			return (Mem::InvokeFunction<Mem::call_this, int>(0x1082590, this, command, data, unknow));
		}

		void EnableBlendMode(int mode, bool enable)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x1083060, this, mode, enable);
		}

		long double GetFOV()
		{
			return this->m_pGameCamera->m_fov * 57.29577791868205;
		}

        vec3_t GetPos() 
        {
            return { *(float*)((DWORD)this + 0x14), *(float*)((DWORD)this + 0x24), *(float*)((DWORD)this + 0x34) };
        }

        vec3_t GetRot() 
        {
            return { *(float*)((DWORD)this + 0x0C), *(float*)((DWORD)this + 0x1C), *(float*)((DWORD)this + 0x2C) };
        }

		float *MapScreenTarget(float *x, float *y)
		{
			return Mem::InvokeFunction<Mem::call_this, float *>(0x107FB30, this, x, y);
		}

		int ModeChange(eCameraMode mode, void *actor, bool unk1, bool unk2)
		{
			return (Mem::InvokeFunction<Mem::call_this, int>(0x1084150, this, mode, actor, unk1, unk2));
		}

		int ModeGetActiveTypeTop()
		{
			return (Mem::InvokeFunction<Mem::call_this, int>(0x1082530, this));
		}

		int ModePop(int unk1, int unk2)
		{
			return (Mem::InvokeFunction<Mem::call_this, int>(0x1082B30, this, unk1, unk2));
		}

        void SetPos(const vec3_t & new_pos) 
        {
            *(float*)((DWORD)this + 0x14) = new_pos.x;
            *(float*)((DWORD)this + 0x24) = new_pos.y;
            *(float*)((DWORD)this + 0x34) = new_pos.z;
        }

        void SetPosition(unsigned long camera_mode, const SetPosCommandData & data)
        {
            __asm
            {
                mov esi, camera_mode
                mov edx, dword ptr ds : [esi]
                mov edx, dword ptr ds : [edx + 48h]

                push 0
                push data
                push 2
                mov ecx, esi
                call edx
            }
        }

        void SetRot(const vec3_t & new_rot) 
        {
            *(float*)((DWORD)this + 0x0C) = new_rot.x;
            *(float*)((DWORD)this + 0x1C) = new_rot.y;
            *(float*)((DWORD)this + 0x2C) = new_rot.z;
        }

        void SetRotation(float x, float y, bool unk)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x1083010, this, x, y, unk);
        }

        int RotationTowards(float x, float y, float z)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0xB55B90, this, x, y, z);
        }
	};
};
