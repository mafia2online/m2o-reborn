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
    class ICCarActionCloseTrunk
    {

    };

    class C_CarActionCloseTrunk : public ICCarActionCloseTrunk
    {

    };

#ifdef MAFIA_SDK_IMPLEMENTATION
    namespace C_CarActionCloseTrunk_Hooks
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
            void __declspec(naked) CCarActionCloseTrunk__TestAction__Hook()
            {
                __asm
                {
                    mov tryCar, esi;
                }

                tryAnswer = Functions::TestAction(tryCar);

                __asm {
                    mov al, tryAnswer;
                    pop esi;
                    retn 4;
                }
            }
        };

        void HookTestAction(std::function<bool(C_Car *)> ptr)
        {
            FunctionPointers::testAction = ptr;
            Mem::Hooks::InstallJmpPatch(0xA3EE72, (DWORD)NakedFunctions::CCarActionCloseTrunk__TestAction__Hook);
        }
    };
#endif
}
