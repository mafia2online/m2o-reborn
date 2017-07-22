#include "CCore.h"

#include "CNetworkManager.h"

CNetworkManager::CNetworkManager() :
	m_client()
{
	enet_initialize();
}

CNetworkManager::~CNetworkManager()
{
	if (m_client != NULL) {
		enet_host_destroy(m_client);
	}
	enet_deinitialize();
}

bool CNetworkManager::Init()
{
	m_client = enet_host_create(NULL, 1, 2, 57600 / 8, 14400 / 8);
	if (m_client == NULL) {
		return false;
	}

	return true;
}

bool CNetworkManager::Connect(SString host, int port, SString password)
{
	ENetEvent event;

	enet_address_set_host(&m_address, "localhost");
	m_address.port = 1234;

	m_peer = enet_host_connect(m_client, &m_address, 2, 0);
	if (m_peer == NULL) {
		return false;
	}

	if (enet_host_service(m_client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
		return true;
	}
	else {
		enet_peer_reset(m_peer);
		return false;
	}
}

bool CNetworkManager::Disconnect()
{
	enet_peer_disconnect(m_peer, 0);
	return true;
}

void CNetworkManager::Pulse()
{
	ENetEvent event;

	if (m_client == NULL) {
		return;
	}

	if (m_peer == NULL) {
		return;
	}

	while (enet_host_service(m_client, &event, 0) > 0)
	{
		//Do stuff here
	}
}