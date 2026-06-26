/*
 * Copyright (C) 2010 - 2024 Eluna Lua Engine <https://elunaluaengine.github.io/>
 * This program is free software licensed under GPL version 3
 * Please see the included DOCS/LICENSE.md for more information
 */

#ifndef _ELUNA_CREATURE_AI_H
#define _ELUNA_CREATURE_AI_H

#include "LuaEngine.h"

// [新增]: 强制声明全局唯一引擎
extern class Eluna* sEluna;

#if defined ELUNA_CMANGOS
#include "AI/BaseAI/CreatureAI.h"
#endif

#if defined ELUNA_TRINITY || defined ELUNA_AZEROTHCORE
struct ScriptedAI;
typedef ScriptedAI NativeScriptedAI;
#elif defined ELUNA_CMANGOS || ELUNA_MANGOS
class CreatureAI;
typedef CreatureAI NativeScriptedAI;
#elif defined ELUNA_VMANGOS
class BasicAI;
typedef BasicAI NativeScriptedAI;
#endif

struct ElunaCreatureAI : NativeScriptedAI
{
    bool justSpawned;
    std::vector< std::pair<uint32, uint32> > movepoints;
#if !defined ELUNA_TRINITY && !defined ELUNA_AZEROTHCORE
#define me  m_creature
#endif
    ElunaCreatureAI(Creature* creature) : NativeScriptedAI(creature), justSpawned(true)
    {
    }
    ~ElunaCreatureAI() { }

#if !defined ELUNA_TRINITY
    void UpdateAI(const uint32 diff) override
#else
    void UpdateAI(uint32 diff) override
#endif
    {
#if !defined ELUNA_TRINITY
        if (justSpawned)
        {
            justSpawned = false;
            JustRespawned();
        }
#endif
        if (!movepoints.empty())
        {
            for (auto& point : movepoints)
            {
                if (!sEluna->MovementInform(me, point.first, point.second)) // [修复]: sEluna
                    NativeScriptedAI::MovementInform(point.first, point.second);
            }
            movepoints.clear();
        }

        if (!sEluna->UpdateAI(me, diff)) // [修复]: sEluna
        {
#if !defined ELUNA_MANGOS
            if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC))
#else
            if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE))
#endif
                NativeScriptedAI::UpdateAI(diff);
        }
    }

/* [屏蔽] 7.3.5 参数不兼容: JustEngagedWith / EnterCombat
#if defined ELUNA_TRINITY || defined ELUNA_AZEROTHCORE
    void JustEngagedWith(Unit* target) 
    {
        if (!sEluna->EnterCombat(me, target))
            NativeScriptedAI::JustEngagedWith(target);
    }
#else
    void EnterCombat(Unit* target) override
    {
        if (!sEluna->EnterCombat(me, target))
            NativeScriptedAI::EnterCombat(target);
    }
#endif
*/

/* [屏蔽] 7.3.5 参数不兼容: DamageTaken
#if defined ELUNA_TRINITY || defined ELUNA_CMANGOS 
    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType damageType, SpellInfo const* spellInfo) 
#elif defined ELUNA_AZEROTHCORE
    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType damagetype, SpellSchoolMask damageSchoolMask) 
#else
    void DamageTaken(Unit* attacker, uint32& damage) 
#endif
    {
        if (!sEluna->DamageTaken(me, attacker, damage))
        {
#if defined ELUNA_TRINITY || defined ELUNA_CMANGOS
            NativeScriptedAI::DamageTaken(attacker, damage, damageType, spellInfo);
#elif defined ELUNA_AZEROTHCORE
            NativeScriptedAI::DamageTaken(attacker, damage, damagetype, damageSchoolMask);
#else
            NativeScriptedAI::DamageTaken(attacker, damage);
#endif
        }
    }
*/

    void JustDied(Unit* killer) override
    {
        if (!sEluna->JustDied(me, killer)) // [修复]: sEluna
            NativeScriptedAI::JustDied(killer);
    }

    void KilledUnit(Unit* victim) override
    {
        if (!sEluna->KilledUnit(me, victim)) // [修复]: sEluna
            NativeScriptedAI::KilledUnit(victim);
    }

    void JustSummoned(Creature* summon) override
    {
        if (!sEluna->JustSummoned(me, summon)) // [修复]: sEluna
            NativeScriptedAI::JustSummoned(summon);
    }

    void SummonedCreatureDespawn(Creature* summon) override
    {
        if (!sEluna->SummonedCreatureDespawn(me, summon)) // [修复]: sEluna
            NativeScriptedAI::SummonedCreatureDespawn(summon);
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        movepoints.push_back(std::make_pair(type, id));
    }

    void AttackStart(Unit* target) override
    {
        if (!sEluna->AttackStart(me, target)) // [修复]: sEluna
            NativeScriptedAI::AttackStart(target);
    }

#if defined ELUNA_TRINITY || defined ELUNA_AZEROTHCORE
    void EnterEvadeMode(EvadeReason /*why*/) override
#else
    void EnterEvadeMode() override
#endif
    {
        if (!sEluna->EnterEvadeMode(me)) // [修复]: sEluna
            NativeScriptedAI::EnterEvadeMode();
    }

/* [屏蔽] 7.3.5 参数不兼容: JustAppeared / JustRespawned
#if defined ELUNA_TRINITY
    void JustAppeared() 
    {
        if (!sEluna->JustRespawned(me))
            NativeScriptedAI::JustAppeared();
    }
#else
    void JustRespawned() override
    {
        if (!sEluna->JustRespawned(me))
            NativeScriptedAI::JustRespawned();
    }
#endif
*/

    void JustReachedHome() override
    {
        if (!sEluna->JustReachedHome(me)) // [修复]: sEluna
            NativeScriptedAI::JustReachedHome();
    }

    void ReceiveEmote(Player* player, uint32 emoteId) override
    {
        if (!sEluna->ReceiveEmote(me, player, emoteId)) // [修复]: sEluna
            NativeScriptedAI::ReceiveEmote(player, emoteId);
    }

    void CorpseRemoved(uint32& respawnDelay) override
    {
        if (!sEluna->CorpseRemoved(me, respawnDelay)) // [修复]: sEluna
            NativeScriptedAI::CorpseRemoved(respawnDelay);
    }

#if !defined ELUNA_TRINITY && !defined ELUNA_VMANGOS && !defined ELUNA_AZEROTHCORE
    bool IsVisible(Unit* who) const override
    {
        return true;
    }
#endif

    void MoveInLineOfSight(Unit* who) override
    {
        if (!sEluna->MoveInLineOfSight(me, who)) // [修复]: sEluna
            NativeScriptedAI::MoveInLineOfSight(who);
    }

/* [屏蔽] 7.3.5 参数不兼容: SpellHit
#if defined ELUNA_TRINITY
    void SpellHit(WorldObject* caster, SpellInfo const* spell) 
#elif defined ELUNA_VMANGOS
    void SpellHit(Unit* caster, SpellInfo const* spell)
#else
    void SpellHit(Unit* caster, SpellInfo const* spell)
#endif
    {
        if (!sEluna->SpellHit(me, caster, spell))
            NativeScriptedAI::SpellHit(caster, spell);
    }
*/

/* [屏蔽] 7.3.5 参数不兼容: SpellHitTarget
#if defined ELUNA_TRINITY
    void SpellHitTarget(WorldObject* target, SpellInfo const* spell) 
#else
    void SpellHitTarget(Unit* target, SpellInfo const* spell) 
#endif
    {
        if (!sEluna->SpellHitTarget(me, target, spell))
            NativeScriptedAI::SpellHitTarget(target, spell);
    }
*/

#if defined ELUNA_TRINITY || defined ELUNA_AZEROTHCORE
/* [屏蔽] 7.3.5 参数不兼容: IsSummonedBy
    void IsSummonedBy(WorldObject* summoner) 
    {
        if (!summoner->ToUnit() || !sEluna->OnSummoned(me, summoner->ToUnit()))
            NativeScriptedAI::IsSummonedBy(summoner);
    }
*/

    void SummonedCreatureDies(Creature* summon, Unit* killer) override
    {
        if (!sEluna->SummonedCreatureDies(me, summon, killer)) // [修复]: sEluna
            NativeScriptedAI::SummonedCreatureDies(summon, killer);
    }

    void OwnerAttackedBy(Unit* attacker) override
    {
        if (!sEluna->OwnerAttackedBy(me, attacker)) // [修复]: sEluna
            NativeScriptedAI::OwnerAttackedBy(attacker);
    }

    void OwnerAttacked(Unit* target) override
    {
        if (!sEluna->OwnerAttacked(me, target)) // [修复]: sEluna
            NativeScriptedAI::OwnerAttacked(target);
    }
#endif

#if !defined ELUNA_TRINITY && !defined ELUNA_AZEROTHCORE
#undef me
#endif
};

#endif