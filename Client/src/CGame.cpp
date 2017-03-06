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

#include <Libraries\Game\include\CGameGuiModule.hpp>
#include <Libraries\Game\include\CGame.hpp>
#include <Libraries\Game\include\CSDSManager.hpp>

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

/* will move later just for testing.. */
class M2EntityVFTable
{
public:
	pad(M2EntityVFTable, pad0, 0x40);
	DWORD IsVisibleFromCamera;
	pad(M2EntityVFTable, pad1, 0x38);
	DWORD SetPosition;
};

class M2Entity
{
public:
	M2EntityVFTable	* m_pVFTable;
};

float ztime = 0;
#include <Libraries\Game\include\CGfxEnvironmentEffects.hpp>
#include <Libraries\Game\include\CEntityFactory.hpp>
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
					int c = atoi(params.c_str());
					M2Entity *ent = Mem::InvokeFunction<Mem::call_std, M2Entity*>(0x9A45E0, c);
					if (ent)
					{
						Mem::InvokeFunction<Mem::call_this, void>(0x1192170, ent); // activate
						Mem::InvokeFunction<Mem::call_this, void>(ent->m_pVFTable->SetPosition, ent, &Vector3(-1334.6199, 1041.8342, -18.4722));
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

			M2Entity *entity = reinterpret_cast<M2Entity*>(dwLocalPlayer);
			Mem::InvokeFunction<Mem::call_this, void>(entity->m_pVFTable->SetPosition, entity, &Vector3(-1334.6199, 1041.8342, -18.4722));

			CommandProcessor::RegisterCommand("spawn",
				[=](const std::string& params)->void
			{
				M2Entity *entity = reinterpret_cast<M2Entity*>(dwLocalPlayer);
				Mem::InvokeFunction<Mem::call_this, void>(entity->m_pVFTable->SetPosition, entity, &Vector3(-1334.6199, 1041.8342, -18.4722));
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
