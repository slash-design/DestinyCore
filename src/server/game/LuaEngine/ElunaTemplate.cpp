/*
* Copyright (C) 2010 - 2024 Eluna Lua Engine <https://elunaluaengine.github.io/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

// Eluna
#include "LuaEngine.h"
#include "ElunaIncludes.h"
#include "ElunaTemplate.h"
#include "ElunaUtility.h"

#if defined TRACKABLE_PTR_NAMESPACE
// NOTE: Legion port - Most Trinity classes (Aura, Battleground, Group, Guild, Map,
// Object, Quest, Spell, Vehicle) do not expose GetWeakPtr(). Stubbing these helpers
// returns empty refs so Lua scripts may hold dangling references to deleted Trinity
// objects. AuraEffect retains its real weak pointer (added explicitly upstream).
ElunaConstrainedObjectRef<Aura> GetWeakPtrFor(Aura const* /*obj*/) { return {}; }

ElunaConstrainedObjectRef<AuraEffect> GetWeakPtrFor(AuraEffect const* obj)
{
    Map* map = obj->GetBase()->GetOwner()->GetMap();
    return { obj->GetWeakPtr(), map };
}

ElunaConstrainedObjectRef<ElunaProcInfo> GetWeakPtrFor(ElunaProcInfo const* obj)
{
    return { obj->GetWeakPtr(), obj->GetMap()};
}
ElunaConstrainedObjectRef<BattleGround> GetWeakPtrFor(BattleGround const* /*obj*/) { return {}; }
ElunaConstrainedObjectRef<Group> GetWeakPtrFor(Group const* /*obj*/) { return {}; }
ElunaConstrainedObjectRef<Guild> GetWeakPtrFor(Guild const* /*obj*/) { return {}; }
ElunaConstrainedObjectRef<Map> GetWeakPtrFor(Map const* obj) { return { {}, obj }; }
ElunaConstrainedObjectRef<Object> GetWeakPtrForObjectImpl(Object const* /*obj*/)
{
    // Legion port - TYPEMASK_WORLDOBJECT not available and Object::GetWeakPtr()
    // is not exposed on most types. Return empty ref.
    return {};
}
ElunaConstrainedObjectRef<Quest> GetWeakPtrFor(Quest const* /*obj*/) { return {}; }
ElunaConstrainedObjectRef<Spell> GetWeakPtrFor(Spell const* /*obj*/) { return {}; }
ElunaConstrainedObjectRef<ElunaSpellInfo> GetWeakPtrFor(ElunaSpellInfo const* obj) { return { obj->GetWeakPtr(), nullptr }; }
#if ELUNA_EXPANSION >= EXP_WOTLK
ElunaConstrainedObjectRef<Vehicle> GetWeakPtrFor(Vehicle const* /*obj*/) { return {}; }
#endif
#endif
