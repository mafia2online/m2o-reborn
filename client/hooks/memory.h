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
        static  void                * InstallDetourPatchInternal(uint32_t dwAddress, uint32_t dwDetourAddress, BYTE byteType, int iSize = 5);
        static  void                UninstallDetourPatchInternal(uint32_t dwAddress, void ** pTrampoline, int iSize = 5);

        static  void                * InstallDetourPatch(char * szLibrary, char * szFunction, uint32_t dwFunctionAddress);
        static  void                * InstallDetourPatch(uint32_t dwAddress, uint32_t dwFunctionAddress);
        static  bool                UninstallDetourPatch(void * pTrampoline, uint32_t dwFunctionAddress);

        static  void                * InstallCallPatch(uint32_t dwAddress, uint32_t dwCallAddress, int iSize = 5);
        static  void                * InstallJmpPatch(uint32_t dwAddress, uint32_t dwJmpAddress, int iSize = 5);

        static  Address             InstallNotDumbJMP(Address target_addr, Address hookfnc_addr, size_t len = 5);

    };

    class Utilites
    {
    public:
        static  void                InstallNopPatch(uint32_t dwAddress, int iSize = 1);
        static  void                PatchAddress(uint32_t dwAddress, BYTE * bPatch, size_t iSize);
        static  void                PatchAddress(uint32_t dwAddress, uint32_t dwPatch);

        static  bool                bDataCompare(const unsigned char * pData, const unsigned char * bMask, const char * szMask);
        static  unsigned long       FindPattern2(uint32_t dwAddress, uint32_t dwLen, unsigned char * bMask, char * szMask);
        static  unsigned long       FindPattern(unsigned char * bMask, char * szMask);


        static  void                DumpVFTable(uint32_t dwAddress, int iFunctionCount);
    };

    void Initialize(void);
}


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

Address Mem::Hooks::InstallNotDumbJMP(Address target_addr, Address hookfnc_addr, size_t len)
{
    ScopedProtect(target_addr, len);
    std::vector<Byte> patch_data(len, 0x90);
    patch_data[0] = X86Instructions::JMP;
    *reinterpret_cast<Address *>(patch_data.data() + 1) = hookfnc_addr - (target_addr + 5);
    //std::copy_n(patch_data.data(), patch_data.size(), reinterpret_cast<std::vector<Byte>::value_type*>(target_addr));
    zpl_memcopy(reinterpret_cast<std::vector<Byte>::value_type*>(target_addr), patch_data.data(), patch_data.size());
    return target_addr + len;
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
