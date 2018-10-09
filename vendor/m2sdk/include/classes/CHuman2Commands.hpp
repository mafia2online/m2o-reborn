/** @file CHuman2.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*  @author Tuxick
*/

#pragma once

namespace M2
{
    class C_Command {
    public:
        pad(C_Command, pad0, 0xC);//0x0000
        uint8_t m_iCommandID; //0x000C
    };

    class S_HumanCommandMoveDir : public C_Command
    {
    public:
        pad(S_HumanCommandMoveDir, pad0, 0x18);//0x000D4
        zpl_vec2 moveVector; //0x0028
        zpl_vec2 potentialMoveVector; //0x0030
        pad(S_HumanCommandMoveDir, pad1, 0x4);//0x0038
        float speedMultiplier; //0x003C
        pad(S_HumanCommandMoveDir, pad2, 0x10);//0x0040
        uint8_t moveSpeed; //0x0050
        pad(S_HumanCommandMoveDir, pad3, 0x3);//0x0051
        uint32_t int32_0x54; //0x0054
    }; //Size=0x0058

    class C_CommandDescription
    {
    public:
        __int32 m_iCommandPriority;     //0x0000 
        void*   m_pCommand;             //0x0004 

    }; //Size=0x0008
};
