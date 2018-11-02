#pragma once

#include <stdint.h>

namespace Utility::Hashing
{
    // jenkins one at a time hash

    inline constexpr uint32_t JenkinsHash(const char* string)
    {
        uint32_t hash = 0;

        for (; *string; ++string)
        {
            hash += *string;
            hash += hash << 10;
            hash ^= hash >> 6;
        }

        hash += hash << 3;
        hash ^= hash >> 11;
        hash += hash << 15;

        return hash;
    }

    constexpr uint32_t val_32_const = 0x811c9dc5;
    constexpr uint32_t prime_32_const = 0x1000193;
    constexpr uint64_t val_64_const = 0xcbf29ce484222325;
    constexpr uint64_t prime_64_const = 0x100000001b3;

    inline constexpr uint32_t fnv1a_32_const(const char* const str, const uint32_t value = val_32_const) noexcept {
        return (str[0] == '\0') ? value : fnv1a_32_const(&str[1], (value ^ uint32_t(str[0])) * prime_32_const);
    }

    inline constexpr uint64_t fnv1a_64_const(const char* const str, const uint64_t value = val_64_const) noexcept {
        return (str[0] == '\0') ? value : fnv1a_64_const(&str[1], (value ^ uint64_t(str[0])) * prime_64_const);
    }

    inline constexpr uint32_t fnv1a_32_const_wide(const wchar_t* const str, const uint32_t value = val_32_const) noexcept {
        return (str[0] == '\0') ? value : fnv1a_32_const_wide(&str[1], (value ^ uint32_t(str[0])) * prime_32_const);
    }

    inline constexpr uint64_t fnv1a_64_const_wide(const wchar_t* const str, const uint64_t value = val_64_const) noexcept {
        return (str[0] == '\0') ? value : fnv1a_64_const_wide(&str[1], (value ^ uint64_t(str[0])) * prime_64_const);
    }
}
