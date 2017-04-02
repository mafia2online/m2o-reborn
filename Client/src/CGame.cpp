/** @file CGame.cpp
*  @brief Beep boop
*
*
*  @author MyU (myudev0@gmail.com)
*/

#include <stdafx.h>
#include <CCore.h>
#include <Shared\Client\CPatcher.h>
#include <SteamDRMPatch.h>
#include <FilePatcher.h>
#include <GameHooks.h>
#include "CGame.h"

#include <Shared\Math\Math.hpp>

#include <Libraries\Game\include\CGfxEnvironmentEffects.hpp>
#include <Libraries\Game\include\CEntityFactory.hpp>

#include <Libraries\Game\include\CGameGuiModule.hpp>
#include <Libraries\Game\include\CGame.hpp>
#include <Libraries\Game\include\CSDSManager.hpp>
#include <Libraries\Game\include\CCameraModule.hpp>
#include <Libraries\Game\include\CCamera.hpp>

#include <Libraries\Game\include\CEntity.hpp>
#include <Libraries\Game\include\CHuman2.hpp>

#include <CGraphicsManager.h>

#include <CDebugConsole.h>

bool CGame::Initialize()
{
	m_base = Mem::GetModuleAddress(nullptr);
	Mem::Initialize();

	HirePreHookers();


	return true;
}

bool CGame::HirePreHookers()
{
	CCore::Instance().GetLogger().Writeln("<CGame::HirePreHookers> Current Thread ID: %x", GetCurrentThreadId());

	FilePatcher::Initialize();
	SteamDRMPatch::Apply();
	GameHooks::Install();

	Patch();

	return true;
}


void CGame::Patch()
{
	// Disable loading screen
	Mem::Utilites::PatchAddress(0x08CA820, 0xC300B0); // mov al, 0; retn

													  // Disable DLC loadings
													  //Mem::Utilites::PatchAddress(0x11A62C0, 0xC300B0); // mov al, 0; retn
}

void CGame::OnGameInit()
{
	CCore::Instance().GetLogger().Writeln("GameInit \\(^o^)/ (Thread: %x)", GetCurrentThreadId());

	GameHooks::InstallLate();

	M2::C_GameGuiModule::Get()->FaderFadeIn(1); // therotically we shouldn't call it here but because it's a sync object it's fine itll work but the local player isn't created just yet.
}

DWORD dwLocalPlayer = false;

float ztime = 0;

M2::C_Human2 *ent = nullptr;

void CGame::OnGameLoop()
{
	//CCore::Instance().GetLogger().Writeln("GameLooopy \\(^o^)/ (Thread: %x)", GetCurrentThreadId());

	// testing dont complain bitch
	if (!dwLocalPlayer)
	{
		dwLocalPlayer = M2::C_Game::Get()->GetEntityFromIndex(0);

		if (dwLocalPlayer)
		{
			if (M2::C_SDSLoadingTable::Get())
			{
				M2::C_SDSLoadingTable::Get()->ActivateStreamMapLine("free_joe_load");
				M2::C_SDSLoadingTable::Get()->ActivateStreamMapLine("free_summer_load");

				M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetDayTemplate("DTFreerideNight");

				CommandProcessor::RegisterCommand("lock",
					[](const std::string& params)->void
				{
					int lock = atoi(params.c_str());
					Mem::InvokeFunction<Mem::call_this, void>(0x438230, dwLocalPlayer, lock);
					CCore::Instance().GetLogger().Writeln("Controls %s!", lock ? ("locked") : ("unlocked"));
				});

				CommandProcessor::RegisterCommand("ent",
					[](const std::string& params)->void
				{
					ent = M2::C_EntityFactory::Get()->CreateEntity<M2::C_Human2>(M2::EntityTypes::Entity_Human);
					DWORD dwPlayerModelMan = *(DWORD*)(0x1ABFE5C);
					DWORD dwPlayerModel = *(DWORD*)(dwPlayerModelMan + 0x14);

					if (ent)
					{
						DWORD coreInstance = *(DWORD*)(0x1AC2778);
						void *own_model = Mem::InvokeFunction<Mem::call_this, void*>((*(Address*)(*(DWORD*)coreInstance + 0x94)), coreInstance, 2);
						Mem::InvokeFunction<Mem::call_this, void>(0x14EC8F0, own_model, dwPlayerModel);

						//14BA350
						Mem::InvokeFunction<Mem::call_this, void>(0x14BA350, own_model, "lawl");
						Mem::InvokeFunction<Mem::call_this, void>(0x14BA3D0, own_model, 2);

						Mem::InvokeFunction<Mem::call_this, void*>((*(Address*)(*(DWORD*)ent + 0xB0)), ent, own_model);

						ent->Setup();

						// set flagsF
						DWORD flags = *(DWORD *)(ent + 32) & 0xFFFFFBF | 0x4800;
						*(DWORD *)(ent + 32) = flags;

						if (flags & 0x20)
							CCore::Instance().GetLogger().Writeln("Flags set sucessfully!");

						ent->Activate();

						if(ent->IsActive())
							CCore::Instance().GetLogger().Writeln("Entity active !");

						Vector3 pos;
						Mem::InvokeFunction<Mem::call_this, void>(((M2::C_Entity*)dwLocalPlayer)->m_pVFTable->GetPosition, dwLocalPlayer, &pos);
						Mem::InvokeFunction<Mem::call_this, void>(ent->m_pVFTable->SetPosition, ent, &pos);
					}

					CCore::Instance().GetLogger().Writeln("Created at %x!", ent);
				});

				CommandProcessor::RegisterCommand("time",
					[=](const std::string& params)->void
				{
										float time = atof(params.c_str());
					M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetTime(time);
					CCore::Instance().GetLogger().Writeln("Set time to %f", time);
				});
			}

			M2::C_Entity *entity = reinterpret_cast<M2::C_Entity*>(dwLocalPlayer);
			Mem::InvokeFunction<Mem::call_this, void>(entity->m_pVFTable->SetPosition, entity, &Vector3(-1334.6199, 1041.8342, -18.4722));

			CommandProcessor::RegisterCommand("spawn",
				[=](const std::string& params)->void
			{
					M2::C_Entity *entity = reinterpret_cast<M2::C_Entity*>(dwLocalPlayer);
				Mem::InvokeFunction<Mem::call_this, void>(entity->m_pVFTable->SetPosition, entity, &Vector3(-1334.6199, 1041.8342, -18.4722));
			});

			CommandProcessor::RegisterCommand("shake", 
				[=](const std::string& params)->void
			{
				M2::ShakeCommandData data;
				data.speed = 50;
				data.strength = 5;
				data.duration = 3;
				M2::C_CameraModule::Get()->GetCamera(1)->BroadcastCommand(M2::CAMCOMMAND_SHAKE, &data, 0);
			});

			CommandProcessor::RegisterCommand("fpv",
				[=](const std::string& params)->void
			{
				M2::C_Entity *entity = reinterpret_cast<M2::C_Entity*>(dwLocalPlayer);
				if (M2::C_CameraModule::Get()->GetCamera(1)->ModeGetActiveTypeTop() != M2::CAMMODE_FPV)
					M2::C_CameraModule::Get()->GetCamera(1)->ModeChange(M2::CAMMODE_FPV, entity, true, true);
				else
					M2::C_CameraModule::Get()->GetCamera(1)->ModePop(1, 1);
			});
		}
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x1)
	{
		ztime -= 0.1f;
		if (ztime < 0)
			ztime = 0;
		M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetTime(ztime);
		CCore::Instance().GetLogger().Writeln("Time shift!");
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x1)
	{
		ztime += 0.1f;
		if (ztime > 1.0f)
			ztime = 1.0f;
		M2::C_GfxEnvironmentEffects::Get()->GetWeatherManager()->SetTime(ztime);
		CCore::Instance().GetLogger().Writeln("Time shift!");
	}
}

bool CGame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
		MSG msg;
	msg.hwnd = hWnd;
	msg.lParam = lParam;
	msg.wParam = wParam;
	msg.message = uMsg;

	if (uMsg == WM_KEYDOWN)
	{
		if (wParam == VK_RETURN && !CDebugConsole::Instance().HasFocus())
		{
			CDebugConsole::Instance().Focus();
			return true;
		}
	}

	if (CCore::Instance().GetGraphics().GetGwenManager()->GetInput().ProcessMessage(msg))
		return true;

	return false; // not handled
}
