#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <Shared/Types.h>

#pragma comment(lib, "Libraries\\detours\\detours.lib")

namespace Mem
{
	enum X86Instructions : Byte
	{
		NOP = 0x90,
		RETN = 0xC3,
		CALL = 0xE8,
		JMP = 0xE9
	};

	enum CallingConventions : Byte
	{
		call_cdecl,
		call_std,
		call_this
	};

	// Helpers
	inline Address GetModuleAddress(const char *szModule) { return reinterpret_cast<uint32_t>(GetModuleHandle(szModule)); }

	template <typename T> T Read(Address addr) { return *reinterpret_cast<T*>(addr); }

	template<typename T>
	inline void Write(Address addr, T value)
	{
		Protect protect(addr, sizeof(T));
		*(T*)(addr) = value;
	}

	template<typename T>
	inline void WriteFast(Address addr, T value)
	{
		*(T*)(addr) = value;
	}

	// Scoped protect
	class ScopedProtect 
	{
	public:
		ScopedProtect(Address addr, size_t size) :m_addr(addr), m_size(size)
		{
			VirtualProtect(reinterpret_cast<void *>(m_addr), m_size, PAGE_EXECUTE_READWRITE, &m_protect);
		}

		~ScopedProtect(void)
		{
			VirtualProtect(reinterpret_cast<void *>(m_addr), m_size, m_protect, nullptr);
		}
	private:
		unsigned long m_protect;
		Address m_addr;
		size_t m_size;
	};

	// Function invoker
	template<CallingConventions CConv, typename Ret, typename ...Args>
	Ret InvokeFunction(Address address, Args ...args)
	{
		if (CConv == call_cdecl)
			return (reinterpret_cast<Ret(__cdecl *)(Args...)>(address))(args...);
		else if (CConv == call_std)
			return (reinterpret_cast<Ret(__stdcall *)(Args...)>(address))(args...);
		else
			return (reinterpret_cast<Ret(__thiscall *)(Args...)>(address))(args...);
	}

	class Hooks
	{
	public:
		static	void				* InstallDetourPatchInternal(uint32_t dwAddress, uint32_t dwDetourAddress, BYTE byteType, int iSize = 5);
		static	void				UninstallDetourPatchInternal(uint32_t dwAddress, void ** pTrampoline, int iSize = 5);

		static	void				* InstallDetourPatch(char * szLibrary, char * szFunction, uint32_t dwFunctionAddress);
		static	void				* InstallDetourPatch(uint32_t dwAddress, uint32_t dwFunctionAddress);
		static	bool				UninstallDetourPatch(void * pTrampoline, uint32_t dwFunctionAddress);

		static	void				* InstallCallPatch(uint32_t dwAddress, uint32_t dwCallAddress, int iSize = 5);
		static	void				* InstallJmpPatch(uint32_t dwAddress, uint32_t dwJmpAddress, int iSize = 5);
	};

	class Utilites
	{
	public:
		static	void				InstallNopPatch(uint32_t dwAddress, int iSize = 1);
		static	void				PatchAddress(uint32_t dwAddress, BYTE * bPatch, size_t iSize);
		static	void				PatchAddress(uint32_t dwAddress, uint32_t dwPatch);

		static	bool				bDataCompare(const unsigned char * pData, const unsigned char * bMask, const char * szMask);
		static	unsigned long		FindPattern2(uint32_t dwAddress, uint32_t dwLen, unsigned char * bMask, char * szMask);
		static	unsigned long		FindPattern(unsigned char * bMask, char * szMask);


		static	void				DumpVFTable(uint32_t dwAddress, int iFunctionCount);
	};

	void Initialize(void);
}