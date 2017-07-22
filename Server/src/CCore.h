#pragma once

#include <Shared\Common.h>
#include <Shared\FileLogger.h>

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

private:
	IO::CFileLogger m_debuglog;
	CNetworkManager m_networkManager;
};