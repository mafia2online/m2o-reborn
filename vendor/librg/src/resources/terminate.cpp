// Copyright ReGuider Team, 2016-2017
//
#include <librg/core.h>
#include <librg/resources.h>

using namespace librg;

void librg::resources_terminate()
{
    core::log("stopping resources");

    for (auto resource : resources::_resources) {
        resources::stop(resource.first);
    }
}
