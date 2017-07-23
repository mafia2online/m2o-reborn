#include "CCore.h"

CCore::CCore() : m_debuglog(true, true), m_readInput()
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

	m_active = true;
	m_readInput = true;
	m_inputThread = std::thread(&CCore::ReadInput, this);

	while (m_active) {
		m_networkManager.Pulse();
		HandleInput();
	}
}

void CCore::Stop()
{
	m_active = false;
	m_readInput = false;
	m_inputThread.join();
}

void CCore::ReadInput()
{
	std::string	input;
	while (m_readInput)
	{
		std::getline(std::cin, input);
		m_inputMutex.lock();
		m_inputQueue.push(input);
		m_inputMutex.unlock();
	}
}

void CCore::ProcessInput(const std::string& command, const std::string& params)
{
	if (command == "exit" || command == "quit" || command == "stop") {
		m_active = false;
	}
}

void CCore::HandleInput()
{
	if (m_inputMutex.try_lock())
	{
		while (!m_inputQueue.empty())
		{
			std::string	input = m_inputQueue.back();
			std::size_t	paramsOffset = input.find(" ") + 1;
			std::string	command = input.substr(0, paramsOffset - 1);
			std::string params = input.substr(paramsOffset);

			if (input.empty())
			{
				m_inputMutex.unlock();
				return;
			}
			ProcessInput(command, params);
			m_inputQueue.pop();
		}
		m_inputMutex.unlock();
	}
}