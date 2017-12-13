/**
 * @file CHuman2CarWrapper.hpp
 * @brief Wrapper for human and car relation
 * @author DavoSK, Inlife
 */

#pragma once
#include "CommonHeaders.h"

class  C_Human2CarWrapper
{
public:
    C_Human2CarWrapper()
    {
        DWORD functionAddress = 0x009560F0;

        __asm
        {
            mov ecx, this
            call functionAddress
        }
    }


    void Init(M2::C_Actor* actor)
    {
        DWORD functionAddress = 0x00956120;

        __asm
        {
            mov ecx, this
            push actor
            call functionAddress
        }

    }

    float GetSpeedFloat()
    {
        DWORD functionAddress = 0x00940FE0;

        __asm
        {
            mov ecx, this
            call functionAddress
        }
    }

    bool IsCabriolet()
    {
        DWORD functionAddress = 0x00940320;

        __asm
        {
            mov ecx, this
            call functionAddress
        }
    }
};
