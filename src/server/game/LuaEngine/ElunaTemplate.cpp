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
ElunaConstrainedObjectRef<Aura> GetWeakPtrFor(Aura const* obj)
{
#if defined ELUNA_TRINITY
    Map* map = obj->GetOwner()->GetMap();
#elif defined ELUNA_CMANGOS
    Map* map = obj->GetTarget()->GetMap();
#endif
    return { const_cast<Aura*>(obj), map };
}

ElunaConstrainedObjectRef<AuraEffect> GetWeakPtrFor(AuraEffect const* obj)
{
    Map* map = obj->GetBase()->GetOwner()->GetMap();
    return { const_cast<AuraEffect*>(obj), map };
}

ElunaConstrainedObjectRef<ElunaProcInfo> GetWeakPtrFor(ElunaProcInfo const* obj)
{
    return { const_cast<ElunaProcInfo*>(obj), obj->GetMap()};
}
ElunaConstrainedObjectRef<BattleGround> GetWeakPtrFor(BattleGround const* obj) { return { const_cast<BattleGround*>(obj), obj->GetBgMap() }; }
ElunaConstrainedObjectRef<Group> GetWeakPtrFor(Group const* obj) { return { const_cast<Group*>(obj), nullptr }; }
ElunaConstrainedObjectRef<Guild> GetWeakPtrFor(Guild const* obj) { return { const_cast<Guild*>(obj), nullptr }; }
ElunaConstrainedObjectRef<Map> GetWeakPtrFor(Map const* obj) { return { const_cast<Map*>(obj), obj }; }

ElunaConstrainedObjectRef<Object> GetWeakPtrForObjectImpl(Object const* obj)
{
    // [关键修改]: 使用 dynamic_cast 绕过 7.3.5 缺失的宏和方法
    if (WorldObject const* worldObj = dynamic_cast<WorldObject const*>(obj))
        return { const_cast<Object*>(obj), worldObj->GetMap() };

    if (obj->GetTypeId() == TYPEID_ITEM)
        if (Player const* player = static_cast<Item const*>(obj)->GetOwner())
            return { const_cast<Object*>(obj), player->GetMap() };

    // possibly dangerous item
    return { const_cast<Object*>(obj), nullptr };
}

ElunaConstrainedObjectRef<Quest> GetWeakPtrFor(Quest const* obj) { return { const_cast<Quest*>(obj), nullptr }; }
ElunaConstrainedObjectRef<Spell> GetWeakPtrFor(Spell const* obj) { return { const_cast<Spell*>(obj), obj->GetCaster()->GetMap() }; }
ElunaConstrainedObjectRef<ElunaSpellInfo> GetWeakPtrFor(ElunaSpellInfo const* obj) { return { const_cast<ElunaSpellInfo*>(obj), nullptr }; }


#endif