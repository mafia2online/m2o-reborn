/** @file CDoor.cpp
*  @brief Game's doors wrapper
*
*
*  @author DavoSK
*/

#include <functional>

#pragma once

namespace M2
{
    struct S_ContactEventInfo
    {

    };

    enum E_DoorContactType
    {
        CONTACT_TYPE_TOUCH = 10,
        CONTACT_TYPE_USE = 42 //Either kick or open
    };
    class C_Door
    {
    public:

        void AILock()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x44A700, this);
        }

        void AIUnlock()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x44A740, this);
        }

        void Close()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4EAD30, this);
        }

        void DisableAction()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x44A850, this);
        }

        void EnableAction()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x44A7E0, this);
        }

        bool IsLocked()
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x44A780, this);
        }

        void Kick(const Vector3 & position)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4F5110, this, position);
        }

        void Lock()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4F10C0, this);
        }

        void Open(const Vector3 & position, bool unk1, unsigned int unk2)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4EAC80, this, &position, unk1, unk2);
        }

        void StartLockpick()
        {
            Mem::InvokeFunction<Mem::call_this, bool>(0x4F1140, this);
        }

        void StopLockpick()
        {
            Mem::InvokeFunction<Mem::call_this, bool>(0x4F1160, this, true);
        }

        void ToggleOpen(const Vector3 & position, bool unk1)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4EAD70, this, position, unk1);
        }

        void Unlock()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4F10F0, this);
        }
    };

#ifdef MAFIA_SDK_IMPLEMENTATION

    namespace C_Door_Hooks
    {
        void HookSolveContact(std::function<void(C_Door *, S_ContactEventInfo const&, E_DoorContactType)>);
        DWORD HookSolveContact__Return;

        namespace FunctionPointers
        {
            std::function<void(C_Door *, S_ContactEventInfo const&, E_DoorContactType)> solveContact;
        }

        namespace Functions
        {
            inline void SolveContact(C_Door *thisInstance, S_ContactEventInfo const& ev, E_DoorContactType contactType)
            {
                if (FunctionPointers::solveContact != nullptr) {
                    FunctionPointers::solveContact(thisInstance, ev, contactType);
                }
            }
        }

        namespace NakedFunctions
        {
            __declspec(naked) void SolveContact()
            {
                __asm {
                    sub esp, 10h;
                    mov eax, [ecx + 358h];

                    pushad;
                }
                
                static C_Door *instance = nullptr;
                static S_ContactEventInfo info;
                static void *infoPtr = nullptr;
                static E_DoorContactType eventType;

                __asm {
                    mov instance, ecx;

                    mov ecx, [esp + 24];
                    mov infoPtr, ecx;

                    mov eax, [esp + 28];
                    mov eventType, eax;
                }

                if (infoPtr) {
                    memcpy(&info, infoPtr, sizeof infoPtr);
                }

                Functions::SolveContact(instance, info, eventType);

                __asm {
                    popad;
                    jmp[HookSolveContact__Return];
                }
            }
        }

        void HookSolveContact(std::function<void(C_Door *, S_ContactEventInfo const& ev, E_DoorContactType contactType)> ptr)
        {
           FunctionPointers::solveContact = ptr;

           HookSolveContact__Return = Mem::Hooks::InstallNotDumbJMP(0x4F1190, (M2_Address)NakedFunctions::SolveContact, 9);
        }
    }
#endif
};
