// Copyright ReGuider Team, 2016-2017
//
#include <exception>
#include <tinyxml2.h>

#include <librg/core.h>
#include <librg/resources.h>
#include <librg/utils/fs.h>

using namespace librg;

std::unordered_map<std::string, resources::resource_t> resources::_resources;

// TODO(inlife): move to config.xml!
std::vector<std::string> resourcesList = {
    "resource-default",
    // "default",
};

std::unordered_map<std::string, resources::script_type> script_types = {
    std::make_pair<std::string, resources::script_type>("server", resources::script_server),
    std::make_pair<std::string, resources::script_type>("client", resources::script_client),
    std::make_pair<std::string, resources::script_type>("shared", resources::script_shared),
};

/**
 * [create_from_xmlstring description]
 * @param  name   [description]
 * @param  buffer [description]
 * @param  len    [description]
 * @return        [description]
 */
resources::resource_t create_from_xmlstring(std::string name, const char* buffer, size_t len)
{
    tinyxml2::XMLDocument document; document.Parse(buffer, len);
    tinyxml2::XMLElement* meta    = document.FirstChildElement("meta");

    if (!meta) {
        throw new std::exception();
    }

    resources::resource_t resource = {};
    tinyxml2::XMLElement* element = meta->FirstChildElement("script");

    // iterate over each script and add to the array
    while (element != nullptr) {
        const char* script_type = nullptr;
        script_type = element->Attribute("type");
        if (!script_type || script_type == nullptr) continue;

        resources::script_t script = {};

        script.filename = std::string(fs::path("resources", name, std::string(element->GetText())));
        script.type = (script_types.find(script_type) == script_types.end()) ? resources::script_server : script_types[script_type];

        resource.scripts.push_back(script);
        element = element->NextSiblingElement("script");
    }

    return resource;
}

/**
 * [librg::resources_initialize description]
 */
void librg::resources_initialize()
{
    if (core::is_client()) {
        return;
    }

    core::log("starting resources");

    // try to create resources folder
    int result = fs::mkdir("resources");

    if (result != 0 && result != UV_EEXIST) {
        core::error(uv_strerror((int)result));
        return;
    }

    //TODO: populate resource list

    for (auto name : resourcesList) {
        fs::read(fs::path("resources", name, "meta.xml"), [name] (fs::result_t* result) -> void {
            try {
                resources::add(name, create_from_xmlstring(name, result->content, result->length));
                resources::start(name);
            } catch (std::exception*) {
                core::error("There is no <meta> tag inside your %s file.", name.c_str());
            }
        });
    }
}
