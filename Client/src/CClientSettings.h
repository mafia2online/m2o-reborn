#pragma once

#include <Shared\AConfiguration.h>
#include <Shared\CSignelton.h>
#include <Libraries\tinyxml2\tinyxml2.h>

#include <string>
#include <ostream>
#include <fstream>

class CClientSettings : public AConfiguration, public Singleton<CClientSettings>
{
public:
	static const std::string	DEFAULT_SETTINGS_FILENAME;
	static const std::string	DEFAULT_PLAYERNAME;

public:
	CClientSettings();
	~CClientSettings();

	bool LoadFile(const std::string& file);
	bool SaveFile(const std::string& file);

	std::string	GetPlayerName() { return m_playerName; }

private:
	std::string		m_playerName;
};