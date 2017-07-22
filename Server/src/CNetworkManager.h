#pragma once

#include <Shared\Common.h>

#include "CPlayerManager.h"

#include <enet\enet.h>

class CNetworkManager : public Singleton<CNetworkManager>
{
public:
	CNetworkManager();
	~CNetworkManager();

	bool Init();
	void Pulse();

private:
	CPlayerManager *m_PlayerManager;
	ENetAddress	m_address;
	ENetHost	*m_host;
};