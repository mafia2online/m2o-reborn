#pragma once

#include <enet\enet.h>

#include "CNetworkEntity.h"

class CNetworkPlayer : public CNetworkEntity
{
public:
	CNetworkPlayer();
	CNetworkPlayer(ENetPeer*);
	~CNetworkPlayer();

	ENetPeer	*GetNetworkPeer() const { return m_peer; }

private:
	ENetPeer	*m_peer;
};