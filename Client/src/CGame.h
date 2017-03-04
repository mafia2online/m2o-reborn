#pragma once

class CGame : public Singleton<CGame>
{
public:
	bool Initialize();
	bool HirePreHookers();
	void Patch();
public:
	void OnGameInit();
	void OnGameLoop();

private:
	uint32_t m_base;
};