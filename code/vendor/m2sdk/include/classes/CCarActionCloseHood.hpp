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
    class ICCarActionCloseHood
    {

    };

    class C_CarActionCloseHood : public ICCarActionCloseHood
    {

    };

#ifdef MAFIA_SDK_IMPLEMENTATION
    namespace C_CarActionCloseHood_Hooks
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
            C_Car *tryHoodCloseCar = nullptr;
            bool tryHoodCloseAnswer;
            void __declspec(naked) CCarActionCloseHood__TestAction__Hook()
            {
                __asm
                {
                    mov tryHoodCloseCar, esi;
                }

                tryHoodCloseAnswer = Functions::TestAction(tryHoodCloseCar);


                __asm {
                    pop edi;
                    mov al, tryHoodCloseAnswer;
                    pop esi;
                    retn 4;
                }
            }
        };

        void HookTestAction(std::function<bool(C_Car *)> ptr)
        {
            FunctionPointers::testAction = ptr;
            Mem::Hooks::InstallJmpPatch(0xA3F05C, (DWORD)NakedFunctions::CCarActionCloseHood__TestAction__Hook);
        }
    };
#endif
}
