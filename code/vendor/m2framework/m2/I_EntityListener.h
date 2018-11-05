class C_Entity;

struct I_EntityListener
{
	enum E_EntityEvents
	{};

	virtual ~I_EntityListener() = 0;
	virtual void OnEvent(C_Entity*, E_EntityEvents) = 0;
};