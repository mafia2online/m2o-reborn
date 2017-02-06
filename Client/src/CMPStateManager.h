#pragma once
#include <Shared\Common.h>
#include <unordered_map>

enum States : Byte
{
	None = 0,
	Title,
	Connecting,
	Connected,
	Playing
};

enum Substates : Byte //?
{
	Subtate_None = 0,
	Subtate_Menu,
	Subtate_DevConsole
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


	States GetActiveState() { return m_state; }

public:
	void Render(void *userptr);
	void Tick(void *userptr);
	void InitializeResources(void * userptr);

private:
	States m_state;
	Substates m_substate;
	std::unordered_map<States, IMPState*> m_states;
	std::unordered_map<Substates, IMPState*> m_substates;
};