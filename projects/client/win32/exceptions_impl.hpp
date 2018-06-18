static std::string _exceptions_folder;

// =======================================================================//
// !
// ! Hooking
// !
// =======================================================================//

void exception_writeln(zpl_file_t *file, const char* format, ...) {
    va_list ap;
    char message[2048] = { 0 };
    va_start(ap, format);
    vsprintf(message, format, ap);
    va_end(ap);

    zpl_file_write(file, message, zpl_strlen(message));
}

long WINAPI exception_callback(_EXCEPTION_POINTERS *pExceptionInfo) {
    SYSTEMTIME systemTime;
    GetSystemTime(&systemTime);
    zpl_file_t file;

    char szFileName[MAX_PATH];
    sprintf(szFileName, "%s\\%04d-%02d-%02d_%02d-%02d-%02d.txt", _exceptions_folder.c_str(),
        systemTime.wYear, systemTime.wMonth, systemTime.wDay,
        systemTime.wHour, systemTime.wMinute, systemTime.wSecond
    );

    HANDLE hModuleSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

    bool bFoundModule = false;
    char szModuleName[MAX_MODULE_NAME32 + 1];
    DWORD dwAddressOfExceptionInModule = NULL;

    if (hModuleSnapShot) {
        MODULEENTRY32 ModuleEntry;
        ModuleEntry.dwSize = sizeof(ModuleEntry);

        if (Module32First(hModuleSnapShot, &ModuleEntry)) {
            while(Module32Next(hModuleSnapShot, &ModuleEntry)) {
                if ((pExceptionInfo->ContextRecord->Eip >= (DWORD)ModuleEntry.modBaseAddr) && (pExceptionInfo->ContextRecord->Eip <= ((DWORD)ModuleEntry.modBaseAddr + ModuleEntry.modBaseSize))) {
                    strcpy_s(szModuleName, MAX_MODULE_NAME32 + 1, ModuleEntry.szModule);
                    dwAddressOfExceptionInModule = (pExceptionInfo->ContextRecord->Eip - (DWORD)ModuleEntry.modBaseAddr);
                    bFoundModule = true;
                    break;
                }
            }
        }
    }

    if (!_stricmp(szModuleName, "KERNELBASE.dll")) {
        return EXCEPTION_EXECUTE_HANDLER;
    }

    mod_log("[exception] capturing the exception...\n");

    if (zpl_file_create(&file, szFileName) != ZPL_FILE_ERROR_NONE) {
        return EXCEPTION_EXECUTE_HANDLER;
    }

    exception_writeln(&file, "Exception at address: 0x%p\r\n", pExceptionInfo->ExceptionRecord->ExceptionAddress);

    if (bFoundModule) {
        exception_writeln(&file, "Exception in module: %s (+0x%p)\r\n", szModuleName, dwAddressOfExceptionInModule);
    }

    switch(pExceptionInfo->ExceptionRecord->ExceptionCode) {
        case EXCEPTION_ACCESS_VIOLATION: {
            exception_writeln(&file, "Exception code: EXCEPTION_ACCESS_VIOLATION\r\n");

            switch(pExceptionInfo->ExceptionRecord->ExceptionInformation[0])
            {
                case 0:
                    exception_writeln(&file, "Attempted to read from: 0x%08X\r\n", pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
                    break;

                case 1:
                    exception_writeln(&file, "Attempted to write to: 0x%08X\r\n", pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
                    break;

                case 8:
                    exception_writeln(&file, "Data Execution Prevention (DEP) at: 0x%08X\r\n", pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
                    break;

                default:
                    exception_writeln(&file, "Unknown access violation at: 0x%08X\r\n", pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
                    break;
            }
            break;
        }

        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: exception_writeln(&file, "Exception code: EXCEPTION_ARRAY_BOUNDS_EXCEEDED\r\n");break;
        case EXCEPTION_BREAKPOINT: exception_writeln(&file, "Exception code: EXCEPTION_BREAKPOINT\r\n");break;
        case EXCEPTION_DATATYPE_MISALIGNMENT: exception_writeln(&file, "Exception code: EXCEPTION_DATATYPE_MISALIGNMENT\r\n");break;
        case EXCEPTION_FLT_DENORMAL_OPERAND: exception_writeln(&file, "Exception code: EXCEPTION_FLT_DENORMAL_OPERAND\r\n");break;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO: exception_writeln(&file, "Exception code: EXCEPTION_FLT_DIVIDE_BY_ZERO\r\n");break;
        case EXCEPTION_FLT_INEXACT_RESULT: exception_writeln(&file, "Exception code: EXCEPTION_FLT_INEXACT_RESULT\r\n");break;
        case EXCEPTION_FLT_INVALID_OPERATION: exception_writeln(&file, "Exception code: EXCEPTION_FLT_INVALID_OPERATION\r\n");break;
        case EXCEPTION_FLT_OVERFLOW: exception_writeln(&file, "Exception code: EXCEPTION_FLT_OVERFLOW\r\n");break;
        case EXCEPTION_FLT_STACK_CHECK: exception_writeln(&file, "Exception code: EXCEPTION_FLT_STACK_CHECK\r\n");break;
        case EXCEPTION_FLT_UNDERFLOW: exception_writeln(&file, "Exception code: EXCEPTION_FLT_UNDERFLOW\r\n");break;
        case EXCEPTION_ILLEGAL_INSTRUCTION: exception_writeln(&file, "Exception code: EXCEPTION_ILLEGAL_INSTRUCTION\r\n");break;
        case EXCEPTION_IN_PAGE_ERROR: {
            exception_writeln(&file, "Exception code: EXCEPTION_IN_PAGE_ERROR\r\n");

            switch(pExceptionInfo->ExceptionRecord->ExceptionInformation[0])
            {
                case 0:
                    exception_writeln(&file, "Attempted to read from: 0x%08X\r\n", pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
                    break;

                case 1:
                    exception_writeln(&file, "Attempted to write to: 0x%08X\r\n", pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
                    break;

                case 8:
                    exception_writeln(&file, "Data Execution Prevention (DEP) at: 0x%08X\r\n", pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
                    break;

                default:
                    exception_writeln(&file, "Unknown access violation at: 0x%08X\r\n", pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
                    break;
            }

            exception_writeln(&file, "NTSTATUS: 0x%08X\r\n", pExceptionInfo->ExceptionRecord->ExceptionInformation[2]);
            break;
        }

        case EXCEPTION_INT_DIVIDE_BY_ZERO: exception_writeln(&file, "Exception code: EXCEPTION_INT_DIVIDE_BY_ZERO\r\n");break;
        case EXCEPTION_INT_OVERFLOW: exception_writeln(&file, "Exception code: EXCEPTION_INT_OVERFLOW\r\n");break;
        case EXCEPTION_INVALID_DISPOSITION: exception_writeln(&file, "Exception code: EXCEPTION_INVALID_DISPOSITION\r\n");break;
        case EXCEPTION_NONCONTINUABLE_EXCEPTION: exception_writeln(&file, "Exception code: EXCEPTION_NONCONTINUABLE_EXCEPTION\r\n");break;
        case EXCEPTION_PRIV_INSTRUCTION: exception_writeln(&file, "Exception code: EXCEPTION_PRIV_INSTRUCTION\r\n");break;
        case EXCEPTION_SINGLE_STEP: exception_writeln(&file, "Exception code: EXCEPTION_SINGLE_STEP\r\n");break;
        case EXCEPTION_STACK_OVERFLOW: exception_writeln(&file, "Exception code: EXCEPTION_STACK_OVERFLOW\r\n");break;
        case DBG_CONTROL_C: exception_writeln(&file, "Exception code: DBG_CONTROL_C (WTF?!)\r\n");break;
        default: exception_writeln(&file, "Exception code: %08x\r\n", pExceptionInfo->ExceptionRecord->ExceptionCode);
    }

    exception_writeln(&file, "\nRegisters:\r\n");

    if (pExceptionInfo->ContextRecord->ContextFlags & CONTEXT_SEGMENTS) {
        exception_writeln(&file, "GS = 0x%p, FS = 0x%p, ES = 0x%p, DS = 0x%p\r\n", pExceptionInfo->ContextRecord->SegGs, pExceptionInfo->ContextRecord->SegFs, pExceptionInfo->ContextRecord->SegEs, pExceptionInfo->ContextRecord->SegDs);
    }

    if (pExceptionInfo->ContextRecord->ContextFlags & CONTEXT_INTEGER) {
        exception_writeln(&file, "EDI = 0x%p, ESI = 0x%p, EBX = 0x%p\r\n", pExceptionInfo->ContextRecord->Edi, pExceptionInfo->ContextRecord->Esi, pExceptionInfo->ContextRecord->Ebx);
        exception_writeln(&file, "EDX = 0x%p, ECX = 0x%p, EAX = 0x%p\r\n", pExceptionInfo->ContextRecord->Edx, pExceptionInfo->ContextRecord->Ecx, pExceptionInfo->ContextRecord->Eax);
    }

    if (pExceptionInfo->ContextRecord->ContextFlags & CONTEXT_CONTROL) {
        exception_writeln(&file, "EBP = 0x%p, EIP = 0x%p, CS = 0x%p\r\n", pExceptionInfo->ContextRecord->Ebp, pExceptionInfo->ContextRecord->Eip, pExceptionInfo->ContextRecord->SegCs);
        exception_writeln(&file, "EFLAGS = 0x%p, ESP = 0x%p, SS = 0x%p\r\n", pExceptionInfo->ContextRecord->EFlags, pExceptionInfo->ContextRecord->Esp, pExceptionInfo->ContextRecord->SegSs);
    }

    DWORD dwEsp = pExceptionInfo->ContextRecord->Esp;

    exception_writeln(&file, "\nStack:\r\n");

    for(int i = 0; i < 320; i += 16) {
        exception_writeln(&file, "+%04X: 0x%08X 0x%08X 0x%08X 0x%08X\r\n", i, *(DWORD*)(dwEsp+i+0), *(DWORD*)(dwEsp+i+4), *(DWORD*)(dwEsp+i+8), *(DWORD*)(dwEsp+i+12));
    }

    exception_writeln(&file, "\nModules:\r\n");

    if (hModuleSnapShot) {
        MODULEENTRY32 ModuleEntry;
        ModuleEntry.dwSize = sizeof(ModuleEntry);

        if (Module32First(hModuleSnapShot, &ModuleEntry)) {
            do {
                if (ModuleEntry.szModule[0] != 'f' && ModuleEntry.szModule[1] != 'l' && ModuleEntry.szModule[2] != 'a')
                    exception_writeln(&file, "%s [B: 0x%08X, S: 0x%08X] (%s)\r\n", ModuleEntry.szModule, ModuleEntry.modBaseAddr, ModuleEntry.modBaseSize, ModuleEntry.szExePath);
            } while(Module32Next(hModuleSnapShot, &ModuleEntry));
        }

        CloseHandle(hModuleSnapShot);
    }

    zpl_file_close(&file);
    return EXCEPTION_EXECUTE_HANDLER;
}


// =======================================================================//
// !
// ! Control methods
// !
// =======================================================================//

bool exceptions_init(const char *folder) {
    /* create folder if doesnt exist */
    zpl_path_mkdir(folder, 0744);
    _exceptions_folder = std::string(folder);

    if (AddVectoredExceptionHandler(1, exception_callback) == NULL) {
        return false;
    }

    return true;
}

void exceptions_free() {}
