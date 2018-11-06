
struct librg_ctx_t;


#include <Net/Entity/EntityManager.h>

namespace nmd::net {
    class NetInterface {
        // its slightly annoying to deal with smart pointers
        // and c code
        librg_ctx_t *ctx;

        EntityManager *entity_mgr;

    public:
        NetInterface();
        ~NetInterface();

        void Init();

        bool IsActive() const { return ctx; }

        bool IsConnected() const;

        void Connect(const char *, uint16_t) const;
        void Disconnect() const;

        void Process() const;
    };
} // namespace nmd::net
