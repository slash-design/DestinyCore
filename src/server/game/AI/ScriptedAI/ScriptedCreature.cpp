/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ObjectMgr.h"
#include "ScriptedCreature.h"
#include "AreaBoundary.h"
#include "DB2Stores.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "InstanceScript.h"
#include "Log.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "Spell.h"
#include "SpellMgr.h"
#include "TemporarySummon.h"
#include "Group.h"
#include "Pet.h"
#include <cmath>

// Spell summary for ScriptedAI::SelectSpell
struct TSpellSummary
{
    uint8 Targets;                                          // set of enum SelectTarget
    uint8 Effects;                                          // set of enum SelectEffect
} extern* SpellSummary;

void SummonList::Summon(Creature const* summon)
{
    storage_.push_back(summon->GetGUID());
}

void SummonList::Despawn(Creature const* summon)
{
    storage_.remove(summon->GetGUID());
}

void SummonList::DoZoneInCombat(uint32 entry, float maxRangeToNearestTarget)
{
    for (StorageType::iterator i = storage_.begin(); i != storage_.end();)
    {
        Creature* summon = ObjectAccessor::GetCreature(*me, *i);
        ++i;
        if (summon && summon->IsAIEnabled
                && (!entry || summon->GetEntry() == entry))
        {
            summon->AI()->DoZoneInCombat(nullptr, maxRangeToNearestTarget);
        }
    }
}

void SummonList::DespawnEntry(uint32 entry)
{
    for (StorageType::iterator i = storage_.begin(); i != storage_.end();)
    {
        Creature* summon = ObjectAccessor::GetCreature(*me, *i);
        if (!summon)
            i = storage_.erase(i);
        else if (summon->GetEntry() == entry)
        {
            i = storage_.erase(i);
            summon->DespawnOrUnsummon();
        }
        else
            ++i;
    }
}

void SummonList::DespawnAll()
{
    while (!storage_.empty())
    {
        Creature* summon = ObjectAccessor::GetCreature(*me, storage_.front());
        storage_.pop_front();
        if (summon)
            summon->DespawnOrUnsummon();
    }
}

void SummonList::RemoveNotExisting()
{
    for (StorageType::iterator i = storage_.begin(); i != storage_.end();)
    {
        if (ObjectAccessor::GetCreature(*me, *i))
            ++i;
        else
            i = storage_.erase(i);
    }
}

bool SummonList::HasEntry(uint32 entry) const
{
    for (StorageType::const_iterator i = storage_.begin(); i != storage_.end(); ++i)
    {
        Creature* summon = ObjectAccessor::GetCreature(*me, *i);
        if (summon && summon->GetEntry() == entry)
            return true;
    }

    return false;
}

void SummonList::DoActionImpl(int32 action, StorageType const& summons)
{
    for (auto const& guid : summons)
    {
        Creature* summon = ObjectAccessor::GetCreature(*me, guid);
        if (summon && summon->IsAIEnabled)
            summon->AI()->DoAction(action);
    }
}

ScriptedAI::ScriptedAI(Creature* creature) : CreatureAI(creature),
    IsFleeing(false),
    summons(creature),
    damageEvents(creature),
    instance(creature->GetInstanceScript()),
    _isCombatMovementAllowed(true),
	haseventdata(false),
    hastalkdata(false)
{
    _isHeroic = me->GetMap()->IsHeroic();
    _difficulty = me->GetMap()->GetDifficultyID();
}

void ScriptedAI::AttackStartNoMove(Unit* who)
{
    if (!who)
        return;

    if (me->Attack(who, true))
        DoStartNoMovement(who);
}

void ScriptedAI::AttackStart(Unit* who)
{
    if (IsCombatMovementAllowed())
        CreatureAI::AttackStart(who);
    else
        AttackStartNoMove(who);
}

void ScriptedAI::UpdateAI(uint32 diff)
{
    //Check if we have a current target
    if (!UpdateVictim())
        return;

    events.Update(diff);

    DoMeleeAttackIfReady();
}

void ScriptedAI::SetUnlock(uint32 time)
{
    me->GetScheduler().Schedule(Milliseconds(time), [this](TaskContext context)
    {
        IsLock = false;
    });
}

void ScriptedAI::DoStartMovement(Unit* victim, float distance, float angle)
{
    if (victim)
        me->GetMotionMaster()->MoveChase(victim, distance, angle);
}

void ScriptedAI::DoStartNoMovement(Unit* victim)
{
    if (!victim)
        return;

    me->GetMotionMaster()->MoveIdle();
}

void ScriptedAI::DoStopAttack()
{
    if (me->GetVictim())
        me->AttackStop();
}

void ScriptedAI::DoCastSpell(Unit* target, SpellInfo const* spellInfo, bool triggered)
{
    if (!target || me->IsNonMeleeSpellCast(false))
        return;

    me->StopMoving();
    me->CastSpell(target, spellInfo, triggered ? TRIGGERED_FULL_MASK : TRIGGERED_NONE);
}

void ScriptedAI::DoPlaySoundToSet(WorldObject* source, uint32 soundId)
{
    if (!source)
        return;

    if (!sSoundKitStore.LookupEntry(soundId))
    {
        TC_LOG_ERROR("scripts", "Invalid soundId %u used in DoPlaySoundToSet (Source: %s)", soundId, source->GetGUID().ToString().c_str());
        return;
    }

    source->PlayDirectSound(soundId);
}

Creature* ScriptedAI::DoSpawnCreature(uint32 entry, float offsetX, float offsetY, float offsetZ, float angle, uint32 type, uint32 despawntime)
{
    return me->SummonCreature(entry, me->GetPositionX() + offsetX, me->GetPositionY() + offsetY, me->GetPositionZ() + offsetZ, angle, TempSummonType(type), despawntime);
}

bool ScriptedAI::HealthBelowPct(uint32 pct) const
{
    return me->HealthBelowPct(pct);
}

bool ScriptedAI::HealthAbovePct(uint32 pct) const
{
    return me->HealthAbovePct(pct);
}

SpellInfo const* ScriptedAI::SelectSpell(Unit* target, uint32 school, uint32 mechanic, SelectTargetType targets, float rangeMin, float rangeMax, SelectEffect effect)
{
    //No target so we can't cast
    if (!target)
        return nullptr;

    //Silenced so we can't cast
    if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED))
        return nullptr;

    //Using the extended script system we first create a list of viable spells
    SpellInfo const* apSpell[MAX_CREATURE_SPELLS];
    memset(apSpell, 0, MAX_CREATURE_SPELLS * sizeof(SpellInfo*));

    uint32 spellCount = 0;

    SpellInfo const* tempSpell = nullptr;

    //Check if each spell is viable(set it to null if not)
    for (uint32 i = 0; i < MAX_CREATURE_SPELLS; i++)
    {
        tempSpell = sSpellMgr->GetSpellInfo(me->m_spells[i]);

        //This spell doesn't exist
        if (!tempSpell)
            continue;

        // Targets and Effects checked first as most used restrictions
        //Check the spell targets if specified
        if (targets && !(SpellSummary[me->m_spells[i]].Targets & (1 << (targets-1))))
            continue;

        //Check the type of spell if we are looking for a specific spell type
        if (effect && !(SpellSummary[me->m_spells[i]].Effects & (1 << (effect-1))))
            continue;

        //Check for school if specified
        if (school && (tempSpell->SchoolMask & school) == 0)
            continue;

        //Check for spell mechanic if specified
        if (mechanic && tempSpell->Mechanic != mechanic)
            continue;

        //Check if the spell meets our range requirements
        if (rangeMin && me->GetSpellMinRangeForTarget(target, tempSpell) < rangeMin)
            continue;
        if (rangeMax && me->GetSpellMaxRangeForTarget(target, tempSpell) > rangeMax)
            continue;

        //Check if our target is in range
        if (me->IsWithinDistInMap(target, float(me->GetSpellMinRangeForTarget(target, tempSpell))) || !me->IsWithinDistInMap(target, float(me->GetSpellMaxRangeForTarget(target, tempSpell))))
            continue;

        //All good so lets add it to the spell list
        apSpell[spellCount] = tempSpell;
        ++spellCount;
    }

    //We got our usable spells so now lets randomly pick one
    if (!spellCount)
        return nullptr;

    return apSpell[urand(0, spellCount - 1)];
}

void ScriptedAI::DoResetThreat()
{
    if (!me->CanHaveThreatList() || me->getThreatManager().isThreatListEmpty())
    {
        TC_LOG_ERROR("scripts", "DoResetThreat called for creature that either cannot have threat list or has empty threat list (me entry = %d)", me->GetEntry());
        return;
    }

    ThreatContainer::StorageType threatlist = me->getThreatManager().getThreatList();

    for (ThreatContainer::StorageType::const_iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
    {
        Unit* unit = ObjectAccessor::GetUnit(*me, (*itr)->getUnitGuid());
        if (unit && DoGetThreat(unit))
            DoModifyThreatPercent(unit, -100);
    }
}

float ScriptedAI::DoGetThreat(Unit* unit)
{
    if (!unit)
        return 0.0f;
    return me->getThreatManager().getThreat(unit);
}

void ScriptedAI::DoModifyThreatPercent(Unit* unit, int32 pct)
{
    if (!unit)
        return;
    me->getThreatManager().modifyThreatPercent(unit, pct);
}

void ScriptedAI::DoTeleportTo(float x, float y, float z, uint32 time)
{
    me->Relocate(x, y, z);
    float speed = me->GetDistance(x, y, z) / ((float)time * 0.001f);
    me->MonsterMoveWithSpeed(x, y, z, speed);
}

void ScriptedAI::DoTeleportTo(const float position[4])
{
    me->NearTeleportTo(position[0], position[1], position[2], position[3]);
}

void ScriptedAI::DoTeleportPlayer(Unit* unit, float x, float y, float z, float o)
{
    if (!unit)
        return;

    if (Player* player = unit->ToPlayer())
        player->TeleportTo(unit->GetMapId(), x, y, z, o, TELE_TO_NOT_LEAVE_COMBAT);
    else
        TC_LOG_ERROR("scripts", "Creature %s Tried to teleport non-player unit (%s) to x: %f y:%f z: %f o: %f. Aborted.",
            me->GetGUID().ToString().c_str(), unit->GetGUID().ToString().c_str(), x, y, z, o);
}

void ScriptedAI::DoTeleportAll(float x, float y, float z, float o)
{
    Map* map = me->GetMap();
    if (!map->IsDungeon())
        return;

    Map::PlayerList const& PlayerList = map->GetPlayers();
    for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
        if (Player* player = itr->GetSource())
            if (player->IsAlive())
                player->TeleportTo(me->GetMapId(), x, y, z, o, TELE_TO_NOT_LEAVE_COMBAT);
}

Unit* ScriptedAI::DoSelectLowestHpFriendly(float range, uint32 minHPDiff)
{
    Unit* unit = nullptr;
    Trinity::MostHPMissingInRange u_check(me, range, minHPDiff);
    Trinity::UnitLastSearcher<Trinity::MostHPMissingInRange> searcher(me, unit, u_check);
    Cell::VisitAllObjects(me, searcher, range);

    return unit;
}

Unit* ScriptedAI::DoSelectBelowHpPctFriendlyWithEntry(uint32 entry, float range, uint8 minHPDiff, bool excludeSelf)
{
    Unit* unit = nullptr;
    Trinity::FriendlyBelowHpPctEntryInRange u_check(me, entry, range, minHPDiff, excludeSelf);
    Trinity::UnitLastSearcher<Trinity::FriendlyBelowHpPctEntryInRange> searcher(me, unit, u_check);
    Cell::VisitAllObjects(me, searcher, range);

    return unit;
}

std::list<Creature*> ScriptedAI::DoFindFriendlyCC(float range)
{
    std::list<Creature*> list;
    Trinity::FriendlyCCedInRange u_check(me, range);
    Trinity::CreatureListSearcher<Trinity::FriendlyCCedInRange> searcher(me, list, u_check);
    Cell::VisitAllObjects(me, searcher, range);

    return list;
}

std::list<Creature*> ScriptedAI::DoFindFriendlyMissingBuff(float range, uint32 uiSpellid)
{
    std::list<Creature*> list;
    Trinity::FriendlyMissingBuffInRange u_check(me, range, uiSpellid);
    Trinity::CreatureListSearcher<Trinity::FriendlyMissingBuffInRange> searcher(me, list, u_check);
    Cell::VisitAllObjects(me, searcher, range);

    return list;
}

Player* ScriptedAI::GetPlayerAtMinimumRange(float minimumRange)
{
    Player* player = nullptr;

    Trinity::PlayerAtMinimumRangeAway check(me, minimumRange);
    Trinity::PlayerSearcher<Trinity::PlayerAtMinimumRangeAway> searcher(me, player, check);
    Cell::VisitWorldObjects(me, searcher, minimumRange);

    return player;
}

void ScriptedAI::SetEquipmentSlots(bool loadDefault, int32 mainHand /*= EQUIP_NO_CHANGE*/, int32 offHand /*= EQUIP_NO_CHANGE*/, int32 ranged /*= EQUIP_NO_CHANGE*/)
{
    if (loadDefault)
    {
        me->LoadEquipment(me->GetOriginalEquipmentId(), true);
        return;
    }

    if (mainHand >= 0)
        me->SetVirtualItem(0, uint32(mainHand));

    if (offHand >= 0)
        me->SetVirtualItem(1, uint32(offHand));

    if (ranged >= 0)
        me->SetVirtualItem(2, uint32(ranged));
}

void ScriptedAI::SetCombatMovement(bool allowMovement)
{
    _isCombatMovementAllowed = allowMovement;
}

enum NPCs
{
    NPC_BROODLORD   = 12017,
    NPC_VOID_REAVER = 19516,
    NPC_JAN_ALAI    = 23578,
    NPC_SARTHARION  = 28860
};

void ScriptedAI::GetInViewBotPlayers(std::list<Player*>& outPlayers, float range)
{
    Map::PlayerList const& players = me->GetMap()->GetPlayers();
    for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
    {
        Player* player = i->GetSource();
        if (!player || !player->IsAlive() || !player->IsPlayerBot())
            continue;
        if (!me->InSamePhase(player->GetPhaseShift()))
            continue;
        if (me->GetDistance(player->GetPosition()) > range)
            continue;
        if (!me->IsWithinLOSInMap(player))
            continue;
        outPlayers.push_back(player);
    }
}

void ScriptedAI::SearchTargetPlayerAllGroup(std::list<Player*>& players, float range)
{
    if (range < 3.0f)
        return;
    ObjectGuid targetGUID = me->GetTarget();
    Player* targetPlayer = NULL;
    if (targetGUID == ObjectGuid::Empty)
    {
        std::list<Player*> playersNearby;
        me->GetPlayerListInGrid(playersNearby, range);
        if (playersNearby.empty())
            return;
        for (Player* p : playersNearby)
        {
            if (p->IsAlive() && p->GetMap() == me->GetMap())
            {
                targetPlayer = p;
                targetGUID = p->GetGUID();
                break;
            }
        }
    }
    if (!targetPlayer)
        targetPlayer = ObjectAccessor::FindPlayer(targetGUID);
    if (!targetPlayer || targetPlayer->GetMap() != me->GetMap())
        return;
    players.clear();
    players.push_back(targetPlayer);

    Group* pGroup = targetPlayer->GetGroup();
    if (!pGroup || pGroup->isBFGroup())
        return;
    Group::MemberSlotList const& memList = pGroup->GetMemberSlots();
    for (Group::MemberSlot const& slot : memList)
    {
        Player* player = ObjectAccessor::FindPlayer(slot.guid);
        if (!player || !player->IsAlive() || targetPlayer->GetMap() != player->GetMap() ||
            !player->IsInWorld() || player == targetPlayer || !player->IsPlayerBot())
            continue;
        if (me->GetDistance(player->GetPosition()) > range)
            continue;
        players.push_back(player);
    }
}

void ScriptedAI::PickBotPullMeToPosition(Position pullPos, ObjectGuid fliterTarget)
{
    std::list<BotGroupAI*> tankAIs;// , healAIs;
    std::list<Player*> targets;
    SearchTargetPlayerAllGroup(targets, 120);
    BotGroupAI* pNearTankAI = NULL;
    float nearTankAIDist = 999999;
    BotGroupAI* pNearHealAI[3] = { NULL };
    float nearHealAIDist[3] = { 999999 };
    for (Player* player : targets)
    {
        if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
        {
            if (pGroupAI->IsHealerBotAI())
            {
                float thisDist = me->GetDistance(player->GetPosition());
                for (int i = 0; i < 3; i++)
                {
                    if (thisDist < nearHealAIDist[i])
                    {
                        pNearHealAI[i] = pGroupAI;
                        nearHealAIDist[i] = thisDist;
                        break;
                    }
                }
            }
            else if (pGroupAI->IsTankBotAI())
            {
                if (fliterTarget != ObjectGuid::Empty)
                {
                    Unit* tankTarget = player->GetSelectedUnit();
                    if (tankTarget != NULL && tankTarget->GetGUID() == fliterTarget)
                    {
                        if (pNearTankAI == NULL)
                        {
                            if (urand(0, 99) > 50)
                                continue;
                        }
                        else
                            continue;
                    }
                }
                tankAIs.push_back(pGroupAI);
                float thisDist = me->GetDistance(player->GetPosition());
                if (thisDist < nearTankAIDist)
                {
                    pNearTankAI = pGroupAI;
                    nearTankAIDist = thisDist;
                }
            }
        }
    }
    if (pNearTankAI == NULL)
        return;
    for (BotGroupAI* pGroupAI : tankAIs)
    {
        if (pGroupAI == pNearTankAI)
        {
            pGroupAI->ClearTankTarget();
            pGroupAI->AddTankTarget(me);
            pGroupAI->SetTankPosition(pullPos);
            pGroupAI->GetAIPayer()->SetSelection(me->GetGUID());
            for (int i = 0; i < 3; i++)
            {
                if (pNearHealAI[i])
                    pNearHealAI[i]->GetAIPayer()->SetSelection(me->GetGUID());
            }
        }
        else
        {
            pGroupAI->ClearTankTarget();
            pGroupAI->GetAIPayer()->SetSelection(ObjectGuid::Empty);
        }
    }
}

bool ScriptedAI::ExistPlayerBotByRange(float range)
{
    std::list<Player*> targets;
    SearchTargetPlayerAllGroup(targets, range);
    return targets.size() > 0;
}

void ScriptedAI::BotBlockCastingMe()
{
    std::list<Player*> targets;
    SearchTargetPlayerAllGroup(targets, 120);
    for (Player* player : targets)
    {
        if (player->HasUnitState(UNIT_STATE_CASTING))
            continue;
        if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
        {
            if (pGroupAI->TryBlockCastingByTarget(me))
                return;
        }
    }
}

void ScriptedAI::ClearBotMeTarget(bool all)
{
    std::list<Player*> targets;
    SearchTargetPlayerAllGroup(targets, 120);
    for (Player* player : targets)
    {
        if (Pet* pPet = player->GetPet())
        {
            if (pPet->GetVictim() == me)
            {
                if (WorldSession* pSession = player->GetSession())
                {
                    pSession->HandlePetActionHelper(pPet, pPet->GetGUID(), 1, 7, ObjectGuid::Empty, pPet->GetPosition());
                }
            }
        }
        if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
        {
            if (all || !pGroupAI->IsTankBotAI())
            {
                player->SetSelection(ObjectGuid::Empty);
                pGroupAI->ToggleFliterCreature(me, true);
            }
        }
    }
}

void ScriptedAI::BotAllMovetoFarByDistance(Unit* pUnit, float range, float dist, float offset)
{
    float onceAngle = float(M_PI) * 2.0f / 8.0f;
    std::list<Position> allPosition;
    for (float angle = 0.0f; angle < (float(M_PI) * 2.0f); angle += onceAngle)
    {
        Position pos;
        pUnit->GetFirstCollisionPosition(dist, angle);
        float dist = pUnit->GetDistance(pos);
        if (!pUnit->IsWithinLOS(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ()))
            continue;
        allPosition.push_back(pos);
    }
    if (allPosition.empty())
        return;
    Position targetPos;
    float maxDist = 0.0f;
    for (Position pos : allPosition)
    {
        float distance = pUnit->GetDistance(pos);
        if (distance > maxDist)
        {
            maxDist = distance;
            targetPos = pos;
        }
    }
    if (maxDist < dist * 0.1f)
        return;
    targetPos.m_positionZ = pUnit->GetMap()->GetHeight(pUnit->GetPhaseShift(), targetPos.GetPositionX(), targetPos.GetPositionY(), targetPos.GetPositionZ());
    std::list<Player*> targets;
    SearchTargetPlayerAllGroup(targets, range);
    for (Player* player : targets)
    {
        if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
        {
            Position offsetPos = Position(targetPos.GetPositionX() + frand(-offset, offset),
                targetPos.GetPositionY() + frand(-offset, offset), targetPos.GetPositionZ());
            pGroupAI->ClearCruxMovement();
            pGroupAI->SetCruxMovement(targetPos);
        }
    }
}

void ScriptedAI::BotCruxFlee(uint32 durTime, ObjectGuid fliter)
{
    std::list<Player*> targets;
    SearchTargetPlayerAllGroup(targets, 120);
    for (Player* player : targets)
    {
        if (fliter != ObjectGuid::Empty)
        {
            if (player->GetGUID() == fliter)
                continue;
        }
        if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
        {
            pGroupAI->AddCruxFlee(durTime, me);
        }
    }
}

void ScriptedAI::BotRndCruxMovement(float dist)
{
    if (dist < 1.0f)
        return;
    std::list<Player*> playersNearby;
    me->GetPlayerListInGrid(playersNearby, 120);
    if (playersNearby.empty())
        return;
    for (Player* player : playersNearby)
    {
        if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
        {
            pGroupAI->RndCruxMovement(dist);
        }
    }
}

void ScriptedAI::BotCruxFleeByRange(float range)
{
    if (range < 3.0f)
        return;
    std::list<Player*> targets;
    SearchTargetPlayerAllGroup(targets, range);
    for (Player* player : targets)
    {
        if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
        {
            Position targetPos;
            if (!BotUtility::FindFirstCollisionPosition(player, range, me, targetPos))
                continue;
            //if (pGroupAI->GetAIPayer()->HasUnitState(UNIT_STATE_CASTING))
            //	pGroupAI->GetAIPayer()->CastStop();
            pGroupAI->SetCruxMovement(targetPos);
            //player->SetSelection(ObjectGuid::Empty);
        }
    }
}

void ScriptedAI::BotCruxFleeByRange(float range, Unit* pCenter)
{
    if (range < 3.0f || !pCenter)
        return;
    std::list<Player*> targets;
    SearchTargetPlayerAllGroup(targets, range);
    for (Player* player : targets)
    {
        if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
        {
            Position targetPos;
            if (!BotUtility::FindFirstCollisionPosition(player, range, pCenter, targetPos))
                continue;
            pGroupAI->SetCruxMovement(targetPos);
            //player->SetSelection(ObjectGuid::Empty);
        }
    }
}

void ScriptedAI::BotCruxFleeByArea(float range, float fleeDist, Unit* pCenter)
{
    if (range < 3.0f || fleeDist < 3.0f || !pCenter)
        return;
    Position centerPos = pCenter->GetPosition();
    std::list<Player*> players;
    SearchTargetPlayerAllGroup(players, 80);
    for (Player* player : players)
    {
        if (player->GetDistance(centerPos) > range)
            continue;
        if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
        {
            Position targetPos;
            if (!BotUtility::FindFirstCollisionPosition(player, fleeDist, pCenter, targetPos))
                continue;
            pGroupAI->SetCruxMovement(targetPos);
            //player->SetSelection(ObjectGuid::Empty);
        }
    }
}

void ScriptedAI::BotAllTargetMe(bool all)
{
    std::list<Player*> targets;
    SearchTargetPlayerAllGroup(targets, 120);
    for (Player* player : targets)
    {
        if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
        {
            if (all)
                player->SetSelection(me->GetGUID());
            else if (!pGroupAI->IsTankBotAI() && !pGroupAI->IsHealerBotAI())
                player->SetSelection(me->GetGUID());
        }
    }
}

void ScriptedAI::BotPhysicsDPSTargetMe(Unit* pUnit)
{
    std::list<Player*> targets;
    SearchTargetPlayerAllGroup(targets, 120);
    for (Player* player : targets)
    {
        if (!player->IsPlayerBot())
            continue;
        if (player->getClass() == Classes::CLASS_ROGUE || player->getClass() == Classes::CLASS_HUNTER)
            player->SetSelection(me->GetGUID());
        else if (player->getClass() == Classes::CLASS_WARRIOR)
        {
            if (player->FindTalentType() != 2)
                player->SetSelection(pUnit->GetGUID());
        }
        else
        {
            if (player->GetSelectedUnit() == pUnit)
            {
                if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
                {
                    if (!pGroupAI->IsHealerBotAI() && !pGroupAI->IsTankBotAI())
                        player->SetSelection(ObjectGuid::Empty);
                }
            }
        }
    }
}

void ScriptedAI::BotMagicDPSTargetMe(Unit* pUnit)
{
    std::list<Player*> targets;
    SearchTargetPlayerAllGroup(targets, 120);
    for (Player* player : targets)
    {
        if (!player->IsPlayerBot())
            continue;
        if (player->getClass() == Classes::CLASS_ROGUE || player->getClass() == Classes::CLASS_HUNTER ||
            player->getClass() == Classes::CLASS_WARRIOR)
        {
            if (player->GetSelectedUnit() == pUnit)
            {
                if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
                {
                    if (!pGroupAI->IsHealerBotAI() && !pGroupAI->IsTankBotAI())
                        player->SetSelection(ObjectGuid::Empty);
                }
            }
        }
        else
        {
            if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
            {
                if (!pGroupAI->IsHealerBotAI() && !pGroupAI->IsTankBotAI())
                    player->SetSelection(pUnit->GetGUID());
            }
        }
    }
}

void ScriptedAI::BotAverageCreatureTarget(std::vector<Creature*>& targets, float searchRange)
{
    if (targets.empty() || searchRange < 3.0f)
        return;
    std::queue<Player*> players;
    std::list<Player*> searchUnits;
    SearchTargetPlayerAllGroup(searchUnits, searchRange);
    for (Player* player : searchUnits)
    {
        players.push(player);
    }
    while (!players.empty())
    {
        for (Creature* pCreature : targets)
        {
            Player* player = players.front();
            players.pop();
            if (pCreature)
                player->SetSelection(pCreature->GetGUID());
            if (players.empty())
                break;
        }
    }
}

void ScriptedAI::BotAllotCreatureTarget(std::vector<Creature*>& targets, float searchRange, uint32 onceCount)
{
    if (onceCount < 1 || targets.empty() || searchRange < 3.0f)
        return;
    std::queue<Player*> players;
    std::list<Player*> searchUnits;
    SearchTargetPlayerAllGroup(searchUnits, searchRange);
    for (Player* player : searchUnits)
    {
        players.push(player);
    }
    while (!players.empty())
    {
        for (Creature* pCreature : targets)
        {
            int32 allot = int32(onceCount);
            while (!players.empty() && allot > 0)
            {
                --allot;
                Player* player = players.front();
                players.pop();
                if (pCreature)
                    player->SetSelection(pCreature->GetGUID());
            }
            if (players.empty())
                break;
        }
    }
}

void ScriptedAI::BotAllToSelectionTarget(Unit* pUnit, float searchRange, bool all)
{
    if (!pUnit)
        return;
    ObjectGuid guid = pUnit->GetGUID();
    std::list<Player*> searchUnits;
    SearchTargetPlayerAllGroup(searchUnits, searchRange);
    for (Player* player : searchUnits)
    {
        if (player->GetTarget() == guid)
            continue;
        if (all)
            player->SetSelection(pUnit->GetGUID());
        else if (BotGroupAI* pAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
        {
            if (!pAI->IsTankBotAI())
                player->SetSelection(pUnit->GetGUID());
        }
    }
}

void ScriptedAI::BotAllFullDispel(bool enables)
{
    std::list<Player*> targets;
    SearchTargetPlayerAllGroup(targets, 120);
    for (Player* player : targets)
    {
        if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
        {
            if (enables)
                pGroupAI->StartFullDispel();
            else
                pGroupAI->ClearFullDispel();
        }
    }
}

void ScriptedAI::BotAllFullDispelByDecPoison(bool enables)
{
    std::list<Player*> targets;
    SearchTargetPlayerAllGroup(targets, 120);
    for (Player* player : targets)
    {
        if (player->getClass() != Classes::CLASS_DRUID && player->getClass() != Classes::CLASS_SHAMAN)
            continue;
        if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
        {
            if (enables)
                pGroupAI->StartFullDispel();
            else
                pGroupAI->ClearFullDispel();
        }
    }
}

void ScriptedAI::BotFleeLineByAngle(Unit* center, float angle, bool force)
{
    angle = Position::NormalizeOrientation(angle);
    std::list<Player*> playersNearby;
    center->GetPlayerListInGrid(playersNearby, center->GetObjectSize() + 80.0f);
    for (Player* player : playersNearby)
    {
        if (!player->IsPlayerBot() || !player->IsInWorld() || player->GetMap() != center->GetMap() || !player->IsAlive())
            continue;
        float fleeRange = center->GetDistance(player->GetPosition());
        if (fleeRange <= 0)
            fleeRange = center->GetObjectSize() + 1.0f;
        float pangle = center->GetAngle(player->GetPosition()) - angle;
        if (pangle >= 0 && pangle <= float(M_PI_4))
        {
            if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
            {
                float fleeAngle = Position::NormalizeOrientation(angle + float(M_PI_4));
                Position fleePos = Position(center->GetPositionX() + fleeRange * std::cos(fleeAngle),
                    center->GetPositionY() + fleeRange * std::sin(fleeAngle), player->GetPositionZ(), player->GetOrientation());
                fleePos.m_positionZ = player->GetMap()->GetHeight(player->GetPhaseShift(), fleePos.GetPositionX(), fleePos.GetPositionY(), fleePos.GetPositionZ());
                //if (pGroupAI->GetAIPayer()->HasUnitState(UNIT_STATE_CASTING))
                //	pGroupAI->GetAIPayer()->CastStop();
                if (force)
                    pGroupAI->ClearCruxMovement();
                pGroupAI->SetCruxMovement(fleePos);
            }
        }
        else if (pangle < 0 && pangle >= float(-M_PI_4))
        {
            if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
            {
                float fleeAngle = Position::NormalizeOrientation(angle - float(M_PI_4));
                Position fleePos = Position(center->GetPositionX() + fleeRange * std::cos(fleeAngle),
                    center->GetPositionY() + fleeRange * std::sin(fleeAngle), player->GetPositionZ(), player->GetOrientation());
                fleePos.m_positionZ = player->GetMap()->GetHeight(player->GetPhaseShift(), fleePos.GetPositionX(), fleePos.GetPositionY(), fleePos.GetPositionZ());
                //if (pGroupAI->GetAIPayer()->HasUnitState(UNIT_STATE_CASTING))
                //	pGroupAI->GetAIPayer()->CastStop();
                if (force)
                    pGroupAI->ClearCruxMovement();
                pGroupAI->SetCruxMovement(fleePos);
            }
        }
    }
}

void ScriptedAI::BotSwitchPullTarget(Unit* pTarget)
{
    if (!pTarget || !pTarget->ToCreature())
        return;
    Player* pTargetPlayer = ObjectAccessor::FindPlayer(pTarget->GetTarget());
    if (pTargetPlayer && pTargetPlayer->IsAlive())
    {
        pTargetPlayer->SetSelection(ObjectGuid::Empty);
        if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(pTargetPlayer->GetAI()))
            pGroupAI->ClearTankTarget();
    }
    std::list<Player*> playersNearby;
    pTarget->GetPlayerListInGrid(playersNearby, pTarget->GetObjectSize() + 80.0f);
    for (Player* player : playersNearby)
    {
        if (player && player == pTargetPlayer)
            continue;
        if (!player->IsPlayerBot() || !player->IsInWorld() || player->GetMap() != pTarget->GetMap() || !player->IsAlive())
            continue;
        if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
        {
            if (pGroupAI->IsTankBotAI())
            {
                if (pGroupAI->ProcessPullSpell(pTarget))
                    return;
            }
        }
    }
}

void ScriptedAI::BotVehicleChaseTarget(Unit* pTarget, float distance)
{
    if (!pTarget || !pTarget->ToCreature())
        return;
    std::list<Player*> playersNearby;
    me->GetPlayerListInGrid(playersNearby, distance * 2);
    if (playersNearby.empty())
        return;
    for (Player* bot : playersNearby)
    {
        if (!bot->IsPlayerBot() || !bot->IsAlive() || bot->GetMap() != pTarget->GetMap())
            continue;
        if (bot->GetTarget() != pTarget->GetGUID())
            bot->SetSelection(pTarget->GetGUID());
        if (BotGroupAI* pBotAI = dynamic_cast<BotGroupAI*>(bot->GetAI()))
        {
            pBotAI->SetVehicle3DNextMoveGap(8.0f);
            pBotAI->SetVehicle3DMoveTarget(pTarget, distance);
        }
        Unit* vehBase = bot->GetCharm();
        if (!vehBase || !vehBase->IsAlive() || vehBase->GetMap() != pTarget->GetMap())
            continue;
        if (vehBase->GetTarget() != pTarget->GetGUID())
            vehBase->SetTarget(pTarget->GetGUID());
        if (vehBase->HasSpell(57092) && !vehBase->HasAura(57092))
            vehBase->CastSpell(vehBase, 57092);
        float power = (float)vehBase->GetPower(POWER_ENERGY) / (float)vehBase->GetMaxPower(POWER_ENERGY);
        if (power >= 0.4f)
        {
            uint8 combo = bot->GetComboPoints();
            if (combo > 4)
            {
                if (vehBase->GetHealthPct() < 75 && urand(0, 99) > 60)
                {
                    if (vehBase->HasSpell(57108) && !vehBase->HasAura(57108) && urand(0, 99) > 60)
                        vehBase->CastSpell(vehBase, 57108);
                    else if (vehBase->HasSpell(57143))
                        vehBase->CastSpell(vehBase, 57143);
                }
                else if (vehBase->HasSpell(56092))
                    vehBase->CastSpell(pTarget, 56092);
            }
            else
            {
                if (vehBase->GetHealthPct() < 75 && vehBase->HasSpell(57090) && urand(0, 99) > 60)
                    vehBase->CastSpell(vehBase, 57090);
                else if (vehBase->HasSpell(56091))
                    vehBase->CastSpell(pTarget, 56091);
            }
        }
    }
}

void ScriptedAI::BotUseGOTarget(GameObject* pGO)
{
    if (!pGO)
        return;
    std::list<Player*> playersNearby;
    me->GetPlayerListInGrid(playersNearby, 100);
    if (playersNearby.empty())
        return;
    ObjectGuid goGUID = pGO->GetGUID();
    std::map<float, Player*> distPlayers;
    for (Player* bot : playersNearby)
    {
        if (!bot->IsPlayerBot() || !bot->IsAlive() || bot->GetMap() != pGO->GetMap())
            continue;
        if (bot->HasUnitState(UNIT_STATE_CASTING))
            continue;
        distPlayers[bot->GetDistance(pGO->GetPosition())] = bot;
    }
    for (std::map<float, Player*>::iterator itDist = distPlayers.begin();
        itDist != distPlayers.end();
        itDist++)
    {
        Player* bot = itDist->second;
        if (BotGroupAI* pAI = dynamic_cast<BotGroupAI*>(bot->GetAI()))
        {
            if (pAI->SetMovetoUseGOTarget(goGUID))
                return;
        }
    }
}

bool NeedBotAttackCreature::UpdateProcess(std::list<ObjectGuid>& freeBots)
{
    Creature* pCreature = atMap->GetCreature(needCreature);
    if (!pCreature || !pCreature->IsAlive())
    {
        allUsedBots.clear();
        return false;
    }
    if (pCreature && !pCreature->IsVisible())
    {
        allUsedBots.clear();
        return true;
    }

    while (int32(allUsedBots.size()) < needCount)
    {
        if (freeBots.empty())
            break;
        allUsedBots.push_back(*freeBots.begin());
        freeBots.erase(freeBots.begin());
    }
    std::list<std::list<ObjectGuid>::iterator > needClearBot;
    for (std::list<ObjectGuid>::iterator itBot = allUsedBots.begin(); itBot != allUsedBots.end(); itBot++)
    {
        ObjectGuid& guid = *itBot;
        Player* player = ObjectAccessor::FindPlayer(guid);
        if (!player || !player->IsAlive() || player->GetMap() != atMap)
            needClearBot.push_back(itBot);
        else if (player->GetDistance(pCreature->GetPosition()) < 120)
            player->SetSelection(needCreature);
    }
    for (std::list<ObjectGuid>::iterator itClear : needClearBot)
    {
        allUsedBots.erase(itClear);
    }
    return true;
}

void BotAttackCreature::UpdateNeedAttackCreatures(uint32 diff, ScriptedAI* affiliateAI, bool attackMain)
{
    currentTick -= int32(diff);
    if (currentTick >= 0)
        return;
    currentTick = updateGap;
    if (!mainCreature)
        return;

    if (!affiliateAI)
        return;
    if (allNeedCreatures.empty())
        return;
    std::list<Player*> allBots;
    affiliateAI->SearchTargetPlayerAllGroup(allBots, 120);
    std::list<ObjectGuid> allBotGUIDs;
    for (Player* player : allBots)
    {
        ObjectGuid guid = player->GetGUID();
        bool canPush = true;
        for (NeedBotAttackCreature* pNeed : allNeedCreatures)
        {
            if (pNeed->IsThisUsedBot(guid))
            {
                canPush = false;
                break;
            }
        }
        if (canPush)
            allBotGUIDs.push_back(guid);
    }
    std::list<std::list<NeedBotAttackCreature*>::iterator > needClears;
    for (std::list<NeedBotAttackCreature*>::iterator itNeed = allNeedCreatures.begin(); itNeed != allNeedCreatures.end(); itNeed++)
    {
        NeedBotAttackCreature* pNeed = *itNeed;
        bool ing = pNeed->UpdateProcess(allBotGUIDs);
        if (!ing)
            needClears.push_back(itNeed);
    }
    for (std::list<NeedBotAttackCreature*>::iterator itClear : needClears)
    {
        NeedBotAttackCreature* pNeed = *itClear;
        delete pNeed;
        allNeedCreatures.erase(itClear);
    }
    if (attackMain && mainCreature && mainCreature->IsAlive())
    {
        for (ObjectGuid guid : allBotGUIDs)
        {
            Player* player = ObjectAccessor::FindPlayer(guid);
            if (player && player->IsAlive() && player->GetMap() == mainCreature->GetMap())
                player->SetSelection(mainCreature->GetGUID());
        }
    }
    else if (attackMain)
    {
        mainCreature = NULL;
    }
}

void BotAttackCreature::AddNewCreatureNeedAttack(Creature* pCreature, int32 needBotCount)
{
    if (!pCreature || !pCreature->IsAlive() || needBotCount < 0 || pCreature == mainCreature)
        return;
    if (pCreature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
        return;
    Map* atMap = pCreature->GetMap();
    if (!atMap)
        return;
    ObjectGuid guid = pCreature->GetGUID();
    for (NeedBotAttackCreature* pNeed : allNeedCreatures)
    {
        if (pNeed->IsThisCreature(guid))
            return;
    }
    NeedBotAttackCreature* pNeedCreature = new NeedBotAttackCreature(atMap, needBotCount, guid);
    allNeedCreatures.push_back(pNeedCreature);
}

void Scripted_NoMovementAI::AttackStart(Unit* target)
{
    if (!target)
        return;

    if (me->Attack(target, true))
        DoStartNoMovement(target);
}

// BossAI - for instanced bosses
BossAI::BossAI(Creature* creature, uint32 bossId) : ScriptedAI(creature),
    _bossId(bossId)
{
    if (instance)
        SetBoundary(instance->GetBossBoundary(bossId));
 	_dungeonEncounterId = sObjectMgr->GetDungeonEncounterID(creature->GetEntry());
}

void BossAI::_Reset()
{
    if (!me->IsAlive())
        return;

    me->SetCombatPulseDelay(0);
    me->ResetLootMode();
    events.Reset();
    summons.DespawnAll();
    me->RemoveAllAreaTriggers();
    me->GetScheduler().CancelAll();
    if (instance)
        instance->SetBossState(_bossId, NOT_STARTED);
}

void BossAI::_JustDied()
{
    events.Reset();
    summons.DespawnAll();
    me->GetScheduler().CancelAll();
    if (instance)
    {
        instance->SetBossState(_bossId, DONE);
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
 		if(_dungeonEncounterId>0)
            instance->SendBossKillCredit(_dungeonEncounterId);
    }
    Talk(BOSS_TALK_JUST_DIED);
    GetTalkData(EVENT_ON_JUSTDIED);/////here
}

void BossAI::_JustReachedHome()
{
    me->setActive(false);

    if (instance)
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
}

void BossAI::_KilledUnit(Unit* victim)
{
    if (victim->IsPlayer() && urand(0, 1))
        Talk(BOSS_TALK_KILL_PLAYER);
    if (victim->IsPlayer() && urand(0, 1))
        GetTalkData(EVENT_ON_KILLEDUNIT);
}

void BossAI::_DamageTaken(Unit* /*attacker*/, uint32& damage)
{
    while (uint32 eventId = damageEvents.OnDamageTaken(damage))
        ExecuteEvent(eventId);
}

void BossAI::_EnterCombat(bool showFrameEngage /*= true*/)
{
    if (instance)
    {
        // bosses do not respawn, check only on enter combat
        if (!instance->CheckRequiredBosses(_bossId))
        {
            EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);
            return;
        }
        instance->SetBossState(_bossId, IN_PROGRESS);

        if (showFrameEngage)
            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me, 1);
    }

    me->SetCombatPulseDelay(5);
    me->setActive(true);
    DoZoneInCombat();
    ScheduleTasks();
    Talk(BOSS_TALK_ENTER_COMBAT);
    GetTalkData(EVENT_ON_ENTERCOMBAT);
}

void BossAI::TeleportCheaters()
{
    float x, y, z;
    me->GetPosition(x, y, z);

    ThreatContainer::StorageType threatList = me->getThreatManager().getThreatList();
    for (ThreatContainer::StorageType::const_iterator itr = threatList.begin(); itr != threatList.end(); ++itr)
        if (Unit* target = (*itr)->getTarget())
            if (target->GetTypeId() == TYPEID_PLAYER && !CheckBoundary(target))
                target->NearTeleportTo(x, y, z, 0);
}

void BossAI::JustSummoned(Creature* summon)
{
    summons.Summon(summon);
    if (me->IsInCombat())
        DoZoneInCombat(summon);
    GetTalkData(EVENT_ON_JUSTSUMMON);
}

void BossAI::SummonedCreatureDespawn(Creature* summon)
{
    summons.Despawn(summon);
}

void BossAI::UpdateAI(uint32 diff)
{
    if (!UpdateVictim())
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = events.ExecuteEvent())
    {
        ExecuteEvent(eventId);
        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;
    }

    DoMeleeAttackIfReady();
}

bool BossAI::CanAIAttack(Unit const* target) const
{
    return CheckBoundary(target);
}

void BossAI::_DespawnAtEvade(uint32 delayToRespawn, Creature* who)
{
    if (delayToRespawn < 2)
    {
        TC_LOG_ERROR("scripts", "_DespawnAtEvade called with delay of %u seconds, defaulting to 2.", delayToRespawn);
        delayToRespawn = 2;
    }

    if (!who)
        who = me;

    if (TempSummon* whoSummon = who->ToTempSummon())
    {
        TC_LOG_WARN("scripts", "_DespawnAtEvade called on a temporary summon.");
        whoSummon->UnSummon();
        return;
    }

    who->DespawnOrUnsummon(0, Seconds(delayToRespawn));

    if (instance && who == me)
        instance->SetBossState(_bossId, FAIL);
}

// StaticBossAI - for bosses that shouldn't move, and cast a spell when player are too far away

void StaticBossAI::_Reset()
{
    BossAI::_Reset();
    SetCombatMovement(false);
    _InitStaticSpellCast();
}

void StaticBossAI::_InitStaticSpellCast()
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(_staticSpell);
    if (!spellInfo)
        return;

    bool isAura = spellInfo->HasAura(me->GetMap()->GetDifficultyID());
    bool isArea = spellInfo->IsAffectingArea(me->GetMap()->GetDifficultyID());

    me->GetScheduler().Schedule(2s, [this, isAura, isArea](TaskContext context)
    {
        // Check if any player in range
        for (auto threat : me->getThreatManager().getThreatList())
        {
            if (me->IsWithinCombatRange(threat->getTarget(), me->GetCombatReach()))
            {
                me->RemoveAurasDueToSpell(_staticSpell);
                context.Repeat();
                return;
            }
        }

        // Else, cast spell depending of its effects
        if (isAura)
        {
            if (!me->HasAura(_staticSpell))
                me->CastSpell(me, _staticSpell, false);
        }
        else if (isArea)
        {
            me->CastSpell(nullptr, _staticSpell, false);
        }
        else
        {
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                me->CastSpell(target, _staticSpell, false);
        }

        context.Repeat();
    });
}

// WorldBossAI - for non-instanced bosses

WorldBossAI::WorldBossAI(Creature* creature) :
    ScriptedAI(creature),
    summons(creature) { }

void WorldBossAI::_Reset()
{
    if (!me->IsAlive())
        return;

    events.Reset();
    summons.DespawnAll();
}

void WorldBossAI::_JustDied()
{
    events.Reset();
    summons.DespawnAll();
}

void WorldBossAI::_EnterCombat()
{
    ScheduleTasks();
    Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);
    if (target)
        AttackStart(target);
}

void WorldBossAI::JustSummoned(Creature* summon)
{
    summons.Summon(summon);
    Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);
    if (target)
        summon->AI()->AttackStart(target);
}

void WorldBossAI::SummonedCreatureDespawn(Creature* summon)
{
    summons.Despawn(summon);
}

void WorldBossAI::UpdateAI(uint32 diff)
{
    if (!UpdateVictim())
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = events.ExecuteEvent())
    {
        ExecuteEvent(eventId);
        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;
    }

    DoMeleeAttackIfReady();
}

void GetPositionWithDistInOrientation(Position* pUnit, float dist, float orientation, float& x, float& y)
{
    x = pUnit->GetPositionX() + (dist * cos(orientation));
    y = pUnit->GetPositionY() + (dist * sin(orientation));
}

void GetPositionWithDistInOrientation(Position* fromPos, float dist, float orientation, Position& movePosition)
{
    float x = 0.0f;
    float y = 0.0f;

    GetPositionWithDistInOrientation(fromPos, dist, orientation, x, y);

    movePosition.m_positionX = x;
    movePosition.m_positionY = y;
    movePosition.m_positionZ = fromPos->GetPositionZ();
}

void GetRandPosFromCenterInDist(float centerX, float centerY, float dist, float& x, float& y)
{
    float randOrientation = frand(0.0f, 2.0f * (float)M_PI);

    x = centerX + (dist * cos(randOrientation));
    y = centerY + (dist * sin(randOrientation));
}

void GetRandPosFromCenterInDist(Position* centerPos, float dist, Position& movePosition)
{
    GetPositionWithDistInOrientation(centerPos, dist, frand(0, 2 * float(M_PI)), movePosition);
}

void GetPositionWithDistInFront(Position* centerPos, float dist, Position& movePosition)
{
    GetPositionWithDistInOrientation(centerPos, dist, centerPos->GetOrientation(), movePosition);
}

void ScriptedAI::LoadEventData(EventData const * data)
{
    if (data)
    {
        eventslist = data;
        haseventdata = true;
    }        
}

void ScriptedAI::GetEventData(uint16 group)
{
    if (!eventslist)
        return;
    if (!haseventdata)
        return;

    EventData const* list = eventslist;
    while (list->eventId)
    {
        if (list->group == group)
            events.ScheduleEvent(list->eventId, list->time, list->group, list->phase);
        ++list;
    }
}

void ScriptedAI::LoadTalkData(TalkData const * data)
{
    if (data)
    {
        talkslist = data;
        hastalkdata = true;
    }      
}

void ScriptedAI::GetTalkData(uint32 eventId)
{
    if (!talkslist)
        return;
    if (!hastalkdata)
        return;

    TalkData const* list = talkslist;
    while (list->eventId)
    {
        if (list->eventId == eventId)
        {
            switch (list->eventType)
            {
            case EVENT_TYPE_TALK:
                me->AI()->Talk(list->eventData);
                break;
            case EVENT_TYPE_CONVERSATION:
                if (list->eventData > 0)
                    if (instance)
                        instance->DoConversation(list->eventData);
                break;
            case EVENT_TYPE_ACHIEVEMENT:
                if (list->eventData > 0)
                    if (instance)
                        instance->DoCompleteAchievement(list->eventData);
                break;
            case EVENT_TYPE_SPELL:
                if (list->eventData > 0)
                    if (instance)
                        instance->DoCastSpellOnPlayers(list->eventData);
                break;
            case EVENT_TYPE_YELL:
                if (list->eventData > 0)
                    me->Yell(list->eventData);
                break;
            case EVENT_TYPE_SAY:
                if (list->eventData > 0)
                    me->Say(list->eventData);
                break;
            }
        }
        ++list;
    }
    
}
