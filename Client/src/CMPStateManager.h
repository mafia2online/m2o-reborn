#pragma once
#include <Shared\Common.h>
#include <unordered_map>

enum States : Byte
{
	None = 0,
	MPManager,
	MPGame,
	Menu
};

class IMPState
{
public:
	virtual void InitializeResources(void *userptr) = 0;
	virtual void Activate(void *userptr) = 0;
	virtual void Deactivate(void *userptr) = 0;
	virtual void Render(void *userptr) = 0;
	virtual void Tick(void *userptr) = 0;
};

class CMPStateManager
{
public:
	CMPStateManager();
	~CMPStateManager();

	void AddState(States state, IMPState *pstate);
	void DeleteState(States state, IMPState *pstate);
	bool IsValidState(States state);

	void ActivateState(States state);
	void DeActivateState(States state);
	
	bool IsStateActive(States state) { return m_states[state].first; }

public:
	void Render(void *userptr);
	void Tick(void *userptr);
	void InitializeResources(void * userptr);

private:
	std::unordered_map<States, std::pair<bool, IMPState*>> m_states;
};