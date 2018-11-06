
#include <memory>

#include "Entity/EntityManager.h"

struct librg_ctx_t;

namespace nmd::net
{
    class NetInterface
    {
        // its slightly annoying to deal with smart pointers
        // and c code
        librg_ctx_t* ctx;

        std::unique_ptr<EntityManager> entitymgr;

    public:

        NetInterface();
        ~NetInterface();

        void Init();

        bool IsActive() const
        {
            return ctx;
        }

        bool IsConnected() const;

        void Connect(const char*, uint16_t) const;
        void Disconnect() const;

        void Process() const;
    };
}
