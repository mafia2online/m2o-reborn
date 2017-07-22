#pragma once

#include <Shared\Common.h>

#include <Shared\Network\CNetworkPlayer.h>
#include <Shared\Network\CEntityManager.h>

#include <enet\enet.h>

class CPlayerManager : public CEntityManager
{
public:
	CPlayerManager();
	~CPlayerManager();

	virtual void CreateEntity(int, ENetPeer*);
	virtual void DeleteEntity(int networkID);
};