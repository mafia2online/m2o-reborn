/** @file lua.hpp
*  @brief Game's lua wrapper
*
*
*  @author Tyldar
*/
#pragma once

namespace M2
{
    namespace Wrappers
    {
        class lua
        {
        public:
            static bool Execute(const std::string &command)
            {
                int iLength = command.size();
                if (iLength <= 0)
                    return false;

                const char *sCommand = command.c_str();
                const char *executeName = "1cff09cfb848cf8935a74c88efe81a47";//MD5("~/Mafia2Online");

                int result;

                auto pScriptManager = M2::C_ScriptMachineManager::Get();
                if (!M2::C_ScriptMachineManager::Get())
                    return false;

                auto pScriptMachine = pScriptManager->GetInterface()->m_pScriptMachine;
                if (!pScriptMachine)
                    return false;

                auto pScriptPool = pScriptMachine->m_pScriptPool;
                if (!pScriptPool)
                    return false;

                auto pUnknow = pScriptPool->m_pUnknow[0];
                if (!pUnknow)
                    return false;

                DWORD luaState = (DWORD)pUnknow->m_pLuaState;
                DWORD address = 0x5D3F80;
                DWORD address2 = 0x5D5BD0;

                if (!luaState) {
                    return false;
                }

                /* Load code into buffer */
                __asm
                {
                    push executeName;
                    push iLength;
                    push sCommand;
                    push luaState;
                    call address2;
                    add esp, 10h;
                }

                /* Call the code */
                __asm
                {
                    push 0;
                    push - 1;
                    push 0;
                    push luaState;
                    call address;
                    mov result, eax;
                    add esp, 10h;
                }

                if (result > 0)
                    return false;

                return true;
            }

            static bool Executef(const char * command, ...)
            {
                char str[2048];
                va_list args;

                va_start(args, command);
                vsprintf_s(str, command, args);
                va_end(args);

                return Execute(str);
            }
        };
    };
};
