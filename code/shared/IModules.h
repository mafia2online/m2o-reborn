#pragma once

class IGameModule
{
public:
    virtual ~IGameModule() = default;

    virtual bool Init(void*) = 0;
    virtual bool Shutdown() = 0;
};