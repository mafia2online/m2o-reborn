
#pragma once

#include <Common.h>

#include "C_TickedModule.h"

#include <cstdint>
#include <vector>


class C_TickedModuleManager {
#pragma message(__FILE__ ": replace addreesses with patterns")

public:
	struct S_Action;

	using TickedModuleCallback = void (C_TickedModule::*)(I_TickedModuleCallEventContext &);

	enum class E_TickedModuleEvent : uint32_t
	{
		SYSTEM_INIT = 1,
		SYSTEM_DONE = 2,
		TICK = 3,
		TICK_PAUSED = 4,
		TICK_ALWAYS = 5,
		RENDER = 7,
		GAME_QUIT = 8,
		MISSION_BEFORE_OPEN = 9,
		MISSION_OPEN = 10,
		MISSION_BEFORE_CLOSE = 11,
		MISSION_CLOSE = 12,
		GAME_INIT = 13,
		GAME_DONE = 14,
		INVALIDATE_ENTITY = 15,
		INVALIDATE_FRAME = 16,
		WRITE_GAME_INFO = 17,
		READ_GAME_INFO = 18,
		GAME_RESTORE = 19,
		NO_GAME_START = 20,
		NO_GAME_END = 21,
		NO_GAME_TICK = 22,
		NO_GAME_RENDER = 23,
		GAMELOOP_AFTER = 24,
		COLLISIONS_LOADED = 25,
		APACK_FROM_SDS_LOADED = 26,
		GAME_SAVE = 27,
		GAMEPARAMS_RELOAD = 28,
		GAMEPARAMS_PRESAVE = 29,
		APP_DEACTIVATE = 30,
		APP_ACTIVATE = 31,
		LOADING_STARTED = 32,
		LOADING_FINISHED = 33,
		GAME_PAUSED = 34,
		GAME_UNPAUSED = 35,
		SLOT_WAITING_TICK = 36,
		SLOT_WAITING_RENDER = 37,
		SHUTDOWNN = 39
	};

public:

	/*0*/virtual ~C_TickedModuleManager() = 0;

	/*4*/ virtual int32_t nullsub_0() = 0;

	/*8*/ virtual uint8_t AddEvent(int32_t, char const*) = 0;

	/*12*/ virtual int32_t CallEventByIndex(E_TickedModuleEvent, void const*, std::vector<C_TickedModuleManager::S_Action *> &) = 0;
	/*16*/ virtual int32_t CallEvent(E_TickedModuleEvent, void const*) = 0;
	/*20*/ virtual int32_t CallEvents(std::vector<E_TickedModuleEvent>&, C_TimeBudgetInfo*, ue::sys::threads::I_Scheduler *, void const*) = 0;

	/*24*/ virtual int32_t GetEventIdFromName(char const*) = 0;

	/*28*/ virtual int32_t AddAction(E_TickedModuleEvent, int32_t, C_TickedModule *, TickedModuleCallback, float, uint32_t, uint32_t) = 0;
	/*32*/ virtual int32_t DelAction(E_TickedModuleEvent, C_TickedModule *, TickedModuleCallback) = 0;
	/*36*/ virtual int32_t DelModuleActions(C_TickedModule *) = 0;
	/*40*/ virtual int32_t EnableAction(E_TickedModuleEvent, C_TickedModule *, TickedModuleCallback, bool) = 0;

	/*44*/ virtual int32_t RegisterStaticPlugins(void) = 0;

	static C_TickedModuleManager * GetInstance()
	{
		return *(C_TickedModuleManager**)(0x1ABE8BC);
	}

	/*static C_TickedModule[178] & GetAllModules()
	{
		01ABE698; int C_TickedModule::m_AllModules[]
			01ABE8B8; C_TickedModule::m_CountAllModules
	}*/

};
