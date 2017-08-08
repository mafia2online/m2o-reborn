// Copyright ReGuider Team, 2016-2017
//
#include <librg/streamer.h>
#include <librg/entities.h>
#include <librg/components/client_streamable.h>

using namespace librg;

void streamer::client::set(entity_t entity, network::peer_t* peer)
{
    auto component = entity.component<client_streamable_t>();

    // replace current entity owner
    if (component) {
        if (component->peer == peer) {
            return;
        }

        component->peer = peer;
    }
    // attach new entity owner
    else {
        component = entity.assign<client_streamable_t>(peer);
    }

    network::msg(network::client_streamer_add, peer, [&entity](network::bitstream_t* message) {
        message->write_uint64(entity.id().id());
    });
}

void streamer::client::remove(entity_t entity)
{
    auto component = entity.component<client_streamable_t>();

    if (!component) {
        return;
    }

    network::msg(network::client_streamer_remove, component->peer, [&entity](network::bitstream_t* message) {
        message->write_uint64(entity.id().id());
    });

    entity.remove<client_streamable_t>();
}
