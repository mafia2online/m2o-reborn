#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>

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
	void Stop();

	void ReadInput();
	void HandleInput();
	void ProcessInput(const std::string& command, const std::string& params);

public:
	IO::CFileLogger& GetLogger() { return m_debuglog; }
	CNetworkManager& GetNetworkManager() { return m_networkManager; }
	CServerSettings& GetServerSettings() { return m_serverSettings; }
	bool IsActive() { return m_active; }

private:
	IO::CFileLogger m_debuglog;
	CNetworkManager m_networkManager;
	CServerSettings m_serverSettings;

	std::atomic<bool>		m_active;
	std::queue<std::string>	m_inputQueue;
	std::mutex				m_inputMutex;
	std::atomic<bool>		m_readInput;
	std::thread				m_inputThread;
};