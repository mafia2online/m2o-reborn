/**
 * @file CHuman2CarWrapper.hpp
 * @brief Wrapper for human and car relation
 * @author DavoSK, Inlife
 */

#pragma once

namespace M2
{
    class ICHuman2CarWrapper
    {

    };

    class  C_Human2CarWrapper : ICHuman2CarWrapper
    {
    public:
        C_Human2CarWrapper()
        {
            Mem::InvokeFunction<Mem::call_this, DWORD*>(0x009560F0, this);
        }

        void Init(M2::C_Actor* actor)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x00956120, this, actor);
        }

        C_Car *GetCar()
        {
            return Mem::InvokeFunction<Mem::call_this, C_Car*>(0x9235F0, this);
        }

        float GetSpeedFloat()
        {
            return Mem::InvokeFunction<Mem::call_this, float>(0x00940FE0, this);
        }

        bool IsCabriolet()
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x00940320, this);
        }
    };

#ifdef MAFIA_SDK_IMPLEMENTATION
    namespace C_Human2CarWrapper_Hooks
    {
        void HookIsFreeToGetIn(std::function<bool(C_Car *)>);

        namespace FunctionPointers
        {
            std::function<bool(C_Car *)> isFreeToGetIn;
        };

        namespace Functions
        {
            inline bool TestAction(C_Car *car)
            {
                if (FunctionPointers::isFreeToGetIn != nullptr) {
                    return FunctionPointers::isFreeToGetIn(car);
                }
                return false;
            }
        };

        namespace NakedFunctions
        {
            DWORD CHuman2CarWrapper__GetCar = 0x9235F0;
            C_Car *tryCar = nullptr;
            C_Human2CarWrapper *carWrapper;
            bool tryAnswer = false;
            void __declspec(naked) CHuman2CarWrapper__IsFreeToGetIn__Hook()
            {
                __asm {
                    mov carWrapper, esi;
                }

                if(carWrapper != nullptr){
                    tryCar = carWrapper->GetCar();
                    tryAnswer = Functions::TestAction(tryCar);
                }

                __asm {
                    mov     al, tryAnswer
                    pop     esi
                    retn    8
                }
            }
        };

        void HookIsFreeToGetIn(std::function<bool(C_Car *)> ptr)
        {
            FunctionPointers::isFreeToGetIn = ptr;
            Mem::Hooks::InstallJmpPatch(0x956143, (DWORD)NakedFunctions::CHuman2CarWrapper__IsFreeToGetIn__Hook);
        }
    };
#endif
}
