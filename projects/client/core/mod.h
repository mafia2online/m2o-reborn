#ifndef MOD_H
#define MOD_H

class m2o_module : public M2::C_TickedModule {
public:
    void init(M2::I_TickedModuleCallEventContext &);
    void tick(M2::I_TickedModuleCallEventContext &);
    void load_start(M2::I_TickedModuleCallEventContext &);
    void load_finish(M2::I_TickedModuleCallEventContext &);
};

struct {
    librg_entity_t *player;

    // game tick props
    f64 last_update;
    f64 last_delta;

    struct {
        i32 streamed_entities;
    } stats;
} mod;

#ifdef M2O_CLIENT
static m2o_module m2o;
static librg_ctx_t *ctx;
#endif

#endif MOD_H
