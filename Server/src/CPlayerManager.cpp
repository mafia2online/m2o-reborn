#include "CPlayerManager.h"

CPlayerManager::CPlayerManager()
{

}

CPlayerManager::~CPlayerManager()
{

}

void CPlayerManager::CreateEntity(int networkID, ENetPeer *peer)
{
	m_entities[networkID] = new CNetworkPlayer(peer);
	
	printf("A new client connected from %x:%u.\n", dynamic_cast<CNetworkPlayer *>(m_entities[networkID])->GetNetworkPeer()->address.host, dynamic_cast<CNetworkPlayer *>(m_entities[networkID])->GetNetworkPeer()->address.port);
	printf("PoolSize = %d\n", GetPoolSize());
}

void CPlayerManager::DeleteEntity(int networkID)
{
	auto it = m_entities.find(networkID);

	if (it != m_entities.end())
	{
		m_entities.erase(it);
		printf("PoolSize = %d\n", GetPoolSize());
		//TODO: Notify other client of current player disconnection
	}
}