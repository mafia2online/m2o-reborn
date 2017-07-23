#include "CClientSettings.h"

const std::string	CClientSettings::DEFAULT_SETTINGS_FILENAME = "settings.xml";
const std::string	CClientSettings::DEFAULT_PLAYERNAME = "M2O_Player";

CClientSettings::CClientSettings() :
	m_playerName(CClientSettings::DEFAULT_PLAYERNAME)
{
}

CClientSettings::~CClientSettings()
{
}

bool CClientSettings::LoadFile(const std::string& file)
{
	tinyxml2::XMLDocument	doc;
	tinyxml2::XMLElement	*element;
	tinyxml2::XMLError		loadResult = doc.LoadFile(file.c_str());

	std::ifstream			currentFile(file, std::ios_base::in);

	if (loadResult != tinyxml2::XMLError::XML_SUCCESS) {

		//TODO: Dump for not found config file

		if (this->SaveFile(file) == true)
		{
			//TODO: Dump for newly created config file
		}
		return true;
	}

	element = this->GetOrCreateElement(doc, doc, "client");
	m_playerName = this->GetOrCreateElementString(doc, *element, "playername", CClientSettings::DEFAULT_PLAYERNAME);

	if (m_playerName.length() < 0) {
		//TODO: Dump error for playername
		m_playerName = CClientSettings::DEFAULT_PLAYERNAME;
	}
	this->SaveFile(file);
	return true;
}

bool CClientSettings::SaveFile(const std::string& file)
{
	tinyxml2::XMLDocument	doc;
	tinyxml2::XMLError		saveResult = tinyxml2::XMLError::XML_ERROR_COUNT;
	tinyxml2::XMLElement	*element = this->GetOrCreateElement(doc, doc, "client");

	this->GetOrCreateElement(doc, *element, "playername")->SetText(m_playerName.c_str());
	saveResult = doc.SaveFile(file.c_str());

	if (saveResult != tinyxml2::XMLError::XML_SUCCESS)
	{
		//TODO: Dump error for being unable to save file
		return false;
	}
	return true;
}