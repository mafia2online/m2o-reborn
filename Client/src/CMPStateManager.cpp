#include <stdafx.h>
#include "CMPStateManager.h"

CMPStateManager::CMPStateManager() :
	m_state(States::None),
	m_substate(Substates::Subtate_None)
{
	//
}

CMPStateManager::~CMPStateManager()
{

}

void CMPStateManager::AddState(States state, IMPState * pstate)
{
	m_states[state] = pstate;
}

void CMPStateManager::DeleteState(States state, IMPState * pstate)
{
	m_states.erase(m_states.find(state));
}

bool CMPStateManager::IsValidState(States state)
{
	return (m_states.find(state) != m_states.end());
}

void CMPStateManager::ActivateState(States state)
{
	if (IsValidState(state))
	{
		if (GetActiveState() != States::None)
			DeActivateState(GetActiveState());

		m_states[state]->Activate(nullptr);
		m_state = state;
	}
}

void CMPStateManager::DeActivateState(States state)
{
	if (IsValidState(state))
	{
		m_states[state]->Deactivate(nullptr);
		m_state = States::None;
	}
}

void CMPStateManager::Render(void * userptr)
{
	if(IsValidState(m_state))
	{
		m_states[m_state]->Render(userptr);
	}
}

void CMPStateManager::Tick(void * userptr)
{
	if (IsValidState(m_state))
	{
		m_states[m_state]->Tick(userptr);
	}
}

void CMPStateManager::InitializeResources(void * userptr)
{
	if (IsValidState(m_state))
	{
		m_states[m_state]->InitializeResources(userptr);
	}
}

