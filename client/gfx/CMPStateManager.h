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

