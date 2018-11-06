
struct librg_ctx_t;
struct librg_entity_t;
class C_Player2;

namespace nmd::net {
class EntityManager {

    librg_ctx_t *ctx;
    librg_entity_t *local_player;

public:
    EntityManager(librg_ctx_t *);

    void Init();

    void CreateLocalNetPlayer(librg_entity_t *p, C_Player2*);
};

} // namespace nmd::net
