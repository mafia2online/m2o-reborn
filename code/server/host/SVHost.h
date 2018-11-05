#pragma once

#include <unordered_map>

// must be kept in sync
class GameServerInterface
{
public:

    using input_poller = void(*)(std::string &str);

    virtual ~GameServerInterface() = default;

    virtual bool Initialize() = 0;
    virtual void Run() = 0;
    virtual bool Shutdown() = 0;
    virtual void SetInputPoller(input_poller) = 0;
};

class SVHost
{
    bool should_terminate;
    void *core_server;
    GameServerInterface *gs;

public:

    explicit SVHost(int, char*[]);

    bool Initialize();
    void Shutdown();

    void Run();
};
