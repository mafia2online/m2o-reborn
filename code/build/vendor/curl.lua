return {
	include = function()
		
	defines
	{
		'CURL_STATICLIB',
		'BUILDING_LIBCURL',
		'USE_IPV6',
		'CURL_DISABLE_TFTP',
		'CURL_DISABLE_FTP',
		'CURL_DISABLE_LDAP',
		'CURL_DISABLE_TELNET',
		'CURL_DISABLE_DICT',
		'CURL_DISABLE_RTSP',
		'CURL_DISABLE_POP3',
		'CURL_DISABLE_IMAP',
		'CURL_DISABLE_SMTP',
		'CURL_DISABLE_RTMP',
		'CURL_DISABLE_GOPHER',
		'CURL_DISABLE_SMB',
	}
		
	end,

	run = function()
		targetname "libcurl"
		language "C"
		kind "StaticLib"
	
		targetdir "../bin/vendor/%{cfg.buildcfg}"
	
		local base = "vendor/curl"
	
		includedirs
		{ 
			base .. "/lib",
			base .. "/include",
		}
	
		files
		{
			base .. "/lib/**.c",
			base .. "/lib/**.h",
		}
		
	end
}