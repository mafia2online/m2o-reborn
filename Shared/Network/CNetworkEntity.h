#pragma once

enum CNetworkType
{
	NET_TYPE_NONE,
	NET_TYPE_PLAYER,
};

class CNetworkEntity
{
private:
	CNetworkType	m_type;

public:
	CNetworkEntity(CNetworkType type = CNetworkType::NET_TYPE_NONE);
	virtual ~CNetworkEntity();
};