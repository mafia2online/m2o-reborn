#include <Windows.h>

#include "SteamWrapper.h"

SteamWrapper::SteamWrapper() :
	m_ctx()
{
}

bool	SteamWrapper::init()
{
	if (!SteamAPI_IsSteamRunning()) {
		m_lastError = STEAM_WRAPPER_ERROR_STEAM_NOT_RUNNING;
		return false;
	}

	if (!SteamAPI_InitSafe()) {
		m_lastError = STEAM_WRAPPER_ERROR_INIT_STEAM;
		return false;
	}

	CSteamAPIContext *ctx = new CSteamAPIContext();
	m_ctx = ctx;

	if (!m_ctx) {
		m_lastError = STEAM_WRAPPER_ERROR_CONTEXT_CREATE;
		return false;
	}

	if (!m_ctx->Init()) {
		m_lastError = STEAM_WRAPPER_ERROR_CONTEXT_INIT;
		return false;
	}

	return true;
}

const CSteamAPIContext *SteamWrapper::GetContext() const
{
	return m_ctx;
}

SteamWrapperError SteamWrapper::GetLastErrorCode() const
{
	return m_lastError;
}