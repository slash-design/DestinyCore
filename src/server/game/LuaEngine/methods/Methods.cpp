/*
* Copyright (C) 2010 - 2024 Eluna Lua Engine <https://elunaluaengine.github.io/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

// Eluna
#include "LuaEngine.h"
#include "ElunaEventMgr.h"
#include "ElunaIncludes.h"
#include "ElunaTemplate.h"
#include "ElunaUtility.h"

// Method includes
#include "GlobalMethods.h"
#include "ObjectMethods.h"
#include "WorldObjectMethods.h"
#include "UnitMethods.h"
#include "PlayerMethods.h"
#include "CreatureMethods.h"


#include "GameObjectMethods.h"

#include "AuraMethods.h"
#include "AuraEffectMethods.h"






#include "MapMethods.h"



#include "BigIntMethods.h"
#include "CustomMethodsInterface.h"

void RegisterMethods(Eluna* E)
{
    ElunaTemplate<>::SetMethods(E, LuaGlobalFunctions::GlobalMethods);

    ElunaTemplate<Object>::Register(E, "Object");
    ElunaTemplate<Object>::SetMethods(E, LuaObject::ObjectMethods);

    ElunaTemplate<WorldObject>::Register(E, "WorldObject");
    ElunaTemplate<WorldObject>::SetMethods(E, LuaObject::ObjectMethods);
    ElunaTemplate<WorldObject>::SetMethods(E, LuaWorldObject::WorldObjectMethods);

    ElunaTemplate<Unit>::Register(E, "Unit");
    ElunaTemplate<Unit>::SetMethods(E, LuaObject::ObjectMethods);
    ElunaTemplate<Unit>::SetMethods(E, LuaWorldObject::WorldObjectMethods);
    ElunaTemplate<Unit>::SetMethods(E, LuaUnit::UnitMethods);

    ElunaTemplate<Player>::Register(E, "Player");
    ElunaTemplate<Player>::SetMethods(E, LuaObject::ObjectMethods);
    ElunaTemplate<Player>::SetMethods(E, LuaWorldObject::WorldObjectMethods);
    ElunaTemplate<Player>::SetMethods(E, LuaUnit::UnitMethods);
    ElunaTemplate<Player>::SetMethods(E, LuaPlayer::PlayerMethods);

    ElunaTemplate<Creature>::Register(E, "Creature");
    ElunaTemplate<Creature>::SetMethods(E, LuaObject::ObjectMethods);
    ElunaTemplate<Creature>::SetMethods(E, LuaWorldObject::WorldObjectMethods);
    ElunaTemplate<Creature>::SetMethods(E, LuaUnit::UnitMethods);
    ElunaTemplate<Creature>::SetMethods(E, LuaCreature::CreatureMethods);

    ElunaTemplate<GameObject>::Register(E, "GameObject");
    ElunaTemplate<GameObject>::SetMethods(E, LuaObject::ObjectMethods);
    ElunaTemplate<GameObject>::SetMethods(E, LuaWorldObject::WorldObjectMethods);
    ElunaTemplate<GameObject>::SetMethods(E, LuaGameObject::GameObjectMethods);
















    ElunaTemplate<Aura>::Register(E, "Aura");
    ElunaTemplate<Aura>::SetMethods(E, LuaAura::AuraMethods);

    ElunaTemplate<AuraEffect>::Register(E, "AuraEffect");
    ElunaTemplate<AuraEffect>::SetMethods(E, LuaAuraEffects::AuraEffectMethods);





  







    ElunaTemplate<Map>::Register(E, "Map");
    ElunaTemplate<Map>::SetMethods(E, LuaMap::MapMethods);










    ElunaTemplate<long long>::Register(E, "long long");
    ElunaTemplate<long long>::SetMethods(E, LuaBigInt::LongLongMethods);

    ElunaTemplate<unsigned long long>::Register(E, "unsigned long long");
    ElunaTemplate<unsigned long long>::SetMethods(E, LuaBigInt::ULongLongMethods);

    ElunaTemplate<ObjectGuid>::Register(E, "ObjectGuid");
    ElunaTemplate<ObjectGuid>::SetMethods(E, LuaBigInt::ObjectGuidMethods);

    LuaCustom::RegisterCustomMethods(E);

    LuaVal::Register(E->L);
}
