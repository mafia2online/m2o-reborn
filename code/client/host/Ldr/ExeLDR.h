/*
* This file is part of the CitizenFX project - http://citizen.re/
*
* See LICENSE and MENTIONS in the root of the source tree for information
* regarding licensing.
*/

#pragma once

#include <functional>
#include <windows.h>
#include <winnt.h>
#include <cstdint>

class ExecutableLoader
{
    const uint8_t* m_origBinary;
    HMODULE m_module{};
    uintptr_t m_loadLimit;

    void* m_entryPoint{};

    HMODULE (*m_libraryLoader)(const char*){};

    LPVOID (*m_functionResolver)(HMODULE, const char*){};

    std::function<bool(const IMAGE_TLS_DIRECTORY*)> m_tlsInitializer;

    void LoadSection(IMAGE_SECTION_HEADER* section);
    void LoadSections(IMAGE_NT_HEADERS* ntHeader);

#ifdef _M_AMD64
    void LoadExceptionTable(IMAGE_NT_HEADERS* ntHeader);
#endif

    void LoadImports(IMAGE_NT_HEADERS* ntHeader);

    HMODULE ResolveLibrary(const char* name);

    template <typename T>
    const T* GetRVA(uint32_t rva)
    {
        return (T*)(m_origBinary + rva);
    }

    template <typename T>
    T* GetTargetRVA(uint32_t rva)
    {
        return (T*)((uint8_t*)m_module + rva);
    }

public:
    ExecutableLoader(const uint8_t* origBinary);

    void SetLoadLimit(uintptr_t loadLimit)
    {
        m_loadLimit = loadLimit;
    }

    void SetLibraryLoader(HMODULE (*loader)(const char*))
    {
        m_libraryLoader = loader;
    }

    void SetFunctionResolver(LPVOID (*functionResolver)(HMODULE, const char*))
    {
        m_functionResolver = functionResolver;
    }

    void SetTLSInitializer(const std::function<bool(const IMAGE_TLS_DIRECTORY*)>& callback)
    {
        m_tlsInitializer = callback;
    }

    void* GetEP()
    {
        return m_entryPoint;
    }

    void LoadIntoModule(HMODULE module);
};
