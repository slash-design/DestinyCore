/*
* Copyright (C) 2010 - 2024 Eluna Lua Engine <https://elunaluaengine.github.io/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef UNITMETHODS_H
#define UNITMETHODS_H

#include "ChatPackets.h"

/***
 * Inherits all methods from: [Object], [WorldObject]
 */
namespace LuaUnit
{
    /**
    * Sets a mechanic immunity for the [Unit].
    *
    * @table
    * @columns [Mechanic, ID, Comment]
    * @values [MECHANIC_NONE, 0,  ""]
    * @values [MECHANIC_CHARM, 1,  ""]
    * @values [MECHANIC_DISORIENTED, 2,  ""]
    * @values [MECHANIC_DISARM, 3,  ""]
    * @values [MECHANIC_DISTRACT, 4,  ""]
    * @values [MECHANIC_FEAR, 5,  ""]
    * @values [MECHANIC_GRIP, 6,  ""]
    * @values [MECHANIC_ROOT, 7,  ""]
    * @values [MECHANIC_SLOW_ATTACK, 8,  ""]
    * @values [MECHANIC_SILENCE, 9,  ""]
    * @values [MECHANIC_SLEEP, 10, ""]
    * @values [MECHANIC_SNARE, 11, ""]
    * @values [MECHANIC_STUN, 12, ""]
    * @values [MECHANIC_FREEZE, 13, ""]
    * @values [MECHANIC_KNOCKOUT, 14, ""]
    * @values [MECHANIC_BLEED, 15, ""]
    * @values [MECHANIC_BANDAGE, 16, ""]
    * @values [MECHANIC_POLYMORPH, 17, ""]
    * @values [MECHANIC_BANISH, 18, ""]
    * @values [MECHANIC_SHIELD, 19, ""]
    * @values [MECHANIC_SHACKLE, 20, ""]
    * @values [MECHANIC_MOUNT, 21, ""]
    * @values [MECHANIC_INFECTED, 22, ""]
    * @values [MECHANIC_TURN, 23, ""]
    * @values [MECHANIC_HORROR, 24, ""]
    * @values [MECHANIC_INVULNERABILITY, 25, ""]
    * @values [MECHANIC_INTERRUPT, 26, ""]
    * @values [MECHANIC_DAZE, 27, ""]
    * @values [MECHANIC_DISCOVERY, 28, ""]
    * @values [MECHANIC_IMMUNE_SHIELD, 29, "Divine (Blessing) Shield/Protection and Ice Block"]
    * @values [MECHANIC_SAPPED, 30, ""]
    * @values [MECHANIC_ENRAGED, 31, ""]
    *
    * @param int32 immunity : new value for the immunity mask
    * @param bool apply = true : if true, the immunity is applied, otherwise it is removed
    */
    int SetImmuneTo(Eluna* E, Unit* unit)
    {
        int32 immunity = E->CHECKVAL<int32>(2);
        bool apply = E->CHECKVAL<bool>(3, true);

        unit->ApplySpellImmune(0, 5, immunity, apply);
        return 0;
    }
    /**
     * The [Unit] tries to attack a given target
     *
     * @param [Unit] who : [Unit] to attack
     * @param bool meleeAttack = false: attack with melee or not
     * @return didAttack : if the [Unit] did not attack
     */
    int Attack(Eluna* E, Unit* unit)
    {
        Unit* who = E->CHECKOBJ<Unit>(2);
        bool meleeAttack = E->CHECKVAL<bool>(3, false);

        E->Push(unit->Attack(who, meleeAttack));
        return 1;
    }

    /**
     * The [Unit] stops attacking its target
     *
     * @return bool isAttacking : if the [Unit] wasn't attacking already
     */
    int AttackStop(Eluna* E, Unit* unit)
    {
        E->Push(unit->AttackStop());
        return 1;
    }

    /**
     * Returns true if the [Unit] is standing.
     *
     * @return bool isStanding
     */
    int IsStandState(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsStandState());
        return 1;
    }

    /**
     * Returns true if the [Unit] is mounted.
     *
     * @return bool isMounted
     */
    int IsMounted(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsMounted());
        return 1;
    }

    /**
     * Returns true if the [Unit] is rooted.
     *
     * @return bool isRooted
     */
    #if 0
    int IsRooted(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsRooted() || unit->HasUnitMovementFlag(MOVEMENTFLAG_ROOT));
        return 1;
    }
    #endif
    /**
     * Returns true if the [Unit] has full health.
     *
     * @return bool hasFullHealth
     */
    int IsFullHealth(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsFullHealth());
        return 1;
    }

    /**
     * Returns true if the [Unit] is in an accessible place for the given [Creature].
     *
     * @param [WorldObject] obj
     * @param float radius
     * @return bool isAccessible
     */
    int IsInAccessiblePlaceFor(Eluna* E, Unit* unit)
    {
        Creature* creature = E->CHECKOBJ<Creature>(2);

        E->Push(unit->isInAccessiblePlaceFor(creature));
        return 1;
    }

    /**
     * Returns true if the [Unit] an auctioneer.
     *
     * @return bool isAuctioneer
     */
    int IsAuctioneer(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsAuctioner());
        return 1;
    }

    /**
     * Returns true if the [Unit] a guild master.
     *
     * @return bool isGuildMaster
     */
    int IsGuildMaster(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsGuildMaster());
        return 1;
    }

    /**
     * Returns true if the [Unit] an innkeeper.
     *
     * @return bool isInnkeeper
     */
    int IsInnkeeper(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsInnkeeper());
        return 1;
    }

    /**
     * Returns true if the [Unit] a trainer.
     *
     * @return bool isTrainer
     */
    int IsTrainer(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsTrainer());
        return 1;
    }

    /**
     * Returns true if the [Unit] is able to show a gossip window.
     *
     * @return bool hasGossip
     */
    int IsGossip(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsGossip());
        return 1;
    }

    /**
     * Returns true if the [Unit] is a taxi master.
     *
     * @return bool isTaxi
     */
    int IsTaxi(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsTaxi());
        return 1;
    }

    /**
     * Returns true if the [Unit] is a spirit healer.
     *
     * @return bool isSpiritHealer
     */
    int IsSpiritHealer(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsSpiritHealer());
        return 1;
    }

    /**
     * Returns true if the [Unit] is a spirit guide.
     *
     * @return bool isSpiritGuide
     */
    int IsSpiritGuide(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsSpiritGuide());
        return 1;
    }

    /**
     * Returns true if the [Unit] is a tabard designer.
     *
     * @return bool isTabardDesigner
     */
    int IsTabardDesigner(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsTabardDesigner());
        return 1;
    }

    /**
     * Returns true if the [Unit] provides services like vendor, training and auction.
     *
     * @return bool isTabardDesigner
     */
    int IsServiceProvider(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsServiceProvider());
        return 1;
    }

    /**
     * Returns true if the [Unit] is a spirit guide or spirit healer.
     *
     * @return bool isSpiritService
     */
    int IsSpiritService(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsSpiritService());
        return 1;
    }

    /**
     * Returns true if the [Unit] is alive.
     *
     * @return bool isAlive
     */
    int IsAlive(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsAlive());
        return 1;
    }

    /**
     * Returns true if the [Unit] is dead.
     *
     * @return bool isDead
     */
    int IsDead(Eluna* E, Unit* unit)
    {
        E->Push(unit->isDead());
        return 1;
    }

    /**
     * Returns true if the [Unit] is dying.
     *
     * @return bool isDying
     */
    int IsDying(Eluna* E, Unit* unit)
    {
        E->Push(unit->isDying());
        return 1;
    }

    /**
     * Returns true if the [Unit] is a banker.
     *
     * @return bool isBanker
     */
    int IsBanker(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsBanker());
        return 1;
    }

    /**
     * Returns true if the [Unit] is a vendor.
     *
     * @return bool isVendor
     */
    int IsVendor(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsVendor());
        return 1;
    }

    /**
     * Returns true if the [Unit] is a battle master.
     *
     * @return bool isBattleMaster
     */
    int IsBattleMaster(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsBattleMaster());
        return 1;
    }

    /**
     * Returns true if the [Unit] is a charmed.
     *
     * @return bool isCharmed
     */
    int IsCharmed(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsCharmed());
        return 1;
    }

    /**
     * Returns true if the [Unit] is an armorer and can repair equipment.
     *
     * @return bool isArmorer
     */
    int IsArmorer(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsArmorer());
        return 1;
    }

    /**
     * Returns true if the [Unit] is attacking a player.
     *
     * @return bool isAttackingPlayer
     */
    int IsAttackingPlayer(Eluna* E, Unit* unit)
    {
        E->Push(unit->isAttackingPlayer());
        return 1;
    }

    /**
     * Returns true if the [Unit] flagged for PvP.
     *
     * @return bool isPvP
     */
    int IsPvPFlagged(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsPvP());
        return 1;
    }

    /**
     * Returns true if the [Unit] is on a [Vehicle].
     *
     * @return bool isOnVehicle
     */
    #if 0
    int IsOnVehicle(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetVehicle());
        return 1;
    }
   #endif
    /**
     * Returns true if the [Unit] is in combat.
     *
     * @return bool inCombat
     */
    int IsInCombat(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsInCombat());
        return 1;
    }

    /**
     * Returns true if the [Unit] is under water.
     *
     * @return bool underWater
     */
    int IsUnderWater(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsUnderWater());
        return 1;
    }

    /**
     * Returns true if the [Unit] is in water.
     *
     * @return bool inWater
     */
    int IsInWater(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsInWater());
        return 1;
    }

    /**
     * Returns true if the [Unit] is not moving.
     *
     * @return bool notMoving
     */
    int IsStopped(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsStopped());
        return 1;
    }

    /**
     * Returns true if the [Unit] is a quest giver.
     *
     * @return bool questGiver
     */
    int IsQuestGiver(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsQuestGiver());
        return 1;
    }

    /**
     * Returns true if the [Unit]'s health is below the given percentage.
     *
     * @param int32 healthpct : percentage in integer from
     * @return bool isBelow
     */
    int HealthBelowPct(Eluna* E, Unit* unit)
    {
        E->Push(unit->HealthBelowPct(E->CHECKVAL<int32>(2)));
        return 1;
    }

    /**
     * Returns true if the [Unit]'s health is above the given percentage.
     *
     * @param int32 healthpct : percentage in integer from
     * @return bool isAbove
     */
    int HealthAbovePct(Eluna* E, Unit* unit)
    {
        E->Push(unit->HealthAbovePct(E->CHECKVAL<int32>(2)));
        return 1;
    }

    /**
     * Returns true if the [Unit] has an aura from the given spell entry.
     *
     * @param uint32 spell : entry of the aura spell
     * @return bool hasAura
     */
    int HasAura(Eluna* E, Unit* unit)
    {
        uint32 spell = E->CHECKVAL<uint32>(2);

        E->Push(unit->HasAura(spell));
        return 1;
    }

    /**
     * Returns true if the [Unit] is casting a spell
     *
     * @return bool isCasting
     */
    int IsCasting(Eluna* E, Unit* unit)
    {
        E->Push(unit->HasUnitState(UNIT_STATE_CASTING));
        return 1;
    }

    /**
     * Returns true if the [Unit] has the given unit state.
     *
     * @param [UnitState] state : an unit state
     * @return bool hasState
     */
    int HasUnitState(Eluna* E, Unit* unit)
    {
        uint32 state = E->CHECKVAL<uint32>(2);

        E->Push(unit->HasUnitState(state));
        return 1;
    }

    /**
     * Returns true if the [Unit] is visible, false otherwise.
     *
     * @return bool isVisible
     */
    int IsVisible(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsVisible());
        return 1;
    }

    /**
     * Returns true if the [Unit] is moving, false otherwise.
     *
     * @return bool isMoving
     */
    int IsMoving(Eluna* E, Unit* unit)
    {
        E->Push(unit->isMoving());
        return 1;
    }

    /**
     * Returns true if the [Unit] is flying, false otherwise.
     *
     * @return bool isFlying
     */
    int IsFlying(Eluna* E, Unit* unit)
    {
        E->Push(unit->IsFlying());
        return 1;
    }

    /**
     * Returns the [Unit]'s owner.
     *
     * @return [Unit] owner
     */
    int GetOwner(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetOwner());
        return 1;
    }

    /**
     * Returns the [Unit]'s owner's GUID.
     *
     * @return ObjectGuid ownerGUID
     */
    int GetOwnerGUID(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetOwnerGUID());
        return 1;
    }

   
    #if 0
    int GetMountId(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetMountDisplayId());
        return 1;
    }
    #endif
    
    int GetCreatorGUID(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetCreatorGUID());
        return 1;
    }

    
    int GetCharmerGUID(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetCharmerGUID());
        return 1;
    }

    
    #if 0
    int GetCharmGUID(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetCharmedGUID());
        return 1;
    }
    #endif
  
    int GetPetGUID(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetPetGUID());
        return 1;
    }

    
    int GetControllerGUID(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetCharmerOrOwnerGUID());
        return 1;
    }

    
    int GetControllerGUIDS(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetCharmerOrOwnerOrOwnGUID());
        return 1;
    }

   
    int GetStat(Eluna* E, Unit* unit)
    {
        uint32 stat = E->CHECKVAL<uint32>(2);

        if (stat >= MAX_STATS)
            return 1;

        E->Push(unit->GetStat((Stats)stat));
        return 1;
    }

    
    int GetBaseSpellPower(Eluna* E, Unit* unit)
    {
        uint32 spellschool = E->CHECKVAL<uint32>(2);

        if (spellschool >= MAX_SPELL_SCHOOL)
            return 1;

        E->Push(unit->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + spellschool));
        return 1;
    }

    
    int GetVictim(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetVictim());
        return 1;
    }

    
    int GetCurrentSpell(Eluna* E, Unit* unit)
    {
        uint32 type = E->CHECKVAL<uint32>(2);
        if (type >= CURRENT_MAX_SPELL)
            return luaL_argerror(E->L, 2, "valid CurrentSpellTypes expected");

        E->Push(unit->GetCurrentSpell(type));
        return 1;
    }

   
    int GetStandState(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetStandState());
        return 1;
    }

    
    int GetDisplayId(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetDisplayId());
        return 1;
    }

    
    int GetNativeDisplayId(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetNativeDisplayId());
        return 1;
    }

    
    #if 0
    int GetLevel(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetLevel());
        return 1;
    }
    #endif
    
    int GetHealth(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetHealth());
        return 1;
    }

    Powers PowerSelectorHelper(Eluna* E, Unit* unit, int powerType = -1)
    {
        if (powerType == -1)
            return unit->GetPowerType();

        if (powerType < 0 || powerType >= int(MAX_POWERS))
            luaL_argerror(E->L, 2, "valid Powers expected");

        return (Powers)powerType;
    }

    
    int GetPower(Eluna* E, Unit* unit)
    {
        int type = E->CHECKVAL<int>(2, -1);
        Powers power = PowerSelectorHelper(E, unit, type);

        E->Push(unit->GetPower(power));
        return 1;
    }

    
    int GetMaxPower(Eluna* E, Unit* unit)
    {
        int type = E->CHECKVAL<int>(2, -1);
        Powers power = PowerSelectorHelper(E, unit, type);

        E->Push(unit->GetMaxPower(power));
        return 1;
    }

   
    int GetPowerPct(Eluna* E, Unit* unit)
    {
        int type = E->CHECKVAL<int>(2, -1);
        Powers power = PowerSelectorHelper(E, unit, type);

        float percent = ((float)unit->GetPower(power) / (float)unit->GetMaxPower(power)) * 100.0f;

        E->Push(percent);
        return 1;
    }

   
    int GetPowerType(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetPowerType());
        return 1;
    }

    
    int GetMaxHealth(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetMaxHealth());
        return 1;
    }

    
    int GetHealthPct(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetHealthPct());
        return 1;
    }

   
    #if 0
    int GetGender(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetGender());
        return 1;
    }
    #endif
    
    #if 0
    int GetRace(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetRace());
        return 1;
    }
    #endif
   
    #if 0
    int GetClass(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetClass());
        return 1;
    }
    #endif
    
    #if 0
    int GetRaceMask(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetRaceMask());
        return 1;
    }
    #endif
    
    #if 0
    int GetClassMask(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetClassMask());
        return 1;
    }
    #endif
    
    int GetCreatureType(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetCreatureType());
        return 1;
    }

    
    #if 0
    int GetClassAsString(Eluna* E, Unit* unit)
    {
        uint8 locale = E->CHECKVAL<uint8>(2, DEFAULT_LOCALE);
        if (locale >= TOTAL_LOCALES)
            return luaL_argerror(E->L, 2, "valid LocaleConstant expected");

        const ChrClassesEntry* entry = sChrClassesStore.LookupEntry(unit->GetClass());
        if (!entry)
            return 1;

        E->Push(entry->Name[locale]);
        return 1;
    }
    #endif
    
    #if 0
    int GetRaceAsString(Eluna* E, Unit* unit)
    {
        uint8 locale = E->CHECKVAL<uint8>(2, DEFAULT_LOCALE);
        if (locale >= TOTAL_LOCALES)
            return luaL_argerror(E->L, 2, "valid LocaleConstant expected");

        const ChrRacesEntry* entry = sChrRacesStore.LookupEntry(unit->GetRace());
        if (!entry)
            return 1;

        E->Push(entry->Name[locale]);
        return 1;
    }
    #endif
    
    #if 0
    int GetFaction(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetFaction());
        return 1;
    }
    #endif
    
    int GetAura(Eluna* E, Unit* unit)
    {
        uint32 spellId = E->CHECKVAL<uint32>(2);
        ObjectGuid caster = E->CHECKVAL<ObjectGuid>(3, ObjectGuid::Empty);
        ObjectGuid itemCaster = E->CHECKVAL<ObjectGuid>(4, ObjectGuid::Empty);
        uint8 reqEffMask = E->CHECKVAL<uint8>(5, 0);
        E->Push(unit->GetAura(spellId, caster, itemCaster, reqEffMask));
        return 1;
    }

   
    int GetOwnedAura(Eluna* E, Unit* unit)
    {
        uint32 spellId = E->CHECKVAL<uint32>(2);
        ObjectGuid caster = E->CHECKVAL<ObjectGuid>(3, ObjectGuid::Empty);
        ObjectGuid itemCaster = E->CHECKVAL<ObjectGuid>(4, ObjectGuid::Empty);
        uint8 reqEffMask = E->CHECKVAL<uint8>(5, 0);
        Aura* exceptAura = E->CHECKOBJ<Aura>(6, false);
        E->Push(unit->GetOwnedAura(spellId, caster, itemCaster, reqEffMask, exceptAura));
        return 1;
    }

    
    int GetFriendlyUnitsInRange(Eluna* E, Unit* unit)
    {
        float range = E->CHECKVAL<float>(2, SIZE_OF_GRIDS);

        std::list<Unit*> list;
        Trinity::AnyFriendlyUnitInObjectRangeCheck checker(unit, unit, range);
        Trinity::UnitListSearcher<Trinity::AnyFriendlyUnitInObjectRangeCheck> searcher(unit, list, checker);
        Cell::VisitAllObjects(unit, searcher, range);

        ElunaUtil::ObjectGUIDCheck guidCheck(unit->GET_GUID());
        list.remove_if(guidCheck);

        lua_createtable(E->L, list.size(), 0);
        int tbl = lua_gettop(E->L);
        uint32 i = 0;

        for (std::list<Unit*>::const_iterator it = list.begin(); it != list.end(); ++it)
        {
            E->Push(*it);
            lua_rawseti(E->L, tbl, ++i);
        }

        lua_settop(E->L, tbl);
        return 1;
    }

    
    int GetUnfriendlyUnitsInRange(Eluna* E, Unit* unit)
    {
        float range = E->CHECKVAL<float>(2, SIZE_OF_GRIDS);

        std::list<Unit*> list;
        Trinity::AnyUnfriendlyUnitInObjectRangeCheck checker(unit, unit, range);
        Trinity::UnitListSearcher<Trinity::AnyUnfriendlyUnitInObjectRangeCheck> searcher(unit, list, checker);
        Cell::VisitAllObjects(unit, searcher, range);

        ElunaUtil::ObjectGUIDCheck guidCheck(unit->GET_GUID());
        list.remove_if(guidCheck);

        lua_createtable(E->L, list.size(), 0);
        int tbl = lua_gettop(E->L);
        uint32 i = 0;

        for (std::list<Unit*>::const_iterator it = list.begin(); it != list.end(); ++it)
        {
            E->Push(*it);
            lua_rawseti(E->L, tbl, ++i);
        }

        lua_settop(E->L, tbl);
        return 1;
    }

    #if 0
    int GetVehicleKit(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetVehicleKit());
        return 1;
    }

   
    int GetVehicle(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetVehicle());
        return 1;
    }
   #endif
   
    int GetCritterGUID(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetCritterGUID());
        return 1;
    }

    
    int GetSpeed(Eluna* E, Unit* unit)
    {
        uint32 type = E->CHECKVAL<uint32>(2);
        if (type >= MAX_MOVE_TYPE)
            return luaL_argerror(E->L, 2, "valid UnitMoveType expected");

        E->Push(unit->GetSpeed((UnitMoveType)type));
        return 1;
    }

   
    int GetMovementType(Eluna* E, Unit* unit)
    {
        E->Push(unit->GetMotionMaster()->GetCurrentMovementGeneratorType());
        return 1;
    }

    /**
     * Sets the [Unit]'s owner GUID to given GUID.
     *
     * @param ObjectGuid guid : new owner guid
     */
    int SetOwnerGUID(Eluna* E, Unit* unit)
    {
        ObjectGuid guid = E->CHECKVAL<ObjectGuid>(2);

        unit->SetOwnerGUID(guid);
        return 0;
    }

    
    int SetPvP(Eluna* E, Unit* unit)
    {
        bool apply = E->CHECKVAL<bool>(2, true);

        unit->SetPvP(apply);
        return 0;
    }

    
    int SetSheath(Eluna* E, Unit* unit)
    {
        uint32 sheathed = E->CHECKVAL<uint32>(2);
        if (sheathed >= MAX_SHEATH_STATE)
            return luaL_argerror(E->L, 2, "valid SheathState expected");

        unit->SetSheath((SheathState)sheathed);
        return 0;
    }

   
    int SetName(Eluna* E, Unit* unit)
    {
        const char* name = E->CHECKVAL<const char*>(2);
        if (std::string(name).length() > 0)
            unit->SetName(name);
        return 0;
    }

    
    int SetSpeed(Eluna* E, Unit* unit)
    {
        uint32 type = E->CHECKVAL<uint32>(2);
        float rate = E->CHECKVAL<float>(3);
        bool forced = E->CHECKVAL<bool>(4, false);
        (void)forced; // ensure that the variable is referenced in order to pass compiler checks
        if (type >= MAX_MOVE_TYPE)
            return luaL_argerror(E->L, 2, "valid UnitMoveType expected");

        unit->SetSpeedRate((UnitMoveType)type, rate);
        return 0;
    }

    


    
    int SetLevel(Eluna* E, Unit* unit)
    {
        uint8 newlevel = E->CHECKVAL<uint8>(2);

        if (newlevel < 1)
            return luaL_argerror(E->L, 2, "level cannot be below 1");

        if (Player* player = unit->ToPlayer())
        {
            player->GiveLevel(newlevel);
            player->InitTalentForLevel();
            player->SetUInt32Value(PLAYER_XP, 0);
        }
        else
            unit->SetLevel(newlevel);

        return 0;
    }

   
    int SetHealth(Eluna* E, Unit* unit)
    {
        uint32 amt = E->CHECKVAL<uint32>(2);
        unit->SetHealth(amt);
        return 0;
    }

    
    int SetMaxHealth(Eluna* E, Unit* unit)
    {
        uint32 amt = E->CHECKVAL<uint32>(2);
        unit->SetMaxHealth(amt);
        return 0;
    }

   
    int SetPower(Eluna* E, Unit* unit)
    {
        uint32 amt = E->CHECKVAL<uint32>(2);
        int type = E->CHECKVAL<int>(3, -1);
        Powers power = PowerSelectorHelper(E, unit, type);

        unit->SetPower(power, amt);
        return 0;
    }

   
    int ModifyPower(Eluna* E, Unit* unit)
    {
        int32 amt = E->CHECKVAL<int32>(2);
        int type = E->CHECKVAL<int>(3, -1);
        Powers power = PowerSelectorHelper(E, unit, type);

        unit->ModifyPower(power, amt);
        return 0;
    }

    
    int SetMaxPower(Eluna* E, Unit* unit)
    {
        int type = E->CHECKVAL<int>(2, -1);
        uint32 amt = E->CHECKVAL<uint32>(3);
        Powers power = PowerSelectorHelper(E, unit, type);

        unit->SetMaxPower(power, amt);
        return 0;
    }

    
    int SetPowerType(Eluna* E, Unit* unit)
    {
        uint32 type = E->CHECKVAL<uint32>(2);
        if (type >= int(MAX_POWERS))
            return luaL_argerror(E->L, 2, "valid Powers expected");

        unit->SetPowerType((Powers)type);
        return 0;
    }

    
    int SetDisplayId(Eluna* E, Unit* unit)
    {
        uint32 model = E->CHECKVAL<uint32>(2);
        unit->SetDisplayId(model);
        return 0;
    }

    
    int SetNativeDisplayId(Eluna* E, Unit* unit)
    {
        uint32 model = E->CHECKVAL<uint32>(2);
        unit->SetNativeDisplayId(model);
        return 0;
    }

    
    int SetFacing(Eluna* E, Unit* unit)
    {
        float o = E->CHECKVAL<float>(2);
        unit->SetFacingTo(o);
        return 0;
    }

    
    int SetFacingToObject(Eluna* E, Unit* unit)
    {
        WorldObject* obj = E->CHECKOBJ<WorldObject>(2);
        unit->SetFacingToObject(obj);
        return 0;
    }

    
    int SetCreatorGUID(Eluna* E, Unit* unit)
    {
        ObjectGuid guid = E->CHECKVAL<ObjectGuid>(2);

        unit->SetCreatorGUID(guid);
        return 0;
    }

    
    int SetPetGUID(Eluna* E, Unit* unit)
    {
        ObjectGuid guid = E->CHECKVAL<ObjectGuid>(2);

        unit->SetPetGUID(guid);
        return 0;
    }

    
    int SetWaterWalk(Eluna* E, Unit* unit)
    {
        bool enable = E->CHECKVAL<bool>(2, true);

        unit->SetWaterWalking(enable);
        return 0;
    }

   
    int SetStandState(Eluna* E, Unit* unit)
    {
        uint8 state = E->CHECKVAL<uint8>(2);

        unit->SetStandState(UnitStandStateType(state));
        return 0;
    }

    
    int SetInCombatWith(Eluna* E, Unit* unit)
    {
        Unit* enemy = E->CHECKOBJ<Unit>(2);
        unit->SetInCombatWith(enemy);
        return 0;
    }

    
    int SetFFA(Eluna* E, Unit* unit)
    {
        bool apply = E->CHECKVAL<bool>(2, true);

        if (apply)
        {
            unit->SetByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);
            for (Unit::ControlList::iterator itr = unit->m_Controlled.begin(); itr != unit->m_Controlled.end(); ++itr)
                (*itr)->SetByteValue(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);
        }
        else
        {
            unit->RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);
            for (Unit::ControlList::iterator itr = unit->m_Controlled.begin(); itr != unit->m_Controlled.end(); ++itr)
                (*itr)->RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);
        }

        return 0;
    }

    int Sanctuary(Eluna* E, Unit* unit)
    {
        bool apply = E->CHECKVAL<bool>(2, true);

        if (apply)
        {
            unit->SetByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_SANCTUARY);
            unit->CombatStop();
            unit->CombatStopWithPets();
        }
        else
            unit->RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_SANCTUARY);

        return 0;
    }

    
    int SetCritterGUID(Eluna* E, Unit* unit)
    {
        ObjectGuid guid = E->CHECKVAL<ObjectGuid>(2);
        unit->SetCritterGUID(guid);
        return 0;
    }

    
    int SetStunned(Eluna* E, Unit* unit)
    {
        bool apply = E->CHECKVAL<bool>(2, true);
        unit->SetControlled(apply, UNIT_STATE_STUNNED);
        return 0;
    }

    
    int SetRooted(Eluna* E, Unit* unit)
    {
        bool apply = E->CHECKVAL<bool>(2, true);

        unit->SetControlled(apply, UNIT_STATE_ROOT);
        return 0;
    }

   
    int SetConfused(Eluna* E, Unit* unit)
    {
        bool apply = E->CHECKVAL<bool>(2, true);

        unit->SetControlled(apply, UNIT_STATE_CONFUSED);
        return 0;
    }

   
    int SetFeared(Eluna* E, Unit* unit)
    {
        bool apply = E->CHECKVAL<bool>(2, true);

        unit->SetControlled(apply, UNIT_STATE_FLEEING);
        return 0;
    }

    
    int SetCanFly(Eluna* E, Unit* unit)
    {
        bool apply = E->CHECKVAL<bool>(2, true);

        unit->SetCanFly(apply);
        return 0;
    }

    
    int SetVisible(Eluna* E, Unit* unit)
    {
        bool x = E->CHECKVAL<bool>(2, true);

        unit->SetVisible(x);
        return 0;
    }

    /**
     * Mounts the [Unit] on the given displayID/modelID.
     *
     * @param uint32 displayId
     */
    int Mount(Eluna* E, Unit* unit)
    {
        uint32 displayId = E->CHECKVAL<uint32>(2);

        unit->Mount(displayId);
        return 0;
    }

    /**
     * Dismounts the [Unit].
     */
    int Dismount(Eluna* /*E*/, Unit* unit)
    {
        if (unit->IsMounted())
        {
            unit->Dismount();
            unit->RemoveAurasByType(SPELL_AURA_MOUNTED);
        }

        return 0;
    }

    /**
     * Makes the [Unit] perform the given emote.
     *
     * @param uint32 emoteId
     */
    int PerformEmote(Eluna* E, Unit* unit)
    {
        Emote emote = static_cast<Emote>(E->CHECKVAL<uint32>(2));
        unit->HandleEmoteCommand(emote);
        return 0;
    }

    /**
     * Makes the [Unit] perform the given emote continuously.
     *
     * @param uint32 emoteId
     */
    int EmoteState(Eluna* E, Unit* unit)
    {
        uint32 emoteId = E->CHECKVAL<uint32>(2);

        unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, emoteId);
        return 0;
    }

    /**
     * Returns calculated percentage from Health
     *
     * @return int32 percentage
     */
    int CountPctFromCurHealth(Eluna* E, Unit* unit)
    {
        E->Push(unit->CountPctFromCurHealth(E->CHECKVAL<int32>(2)));
        return 1;
    }

    /**
     * Returns calculated percentage from Max Health
     *
     * @return int32 percentage
     */
    int CountPctFromMaxHealth(Eluna* E, Unit* unit)
    {
        E->Push(unit->CountPctFromMaxHealth(E->CHECKVAL<int32>(2)));
        return 1;
    }

    /**
     * Sends chat message to [Player]
     *
     * @param uint8 type : chat, whisper, etc
     * @param uint32 lang : language to speak
     * @param string msg
     * @param [Player] target
     */
    int SendChatMessageToPlayer(Eluna* E, Unit* unit)
    {
        uint8 type = E->CHECKVAL<uint8>(2);
        uint32 lang = E->CHECKVAL<uint32>(3);
        std::string msg = E->CHECKVAL<std::string>(4);
        Player* target = E->CHECKOBJ<Player>(5);

        if (type >= MAX_CHAT_MSG_TYPE)
            return luaL_argerror(E->L, 2, "valid ChatMsg expected");
        if (lang >= LANGUAGES_COUNT)
            return luaL_argerror(E->L, 3, "valid Language expected");

        WorldPackets::Chat::Chat chat;
        chat.Initialize(ChatMsg(type), Language(lang), unit, target, msg);

        target->GetSession()->SendPacket(chat.Write());
        return 0;
    }

    /**
     * Stops the [Unit]'s movement
     */
    int MoveStop(Eluna* /*E*/, Unit* unit)
    {
        unit->StopMoving();
        return 0;
    }

    /**
     * The [Unit]'s movement expires and clears movement
     *
     * @param bool reset = true : cleans movement
     */
    int MoveExpire(Eluna* /*E*/, Unit* unit)
    {
        unit->GetMotionMaster()->Clear();
        return 0;
    }

    /**
     * Clears the [Unit]'s movement
     *
     * @param bool reset = true : clean movement
     */
    int MoveClear(Eluna* /*E*/, Unit* unit)
    {
        unit->GetMotionMaster()->Clear();
        return 0;
    }

    /**
     * The [Unit] will be idle
     */
    int MoveIdle(Eluna* /*E*/, Unit* unit)
    {
        unit->GetMotionMaster()->MoveIdle();
        return 0;
    }

    /**
     * The [Unit] will move at random
     *
     * @param float radius : limit on how far the [Unit] will move at random
     */
    int MoveRandom(Eluna* E, Unit* unit)
    {
        float radius = E->CHECKVAL<float>(2);
        float x, y, z;
        unit->GetPosition(x, y, z);
        unit->GetMotionMaster()->MoveRandom(radius);
        return 0;
    }

    /**
     * The [Unit] will move to its set home location
     */
    int MoveHome(Eluna* /*E*/, Unit* unit)
    {
        unit->GetMotionMaster()->MoveTargetedHome();
        return 0;
    }

    /**
     * The [Unit] will follow the target
     *
     * @param [Unit] target : target to follow
     * @param float dist = 0 : distance to start following
     * @param float angle = 0
     */
    int MoveFollow(Eluna* E, Unit* unit)
    {
        Unit* target = E->CHECKOBJ<Unit>(2);
        float dist = E->CHECKVAL<float>(3, 0.0f);
        float angle = E->CHECKVAL<float>(4, 0.0f);
        unit->GetMotionMaster()->MoveFollow(target, dist, angle);
        return 0;
    }

    /**
     * The [Unit] will chase the target
     *
     * @param [Unit] target : target to chase
     * @param float dist = 0 : distance start chasing
     * @param float angle = 0
     */
    int MoveChase(Eluna* E, Unit* unit)
    {
        Unit* target = E->CHECKOBJ<Unit>(2);
        float dist = E->CHECKVAL<float>(3, 0.0f);
        float angle = E->CHECKVAL<float>(4, 0.0f);
        unit->GetMotionMaster()->MoveChase(target, dist, angle);
        return 0;
    }

    /**
     * The [Unit] will move confused
     */
    int MoveConfused(Eluna* /*E*/, Unit* unit)
    {
        unit->GetMotionMaster()->MoveConfused();
        return 0;
    }

    /**
     * The [Unit] will flee
     *
     * @param [Unit] target
     * @param uint32 time = 0 : flee delay
     */
    int MoveFleeing(Eluna* E, Unit* unit)
    {
        Unit* target = E->CHECKOBJ<Unit>(2);
        uint32 time = E->CHECKVAL<uint32>(3, 0);
        unit->GetMotionMaster()->MoveFleeing(target, time);
        return 0;
    }

    /**
     * The [Unit] will move to the coordinates
     *
     * @param uint32 id : unique waypoint Id
     * @param float x
     * @param float y
     * @param float z
     * @param bool genPath = true : if true, generates path
     */
    int MoveTo(Eluna* E, Unit* unit)
    {
        uint32 id = E->CHECKVAL<uint32>(2);
        float x = E->CHECKVAL<float>(3);
        float y = E->CHECKVAL<float>(4);
        float z = E->CHECKVAL<float>(5);
        bool genPath = E->CHECKVAL<bool>(6, true);

        unit->GetMotionMaster()->MovePoint(id, x, y, z, genPath);
        return 0;
    }

    /**
     * Makes the [Unit] jump to the coordinates
     *
     * @param float x
     * @param float y
     * @param float z
     * @param float zSpeed : start velocity
     * @param float maxHeight : maximum height
     * @param uint32 id = 0 : unique movement Id
     */
    int MoveJump(Eluna* E, Unit* unit)
    {
        float x = E->CHECKVAL<float>(2);
        float y = E->CHECKVAL<float>(3);
        float z = E->CHECKVAL<float>(4);
        float zSpeed = E->CHECKVAL<float>(5);
        float maxHeight = E->CHECKVAL<float>(6);
        uint32 id = E->CHECKVAL<uint32>(7, 0);

        Position pos(x, y, z);

        unit->GetMotionMaster()->MoveJump(pos, zSpeed, maxHeight, id);
        return 0;
    }

    /**
     * The [Unit] will whisper the message to a [Player]
     *
     * @param string msg : message for the [Unit] to emote
     * @param uint32 lang : language for the [Unit] to speak
     * @param [Player] receiver : specific [Unit] to receive the message
     * @param bool bossWhisper = false : is a boss whisper
     */
    int SendUnitWhisper(Eluna* E, Unit* unit)
    {
        const char* msg = E->CHECKVAL<const char*>(2);
        uint32 lang = E->CHECKVAL<uint32>(3);
        (void)lang; // ensure that the variable is referenced in order to pass compiler checks
        Player* receiver = E->CHECKOBJ<Player>(4);
        bool bossWhisper = E->CHECKVAL<bool>(5, false);

        if (std::string(msg).length() > 0)
            unit->Whisper(msg, (Language)lang, receiver, bossWhisper);

        return 0;
    }

    /**
     * The [Unit] will emote the message
     *
     * @param string msg : message for the [Unit] to emote
     * @param [Unit] receiver = nil : specific [Unit] to receive the message
     * @param bool bossEmote = false : is a boss emote
     */
    int SendUnitEmote(Eluna* E, Unit* unit)
    {
        const char* msg = E->CHECKVAL<const char*>(2);
        Unit* receiver = E->CHECKOBJ<Unit>(3, false);
        bool bossEmote = E->CHECKVAL<bool>(4, false);

        if (std::string(msg).length() > 0)
            unit->TextEmote(msg, receiver, bossEmote);

        return 0;
    }

    /**
     * The [Unit] will say the message
     *
     * @param string msg : message for the [Unit] to say
     * @param uint32 language : language for the [Unit] to speak
     */
    int SendUnitSay(Eluna* E, Unit* unit)
    {
        const char* msg = E->CHECKVAL<const char*>(2);
        uint32 language = E->CHECKVAL<uint32>(3);

        if (std::string(msg).length() > 0)
            unit->Say(msg, (Language)language, unit);

        return 0;
    }

    /**
     * The [Unit] will yell the message
     *
     * @param string msg : message for the [Unit] to yell
     * @param uint32 language : language for the [Unit] to speak
     */
    int SendUnitYell(Eluna* E, Unit* unit)
    {
        const char* msg = E->CHECKVAL<const char*>(2);
        uint32 language = E->CHECKVAL<uint32>(3);

        if (std::string(msg).length() > 0)
            unit->Yell(msg, (Language)language, unit);

        return 0;
    }

    /**
     * Unmorphs the [Unit] setting it's display ID back to the native display ID.
     */
    int DeMorph(Eluna* /*E*/, Unit* unit)
    {
        unit->DeMorph();
        return 0;
    }

    /**
     * Makes the [Unit] cast the spell on the target.
     *
     * @param [Unit] target = nil : can be self or another unit
     * @param uint32 spell : entry of a spell
     * @param bool triggered = false : if true the spell is instant and has no cost
     */

    int CastSpell(Eluna* E, Unit* unit)
    {
        Unit* target = E->CHECKOBJ<Unit>(2, false);
        uint32 spell = E->CHECKVAL<uint32>(3);
        bool triggered = E->CHECKVAL<bool>(4, false);

        SpellInfo const* spellEntry = sSpellMgr->GetSpellInfo(spell);
        if (!spellEntry)
            return 0;

        unit->CastSpell(target, spell, triggered);
        return 0;
    }

    
    #if 0
    int CastCustomSpell(Eluna* E, Unit* unit)
    {
        Unit* target = E->CHECKOBJ<Unit>(2, false);
        uint32 spell = E->CHECKVAL<uint32>(3);
        bool triggered = E->CHECKVAL<bool>(4, false);
        bool has_bp0 = !lua_isnoneornil(E->L, 5);
        int32 bp0 = E->CHECKVAL<int32>(5, 0);
        bool has_bp1 = !lua_isnoneornil(E->L, 6);
        int32 bp1 = E->CHECKVAL<int32>(6, 0);
        bool has_bp2 = !lua_isnoneornil(E->L, 7);
        int32 bp2 = E->CHECKVAL<int32>(7, 0);
        Item* castItem = E->CHECKOBJ<Item>(8, false);
        ObjectGuid originalCaster = E->CHECKVAL<ObjectGuid>(9, ObjectGuid());

        CastSpellExtraArgs args;
        if (has_bp0)
            args.AddSpellMod(SPELLVALUE_BASE_POINT0, bp0);
        if (has_bp1)
            args.AddSpellMod(SPELLVALUE_BASE_POINT1, bp1);
        if (has_bp2)
            args.AddSpellMod(SPELLVALUE_BASE_POINT2, bp2);
        if (triggered)
            args.TriggerFlags = TRIGGERED_FULL_MASK;
        if (castItem)
            args.SetCastItem(castItem);
        if (!originalCaster.IsEmpty())
            args.SetOriginalCaster(originalCaster);

        unit->CastSpell(target, spell, args);
        return 0;
    }
    #endif

    #if 0
    int CastSpellAoF(Eluna* E, Unit* unit)
    {
        float _x = E->CHECKVAL<float>(2);
        float _y = E->CHECKVAL<float>(3);
        float _z = E->CHECKVAL<float>(4);
        uint32 spell = E->CHECKVAL<uint32>(5);
        bool triggered = E->CHECKVAL<bool>(6, true);

        CastSpellExtraArgs args;
        if (triggered)
            args.TriggerFlags = TRIGGERED_FULL_MASK;

        unit->CastSpell(Position(_x, _y, _z), spell, args);
        return 0;
    }
    #endif

    int StopSpellCast(Eluna* E, Unit* unit)
    {
        uint32 spellId = E->CHECKVAL<uint32>(2, 0);
        unit->CastStop(spellId);
        return 0;
    }


    int InterruptSpell(Eluna* E, Unit* unit)
    {
        int spellType = E->CHECKVAL<int>(2);
        bool delayed = E->CHECKVAL<bool>(3, true);
        switch (spellType)
        {
            case 0:
                spellType = CURRENT_MELEE_SPELL;
                break;
            case 1:
                spellType = CURRENT_GENERIC_SPELL;
                break;
            case 2:
                spellType = CURRENT_CHANNELED_SPELL;
                break;
            case 3:
                spellType = CURRENT_AUTOREPEAT_SPELL;
                break;
            default:
                return luaL_argerror(E->L, 2, "valid CurrentSpellTypes expected");
        }

        unit->InterruptSpell((CurrentSpellTypes)spellType, delayed);
        return 0;
    }

   
    int AddAura(Eluna* E, Unit* unit)
    {
        uint32 spell = E->CHECKVAL<uint32>(2);
        Unit* target = E->CHECKOBJ<Unit>(3);

        SpellInfo const* spellEntry = sSpellMgr->GetSpellInfo(spell);
        if (!spellEntry)
            return 1;

        E->Push(unit->AddAura(spell, target));
        return 1;
    }

    
    int RemoveAura(Eluna* E, Unit* unit)
    {
        uint32 spellId = E->CHECKVAL<uint32>(2);
        unit->RemoveAurasDueToSpell(spellId);
        return 0;
    }

    
    int RemoveAllAuras(Eluna* /*E*/, Unit* unit)
    {
        unit->RemoveAllAuras();
        return 0;
    }

    
    int RemoveArenaAuras(Eluna* /*E*/, Unit* unit)
    {
        unit->RemoveArenaAuras();
        return 0;
    }

    
    int AddUnitState(Eluna* E, Unit* unit)
    {
        uint32 state = E->CHECKVAL<uint32>(2);

        unit->AddUnitState(state);
        return 0;
    }

    
    int ClearUnitState(Eluna* E, Unit* unit)
    {
        uint32 state = E->CHECKVAL<uint32>(2);

        unit->ClearUnitState(state);
        return 0;
    }

    
    int NearTeleport(Eluna* E, Unit* unit)
    {
        float x = E->CHECKVAL<float>(2);
        float y = E->CHECKVAL<float>(3);
        float z = E->CHECKVAL<float>(4);
        float o = E->CHECKVAL<float>(5);

        unit->NearTeleportTo(x, y, z, o);
        return 0;
    }

    
    #if 0
    int DealDamage(Eluna* E, Unit* unit)
    {
        Unit* target = E->CHECKOBJ<Unit>(2);
        uint32 damage = E->CHECKVAL<uint32>(3);
        bool durabilityloss = E->CHECKVAL<bool>(4, true);
        uint32 school = E->CHECKVAL<uint32>(5, MAX_SPELL_SCHOOL);
        uint32 spell = E->CHECKVAL<uint32>(6, 0);
        if (school > MAX_SPELL_SCHOOL)
            return luaL_argerror(E->L, 6, "valid SpellSchool expected");

        // flat melee damage without resistence/etc reduction
        if (school == MAX_SPELL_SCHOOL)
        {
            Unit::DealDamage(unit, target, damage, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, durabilityloss);
            unit->SendAttackStateUpdate(HITINFO_AFFECTS_VICTIM, target, 1, SPELL_SCHOOL_MASK_NORMAL, damage, 0, 0, VICTIMSTATE_HIT, 0);
            return 0;
        }

        SpellSchoolMask schoolmask = SpellSchoolMask(1 << school);

        if (Unit::IsDamageReducedByArmor(schoolmask))
            damage = Unit::CalcArmorReducedDamage(unit, target, damage, NULL, BASE_ATTACK);

        // melee damage by specific school
        if (!spell)
        {
            DamageInfo dmgInfo(unit, target, damage, nullptr, schoolmask, SPELL_DIRECT_DAMAGE, BASE_ATTACK);
            unit->CalcAbsorbResist(dmgInfo);

            if (!dmgInfo.GetDamage())
                damage = 0;
            else
                damage = dmgInfo.GetDamage();

            uint32 absorb = dmgInfo.GetAbsorb();
            uint32 resist = dmgInfo.GetResist();
            unit->DealDamageMods(target, damage, &absorb);

            Unit::DealDamage(unit, target, damage, NULL, DIRECT_DAMAGE, schoolmask, NULL, false);
            unit->SendAttackStateUpdate(HITINFO_AFFECTS_VICTIM, target, 0, schoolmask, damage, absorb, resist, VICTIMSTATE_HIT, 0);
            return 0;
        }

        if (!spell)
            return 0;

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell);
        if (!spellInfo)
            return 0;

        SpellNonMeleeDamage dmgInfo(unit, target, spell, spellInfo->GetSchoolMask());
        Unit::DealDamageMods(dmgInfo.target, dmgInfo.damage, &dmgInfo.absorb);

        unit->SendSpellNonMeleeDamageLog(&dmgInfo);
        unit->DealSpellDamage(&dmgInfo, true);
        return 0;
    }
    #endif
    
    int DealHeal(Eluna* E, Unit* unit)
    {
        Unit* target = E->CHECKOBJ<Unit>(2);
        uint32 spell = E->CHECKVAL<uint32>(3);
        uint32 amount = E->CHECKVAL<uint32>(4);
        bool critical = E->CHECKVAL<bool>(5, false);

        if (const SpellInfo* info = sSpellMgr->GetSpellInfo(spell))
        {
            HealInfo healInfo(unit, target, amount, info, info->GetSchoolMask());
            unit->HealBySpell(healInfo, critical);
        }

        return 0;
    }

    
    #if 0
    int Kill(Eluna* E, Unit* unit)
    {
        Unit* target = E->CHECKOBJ<Unit>(2);
        bool durLoss = E->CHECKVAL<bool>(3, true);

        Unit::Kill(unit, target, durLoss);
        return 0;
    }
    #endif
    
    int RestoreDisplayId(Eluna* /*E*/, Unit* unit)
    {
        unit->RestoreDisplayId();
        return 0;
    }

   
    int RestoreFaction(Eluna* /*E*/, Unit* unit)
    {
        unit->RestoreFaction();
        return 0;
    }

    
    int RemoveBindSightAuras(Eluna* /*E*/, Unit* unit)
    {
        unit->RemoveBindSightAuras();
        return 0;
    }

   
    int RemoveCharmAuras(Eluna* /*E*/, Unit* unit)
    {
        unit->RemoveCharmAuras();
        return 0;
    }

    
    int DisableMelee(Eluna* E, Unit* unit)
    {
        bool apply = E->CHECKVAL<bool>(2, true);

        if (apply)
            unit->AddUnitState(UNIT_STATE_CANNOT_AUTOATTACK);
        else
            unit->ClearUnitState(UNIT_STATE_CANNOT_AUTOATTACK);

        return 0;
    }

    
    int CanModifyStats(Eluna* E, Unit* unit)
    {
        E->Push(unit->CanModifyStats());
        return 1;
    }

    
    #if 0
    int AddFlatStatModifier(Eluna* E, Unit* unit)
    {
        uint32 statType = E->CHECKVAL<uint32>(2);
        uint8 modType = E->CHECKVAL<uint8>(3);
        float value = E->CHECKVAL<float>(4);
        bool apply = E->CHECKVAL<bool>(5, true);

        unit->HandleStatFlatModifier(UnitMods(UNIT_MOD_STAT_START + statType), (UnitModifierFlatType)modType, value, apply);
        return 0;
    }
    #endif
    
     #if 0
    int AddPctStatModifier(Eluna* E, Unit* unit)
    {
        uint32 statType = E->CHECKVAL<uint32>(2);
        uint8 modType = E->CHECKVAL<uint8>(3);
        float value = E->CHECKVAL<float>(4);

        unit->ApplyStatPctModifier(UnitMods(UNIT_MOD_STAT_START + statType), (UnitModifierPctType)modType, value);
        return 0;
    }
    #endif

        // ==========================================
        // [修复补丁] 还原被阉割的 SpawnCreature / SummonCreature
        // ==========================================
        static int SpawnCreature(Eluna* E, Unit* unit)
        {
            // 严格按顺序读取 Lua 传进来的所有 7 个参数（因为参数 1 是 player 自己）
            uint32 entry = E->CHECKVAL<uint32>(2);
            float x = E->CHECKVAL<float>(3);
            float y = E->CHECKVAL<float>(4);
            float z = E->CHECKVAL<float>(5);
            float o = E->CHECKVAL<float>(6);
            uint32 spawnType = E->CHECKVAL<uint32>(7); 
            uint32 despawnTimer = E->CHECKVAL<uint32>(8);

            // 调用 C++ 原生的召唤方法
            Creature* creature = unit->SummonCreature(entry, x, y, z, o, (TempSummonType)spawnType, despawnTimer);
            
            // 将召唤出来的实体返回给 Lua
            if (creature)
                E->Push(creature); 
            else
                E->Push(); // 返回 nil
                
            return 1;
        }





    ElunaRegister<Unit> UnitMethods[] =
    {
        // Getters
        { "GetHealth", &LuaUnit::GetHealth },
        { "GetDisplayId", &LuaUnit::GetDisplayId },
        { "GetNativeDisplayId", &LuaUnit::GetNativeDisplayId },
        { "GetPower", &LuaUnit::GetPower },
        { "GetMaxPower", &LuaUnit::GetMaxPower },
        { "GetPowerType", &LuaUnit::GetPowerType },
        { "GetMaxHealth", &LuaUnit::GetMaxHealth },
        { "GetHealthPct", &LuaUnit::GetHealthPct },
        { "GetPowerPct", &LuaUnit::GetPowerPct },


        { "SpawnCreature", &LuaUnit::SpawnCreature },
        { "SummonCreature", &LuaUnit::SpawnCreature }, // 顺便起个别名以防万一




        { "GetAura", &LuaUnit::GetAura },
        { "GetOwnedAura", &LuaUnit::GetOwnedAura },

        { "GetCurrentSpell", &LuaUnit::GetCurrentSpell },
        { "GetCreatureType", &LuaUnit::GetCreatureType },

        { "GetOwner", &LuaUnit::GetOwner },
        { "GetFriendlyUnitsInRange", &LuaUnit::GetFriendlyUnitsInRange },
        { "GetUnfriendlyUnitsInRange", &LuaUnit::GetUnfriendlyUnitsInRange },
        { "GetOwnerGUID", &LuaUnit::GetOwnerGUID },
        { "GetCreatorGUID", &LuaUnit::GetCreatorGUID },
        { "GetMinionGUID", &LuaUnit::GetPetGUID },
        { "GetCharmerGUID", &LuaUnit::GetCharmerGUID },

        { "GetPetGUID", &LuaUnit::GetPetGUID },
        { "GetCritterGUID", &LuaUnit::GetCritterGUID },
        { "GetControllerGUID", &LuaUnit::GetControllerGUID },
        { "GetControllerGUIDS", &LuaUnit::GetControllerGUIDS },
        { "GetStandState", &LuaUnit::GetStandState },
        { "GetVictim", &LuaUnit::GetVictim },
        { "GetSpeed", &LuaUnit::GetSpeed },
        { "GetStat", &LuaUnit::GetStat },
        { "GetBaseSpellPower", &LuaUnit::GetBaseSpellPower },

        { "GetMovementType", &LuaUnit::GetMovementType },

        // Setters

        { "SetLevel", &LuaUnit::SetLevel },
        { "SetHealth", &LuaUnit::SetHealth },
        { "SetMaxHealth", &LuaUnit::SetMaxHealth },
        { "SetPower", &LuaUnit::SetPower },
        { "SetMaxPower", &LuaUnit::SetMaxPower },
        { "SetPowerType", &LuaUnit::SetPowerType },
        { "SetDisplayId", &LuaUnit::SetDisplayId },
        { "SetNativeDisplayId", &LuaUnit::SetNativeDisplayId },
        { "SetFacing", &LuaUnit::SetFacing },
        { "SetFacingToObject", &LuaUnit::SetFacingToObject },
        { "SetSpeed", &LuaUnit::SetSpeed },
        { "SetStunned", &LuaUnit::SetStunned },
        { "SetRooted", &LuaUnit::SetRooted },
        { "SetConfused", &LuaUnit::SetConfused },
        { "SetFeared", &LuaUnit::SetFeared },
        { "SetPvP", &LuaUnit::SetPvP },
        { "SetFFA", &LuaUnit::SetFFA },

        { "SetCanFly", &LuaUnit::SetCanFly },
        { "SetVisible", &LuaUnit::SetVisible },
        { "SetOwnerGUID", &LuaUnit::SetOwnerGUID },
        { "SetName", &LuaUnit::SetName },
        { "SetSheath", &LuaUnit::SetSheath },
        { "SetCreatorGUID", &LuaUnit::SetCreatorGUID },
        { "SetMinionGUID", &LuaUnit::SetPetGUID },
        { "SetPetGUID", &LuaUnit::SetPetGUID },
        { "SetCritterGUID", &LuaUnit::SetCritterGUID },
        { "SetWaterWalk", &LuaUnit::SetWaterWalk },
        { "SetStandState", &LuaUnit::SetStandState },
        { "SetInCombatWith", &LuaUnit::SetInCombatWith },
        { "ModifyPower", &LuaUnit::ModifyPower },
        { "SetImmuneTo", &LuaUnit::SetImmuneTo },

        // Boolean
        { "IsAlive", &LuaUnit::IsAlive },
        { "IsDead", &LuaUnit::IsDead },
        { "IsDying", &LuaUnit::IsDying },
        { "IsPvPFlagged", &LuaUnit::IsPvPFlagged },
        { "IsInCombat", &LuaUnit::IsInCombat },
        { "IsBanker", &LuaUnit::IsBanker },
        { "IsBattleMaster", &LuaUnit::IsBattleMaster },
        { "IsCharmed", &LuaUnit::IsCharmed },
        { "IsArmorer", &LuaUnit::IsArmorer },
        { "IsAttackingPlayer", &LuaUnit::IsAttackingPlayer },
        { "IsInWater", &LuaUnit::IsInWater },
        { "IsUnderWater", &LuaUnit::IsUnderWater },
        { "IsAuctioneer", &LuaUnit::IsAuctioneer },
        { "IsGuildMaster", &LuaUnit::IsGuildMaster },
        { "IsInnkeeper", &LuaUnit::IsInnkeeper },
        { "IsTrainer", &LuaUnit::IsTrainer },
        { "IsGossip", &LuaUnit::IsGossip },
        { "IsTaxi", &LuaUnit::IsTaxi },
        { "IsSpiritHealer", &LuaUnit::IsSpiritHealer },
        { "IsSpiritGuide", &LuaUnit::IsSpiritGuide },
        { "IsTabardDesigner", &LuaUnit::IsTabardDesigner },
        { "IsServiceProvider", &LuaUnit::IsServiceProvider },
        { "IsSpiritService", &LuaUnit::IsSpiritService },
        { "HealthBelowPct", &LuaUnit::HealthBelowPct },
        { "HealthAbovePct", &LuaUnit::HealthAbovePct },
        { "IsMounted", &LuaUnit::IsMounted },
        { "AttackStop", &LuaUnit::AttackStop },
        { "Attack", &LuaUnit::Attack },
        { "IsVisible", &LuaUnit::IsVisible },
        { "IsMoving", &LuaUnit::IsMoving },
        { "IsFlying", &LuaUnit::IsFlying },
        { "IsStopped", &LuaUnit::IsStopped },
        { "HasUnitState", &LuaUnit::HasUnitState },
        { "IsQuestGiver", &LuaUnit::IsQuestGiver },
        { "IsInAccessiblePlaceFor", &LuaUnit::IsInAccessiblePlaceFor },
        { "IsVendor", &LuaUnit::IsVendor },

        { "IsFullHealth", &LuaUnit::IsFullHealth },
        { "HasAura", &LuaUnit::HasAura },
        { "IsCasting", &LuaUnit::IsCasting },
        { "IsStandState", &LuaUnit::IsStandState },

        { "CanModifyStats", &LuaUnit::CanModifyStats },

        // Other
        { "AddAura", &LuaUnit::AddAura },
        { "RemoveAura", &LuaUnit::RemoveAura },
        { "RemoveAllAuras", &LuaUnit::RemoveAllAuras },
        { "RemoveArenaAuras", &LuaUnit::RemoveArenaAuras },

        { "DeMorph", &LuaUnit::DeMorph },
        { "SendUnitWhisper", &LuaUnit::SendUnitWhisper },
        { "SendUnitEmote", &LuaUnit::SendUnitEmote },
        { "SendUnitSay", &LuaUnit::SendUnitSay },
        { "SendUnitYell", &LuaUnit::SendUnitYell },
        
        { "CastSpell", &LuaUnit::CastSpell },       



        { "StopSpellCast", &LuaUnit::StopSpellCast },
        { "InterruptSpell", &LuaUnit::InterruptSpell },
        { "SendChatMessageToPlayer", &LuaUnit::SendChatMessageToPlayer },
        { "PerformEmote", &LuaUnit::PerformEmote },
        { "EmoteState", &LuaUnit::EmoteState },
        { "CountPctFromCurHealth", &LuaUnit::CountPctFromCurHealth },
        { "CountPctFromMaxHealth", &LuaUnit::CountPctFromMaxHealth },
        { "Dismount", &LuaUnit::Dismount },
        { "Mount", &LuaUnit::Mount },
        { "RestoreDisplayId", &LuaUnit::RestoreDisplayId },
        { "RestoreFaction", &LuaUnit::RestoreFaction },
        { "RemoveBindSightAuras", &LuaUnit::RemoveBindSightAuras },
        { "RemoveCharmAuras", &LuaUnit::RemoveCharmAuras },
        { "ClearUnitState", &LuaUnit::ClearUnitState },
        { "AddUnitState", &LuaUnit::AddUnitState },
        { "DisableMelee", &LuaUnit::DisableMelee },
        { "NearTeleport", &LuaUnit::NearTeleport },
        { "MoveIdle", &LuaUnit::MoveIdle },
        { "MoveRandom", &LuaUnit::MoveRandom },
        { "MoveHome", &LuaUnit::MoveHome },
        { "MoveFollow", &LuaUnit::MoveFollow },
        { "MoveChase", &LuaUnit::MoveChase },
        { "MoveConfused", &LuaUnit::MoveConfused },
        { "MoveFleeing", &LuaUnit::MoveFleeing },
        { "MoveTo", &LuaUnit::MoveTo },
        { "MoveJump", &LuaUnit::MoveJump },
        { "MoveStop", &LuaUnit::MoveStop },
        { "MoveExpire", &LuaUnit::MoveExpire },
        { "MoveClear", &LuaUnit::MoveClear },

        { "DealHeal", &LuaUnit::DealHeal },



        // Not implemented methods
        { "SummonGuardian", METHOD_REG_NONE } // not implemented
    };
};
#endif
