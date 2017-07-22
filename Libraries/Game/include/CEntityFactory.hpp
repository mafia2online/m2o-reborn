#pragma once
#include "CommonHeaders.h"

namespace M2
{
	struct C_EntityFactoryBaseRecord {};

	enum EntityTypes : int32_t
	{
		Entity_Human = 0xE,
		Entity_Player = 0x10,
		Entity_Car = 0x12,
		Entity_Train = 0x13,
		Entity_TrafficCar = 0x15,
		Entity_TrafficHuman = 0x16,
		Entity_TrafficTrain = 0x17,
		Entity_ActionPoint = 0x19,
		Entity_ActionPointSideWalk = 0x1A,
		Entity_ActionPointCrossing = 0x1B,
		Entity_ActionPointScript = 0x1E,
		Entity_ActionPointSearch = 0x20,
		Entity_Item = 0x24,
		Entity_Door = 0x26,
		Entity_Tree = 0x27,
		Entity_Lift = 0x28,
		Entity_Sound = 0x29,
		Entity_SoundMixer = 0x2B,
		Entity_Grenade = 0x2D,
		Entity_Molotov = 0x2E,
		Entity_Boat = 0x2F,
		Entity_Radio = 0x30,
		Entity_Jukebox = 0x31,
		Entity_StaticEntity = 0x34,
		Entity_TranslocatedCar = 0x35,
		Entity_Garage = 0x36,
		Entity_ActorDetector = 0x38,
		Entity_FrameWrapper = 0x37,
		Entity_Blocker = 0x3F,
		Entity_StaticWeapon = 0x41,
		Entity_StaticParticle = 0x42,
		Entity_FireTarget = 0x46,
		Entity_LightEntity = 0x47,
		Entity_CutScene = 0x49,
		Entity_Telephone = 0x5F,
		Entity_ScriptEntity = 0x62,
		Entity_DamageZone = 0x67,
		Entity_Aircraft = 0x68,
		Entity_Pinup = 0x6A,
		Entity_SpikeStrip = 0x6B,
		Entity_ActionPointRoadBlock = 0x6C,
		Entity_DummyDoor = 0x6D,
		Entity_FramesController = 0x6E,
		Entity_ShopMenuScriptEntity = 0x6F,
		Entity_Wardrobe = 0x70,
		Entity_PhysicsScene = 0x71,
		Entity_CleanEntity = 0x72
	};

	// we do not need that probs
	class C_EntityFactory : public GameFakeClassWrapper<C_EntityFactory>
	{
	public:
		bool RegisterEntity(C_EntityFactoryBaseRecord *pRecord)
		{
			return Mem::InvokeFunction<Mem::call_cdecl, bool>(0x11678D0, pRecord);
		}

		template < typename T >
		T* CreateEntity(EntityTypes type)
		{
			return Mem::InvokeFunction<Mem::call_cdecl, T*> (0x1167A60, type);
		}

		EntityTypes GetEntityType(const char *pszHashName)
		{
			return Mem::InvokeFunction<Mem::call_cdecl, EntityTypes>(0x1167B40, pszHashName);
		}

		int32_t GetEntityInitPropsSize(const char *pszHashName)
		{
			return Mem::InvokeFunction<Mem::call_cdecl, EntityTypes>(0x1167AD0, pszHashName);
		}
	};
};