#include "CServerSettings.h"

const std::string	CServerSettings::DEFAULT_SETTINGS_FILENAME = "settings.xml";
const std::string	CServerSettings::DEFAULT_NAME = "Default Mafia2-Online Server";
const std::string	CServerSettings::DEFAULT_PASSWORD = "";
const std::string	CServerSettings::DEFAULT_ADDRESS = "127.0.0.1";
const short		CServerSettings::DEFAULT_PORT = 1234;

CServerSettings::CServerSettings():
	m_name(CServerSettings::DEFAULT_NAME),
	m_password(CServerSettings::DEFAULT_PASSWORD),
	m_address(CServerSettings::DEFAULT_ADDRESS),
	m_port(CServerSettings::DEFAULT_PORT)
{
}

CServerSettings::~CServerSettings()
{
}

bool CServerSettings::LoadFile(const std::string& file)
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

	element = this->GetOrCreateElement(doc, doc, "server");
	m_name = this->GetOrCreateElementString(doc, *element, "name", CServerSettings::DEFAULT_NAME);
	m_password = this->GetOrCreateElementString(doc, *element, "password", CServerSettings::DEFAULT_PASSWORD);
	m_address = this->GetOrCreateElementString(doc, *element, "address", CServerSettings::DEFAULT_ADDRESS);
	m_port = std::stoi(this->GetOrCreateElementString(doc, *element, "port", std::to_string(CServerSettings::DEFAULT_PORT)));

	if (m_port < 0 || m_port > 65535){
		//TODO: Dump error for port
		m_port = CServerSettings::DEFAULT_PORT;
	}
	if (m_name.length() < 0) {
		//TODO: Dump error for name
		m_name = CServerSettings::DEFAULT_NAME;
	}
	if (m_address.length() < 0) {
		//TODO: Dump error for address
		m_address = CServerSettings::DEFAULT_ADDRESS;
	}
	this->SaveFile(file);
	return true;
}

bool CServerSettings::SaveFile(const std::string& file)
{
	tinyxml2::XMLDocument	doc;
	tinyxml2::XMLError		saveResult = tinyxml2::XMLError::XML_ERROR_COUNT;
	tinyxml2::XMLElement	*element = this->GetOrCreateElement(doc, doc, "server");

	this->GetOrCreateElement(doc, *element, "name")->SetText(m_name.c_str());
	this->GetOrCreateElement(doc, *element, "password")->SetText(m_password.c_str());
	this->GetOrCreateElement(doc, *element, "address")->SetText(m_address.c_str());
	this->GetOrCreateElement(doc, *element, "port")->SetText(std::to_string(m_port).c_str());
	saveResult = doc.SaveFile(file.c_str());

	if (saveResult != tinyxml2::XMLError::XML_SUCCESS)
	{
		//TODO: Dump error for being unable to save file
		return false;
	}
	return true;
}