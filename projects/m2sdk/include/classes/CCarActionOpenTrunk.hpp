/** @file CCarOwnerDB.cpp
*  @brief Game's action wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/
#include <functional>

#pragma once

namespace M2
{
    class ICCarActionOpenTrunk
    {

    };

    class C_CarActionOpenTrunk : public ICCarActionOpenTrunk
    {

    };

#ifdef MAFIA_SDK_IMPLEMENTATION
    namespace C_CarActionOpenTrunk_Hooks
    {
        void HookTestAction(std::function<bool(C_Car *)>);

        namespace FunctionPointers
        {
            std::function<bool(C_Car *)> testAction;
        };

        namespace Functions
        {
            inline bool TestAction(C_Car *car)
            {
                if (FunctionPointers::testAction != nullptr) {
                    return FunctionPointers::testAction(car);
                }
                return false;
            }
        };

        namespace NakedFunctions
        {
            C_Car *tryCar = nullptr;
            bool tryAnswer = false;
            void __declspec(naked) CCarActionOpenTrunk__TestAction__Hook()
            {
                __asm
                {
                    mov tryCar, esi;
                }

                tryAnswer = Functions::TestAction(tryCar);


                __asm {
                    pop edi;
                    pop esi;
                    mov al, tryAnswer;
                    pop ebx;
                    retn 4;
                }
            }
        };

        void HookTestAction(std::function<bool(C_Car *)> ptr)
        {
            FunctionPointers::testAction = ptr;
            Mem::Hooks::InstallJmpPatch(0xA3EC95, (DWORD)NakedFunctions::CCarActionOpenTrunk__TestAction__Hook);
        }
    };
#endif
}
