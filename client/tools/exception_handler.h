namespace ExceptionHandler
{
    bool Install();
    long WINAPI ExceptionHandlerCallback(_EXCEPTION_POINTERS *pExceptionInfo);
};

IO::CFileLogger exception_logger(true, false);

bool ExceptionHandler::Install()
{
    if (AddVectoredExceptionHandler(1, ExceptionHandlerCallback) == NULL)
        return false;

    return true;
}

long WINAPI ExceptionHandler::ExceptionHandlerCallback(_EXCEPTION_POINTERS *pExceptionInfo)
{
    SYSTEMTIME systemTime;
    GetSystemTime(&systemTime);

    char szFileName[MAX_PATH];
    /// \todo Stop that hardcoding oh... and standardize with our SString class - myu
    sprintf(szFileName, "%s\\Exceptions\\%04d-%02d-%02d_%02d-%02d-%02d.txt", mod.paths.index.c_str(), systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

    HANDLE hModuleSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

    bool bFoundModule = false;
    char szModuleName[MAX_MODULE_NAME32 + 1];
    DWORD dwAddressOfExceptionInModule = NULL;

    if(hModuleSnapShot)
    {
        MODULEENTRY32 ModuleEntry;
        ModuleEntry.dwSize = sizeof(ModuleEntry);

        if(Module32First(hModuleSnapShot, &ModuleEntry))
        {
            while(Module32Next(hModuleSnapShot, &ModuleEntry))
            {
                if((pExceptionInfo->ContextRecord->Eip >= (DWORD)ModuleEntry.modBaseAddr) && (pExceptionInfo->ContextRecord->Eip <= ((DWORD)ModuleEntry.modBaseAddr + ModuleEntry.modBaseSize)))
                {
                    strcpy_s(szModuleName, MAX_MODULE_NAME32 + 1, ModuleEntry.szModule);
                    dwAddressOfExceptionInModule = (pExceptionInfo->ContextRecord->Eip - (DWORD)ModuleEntry.modBaseAddr);
                    bFoundModule = true;
                    break;
                }
            }
        }
    }

    if(!_stricmp(szModuleName, "KERNELBASE.dll"))
    {
        return EXCEPTION_EXECUTE_HANDLER;
    }


    if(!exception_logger.Open(szFileName))
    {
        return EXCEPTION_EXECUTE_HANDLER;
    }

    exception_logger.Writeln("Exception at address: 0x%p\n", pExceptionInfo->ExceptionRecord->ExceptionAddress);

    if(bFoundModule)
    {
        exception_logger.Writeln("Exception in module: %s (+0x%p)\n", szModuleName, dwAddressOfExceptionInModule);
    }

    switch(pExceptionInfo->ExceptionRecord->ExceptionCode)
    {
        case EXCEPTION_ACCESS_VIOLATION:
        {
            exception_logger.Writeln("Exception code: EXCEPTION_ACCESS_VIOLATION\n");

            switch(pExceptionInfo->ExceptionRecord->ExceptionInformation[0])
            {
                case 0:
                    exception_logger.Writeln("Attempted to read from: 0x%08X\n", pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
                    break;

                case 1:
                    exception_logger.Writeln("Attempted to write to: 0x%08X\n", pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
                    break;

                case 8:
                    exception_logger.Writeln("Data Execution Prevention (DEP) at: 0x%08X\n", pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
                    break;

                default:
                    exception_logger.Writeln("Unknown access violation at: 0x%08X\n", pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
                    break;
            }
            break;
        }

        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            exception_logger.Writeln("Exception code: EXCEPTION_ARRAY_BOUNDS_EXCEEDED\n");
            break;

        case EXCEPTION_BREAKPOINT:
            exception_logger.Writeln("Exception code: EXCEPTION_BREAKPOINT\n");
            break;

        case EXCEPTION_DATATYPE_MISALIGNMENT:
            exception_logger.Writeln("Exception code: EXCEPTION_DATATYPE_MISALIGNMENT\n");
            break;

        case EXCEPTION_FLT_DENORMAL_OPERAND:
            exception_logger.Writeln("Exception code: EXCEPTION_FLT_DENORMAL_OPERAND\n");
            break;

        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            exception_logger.Writeln("Exception code: EXCEPTION_FLT_DIVIDE_BY_ZERO\n");
            break;

        case EXCEPTION_FLT_INEXACT_RESULT:
            exception_logger.Writeln("Exception code: EXCEPTION_FLT_INEXACT_RESULT\n");
            break;

        case EXCEPTION_FLT_INVALID_OPERATION:
            exception_logger.Writeln("Exception code: EXCEPTION_FLT_INVALID_OPERATION\n");
            break;

        case EXCEPTION_FLT_OVERFLOW:
            exception_logger.Writeln("Exception code: EXCEPTION_FLT_OVERFLOW\n");
            break;

        case EXCEPTION_FLT_STACK_CHECK:
            exception_logger.Writeln("Exception code: EXCEPTION_FLT_STACK_CHECK\n");
            break;

        case EXCEPTION_FLT_UNDERFLOW:
            exception_logger.Writeln("Exception code: EXCEPTION_FLT_UNDERFLOW\n");
            break;

        case EXCEPTION_ILLEGAL_INSTRUCTION:
            exception_logger.Writeln("Exception code: EXCEPTION_ILLEGAL_INSTRUCTION\n");
            break;

        case EXCEPTION_IN_PAGE_ERROR:
        {
            exception_logger.Writeln("Exception code: EXCEPTION_IN_PAGE_ERROR\n");

            switch(pExceptionInfo->ExceptionRecord->ExceptionInformation[0])
            {
                case 0:
                    exception_logger.Writeln("Attempted to read from: 0x%08X\n", pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
                    break;

                case 1:
                    exception_logger.Writeln("Attempted to write to: 0x%08X\n", pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
                    break;

                case 8:
                    exception_logger.Writeln("Data Execution Prevention (DEP) at: 0x%08X\n", pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
                    break;

                default:
                    exception_logger.Writeln("Unknown access violation at: 0x%08X\n", pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
                    break;
            }

            exception_logger.Writeln("NTSTATUS: 0x%08X\n", pExceptionInfo->ExceptionRecord->ExceptionInformation[2]);
            break;
        }

        case EXCEPTION_INT_DIVIDE_BY_ZERO:
            exception_logger.Writeln("Exception code: EXCEPTION_INT_DIVIDE_BY_ZERO\n");
            break;

        case EXCEPTION_INT_OVERFLOW:
            exception_logger.Writeln("Exception code: EXCEPTION_INT_OVERFLOW\n");
            break;

        case EXCEPTION_INVALID_DISPOSITION:
            exception_logger.Writeln("Exception code: EXCEPTION_INVALID_DISPOSITION\n");
            break;

        case EXCEPTION_NONCONTINUABLE_EXCEPTION:
            exception_logger.Writeln("Exception code: EXCEPTION_NONCONTINUABLE_EXCEPTION\n");
            break;

        case EXCEPTION_PRIV_INSTRUCTION:
            exception_logger.Writeln("Exception code: EXCEPTION_PRIV_INSTRUCTION\n");
            break;

        case EXCEPTION_SINGLE_STEP:
            exception_logger.Writeln("Exception code: EXCEPTION_SINGLE_STEP\n");
            break;

        case EXCEPTION_STACK_OVERFLOW:
            exception_logger.Writeln("Exception code: EXCEPTION_STACK_OVERFLOW\n");
            break;

        case DBG_CONTROL_C:
            exception_logger.Writeln("Exception code: DBG_CONTROL_C (WTF?!)\n");
            break;

        default:
            exception_logger.Writeln("Exception code: %08x\n", pExceptionInfo->ExceptionRecord->ExceptionCode);
    }

    exception_logger.Writeln("\nRegisters:\n");

    if(pExceptionInfo->ContextRecord->ContextFlags & CONTEXT_SEGMENTS)
    {
        exception_logger.Writeln("GS = 0x%p, FS = 0x%p, ES = 0x%p, DS = 0x%p\n", pExceptionInfo->ContextRecord->SegGs, pExceptionInfo->ContextRecord->SegFs, pExceptionInfo->ContextRecord->SegEs, pExceptionInfo->ContextRecord->SegDs);
    }

    if(pExceptionInfo->ContextRecord->ContextFlags & CONTEXT_INTEGER)
    {
        exception_logger.Writeln("EDI = 0x%p, ESI = 0x%p, EBX = 0x%p\n", pExceptionInfo->ContextRecord->Edi, pExceptionInfo->ContextRecord->Esi, pExceptionInfo->ContextRecord->Ebx);
        exception_logger.Writeln("EDX = 0x%p, ECX = 0x%p, EAX = 0x%p\n", pExceptionInfo->ContextRecord->Edx, pExceptionInfo->ContextRecord->Ecx, pExceptionInfo->ContextRecord->Eax);
    }

    if(pExceptionInfo->ContextRecord->ContextFlags & CONTEXT_CONTROL)
    {
        exception_logger.Writeln("EBP = 0x%p, EIP = 0x%p, CS = 0x%p\n", pExceptionInfo->ContextRecord->Ebp, pExceptionInfo->ContextRecord->Eip, pExceptionInfo->ContextRecord->SegCs);
        exception_logger.Writeln("EFLAGS = 0x%p, ESP = 0x%p, SS = 0x%p\n", pExceptionInfo->ContextRecord->EFlags, pExceptionInfo->ContextRecord->Esp, pExceptionInfo->ContextRecord->SegSs);
    }

    DWORD dwEsp = pExceptionInfo->ContextRecord->Esp;

    exception_logger.Writeln("\nStack:\n");

    for(int i = 0; i < 320; i += 16)
    {
        exception_logger.Writeln("+%04X: 0x%08X 0x%08X 0x%08X 0x%08X\n", i, *(DWORD*)(dwEsp+i+0), *(DWORD*)(dwEsp+i+4), *(DWORD*)(dwEsp+i+8), *(DWORD*)(dwEsp+i+12));
    }

    exception_logger.Writeln("\nModules:\n");

    if(hModuleSnapShot)
    {
        MODULEENTRY32 ModuleEntry;
        ModuleEntry.dwSize = sizeof(ModuleEntry);

        if(Module32First(hModuleSnapShot, &ModuleEntry))
        {
            do
            {
                if(ModuleEntry.szModule[0] != 'f' && ModuleEntry.szModule[1] != 'l' && ModuleEntry.szModule[2] != 'a')
                    exception_logger.Writeln("%s [B: 0x%08X, S: 0x%08X] (%s)\n", ModuleEntry.szModule, ModuleEntry.modBaseAddr, ModuleEntry.modBaseSize, ModuleEntry.szExePath);
            }
            while(Module32Next(hModuleSnapShot, &ModuleEntry));
        }

        CloseHandle(hModuleSnapShot);
    }

    exception_logger.Close();

    return EXCEPTION_EXECUTE_HANDLER;
}
