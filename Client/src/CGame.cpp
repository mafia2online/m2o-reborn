/** @file CGame.cpp
*  @brief Beep boop
*
*
*  @author MyU (myudev0@gmail.com)
*/

#include <stdafx.h>
#include <CCore.h>
#include <Shared\Client\CPatcher.h>
#include <CGame.h>
#include <SteamDRMPatch.h>
#include <FilePatcher.h>

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

	Patch();

	return true;
}


#include <Libraries\Game\include\CHud.hpp>
#include <Libraries\Game\include\CSDSManager.hpp>


const char * szStreamMapLine = "";
DWORD C_SDSManager__ActivateStreamMapLine_JMP = 0x5CFCD7;
DWORD C_SDSManager__ActivateStreamMapLine_END = 0x5CFFC1;

void meh()
{
	if (strcmp(szStreamMapLine, "free_area_state") == 0) // freeraid_main
	{
		M2::C_SDSLoadingTable *hud = M2::C_SDSLoadingTable::Get();

		CCore::Instance().GetLogger().Writeln("SDS::ActivateStreamMapLine Activating");
		hud->ActivateStreamMapLine("free_joe_load");
		hud->ActivateStreamMapLine("free_summer_load");
	}
	else if (strcmp(szStreamMapLine, "free_summer_load") == 0)
	{
		M2::C_Hud::Get()->FaderFadeIn(1000);
	}
}
DWORD _this_ebx;
DWORD _ecx;
void __declspec (naked) HOOK_C_SDSManager__ActivateStreamMapLine(void)
{
	_asm push ebp;
	_asm mov ebp, esp;
	_asm mov eax, [ebp + 4];
	_asm mov _this_ebx, eax;
	_asm mov eax, [ebp + 12];
	_asm mov szStreamMapLine, eax;
	_asm mov _ecx, ecx;
	_asm pop ebp;
	_asm pushad;

	meh();
	CCore::Instance().GetLogger().Writeln("SDS::ActivateStreamMapLine Requesting: '%s' (%x)", szStreamMapLine, _ecx);

	_asm popad;
	_asm sub esp, 2Ch;
	_asm push ebx;
	_asm push ebp;
	_asm push esi;
	_asm push edi;
	_asm jmp C_SDSManager__ActivateStreamMapLine_JMP;
}

void CGame::Patch()
{
	// Disable loading screen
	Mem::Utilites::PatchAddress(0x08CA820, 0xC300B0); // mov al, 0; retn

	// Disable DLC loadings
	Mem::Utilites::PatchAddress(0x11A62C0, 0xC300B0); // mov al, 0; retn


	Mem::Hooks::InstallJmpPatch(0x5CFCD0, (DWORD)HOOK_C_SDSManager__ActivateStreamMapLine);
}