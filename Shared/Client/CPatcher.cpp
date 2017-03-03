#include "CPatcher.h"
#include <Libraries\detours\detours.h>

void * Mem::Hooks::InstallDetourPatchInternal(uint32_t dwAddress, uint32_t dwDetourAddress, BYTE byteType, int iSize)
{
	BYTE * pbyteAddr = (BYTE *)dwAddress;

	// Allocate the trampoline memory
	BYTE * pbyteTrampoline = (BYTE *)malloc(iSize + 5);
	ScopedProtect((Address)pbyteTrampoline, iSize + 5);
	{
		ScopedProtect((Address)pbyteAddr, iSize + 5);

		memcpy(pbyteTrampoline, pbyteAddr, iSize);
		pbyteTrampoline[iSize] = byteType;
		*(void **)(&pbyteTrampoline[iSize + 1]) = (void *)((pbyteAddr + iSize) - (pbyteTrampoline + iSize) - 5);
		pbyteAddr[0] = byteType;
		*(void **)(&pbyteAddr[1]) = (void *)((BYTE *)dwDetourAddress - pbyteAddr - 5);
	}

	return pbyteTrampoline;
}

void Mem::Hooks::UninstallDetourPatchInternal(uint32_t dwAddress, void ** pTrampoline, int iSize)
{
	BYTE * pTramp = (BYTE *)*pTrampoline;
	BYTE * pbyteAddr = (BYTE *)dwAddress;

	{
		ScopedProtect((Address)pbyteAddr, iSize);
		memcpy(pbyteAddr, pTramp, iSize);
	}

	free(pTramp);
	*pTrampoline = pbyteAddr;
}

void * Mem::Hooks::InstallDetourPatch(char * szLibrary, char * szFunction, uint32_t dwFunctionAddress)
{
	return DetourFunction(DetourFindFunction(szLibrary, szFunction), (BYTE *)dwFunctionAddress);
}

void * Mem::Hooks::InstallDetourPatch(uint32_t dwAddress, uint32_t dwFunctionAddress)
{
	return DetourFunction((BYTE *)dwAddress, (BYTE *)dwFunctionAddress);
}

bool Mem::Hooks::UninstallDetourPatch(void * pTrampoline, uint32_t dwFunctionAddress)
{
	return DetourRemove((BYTE *)pTrampoline, (BYTE *)dwFunctionAddress);
}

void Mem::Utilites::PatchAddress(uint32_t dwAddress, BYTE *bPatch, size_t iSize)
{
	ScopedProtect(dwAddress, sizeof(iSize));
	memcpy((void*)dwAddress, bPatch, iSize);
}

void Mem::Utilites::PatchAddress(uint32_t dwAddress, uint32_t dwPatch)
{
	ScopedProtect(dwAddress, sizeof(uint32_t));
	*(uint32_t *)(dwAddress) = dwPatch;
}

void * Mem::Hooks::InstallCallPatch(uint32_t dwAddress, uint32_t dwCallAddress, int iSize)
{
	return InstallDetourPatchInternal(dwAddress, dwCallAddress, CALL, iSize);
}

void * Mem::Hooks::InstallJmpPatch(uint32_t dwAddress, uint32_t dwJmpAddress, int iSize)
{
	return InstallDetourPatchInternal(dwAddress, dwJmpAddress, JMP, iSize);
}

void Mem::Utilites::InstallNopPatch(uint32_t dwAddress, int iSize)
{
	ScopedProtect(dwAddress, iSize);
	memset((void *)dwAddress, NOP, iSize);
}

bool Mem::Utilites::bDataCompare(const unsigned char * pData, const unsigned char * bMask, const char * szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
	{
		if (*szMask == 'x' && *pData != *bMask)
		{
			return false;
		}
	}

	return (*szMask) == 0;
}

unsigned long Mem::Utilites::FindPattern2(uint32_t dwAddress, uint32_t dwLen, unsigned char * bMask, char * szMask)
{
	for (unsigned long i = 0; i < dwLen; i++)
	{
		if (bDataCompare((unsigned char *)(dwAddress + i), bMask, szMask))
		{
			return (unsigned long)(dwAddress + i);
		}
	}

	return 0;
}

unsigned long Mem::Utilites::FindPattern(unsigned char * bMask, char * szMask)
{
	return FindPattern2((uint32_t)GetModuleHandle(NULL), 0xFFFFFFFF, bMask, szMask);
}

void Mem::Utilites::DumpVFTable(uint32_t dwAddress, int iFunctionCount)
{
	/*CLogFile::Printf("Dumping Virtual Function Table at 0x%p...", dwAddress);
	for (int i = 0; i < iFunctionCount; i++)
	{
		CLogFile::Printf("VFTable Offset: %d, Function: 0x%p (At Address: 0x%p)", (i * 4), *(Puint32_t)(dwAddress + (i * 4)), (dwAddress + (i * 4)));
	}*/
}

void Mem::Initialize(void)
{
	// Prepare headers
	PBYTE pbImageBase = (PBYTE)GetModuleHandle(NULL);
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pbImageBase;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)(pbImageBase + pDosHeader->e_lfanew);
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNtHeaders);

	// Loop thought all sections
	for (int iSection = 0; iSection < pNtHeaders->FileHeader.NumberOfSections; iSection++, pSection++)
	{
		char * szSectionName = (char*)pSection->Name;
		if (!strcmp(szSectionName, ".text") || !strcmp(szSectionName, ".rdata") || !strcmp(szSectionName, ".textnc"))
		{
			// Unprotect segment
			DWORD dwOld = 0;//Temp variable
			VirtualProtect((void *)(pbImageBase + pSection->VirtualAddress), ((pSection->Misc.VirtualSize + 4095)&~4095), PAGE_EXECUTE_READWRITE, &dwOld);
		}
	}
}