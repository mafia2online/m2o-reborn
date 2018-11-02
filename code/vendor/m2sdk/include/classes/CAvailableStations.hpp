/** @file CAvailableStations.cpp
*  @brief Game's radio Class
*
*  @author Tyldar (darreugne@gmail.com)
*/

#include "CHashName.hpp"

#pragma once

namespace M2
{
    class ICAvailableStations
    {
    public:
        void *m_pVFTable;
    };

    class C_AvailableStations : public GameClassWrapper<C_AvailableStations, ICAvailableStations, 0x1CC2870>
    {
    public:
        char * GetStationByName(const char **a2, C_HashName **a3)
        {
           return Mem::InvokeFunction<Mem::call_this, char *>(0xB197D0, this, a2, a3);
        }
    };
};
