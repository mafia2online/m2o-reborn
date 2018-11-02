/** @file ISysAudio.hpp
*  @brief Game's audio class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
    class I_ISysAudio
    {

    };
    class I_SysAudio : public GameClassWrapper<I_SysAudio, I_ISysAudio, 0x1AC2388>
    {

    };
};
