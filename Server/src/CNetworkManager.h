#pragma once

#include <Shared\Common.h>
#include <enet\enet.h>

class CNetworkManager : public Singleton<CNetworkManager>
{
public:
	CNetworkManager();
	~CNetworkManager();

	bool Init();
	void Pulse();

private:
	ENetAddress	m_address;
	ENetHost	*m_host;
};