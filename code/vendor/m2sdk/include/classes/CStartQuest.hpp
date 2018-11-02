#pragma once

namespace M2
{
    class ICStartQuest
    {
    public:
    };

    class C_StartQuest : public ICStartQuest
    {
    public:
        void Show(M2::C_SyncObject **pSyncObject)
        {
            Mem::InvokeFunction<Mem::call_this, void*>(0x8DFF40, this, pSyncObject);
        }
    };
};
