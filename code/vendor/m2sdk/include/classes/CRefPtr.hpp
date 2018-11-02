/** @file CRefPtr.hpp
*  @brief Game's generic class
*
*
*  @author Tuxick
*/
#pragma once
namespace M2
{
    template<typename T>
    class C_RefPtr {
    public:
        T * ptr = nullptr;
        uint32_t countUsed = 0;
    };
};
