// Copyright ReGuider Team, 2016-2017
//
#include <enet/enet.h>

#include <librg/core.h>
#include <librg/events.h>
#include <librg/entities.h>
#include <librg/network.h>
#include <librg/streamer.h>

#include <librg/components/client.h>
#include <librg/components/server_owned.h>
#include <librg/components/client_streamable.h>

using namespace librg;

/**
 * Regulated ticker, should be used
 * to send regular messages to all connected clients
 */
void streamer::update()
{
    if (core::is_client()) {
        uint16_t amount = 0;
        auto data = network::bitstream_t();

        data.write_uint16(network::client_streamer_update);
        data.write_uint16(0); // amount of entities to be sent (updates)

        entities->each<client_streamable_t, streamable_t>([&data, &amount](entity_t entity, client_streamable_t &client, streamable_t &streamable) {
            auto transform      = entity.component<transform_t>();
            auto server_owned   = entity.component<server_owned_t>();
            auto subdata        = network::bitstream_t();

            events::trigger(events::on_client_stream_entity, new events::event_bs_entity_t(
                &subdata, entity, server_owned->guid, streamable.type
            ));

            subdata.write(*transform);

            data.write_uint64(server_owned->guid);
            data.write_uint32(subdata.raw_size());

            // write sub-bitstream to main bitstream
            data.write(subdata.raw(), subdata.raw_size());

            amount++;
        });

        if (amount < 1) {
            return;
        }

        data.write_at(amount, sizeof(uint16_t));

        for (auto pair : network::connected_peers) {
            enet_peer_send(pair.first, 1, enet_packet_create(data.raw(), data.raw_size(), ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT));
        }

        return;
    }

    entities->each<client_t, streamable_t>([](entity_t player, client_t &client, streamable_t &streamable) {
        if (!client.active) return;

        // copy last to local last, alias next snapshot as last and clear it
        auto next_snapshot = &client.last_snapshot;
        auto last_snapshot =  client.last_snapshot;

        next_snapshot->clear();
        auto queue = streamer::query(player);

        uint16_t created_entities = 0;
        uint16_t updated_entities = (uint16_t) queue.size();

        // create data and write inital stuff
        network::bitstream_t for_create;
        network::bitstream_t for_update;

        for_create.write_uint16(network::entity_create);
        for_create.write_uint16(created_entities);

        for_update.write_uint16(network::entity_update);
        for_update.write_uint16(updated_entities);

        // add entity creates and updates
        for (auto entity : queue) {
            uint64_t guid = entity.id().id();

            auto streamable = entity.component<streamable_t>();
            auto transform  = entity.component<transform_t>();
            auto cli_stream = entity.component<client_streamable_t>();

            // write create
            if (last_snapshot.erase(guid) == 0) {
                created_entities++;
                updated_entities--;

                for_create.write_uint64(guid);
                for_create.write_uint8(streamable->type);
                for_create.write(*transform);

                events::trigger(events::on_create, new events::event_bs_entity_t(
                    &for_create, entity, guid, streamable->type
                ));
            }
            else {
                // if this entity is client streamable and this client is owner
                if (cli_stream && cli_stream->peer == client.peer) {
                    updated_entities--;
                }
                // write update
                else {
                    for_update.write_uint64(guid);
                    for_update.write_uint8(streamable->type);
                    for_update.write(*transform);

                    events::trigger(events::on_update, new events::event_bs_entity_t(
                        &for_update, entity, guid, streamable->type
                    ));
                }
            }

            next_snapshot->insert(std::make_pair(guid, true));
        }

        for_create.write_at(created_entities, sizeof(uint16_t));
        for_update.write_at(updated_entities, sizeof(uint16_t));

        for_create.write_uint16(last_snapshot.size());

        // add entity removes
        for (auto pair : last_snapshot) {
            for_create.write_uint64(pair.first);

            // skip calling callback, if the entity is destroyed already.
            if (!entities->valid((entityx::Entity::Id)pair.first)) continue;

            auto entity     = entities->get(entity_t::Id(pair.first));
            auto streamable = entity.component<streamable_t>();
            auto type       = streamable ? streamable->type : -1;

            events::trigger(events::on_remove, new events::event_bs_entity_t(
                &for_create, entity, pair.first, type
            ));
        }

        enet_peer_send(client.peer, 0, enet_packet_create( for_create.raw(), for_create.raw_size(), ENET_PACKET_FLAG_RELIABLE ));
        enet_peer_send(client.peer, 1, enet_packet_create( for_update.raw(), for_update.raw_size(), ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT ));
    });

    /**
     * Rebuilding entity tree
     * TODO(inlife): move to a separate thread and place
     */
    streamer::clear();

    entities->each<streamable_t>([](entity_t entity, streamable_t& streamable) {
        streamer::insert(entity);
    });
}
