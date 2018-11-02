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
    class ICCarActionOpenHood
    {

    };

    class C_CarActionOpenHood : public ICCarActionOpenHood
    {

    };

#ifdef MAFIA_SDK_IMPLEMENTATION
    namespace C_CarActionOpenHood_Hooks
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
            C_Car *tryHoodOpenCar = nullptr;
            bool tryHoodOpenAnswer;
            void __declspec(naked) CCarActionOpenHood__TestAction__Hook()
            {
                __asm
                {
                    mov tryHoodOpenCar, esi;
                }

                tryHoodOpenAnswer = Functions::TestAction(tryHoodOpenCar);


                __asm {
                    pop     edi;
                    pop     esi;
                    mov     al, tryHoodOpenAnswer;
                    pop     ebx;
                    retn    4;
                }
            }
        };

        void HookTestAction(std::function<bool(C_Car *)> ptr)
        {
            FunctionPointers::testAction = ptr;
            Mem::Hooks::InstallJmpPatch(0xA23088, (DWORD)NakedFunctions::CCarActionOpenHood__TestAction__Hook);
        }
    };
#endif
}
