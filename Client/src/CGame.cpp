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

#include <Libraries\Game\include\CHud.hpp>
#include <Libraries\Game\include\CGame.hpp>

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

void CGame::OnGameLoop()
{
	//CCore::Instance().GetLogger().Writeln("GameLooopy \\(^o^)/ (Thread: %x)", GetCurrentThreadId());

	// testing dont complain bitch
	if (!dwLocalPlayer)
	{
		dwLocalPlayer = M2::C_Game::Get()->GetEntityFromIndex(0);
	}

	if (dwLocalPlayer && GetAsyncKeyState(VK_F1) & 0x1)
	{
		M2Entity *entity = reinterpret_cast<M2Entity*>(dwLocalPlayer);
		CCore::Instance().GetLogger().Writeln("Func: %x", entity->m_pVFTable->SetPosition);
		Mem::InvokeFunction<Mem::call_this, void>(entity->m_pVFTable->SetPosition, entity, &Vector3(-1551.560181, -169.915466, -19.672523));


		// educate him
	}
}
