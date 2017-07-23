#pragma once

#include <Shared\Common.h>
#include <Shared\FileLogger.h>

#include "CServerSettings.h"

#include "CNetworkManager.h"

class CCore : public Singleton<CCore>
{
public:
	CCore();
	~CCore();

	bool Init();

public:
	IO::CFileLogger& GetLogger() { return m_debuglog; }
	CNetworkManager& GetNetworkManager() { return m_networkManager; }
	CServerSettings& GetServerSettings() { return m_serverSettings; }

private:
	IO::CFileLogger m_debuglog;
	CNetworkManager m_networkManager;
	CServerSettings m_serverSettings;
};