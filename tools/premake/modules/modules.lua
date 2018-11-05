
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

smodule = function(inputname)
	local modulef = dofile('modules/' .. inputname .. '/module.lua')
	if modulef then
		component {
			name = 'vendor:' .. inputname,
			vendor = modulef,
			rawName = inputname
		}
	end
end

smod_ref = function(inputname)
	local abs_path = path.getabsolute('')
	local mod_path = abs_path .. '/modules/' .. inputname
	
	includedirs
	{
		mod_path .. "/include",
	}
	
	links
	{
		inputname,
	}
end

v_ref = function(v_path)
	local abs_path = path.getabsolute('')
	local mod_path = abs_path .. '/vendor/' .. v_path
	
	includedirs
	{
		mod_path,
	}
	
	links
	{
		--inputname,
	}
end

do_modules = function()
	local changed = false
	
	for _, comp in ipairs(components) do
	
		if comp.vendor and comp.vendor.run and not comp.ran then
			comp.ran = true
			changed = true
			
			local abs_path = path.getabsolute('')
			local mod_path = abs_path .. '/modules/' .. comp.rawName

			project(comp.rawName)

			if comp.vendor.include then
				comp.vendor.include()
			end

			language "C++"
			kind "SharedLib"
			optimize "Speed"
			
			if not string.find(comp.rawName, 'sv') then
				print('(client) submodule : ' .. comp.rawName)
			else
			
			    if os.host() == "linux" then
				
					linkoptions { "-ldl -pthread -fvisibility=hidden" }
					targetdir ("../bin/" .. _OPTIONS['type'] .. "_sv_unix/%{cfg.buildcfg}/")
				else
					targetdir ("../bin/" .. _OPTIONS['type'] .. "_sv/%{cfg.buildcfg}/")
				end
				
				print('(server) submodule : ' .. comp.rawName)
			end
			
			defines {(string.upper("compiling_" .. comp.rawName:gsub("%-", "_")))}
			
			-- mount "default" files and directorys
			files
			{
				mod_path .. "/src/**.cpp",
				mod_path .. "/src/**.cc",
				mod_path .. "/src/**.h",
				mod_path .. "/include/**.h",
			}
			
			includedirs
			{ 
				mod_path .. "/include", 
				abs_path .. "/shared",
				
				abs_path .. "/vendor/nomad-common",
				abs_path .. "/vendor/nomad-io",
				abs_path .. "/vendor/minhook",
			}
			
			links
			{
				"Shared",
				"nomad-common",
				"minhook",
			}
			
			libdirs 
			{
				abs_path .. "/../../bin/vendor/%{cfg.buildcfg}/",
			}
			
			
			--targetdir "../bin/%{cfg.buildcfg}/" .. _OPTIONS['type']
			
			comp.vendor.run()
		end
	end

	if changed then
		do_modules()
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