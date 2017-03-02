#pragma once

class CGame
{
public:
	bool Initialize();
	bool HirePreHookers();
	void Patch();

private:
	uint32_t m_base;
};