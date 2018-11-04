#define LIBRG_IMPLEMENTATION

#include <librg.h>
#include <librg_ext.h>


#include <Net/NetInterface.h>
#include "m2o_types.h"

namespace nmd::net
{
    NetInterface::NetInterface()
    {
        ctx = new librg_ctx_t;
        memset(ctx, 0, sizeof(librg_ctx_t));
    }

    NetInterface::~NetInterface()
    {
        delete ctx;
    }

    void NetInterface::Init() const
    {
        // set librg config
        ctx->tick_delay = 1000.0f / M2O_TICKRATE_CLIENT;
        ctx->mode = LIBRG_MODE_CLIENT;
        ctx->world_size = zplm_vec3f(5000.0f, 5000.0f, 0);
        ctx->max_entities = M2O_ENTITY_LIMIT;
        ctx->user_data = (void*)this;

        librg_option_set(LIBRG_PLATFORM_ID, M2O_PLATFORM_ID);
        librg_option_set(LIBRG_PLATFORM_BUILD, M2O_VERSION_NUMBER);
        librg_option_set(LIBRG_PLATFORM_PROTOCOL, M2O_VERSION_PROTOCOL);

        librg_init(ctx);

        librg_event_add(ctx, LIBRG_CONNECTION_REQUEST, [](librg_event_t* event)
        {
            // TODO: password sending
        });

        librg_event_add(ctx, LIBRG_CONNECTION_ACCEPT, [](librg_event_t* event)
        {
            printf("[net] connected to the server\n");

            // TODO : enable sound

            /* setup default timeout */
            enet_peer_timeout(event->peer, 10, 5000, 10000);

            //  mod.player = event->entity;
            //  mod.player->user_data = m2o_ped_alloc(M2::C_Game::Get()->GetLocalPed());

            //  mod_nickname_set("the playah");
            //  mod_respawn();
        });

        librg_event_add(ctx, LIBRG_CONNECTION_REFUSE, [](librg_event_t* event)
        {

        });

        librg_event_add(ctx, LIBRG_CONNECTION_DISCONNECT, [](librg_event_t* event)
        {
            printf("[net] disconnected from the server\n");
            //   M2::Wrappers::lua::Execute("game.game:SoundFadeOut(1000)");


            //   M2::C_Game::Get()->GetLocalPed()->LockControls(true);
            // object->SetPosition(zplm_vec3_zero()); // creates black textures :O

            // if we have been disconnected before being connected
        });
    }

    bool NetInterface::IsConnected() const
    {
        return librg_is_connected(ctx);
    }

    void NetInterface::Connect(const char* host, uint16_t port) const
    {
        librg_address_t addr = {port, const_cast<char*>(host)};
        librg_network_start(ctx, addr);
    }

    void NetInterface::Disconnect() const
    {
        librg_network_stop(ctx);
    }

    void NetInterface::Process() const
    {
        librg_tick(ctx);
    }
}
