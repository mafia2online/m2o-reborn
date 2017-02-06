#pragma once

#define VERSION_SAFE_STEAM_API_INTERFACES
#include <steam_api.h>

enum SteamWrapperError
{
	STEAM_WRAPPER_ERROR_STEAM_NOT_RUNNING = 1,
	STEAM_WRAPPER_ERROR_INIT_STEAM,
	STEAM_WRAPPER_ERROR_CONTEXT_CREATE,
	STEAM_WRAPPER_ERROR_CONTEXT_INIT
};

class SteamWrapper
{
private:
	CSteamAPIContext	*m_ctx;
	SteamWrapperError	m_lastError;

public:
	SteamWrapper();
	~SteamWrapper() = default;

	bool	init();

	const	CSteamAPIContext	*GetContext() const;
	SteamWrapperError			GetLastErrorCode() const;
};