/** @file CProgramsDB.cpp
*  @brief Game's radio Class
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

#include "CHashName.hpp"

namespace M2
{
    class ICProgramsDB
    {
    public:
        void *m_pVFTable;
    };

    class C_ProgramsDB : public GameClassWrapper<C_ProgramsDB, ICProgramsDB, 0x1CC288C>
    {
    public:
        int GetProgram(C_HashName **hash)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0xB1AE00, this, hash);
        }
    };
};
