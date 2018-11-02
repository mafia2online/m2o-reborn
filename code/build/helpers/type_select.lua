
newoption {
	trigger		= "type",
	value 		= "TARGET",
	description = "Choose a game to target",
	allowed 	= {
		{ "w3",   "Red Dev for The witcher 3" },
		{ "cy77", "Red Dev for Cyberpunk 2077" },
	}
}

if not _OPTIONS['type'] then
	_OPTIONS['type'] = 'unknown'
end