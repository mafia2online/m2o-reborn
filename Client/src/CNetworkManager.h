#pragma once

#include <Shared\Common.h>

#include <enet\enet.h>

class CNetworkManager : public Singleton<CNetworkManager>
{
public:
	CNetworkManager();
	~CNetworkManager();

	bool Init();
	bool Connect(SString host, int port, SString password);
	bool Disconnect();
	void Pulse();

private:
	ENetHost	*m_client;
	ENetAddress	m_address;
	ENetPeer	*m_peer;
};