#include "CCore.h"

CCore::CCore() : m_debuglog(true, true)
{

}

CCore::~CCore()
{

}

bool CCore::Init()
{
	if (m_networkManager.Init() == false) {
		return false;
	}

	printf("Server started\n");

	while (true) {
		m_networkManager.Pulse();
	}
}