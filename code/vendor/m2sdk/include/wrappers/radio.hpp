#pragma once

#include <iostream>

namespace M2
{
    namespace Wrappers
    {
        namespace Radio
        {
            static void SetContent(const char *station, const char *program, const char *content)
            {
                Mem::InvokeFunction<Mem::call_std, int>(0xACF8B0, station, program, content);
            }
        };
    };
};
