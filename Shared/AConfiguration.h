#pragma once

#include <string>
#include <Libraries\tinyxml2\tinyxml2.h>

class AConfiguration
{
public:
	AConfiguration() = default;
	virtual ~AConfiguration() = default;

	virtual bool	LoadFile(const std::string& file) = 0;
	virtual bool	SaveFile(const std::string& file) = 0;

protected:
	tinyxml2::XMLElement*	GetOrCreateElement(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& parentNode, const std::string& elementName)											const;
	std::string				GetOrCreateElementString(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& parentNode, const std::string& elementName, const std::string& defaultValue)	const;
};