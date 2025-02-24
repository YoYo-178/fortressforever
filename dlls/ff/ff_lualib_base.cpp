
// ff_lualib_base.cpp

//---------------------------------------------------------------------------
// includes
//---------------------------------------------------------------------------
// includes
#include "cbase.h"
#include "ff_lualib.h"
#include "EntityList.h"
#include "ff_item_flag.h"
#include "ff_triggerclip.h"
#include "ff_projectile_base.h"
#include "ff_team.h"

#include "triggers.h"

// Lua includes
extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//---------------------------------------------------------------------------
using namespace luabind;

//---------------------------------------------------------------------------
/// tostring implemenation for CBaseEntity
std::ostream& operator<<(std::ostream& stream, const CBaseEntity& entity)
{
	stream << const_cast<CBaseEntity&>(entity).GetClassname() << ":";
	const char *szEntityName = const_cast<CBaseEntity&>(entity).GetName();
	if (szEntityName[0])
	{
		stream << szEntityName << ":";
	}
	return stream << entity.entindex();
}

//---------------------------------------------------------------------------
void CFFLuaLib::InitBase(lua_State* L)
{
	ASSERT(L);

	module(L)
	[
		class_<CGlobalEntityList>("EntityList")
			.def("FirstEntity",			&CGlobalEntityList::FirstEnt)
			.def("NextEntity",			&CGlobalEntityList::NextEnt)
			.def("NumEntities",			&CGlobalEntityList::NumberOfEntities),

		// CBaseEntity
		class_<CBaseEntity>("BaseEntity")
			.def(tostring(self))
			.def("EmitSound",			&CBaseEntity::PlaySound)
			.def("StopSound",			(void(CBaseEntity::*)(const char*))&CBaseEntity::StopSound)
			.def("GetClassName",		&CBaseEntity::GetClassname)
			.def("GetName",				&CBaseEntity::GetName)
			.def("SetName",				&CBaseEntity::SetName)
			.def("GetTeam",				&CBaseEntity::GetTeam)
			.def("GetTeamId",			&CBaseEntity::GetTeamNumber)
			.def("GetId",				&CBaseEntity::entindex)
			.def("GetVelocity",			&CBaseEntity::GetAbsVelocity)
			.def("SetVelocity",			&CBaseEntity::SetAbsVelocity)
			.def("GetOwner",			&CBaseEntity::GetOwnerEntity)
			.def("SetModel",			(void(CBaseEntity::*)(const char*))&CBaseEntity::SetModel)
			.def("SetModel",			(void(CBaseEntity::*)(const char*, int))&CBaseEntity::SetModel)
            .def("StartTrail",          (void(CBaseEntity::*)(int))&CBaseEntity::StartTrail)
            .def("StartTrail",          (void(CBaseEntity::*)(int, float, float, float))&CBaseEntity::StartTrail)
            .def("StopTrail",           &CBaseEntity::StopTrail)
			.def("SetSkin",				&CBaseEntity::SetSkin)
			.def("GetOrigin",			&CBaseEntity::GetAbsOrigin)
			.def("SetOrigin",			&CBaseEntity::SetAbsOrigin)
			.def("GetWorldMins",		&CBaseEntity::WorldAlignMins)
			.def("GetWorldMaxs",		&CBaseEntity::WorldAlignMaxs)
			.def("GetAngles",			&CBaseEntity::GetAbsAngles)
			.def("SetAngles",			&CBaseEntity::SetAbsAngles)
			.def("GetAbsFacing",		&CBaseEntity::GetAbsFacing)
			.def("Teleport",			&CBaseEntity::Teleport)
			.def("IsOnFire",			&CBaseEntity::IsOnFire)
			.def("GetGravity",			&CBaseEntity::GetGravity)
			.def("SetGravity",			&CBaseEntity::SetGravity)
			.def("SetRenderColor",		(void(CBaseEntity::*)(byte,byte,byte))&CBaseEntity::SetRenderColor)
			.def("SetRenderMode",		&CBaseEntity::SetRenderMode)

			.def("SetRenderFx",			&CBaseEntity::SetRenderFx)
			.def("GetRenderFx",			&CBaseEntity::GetRenderFx)
			.def("ClearRenderFx",		&CBaseEntity::ClearRenderFx)

			.def("GetFriction",			&CBaseEntity::GetFriction)
			.def("SetFriction",			&CBaseEntity::SetFriction),

		// CFFProjectileBase
		class_<CFFProjectileBase, CBaseEntity>("Projectile"),

		// CFFInfoScript
		class_<CFFInfoScript, CBaseEntity>("InfoScript")
			.def("Drop",				(void(CFFInfoScript::*)(float, float))&CFFInfoScript::Drop)
			.def("Drop",				(void(CFFInfoScript::*)(float, Vector, Vector))&CFFInfoScript::Drop)
			.def("Pickup",				&CFFInfoScript::Pickup)
			.def("Respawn",				&CFFInfoScript::Respawn)
			.def("Return",				&CFFInfoScript::Return)
			.def("IsCarried",			&CFFInfoScript::IsCarried)
			.def("IsReturned",			&CFFInfoScript::IsReturned)
			.def("IsDropped",			&CFFInfoScript::IsDropped)
			.def("IsActive",			&CFFInfoScript::IsActive)
			.def("IsInactive",			&CFFInfoScript::IsInactive)
			.def("IsRemoved",			&CFFInfoScript::IsRemoved)
			.def("GetCarrier",			&CFFInfoScript::GetCarrier)
			.def("GetDropper",			&CFFInfoScript::GetDropper)
			.def("Remove",				&CFFInfoScript::LUA_Remove)
			.def("Restore",				&CFFInfoScript::LUA_Restore)
			.def("GetOrigin",			&CFFInfoScript::LUA_GetOrigin)
			.def("SetOrigin",			&CFFInfoScript::LUA_SetOrigin)
			.def("GetAngles",			&CFFInfoScript::LUA_GetAngles)
			.def("SetAngles",			&CFFInfoScript::LUA_SetAngles)
			.def("SetBotGoalInfo",		&CFFInfoScript::SetBotGoalInfo)
			.def("SpawnBot",			&CFFInfoScript::SpawnBot)
			.def("SetModel",			&CFFInfoScript::LUA_SetModel) // Leave this!
			.def("GetModel",			&CFFInfoScript::LUA_GetModel)
			.def("SetStartOrigin",		&CFFInfoScript::LUA_SetStartOrigin)
			.def("GetStartOrigin",		&CFFInfoScript::LUA_GetStartOrigin)
			.def("SetStartAngles",		&CFFInfoScript::LUA_SetStartAngles)
			.def("GetStartAngles",		&CFFInfoScript::LUA_GetStartAngles)
			.def("SetTouchFlags",		&CFFInfoScript::SetTouchFlags)
			.def("SetDisallowTouchFlags",&CFFInfoScript::SetDisallowTouchFlags)
			.def("GetAngularVelocity",	&CFFInfoScript::GetLocalAngularVelocity)
			.def("SetAngularVelocity",	&CFFInfoScript::SetLocalAngularVelocity),

		// CFuncFFScript - trigger_ff_script
		class_<CFuncFFScript>("TriggerScript")
			.def("IsActive",			&CFuncFFScript::IsActive)
			.def("IsInactive",			&CFuncFFScript::IsInactive)
			.def("IsRemoved",			&CFuncFFScript::IsRemoved)
			.def("Remove",				&CFuncFFScript::LuaRemove)
			.def("Restore",				&CFuncFFScript::LuaRestore)
			.def("IsTouching",			&CFuncFFScript::IsTouching)
			//.def("SetLocation",			&CFuncFFScript::LuaSetLocation)
			.def("SetBotGoalInfo",		&CFuncFFScript::SetBotGoalInfo),

		class_<CFFTriggerClip>("TriggerClip")
			.def("SetClipFlags",		&CFFTriggerClip::LUA_SetClipFlags)
	];

	(globals(L))["GlobalEntityList"] = &gEntList;
};
