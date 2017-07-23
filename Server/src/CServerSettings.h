#pragma once

#include <Shared\AConfiguration.h>
#include <Shared\CSignelton.h>
#include <Libraries\tinyxml2\tinyxml2.h>

#include <string>
#include <ostream>
#include <fstream>

class CServerSettings : public AConfiguration, public Singleton<CServerSettings>
{
public:
	static const std::string	DEFAULT_SETTINGS_FILENAME;
	static const std::string	DEFAULT_NAME;
	static const std::string	DEFAULT_PASSWORD;
	static const std::string	DEFAULT_ADDRESS;
	static const short			DEFAULT_PORT;

public:
	CServerSettings();
	~CServerSettings();

	bool LoadFile(const std::string& file);
	bool SaveFile(const std::string& file);

	std::string	GetServerName() { return m_name; }
	std::string	GetServerPassword() { return m_password; }
	std::string GetServerAddress() { return m_address; }
	int	GetServerPort() { return m_port; }

private:
	std::string		m_name;
	std::string		m_password;
	std::string		m_address;
	int				m_port;
};