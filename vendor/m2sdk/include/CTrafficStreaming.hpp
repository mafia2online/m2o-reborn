/** @file CTrafficStreaming.cpp
*  @brief Game's traffic streaming wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
    class ICTrafficStreaming
    {
    public:
    };

    class C_TrafficStreaming : public GameClassWrapper<C_TrafficStreaming, ICTrafficStreaming, 0x1CC163C>
    {
    public:
        void * CreateSDSWrapper(const char *path)
        {
            return Mem::InvokeFunction<Mem::call_this, void*>(0xADFB80,this, 8, path);
        }


        bool CModel__LoadModel(const char *model)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x0AE12C0, this, model);
        }
    };
};
