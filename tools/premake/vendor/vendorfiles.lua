-- thanks to the CFX guys for this.
local components = { }

component = function(name)
    -- add to the list
    table.insert(components, name)

    -- return a function to allow table merging for additional parameters
    return function(t)
        for k, v in pairs(t) do
            name[k] = v
        end
    end
end

vendor_submodule = function(inputname)
    local vendorTable = dofile(inputname .. '.lua')
    if vendorTable then
        component {
            name = 'vendor:' .. inputname,
            vendor = vendorTable,
            rawName = inputname
        }
    end
end

do_vendor = function()
    local changed = false

    for _, comp in ipairs(components) do

        if comp.vendor and comp.vendor.run and not comp.ran then
            comp.ran = true
            changed = true

            print('vendorfile : ' .. comp.rawName)

            project(comp.rawName)

            if comp.vendor.include then
                comp.vendor.include()
            end

            comp.vendor.run()
        end
    end

    if changed then
        do_vendor()
    end
end

function files_project(name)
    return function(f)
        local t = {}

        for k, file in ipairs(f) do
            table.insert(t, name .. file)
        end

        files(t)
    end
end

if os.target() == "windows" then
    vendor_submodule 'cefwrapper'
end
