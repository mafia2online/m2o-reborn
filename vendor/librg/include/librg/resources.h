// Copyright ReGuider Team, 2016-2017
//
#ifndef librg_resources_h
#define librg_resources_h

#include <uv.h>
#include <squirrel.h>

#include <librg/core.h>

namespace librg
{
    void resources_initialize();
    void resources_terminate();

    namespace resources
    {
        enum script_type {
            script_server,
            script_client,
            script_shared,
        };

        struct script_t {
            std::string filename;
            script_type type;
        };

        struct resource_t {
            HSQUIRRELVM vm;
            std::vector<script_t> scripts;
            std::string name;
            bool running;
        };

        extern std::unordered_map<std::string, resource_t> _resources;

        /**
         * Check if resource exists
         * @param  name
         * @return state
         */
        static inline bool exists(std::string name) {
            return (_resources.find(name) != _resources.end());
        };

        /**
         * Add new resource to the storage
         * @param name
         * @param resource
         */
        static inline void add(std::string name, resource_t resource) {
            _resources.insert(std::pair<std::string, resource_t>(name, resource));
        };

        /**
         * Get reseource by name or nullptr
         * @param  name
         * @return
         */
        static inline resource_t* get(std::string name) {
            return exists(name) ? &_resources[name] : nullptr;
        };

        /**
         * Chwck if resource is running
         * @param  name
         * @return
         */
        static inline bool running(std::string name) {
            return exists(name) && get(name)->running;
        };

        /**
         * Start resource by given name
         * @param  name
         * @return result of operation
         */
        bool start(std::string name);

        /**
         * Stop resource by given name
         * @param  name
         * @return result of operation
         */
        bool stop(std::string name);

        // resource_t create(std::string name, std::vector<script_t> scripts);
        // void destroy(resource_t* resource);

    }
}

#endif // librg_resources_h
