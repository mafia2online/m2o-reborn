
class I_TicketModuleCallEventContext;

class C_Entity;

class I_Game
{
public:

	virtual ~I_Game() = 0;
	
	virtual void unk_Function1() = 0;
	virtual void unk_Function2() = 0;
	virtual void unk_Function3() = 0;
	virtual void unk_Function4() = 0;
	virtual void Open(const char*) = 0;
	virtual void Close() = 0;
	virtual void unk_Function7() = 0;
	virtual void GameInit(I_TicketModuleCallEventContext&) = 0;
	virtual void GameDone(I_TicketModuleCallEventContext&) = 0;
	virtual void unk_Function10() = 0;
	virtual void unk_Function11() = 0;
	virtual void GameRestore(I_TicketModuleCallEventContext&) = 0;
	virtual void unk_Function13() = 0;
	virtual void unk_Function14() = 0;
	virtual void Tick(I_TicketModuleCallEventContext&) = 0;
	virtual void InvalidateEntity(I_TicketModuleCallEventContext&) = 0;
	virtual void unk_Function17() = 0;
	virtual void unk_Function18() = 0;
	virtual void unk_Function19() = 0;
	virtual void unk_Function20() = 0;
	virtual void unk_Function21() = 0;
	virtual void unk_Function22() = 0;
	virtual void ActivateEntity(C_Entity*, bool) = 0;
	virtual void OnEntityDeleted(C_Entity*) = 0;	
};
