
struct librg_ctx_t;
struct librg_entity_t;
struct librg_event_t;

class C_Player2;

namespace nmd::net {
class EntityManager {

    librg_ctx_t *ctx;
    librg_entity_t *local_player;

public:

    EntityManager(librg_ctx_t *);

    void Init() const;

    void CreateLocalNetPlayer(librg_entity_t *p, C_Player2*);

    // events
    static void CreateCar(librg_event_t*);
    static void DestroyCar(librg_event_t*);

    static void CreatePed(librg_event_t*);
    static void DestroyPed(librg_event_t*);

    inline librg_entity_t *GetLocalNetPlayer() const
    {
        return local_player;
    }

    void Update();
};

} // namespace nmd::net
