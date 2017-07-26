// Copyright ReGuider Team, 2016-2017
//
#include <librg/resources.h>
#include <librg/scripting/scripting.hpp>

using namespace librg;

bool librg::resources::start(std::string name)
{
    if (!resources::exists(name)) {
        return false;
    }

    auto resource = resources::get(name);

    if (resource->running) {
        core::error("cannot start resource, its already running!");
        return false;
    }

    core::log("starting resource '%s'!", name.c_str());

    // setup data
    resource->running = true;
    resource->vm = sq_open(1024);

    // load all scripting bindings
    scripting::install(resource->vm);

    // iterate over each script and start it
    // and clean up filename string pointer
    for (auto script : resource->scripts) {
        if (script.type == resources::script_client) continue;

        try {
            Sqrat::Script vscript = Sqrat::Script(resource->vm);
            vscript.CompileFile(script.filename.c_str());
            vscript.Run();
        }
        catch(Sqrat::Exception e) {
            core::error("script loading exception: %s", e.Message().c_str());
        }
    }

    return true;
}
