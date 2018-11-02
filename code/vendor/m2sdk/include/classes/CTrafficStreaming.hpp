/** @file CTrafficStreaming.cpp
*  @brief Game's traffic streaming wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
    class ICTrafficStreaming
    {
    public:
    };

    class C_TrafficStreaming : public GameClassWrapper<C_TrafficStreaming, ICTrafficStreaming, 0x1CC163C>
    {
    public:
        C_Model *CModel__AllocateModel(const char *path)
        {
            return Mem::InvokeFunction<Mem::call_this, C_Model*>(0xACFF50, this, path);
        }

        bool CModel__LoadModel(const char *model)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x0AE12C0, this, model);
        }

        void CModel__ReleaseModel()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xAD7C10, this);
        }

        void * CreateSDSWrapper(int E_SlotType, const char *path)
        {
            return Mem::InvokeFunction<Mem::call_this, void*>(0xADFB80,this, E_SlotType, path);
        }
    };
};
