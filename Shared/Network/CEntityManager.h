#pragma once

#include <map>
#include "CNetworkEntity.h"

class CEntityManager
{
public:
	CEntityManager() = default;
	~CEntityManager();

	bool							HasEntity(int entityID) const;
	CNetworkEntity					*FindEntity(int entityID) const;

	int								GetPoolSize() const;
	std::map<int, CNetworkEntity*>	GetEntities() const;

protected:
	std::map<int, CNetworkEntity*>	m_entities;
};