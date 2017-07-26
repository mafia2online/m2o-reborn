// Copyright ReGuider Team, 2016-2017
//
#include <librg/core.h>
#include <librg/resources.h>

using namespace librg;

bool librg::resources::stop(std::string name)
{
    if (!resources::exists(name)) {
        return false;
    }

    auto resource = resources::get(name);

    if (!resource->running) {
        core::error("Cannot stop resource, its already stopped!");
        return false;
    }

    core::log("Stopping resource '%s'!", name.c_str());
    sq_close(resource->vm);

    resource->running = false;

    return true;
}
