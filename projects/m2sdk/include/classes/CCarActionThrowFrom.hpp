
#include <functional>

#pragma once

namespace M2
{
    class ICCarActionThrowFrom
    {

    };

    class C_CarActionThrowFrom : public ICCarActionThrowFrom
    {

    };

#ifdef MAFIA_SDK_IMPLEMENTATION
    namespace C_CarActionThrowFrom_Hooks
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
            void __declspec(naked) CCarActionThrowFrom__TestAction__Hook()
            {
                __asm
                {
                    mov tryCar, esi;
                }

                tryAnswer = Functions::TestAction(tryCar);

                __asm {
                    pop edi;
                    mov al, tryAnswer;
                    pop esi;
                    retn 4;
                }
            }
        };

        void HookTestAction(std::function<bool(C_Car *)> ptr)
        {
            FunctionPointers::testAction = ptr;
            Mem::Hooks::InstallJmpPatch(0xA3F173, (DWORD)NakedFunctions::CCarActionThrowFrom__TestAction__Hook);
        }
    };
#endif
}
