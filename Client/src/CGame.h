#pragma once

#include <Shared\Common.h>

class CGame : public Singleton<CGame>
{
public:
	bool Initialize();
	bool HirePreHookers();
	void Patch();
public:
	void OnGameInit();
	void OnGameLoop();
	bool WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	uint32_t m_base;
};