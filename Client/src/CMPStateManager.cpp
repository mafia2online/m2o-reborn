#include <stdafx.h>
#include "CMPStateManager.h"

CMPStateManager::CMPStateManager()
{
	//
}

CMPStateManager::~CMPStateManager()
{

}

void CMPStateManager::AddState(States state, IMPState * pstate)
{
	m_states[state] = std::make_pair(false, pstate);
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
		m_states[state].first = true;
		m_states[state].second->Activate(nullptr);
	}
}

void CMPStateManager::DeActivateState(States state)
{
	if (IsValidState(state))
	{
		m_states[state].first = false;
		m_states[state].second->Deactivate(nullptr);
	}
}

void CMPStateManager::Render(void * userptr)
{
	for(const auto &state : m_states)
	{
		if (!state.second.first)
			continue;

		state.second.second->Render(userptr);
	}
}

void CMPStateManager::Tick(void * userptr)
{
	for (const auto &state : m_states)
	{
		if (!state.second.first)
			continue;

		state.second.second->Tick(userptr);
	}
}

void CMPStateManager::InitializeResources(void * userptr)
{
	for (const auto &state : m_states)
	{
		state.second.second->InitializeResources(userptr);
	}
}

