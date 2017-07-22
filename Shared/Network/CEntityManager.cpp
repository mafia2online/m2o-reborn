#include "CEntityManager.h"

CEntityManager::~CEntityManager()
{
	for (auto entity : m_entities)
		delete entity.second;
}

bool	CEntityManager::HasEntity(int networkID) const
{
	return m_entities.find(networkID) != m_entities.end();
}

CNetworkEntity	*CEntityManager::FindEntity(int networkID) const
{
	auto it = m_entities.find(networkID);

	if (it != m_entities.end()) {
		return it->second;
	}
	else {
		return nullptr;
	}
}

int	CEntityManager::GetPoolSize() const
{
	return m_entities.size();
}

std::map<int, CNetworkEntity*>	CEntityManager::GetEntities() const
{
	return m_entities;
}