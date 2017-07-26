// Copyright ReGuider Team, 2016-2017
//
#ifndef librg_streamer_h
#define librg_streamer_h

#include <librg/network.h>
#include <librg/entities.h>
#include <librg/utils/aabb.h>
#include <librg/components/streamable.h>

namespace librg
{
    void streamer_initialize();
    void streamer_terminate();

    namespace streamer
    {
        constexpr int MAX_NODES = 4;

        struct qtree_t {
            qtree_t() {}
            qtree_t(aabb_t boundary)
                : boundary(boundary), entities(), blacklistedEntities(), trees() {}

            void subdivide();
            void create_child(aabb_t boundary);
            bool insert(entity_t entity);
            bool remove(entity_t entity);
            void clear();
            void query(std::vector<entity_t> &visible, aabb_t range, entityx::ComponentHandle<streamable_t> streamable, entity_t caller);

            aabb_t boundary;
            std::vector<entity_t> entities;
            std::vector<entity_t> blacklistedEntities;
            std::vector<qtree_t> trees;
        };

        extern qtree_t _root;

        /**
         * Client-side streamer cache that stores current entities
         */
        extern std::unordered_map<uint64_t, entity_t> entity_pool;

        /**
         * Remove queue for unstreamed entities
         */
        extern std::vector<entity_t> remove_queue;

        /**
         * Cache for entities in current query
         */
        extern std::unordered_map<entity_t, std::vector<entity_t>> entity_cache;

        /**
         * Public API method for inserting an entity to the quad tree.
         * @param entity An entity to insert.
         */
        bool insert(entity_t entity);

        /**
         * Public API method for removing entity from the streamer
         * @param  entity
         * @param  soft_remove if set to true, will not destroy entity after removing from the streamer
         * @return
         */
        bool remove(entity_t entity, bool soft_remove = false);

        /**
         * Public API method for cleaning up the quad-tree.
         */
        void clear();

        /**
         * Generate and send update packets
         * for every client entity inside streamer
         */
        void update();

        /**
         * Public API method for cleaning up the quad-tree and
         * setting up a new boundary.
         * @param bounds A tree world boundary.
         */
        void clear(aabb_t bounds);

        /**
         * Public API method for querying a list of entities that should be
         * streamed.
         * @param entity An entity to be queried against.
         */
        std::vector<entity_t> query(entity_t entity);

        /**
         * Public API method for setting the global visibility of the entity.
         * @param  entity An entity to deal with.
         * @param  state  A visibility state.
         * @return        Returns zero if no change has happened, one otherwise.
         */
        bool set_visible(entity_t entity, bool state);

        /**
         * Public API method for setting visibility of the entity for a specific
         * entity target.
         *
         * Only works if the target entity has a streamable component.
         *
         * @param  target An entity target we relate entity state with.
         * @param  entity An entity we set states for.
         * @param  state  A visibility state.
         * @return        Returns zero if no change has happened, one otherwise.
         */
        bool set_visible_for(entity_t target, entity_t entity, bool state);

        /**
         * Client side streamer methods
         */
        namespace client
        {
            /**
             * Set entity to be streamed by particular client
             * @param entity
             * @param client
             */
            void set(entity_t entity, network::peer_t* peer);

            /**
             * Remove entity from client streamer
             * @param entity
             */
            void remove(entity_t entity);
        }
    }
}

#endif // librg_streamer_h
