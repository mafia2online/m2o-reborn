/** @file CPlaylistsDB.cpp
*  @brief Game's radio Class
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

#include "CHashName.hpp"

namespace M2
{
    class ICPlaylistsDB
    {
    public:
        void *m_pVFTable;
    };

    class C_PlaylistsDB : public GameClassWrapper<C_PlaylistsDB, ICPlaylistsDB, 0x1CC2888>
    {
    public:
        //Exactly method is "Get" but due to wrappers, cannot name it like this
        int GetPlaylist(C_HashName *name)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0xB1AD90, this, name, 1);
        }
    };
};
