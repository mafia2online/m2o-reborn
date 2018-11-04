#include <memory>

#include <m2o_config.h>

struct librg_ctx_t;

namespace nmd::net
{
    class NetInterface
    {
        // its slightly annoying to deal with smart pointers
        // and c code
        librg_ctx_t* ctx;

    public:

        NetInterface();
        ~NetInterface();

        void Init() const;

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
