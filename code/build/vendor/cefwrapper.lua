return {
	include = function()
		
	defines
	{
		'USING_CEF_SHARED',
		'WRAPPING_CEF_SHARED',
		'WIN32_LEAN_AND_MEAN',
		'NOMINMAX',
		'WIN32'
	}
		
	end,

	run = function()
		targetname "cefwrapper"
		language "C++"
		kind "StaticLib"
	
		-- dont question this output path!
		targetdir "../bin/vendor/%{cfg.buildcfg}"
	
		local cef_base = "vendor/cef/" .. CEF_VERSION .. "/libcef_dll/"
	
		includedirs
		{ 
			cef_base,
			cef_base .. "../",
		}
	
		-- dont question those paths.
		files
		{
			cef_base .. "/**.cpp",
			cef_base .. "/**.cc",
			cef_base .. "/**.hpp",
			cef_base .. "/**.h",
		}
		
	end
}