/** @file CGuiGame.cpp
*  @brief Game's Main Class
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
    class ICActorActionModule
    {
    public:
        void *m_pVFTable;               //0000 - 0004
    };

    class I_ActorActionModule : public GameClassWrapper<I_ActorActionModule, ICActorActionModule, 0x1ABE8D4>
    {
    public:

        void RegisterPlayer(C_Actor* actor)
        {
            Mem::InvokeFunction<Mem::call_this, I_ActorActionModule*>(0x119E480, this, actor);
        }
    };
};
