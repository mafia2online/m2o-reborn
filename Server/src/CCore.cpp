#include "CCore.h"

CCore::CCore() : m_debuglog(true, true)
{

}

CCore::~CCore()
{

}

bool CCore::Init()
{
	if (m_serverSettings.LoadFile(CServerSettings::DEFAULT_SETTINGS_FILENAME) == false) {
		return false;
	}

	if (m_networkManager.Init(m_serverSettings.GetServerAddress(), m_serverSettings.GetServerPort()) == false) {
		return false;
	}

	printf("Server Name = %s\n", m_serverSettings.GetServerName().c_str());
	printf("Server Address = %s\n", m_serverSettings.GetServerAddress().c_str());
	printf("Server Port = %d\n", m_serverSettings.GetServerPort());
	printf("Server Password = %s\n", m_serverSettings.GetServerPassword().c_str());

	while (true) {
		m_networkManager.Pulse();
	}
}