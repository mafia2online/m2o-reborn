#pragma once

#include <cstdint>
#include <string>

// fixes the C4307 warning being thrown everywhere
// credits to mindriot for the idea (https://stackoverflow.com/a/37661559/3676210)
#define FNV1_HASH(type,data,value,prime) \
static_cast<type>( \
    (static_cast<uint64_t>(value) * prime) ^ static_cast<type>(data) \
)

template <typename TNumber, const TNumber Prime, const TNumber OffsetBasis>
struct basic_fnv_1 {
    using type = TNumber;

    static constexpr type prime = Prime;
    static constexpr type offset_basis = OffsetBasis;

    type operator()(const std::string &text) const {
        type hash = offset_basis;

        for (auto it = text.cbegin(), end = text.cend(); it != end; it++)
        {
            hash *= prime;
            hash ^= *it;
        }

        return hash;
    }

    inline static constexpr type hash(const char *str, size_t length, type value = offset_basis) {
        return (length == 0u) ? value : hash(str + 1, length - 1, FNV1_HASH(type, str[0], value, prime));
    }
};

using fnv_1_32 = basic_fnv_1<uint32_t, 0x1000193u, 0x811C9DC5u>;
using fnv_1_64 = basic_fnv_1<uint64_t, 0x100000001B3u, 0xCBF29CE484222325u>;

inline constexpr fnv_1_32::type operator "" _fnv1_32(const char *str, size_t length) {
    return fnv_1_32::hash(str, length);
}

inline constexpr fnv_1_64::type operator "" _fnv1_64(const char *str, size_t length) {
    return fnv_1_64::hash(str, length);
}

// remove temporary macro
#undef FNV1_HASH
