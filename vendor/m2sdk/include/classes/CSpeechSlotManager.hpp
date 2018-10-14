/** @file CSpeechSlotManager.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
    class ICSpeechSlotManager
    {

    };

    class C_SpeechSlotManager : public GameClassWrapper<C_SpeechSlotManager, ICSpeechSlotManager, 0x1CC8138>
    {
    public:
        void LoadStage(int stageID)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xBA66D0, this, stageID);
        }
    };
};
