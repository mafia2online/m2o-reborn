/*
 * This file is part of the CitizenFX project - http://citizen.re/
 *
 * See LICENSE and MENTIONS in the root of the source tree for information
 * regarding licensing.
 */

#pragma once

#include <stdint.h>
#include <Windows.h>
#include <memory>
#include <functional>

namespace hook
{
// for link /DYNAMICBASE executables
static ptrdiff_t baseAddressDifference;

// sets the base address difference based on an obtained pointer
inline void set_base(uintptr_t address)
{
#ifdef _M_IX86
	uintptr_t addressDiff = (address - 0x400000);
#elif defined(_M_AMD64)
	uintptr_t addressDiff = (address - 0x140000000);
#endif

	// pointer-style cast to ensure unsigned overflow ends up copied directly into a signed value
	baseAddressDifference = *(ptrdiff_t*)&addressDiff;
}

// sets the base to the process main base
inline void set_base()
{
	set_base((uintptr_t)GetModuleHandle(NULL));
}

// adjusts the address passed to the base as set above
template<typename T>
inline void adjust_base(T& address)
{
#if 1
	*(uintptr_t*)&address += baseAddressDifference;
#endif
}

// returns the adjusted address to the stated base
template<typename T>
inline uintptr_t get_adjusted(T address)
{
	return (uintptr_t)address + baseAddressDifference;
}


template<typename ValueType, typename AddressType>
inline void write(AddressType address, ValueType value)
{
	adjust_base(address);

	memcpy((void*)address, &value, sizeof(value));
}

template<typename ValueType, typename AddressType>
inline void writeVP(AddressType address, ValueType value)
{
	adjust_base(address);

	DWORD oldProtect;
	VirtualProtect((void*)address, sizeof(value), PAGE_EXECUTE_READWRITE, &oldProtect);

	memcpy((void*)address, &value, sizeof(value));

	VirtualProtect((void*)address, sizeof(value), oldProtect, &oldProtect);
}

template <typename TRet = void, typename... TArgs, typename AddressType>
constexpr inline TRet call(AddressType address, TArgs... args)
{
    return reinterpret_cast<TRet(*)(TArgs...)>(address)(args ...);
}

template <typename TRet = void, typename... TArgs, typename AddressType>
constexpr inline TRet std_call(AddressType address, TArgs... args)
{
    return reinterpret_cast<TRet(__stdcall *)(TArgs...)>(address)(args ...);
}

template <typename TRet = void, typename... TArgs>
constexpr inline TRet this_call(TRet address, TArgs... args)
{
    return reinterpret_cast<TRet(__thiscall *)(TArgs...)>(address)(args ...);
}

template<typename AddressType>
inline void nop(AddressType address, size_t length)
{
	adjust_base(address);

    DWORD op;
    VirtualProtect((void*)address, length, PAGE_EXECUTE_READWRITE, &op);
	memset((void*)address, 0x90, length);
}

template<typename AddressType>
inline void return_function(AddressType address, uint16_t stackSize = 0)
{
	if (stackSize == 0)
	{
		writeVP<uint8_t>(address, 0xC3);
	}
	else
	{
        writeVP<uint8_t>(address, 0xC2);
        writeVP<uint16_t>((uintptr_t)address + 1, stackSize);
	}
}

template<typename T>
inline T* getRVA(uintptr_t rva)
{
#ifdef _M_IX86
	return (T*)(baseAddressDifference + 0x400000 + rva);
#elif defined(_M_AMD64)
	return (T*)(0x140000000 + rva);
#endif
}

namespace
{
	template<typename TOrdinal>
	inline bool iat_matches_ordinal(uintptr_t* nameTableEntry, TOrdinal ordinal)
	{
		
	}

	template<>
	inline bool iat_matches_ordinal(uintptr_t* nameTableEntry, int ordinal)
	{
		if (IMAGE_SNAP_BY_ORDINAL(*nameTableEntry))
		{
			return IMAGE_ORDINAL(*nameTableEntry) == ordinal;
		}

		return false;
	}

	template<>
	inline bool iat_matches_ordinal(uintptr_t* nameTableEntry, const char* ordinal)
	{
		if (!IMAGE_SNAP_BY_ORDINAL(*nameTableEntry))
		{
			auto import = getRVA<IMAGE_IMPORT_BY_NAME>(*nameTableEntry);

			return !_stricmp(import->Name, ordinal);
		}

		return false;
	}
}

template<typename T, typename TOrdinal>
T iat(const char* moduleName, T function, TOrdinal ordinal)
{
#ifdef _M_IX86
	IMAGE_DOS_HEADER* imageHeader = (IMAGE_DOS_HEADER*)(baseAddressDifference + 0x400000);
#elif defined(_M_AMD64)
	IMAGE_DOS_HEADER* imageHeader = (IMAGE_DOS_HEADER*)(baseAddressDifference + 0x140000000);
#endif
	IMAGE_NT_HEADERS* ntHeader = getRVA<IMAGE_NT_HEADERS>(imageHeader->e_lfanew);

	IMAGE_IMPORT_DESCRIPTOR* descriptor = getRVA<IMAGE_IMPORT_DESCRIPTOR>(ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	while (descriptor->Name)
	{
		const char* name = getRVA<char>(descriptor->Name);

		if (_stricmp(name, moduleName))
		{
			descriptor++;

			continue;
		}

		if (descriptor->OriginalFirstThunk == 0)
		{
			return nullptr;
		}

		auto nameTableEntry = getRVA<uintptr_t>(descriptor->OriginalFirstThunk);
		auto addressTableEntry = getRVA<uintptr_t>(descriptor->FirstThunk);

		while (*nameTableEntry)
		{
			if (iat_matches_ordinal(nameTableEntry, ordinal))
			{
				T origEntry = (T)*addressTableEntry;

				DWORD oldProtect;
				VirtualProtect(addressTableEntry, sizeof(T), PAGE_READWRITE, &oldProtect);

				*addressTableEntry = (uintptr_t)function;

				VirtualProtect(addressTableEntry, sizeof(T), oldProtect, &oldProtect);

				return origEntry;
			}

			nameTableEntry++;
			addressTableEntry++;
		}

		return nullptr;
	}

	return nullptr;
}

#if 0
// a context for the hook function to party on
struct HookContext
{
	uint32_t m_jumpRet;
	uint32_t m_edi, m_esi, m_ebp, m_esp, m_ebx, m_edx, m_ecx, m_eax; // matches pushad format
};

class inject_hook
{
friend struct inject_hook_frontend;

private:
	HookContext* m_curContext;
	std::shared_ptr<FunctionAssembly> m_assembly;
	uintptr_t m_address;

public:
	// return value type container
	typedef int ReturnType;

private:
	// set context and run
	ReturnType do_run(HookContext* context)
	{
		m_curContext = context;

		return run();
	}

protected:
	// return type values for use in return;

	// return without jumping anywhere
	inline ReturnType DoNowt()
	{
		return ReturnType(0);
	}

	// jump to this place after the hook
	inline ReturnType JumpTo(uint32_t address)
	{
		m_curContext->m_eax = address;

		return ReturnType(1);
	}

	// accessors for context registers
	inline uint32_t Eax() { return m_curContext->m_eax; }
	inline void Eax(uint32_t a) { m_curContext->m_eax = a; }

	inline uint32_t Ebx() { return m_curContext->m_ebx; }
	inline void Ebx(uint32_t a) { m_curContext->m_ebx = a; }

	inline uint32_t Ecx() { return m_curContext->m_ecx; }
	inline void Ecx(uint32_t a) { m_curContext->m_ecx = a; }

	inline uint32_t Edx() { return m_curContext->m_edx; }
	inline void Edx(uint32_t a) { m_curContext->m_edx = a; }

	inline uint32_t Esi() { return m_curContext->m_esi; }
	inline void Esi(uint32_t a) { m_curContext->m_esi = a; }

	inline uint32_t Edi() { return m_curContext->m_edi; }
	inline void Edi(uint32_t a) { m_curContext->m_edi = a; }

	inline uint32_t Esp() { return m_curContext->m_esp; }
	inline void Esp(uint32_t a) { m_curContext->m_esp = a; }

	inline uint32_t Ebp() { return m_curContext->m_ebp; }
	inline void Ebp(uint32_t a) { m_curContext->m_ebp = a; }

public:
	virtual ReturnType run() = 0;

	inject_hook(uint32_t address)
	{
		m_address = address;
	}

	void inject();
	void injectCall();
};

struct inject_hook_frontend : jitasm::Frontend
{
private:
	inject_hook* m_hook;

	static inject_hook::ReturnType CallHookFunction(inject_hook* hook, HookContext* context)
	{
		return hook->do_run(context);
	}

public:
	inject_hook_frontend(inject_hook* hook)
	{
		m_hook = hook;
	}

	void InternalMain()
	{
		// set up the context stack frame
		pushad(); // registers
		sub(esp, 4); // jump target area
		mov(dword_ptr[esp], 0);

		// load the context address into eax
		lea(eax, dword_ptr[esp]);

		// push eax (second argument to our call)
		push(eax);

		// push the (softcoded, heh) hook function
		push((uint32_t)m_hook);

		// call the call stub
		mov(eax, (uint32_t)CallHookFunction);
		call(eax);

		// remove garbage from the stack
		add(esp, 8);

		// do we want to jump somewhere?
		test(eax, eax);
		jnz("actuallyJump");

		// guess not, remove jump target area and popad
		add(esp, 4);
		popad();

		// get esp back from the context bit
		mov(esp, dword_ptr[esp - 20]);

		ret();

		L("actuallyJump");

		add(esp, 4);
		popad();

		mov(esp, dword_ptr[esp - 20]);
		
		AppendInstr(jitasm::I_CALL, 0xFF, 0, jitasm::Imm8(4), R(eax));
	}
};

#define DEFINE_INJECT_HOOK(hookName, hookAddress) class _zz_inject_hook_##hookName : public hook::inject_hook { public: _zz_inject_hook_##hookName(uint32_t address) : hook::inject_hook(address) {}; ReturnType run(); }; \
	static _zz_inject_hook_##hookName hookName(hookAddress); \
	_zz_inject_hook_##hookName::ReturnType _zz_inject_hook_##hookName::run()


#if 0
struct AsmHookStub : jitasm::Frontend
{
private:
	hook_function* m_safeFunction;

	static bool CallHookFunction(hook_function* function, HookContext& context)
	{
		return (*function)(context);
	}

public:
	AsmHookStub(hook_function* function)
	{
		m_safeFunction = function;
	}

	void InternalMain()
	{
		// set up the context stack frame
		pushad(); // registers
		sub(esp, 4); // jump target area
		mov(dword_ptr[esp], 0);

		// load the jump target into eax
		lea(eax, dword_ptr[esp]);

		// push eax (second argument to our call)
		push(eax);

		// push the (softcoded, heh) hook function
		push((uint32_t)m_safeFunction);

		// call the call stub
		mov(eax, (uint32_t)CallHookFunction);
		call(eax);

		// remove garbage from the stack
		add(esp, 8);

		// check if we want to execute the original trampoline
		test(al, al);
		jnz("noTrampolineReturn");



		L("noTrampolineReturn");

		
	}
};
#endif
#endif

template<typename T, typename AT>
inline void put_ljump(AT address, T func)
{
    writeVP<uint8_t>(address, 0xE9);
    writeVP<int>((uintptr_t)address + 1, (intptr_t)func - (intptr_t)get_adjusted(address) - 5);
}

template<typename T, typename AT>
inline void put_call(AT address, T func)
{
	writeVP<uint8_t>(address, 0xE8);
	writeVP<int>((uintptr_t)address + 1, (intptr_t)func - (intptr_t)get_adjusted(address) - 5);
}

template<typename T>
inline T get_call(T address)
{
	intptr_t target = *(uintptr_t*)(get_adjusted(address) + 1);
	target += (get_adjusted(address) + 5);

	return (T)target;
}

template<typename TTarget, typename T>
inline void set_call(TTarget* target, T address)
{
	*(T*)target = get_call(address);
}

template<typename TTarget, typename T, typename TFunc>
inline void replace_call(TTarget *target, T address, TFunc fn)
{
    *(T*)target = get_call(address);
    put_call(address, fn);
}

namespace vp
{
	template<typename T, typename AT>
	inline void jump(AT address, T func)
	{
        writeVP<uint8_t>(address, 0xE9);
        writeVP<int>((uintptr_t)address + 1, (intptr_t)func - (intptr_t)get_adjusted(address) - 5);
	}

	template<typename T, typename AT>
	inline void call(AT address, T func)
	{
        writeVP<uint8_t>(address, 0xE8);
        writeVP<int>((uintptr_t)address + 1, (intptr_t)func - (intptr_t)get_adjusted(address) - 5);
	}
}

#if 0
void* AllocateFunctionStub(void* ptr, int type = 0);
void* AllocEx(uintptr_t size);

template<typename T>
struct get_func_ptr
{
	static void* get(T func)
	{
		return (void*)func;
	}
};

template<typename T, typename AT>
inline void put_ljump(AT address, T func)
{
	LPVOID funcStub = AllocateFunctionStub(get_func_ptr<T>::get(func));

	write<uint8_t>(address, 0xE9);
	write<int>((uintptr_t)address + 1, (intptr_t)funcStub- (intptr_t)get_adjusted(address) - 5);
}

template<typename T, typename AT>
inline void jump_rcx(AT address, T func)
{
	LPVOID funcStub = AllocateFunctionStub(get_func_ptr<T>::get(func), 1);

	write<uint8_t>(address, 0xE9);
	write<int>((uintptr_t)address + 1, (intptr_t)funcStub - (intptr_t)get_adjusted(address) - 5);
}

template<int Register, typename T, typename AT>
inline std::enable_if_t<(Register < 8 && Register >= 0)> call_reg(AT address, T func)
{
	LPVOID funcStub = AllocateFunctionStub(get_func_ptr<T>::get(func), Register);

	write<uint8_t>(address, 0xE8);
	write<int>((uintptr_t)address + 1, (intptr_t)funcStub - (intptr_t)get_adjusted(address) - 5);
}

template<typename T, typename AT>
inline void put_call(AT address, T func)
{
	call_reg<0>(address, func);
}

template<typename T, typename AT>
inline void call_rcx(AT address, T func)
{
	call_reg<1>(address, func);
}

template<typename T, typename TAddr>
inline T get_address(TAddr address)
{
	intptr_t target = *(int32_t*)(get_adjusted(address));
	target += (get_adjusted(address) + 4);

	return (T)target;
}

template<typename T>
inline T get_call(T address)
{
	intptr_t target = *(int32_t*)(get_adjusted(address) + 1);
	target += (get_adjusted(address) + 5);

	return (T)target;
}

template<typename TTarget, typename T>
inline void set_call(TTarget* target, T address)
{
	*(T*)target = get_call(address);
}

template<typename TBackup, typename TReplace, typename TAddr>
inline void replace_call(TBackup *target, TReplace replace, TAddr addr)
{
    set_call(target, addr);
    put_call(addr, replace);
}

inline uintptr_t get_member_internal(void* function)
{
	return *(uintptr_t*)function;
}

template<typename T>
inline uintptr_t get_member_old(T function)
{
	return ((uintptr_t(*)(T))get_member_internal)(function);
}

template<typename TClass, typename TMember>
inline uintptr_t get_member(TMember TClass::*function)
{
	union member_cast
	{
		TMember TClass::*function;
		void* ptr;
	};

	member_cast cast;

	if (sizeof(cast.function) != sizeof(cast.ptr))
	{
		return get_member_old(function);
	}

	cast.function = function;

	return (uintptr_t)cast.ptr;
}

template<typename TClass, typename TMember>
struct get_func_ptr<TMember TClass::*>
{
	static void* get(TMember TClass::* function)
	{
		return (void*)get_member(function);
	}
};
#endif
}

#include "Hooking.Invoke.h"
#include "Hooking.Patterns.h"

// haha, hell !
namespace nio = hook;
