#pragma once

#include <string>

namespace Utility
{
    std::wstring MakeAbsolutePathW(const std::wstring &rel_to);
    std::string MakeAbsolutePathA(const std::string &rel_to);
}
