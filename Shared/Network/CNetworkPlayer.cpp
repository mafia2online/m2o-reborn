#include "CNetworkPlayer.h"

CNetworkPlayer::CNetworkPlayer():
	CNetworkEntity(CNetworkType::NET_TYPE_PLAYER),
	m_peer(nullptr)
{
}

CNetworkPlayer::CNetworkPlayer(ENetPeer *peer) :
	CNetworkEntity(CNetworkType::NET_TYPE_PLAYER),
	m_peer(peer)
{
}

CNetworkPlayer::~CNetworkPlayer()
{
}