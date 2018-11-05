#define LIBRG_IMPLEMENTATION

#include <librg.h>
#include <librg_ext.h>

#include <m2o_config.h>

#include <Net/NetInterface.h>

#include <m2/entity/C_Game.h>

namespace nmd::net {
NetInterface::NetInterface() 
    {
    ctx = new librg_ctx_t;
    memset(ctx, 0, sizeof(librg_ctx_t));
}

NetInterface::~NetInterface() { delete ctx; }

void NetInterface::Init() {
    // set librg config
    ctx->tick_delay = 1000.0f / M2O_TICKRATE_CLIENT;
    ctx->mode = LIBRG_MODE_CLIENT;
    ctx->world_size = zplm_vec3f(5000.0f, 5000.0f, 0);
    ctx->max_entities = M2O_ENTITY_LIMIT;

    librg_option_set(LIBRG_PLATFORM_ID, M2O_PLATFORM_ID);
    librg_option_set(LIBRG_PLATFORM_BUILD, M2O_VERSION_NUMBER);
    librg_option_set(LIBRG_PLATFORM_PROTOCOL, M2O_VERSION_PROTOCOL);

    librg_init(ctx);

    // = std::make_unique<EntityManager>(ctx);

    // investigate crash when using unique_ptr
    // very temp !
    ctx->user_data = new EntityManager(ctx);

    librg_event_add(ctx, LIBRG_CONNECTION_REQUEST, [](librg_event_t *event) {
        // TODO: password sending
        // v8 binding stuff ?
    });

    librg_event_add(ctx, LIBRG_CONNECTION_ACCEPT, [](librg_event_t *event) {
        printf("[net] connected to the server\n");

        // TODO : enable sound

        /* setup default timeout */
        enet_peer_timeout(event->peer, 10, 5000, 10000);

        auto player = C_Game::GetInstance()->GetLocalPlayer();

        // temp debug :
        player->SetPos({240.641052f, 699.223755f, -24.153996f});
        player->SetColor(255);

        ((EntityManager *)event->ctx->user_data)->CreateLocalNetPlayer(event->entity, player);
    });

    librg_event_add(ctx, LIBRG_CONNECTION_REFUSE, [](librg_event_t *event) {

    });

    librg_event_add(ctx, LIBRG_CONNECTION_DISCONNECT, [](librg_event_t *event) {
        printf("[net] disconnected from the server\n");
        //   M2::Wrappers::lua::Execute("game.game:SoundFadeOut(1000)");

        NetInterface *net = (NetInterface *)event->user_data;

        C_Game::GetInstance()->GetLocalPlayer()->LockControls(true);

        // if we have been disconnected before being connected

        // destroy local player
    });
}

bool NetInterface::IsConnected() const { return librg_is_connected(ctx); }

void NetInterface::Connect(const char *host, uint16_t port) const {
    librg_address_t addr = {port, const_cast<char *>(host)};
    librg_network_start(ctx, addr);
}

void NetInterface::Disconnect() const { librg_network_stop(ctx); }

void NetInterface::Process() const { librg_tick(ctx); }
} // namespace nmd::net
