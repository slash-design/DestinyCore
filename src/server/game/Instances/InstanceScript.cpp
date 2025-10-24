/*
 * This file is part of the DestinyCore Project. See AUTHORS file for Copyright information
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

#include "InstanceScript.h"
#include "AreaBoundary.h"
#include "AchievementMgr.h"
#include "ChallengeModeMgr.h"
#include "ChallengeModePackets.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "CreatureAIImpl.h"
#include "DatabaseEnv.h"
#include "GameObject.h"
#include "Group.h"
#include "InstancePackets.h"
#include "InstanceScenario.h"
#include "LFGMgr.h"
#include "Log.h"
#include "Map.h"
#include "MiscPackets.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Pet.h"
#include "PhasingHandler.h"
#include "Player.h"
#include "RBAC.h"
#include "ScriptMgr.h"
#include "ScriptReloadMgr.h"
#include "World.h"
#include "WorldSession.h"
#include <sstream>
#include <cstdarg>
#include "SpellMgr.h"
#include "ScenarioMgr.h"
#include "GridNotifiers.h"
#include "CellImpl.h"
#include "ScriptedCreature.h"

inline uint32 secsToTimeBitFields(time_t secs)
{
    tm* lt = localtime(&secs);
    return uint32((lt->tm_year - 100) << 24 | lt->tm_mon << 20 | (lt->tm_mday - 1) << 14 | lt->tm_wday << 11 | lt->tm_hour << 6 | lt->tm_min);
}

BossBoundaryData::~BossBoundaryData()
{
    for (const_iterator it = begin(); it != end(); ++it)
        delete it->Boundary;
}

InstanceScript::InstanceScript(InstanceMap* map) : instance(map), completedEncounters(0),
_entranceId(0), _temporaryEntranceId(0), _combatResurrectionTimer(0), _combatResurrectionCharges(0), _combatResurrectionTimerStarted(false),
_challengeModeStarted(false), _challengeModeLevel(0), _challengeModeStartTime(0), _challengeModeDeathCount(0)
{
    _scriptType = ZONE_SCRIPT_TYPE_INSTANCE;

#ifdef TRINITY_API_USE_DYNAMIC_LINKING
    uint32 scriptId = sObjectMgr->GetInstanceTemplate(map->GetId())->ScriptId;
    auto const scriptname = sObjectMgr->GetScriptName(scriptId);
    ASSERT(!scriptname.empty());
   // Acquire a strong reference from the script module
   // to keep it loaded until this object is destroyed.
    module_reference = sScriptMgr->AcquireModuleReferenceOfScriptName(scriptname);
#endif // #ifndef TRINITY_API_USE_DYNAMIC_LINKING

m_IsScenarioComplete = false;

    m_ChallengeStarted = false;
    m_ConditionCompleted = false;
    m_CreatureKilled = 0;
    m_StartChallengeTime = 0;

    m_ChallengeDoorGuids.clear();

    m_ChallengeOrbGuid = 0;
    m_ChallengeTime = 0;
    //m_MedalType = eChallengeMedals::MedalTypeNone;

    m_InstanceGuid = map->GetId();//  MAKE_NEW_GUID(map->GetId(), 0, HIGHGUID_INSTANCE_SAVE);
    m_BeginningTime = 0;
    m_ScenarioID = 0;
    m_ScenarioStep = 0;
    m_LastResetTime = 0;
    m_ChallengeLevel = 0;
    m_DeathCount = 0;
}

void InstanceScript::SaveToDB()
{
    if (InstanceScenario* scenario = instance->GetInstanceScenario())
        scenario->SaveToDB();

    std::string data = GetSaveData();
    if (data.empty())
        return;

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_INSTANCE_DATA);
    stmt->setUInt32(0, GetCompletedEncounterMask());
    stmt->setString(1, data);
    stmt->setUInt32(2, _entranceId);
    stmt->setUInt32(3, instance->GetInstanceId());
    CharacterDatabase.Execute(stmt);
}

bool InstanceScript::IsEncounterInProgress() const
{
    for (std::vector<BossInfo>::const_iterator itr = bosses.begin(); itr != bosses.end(); ++itr)
        if (itr->state == IN_PROGRESS)
            return true;

    return false;
}

void InstanceScript::OnCreatureCreate(Creature* creature)
{
    AddObject(creature, true);
    AddMinion(creature, true);

    if (IsChallengeModeStarted())
        if (!creature->IsPet())
            CastChallengeCreatureSpell(creature);
}

void InstanceScript::OnCreatureRemove(Creature* creature)
{
    AddObject(creature, false);
    AddMinion(creature, false);
}

void InstanceScript::OnGameObjectCreate(GameObject* go)
{
    AddObject(go, true);
    AddDoor(go, true);

    if (sChallengeModeMgr->IsChest(go->GetEntry()))
        _challengeChest = go->GetGUID();

    if (sChallengeModeMgr->IsDoor(go->GetEntry()))
        AddChallengeModeDoor(go->GetGUID());

    if (go->GetEntry() == ChallengeModeOrb)
        AddChallengeModeOrb(go->GetGUID());
}

void InstanceScript::OnGameObjectRemove(GameObject* go)
{
    AddObject(go, false);
    AddDoor(go, false);
}

ObjectGuid InstanceScript::GetObjectGuid(uint32 type) const
{
    ObjectGuidMap::const_iterator i = _objectGuids.find(type);
    if (i != _objectGuids.end())
        return i->second;
    return ObjectGuid::Empty;
}

ObjectGuid InstanceScript::GetGuidData(uint32 type) const
{
    return GetObjectGuid(type);
}

Creature* InstanceScript::GetCreature(uint32 type)
{
    return instance->GetCreature(GetObjectGuid(type));
}

GameObject* InstanceScript::GetGameObject(uint32 type)
{
    return instance->GetGameObject(GetObjectGuid(type));
}

void InstanceScript::OnPlayerEnter(Player* player)
{
    if (IsChallengeModeStarted())
    {
        WorldPackets::ChallengeMode::ChangePlayerDifficultyResult changePlayerDifficultyResult(11);
        changePlayerDifficultyResult.InstanceDifficultyID = instance->GetId();
        changePlayerDifficultyResult.DifficultyRecID = DIFFICULTY_MYTHIC_KEYSTONE;
        player->SendDirectMessage(changePlayerDifficultyResult.Write());

        SendChallengeModeStart(player);
        SendChallengeModeElapsedTimer(player);
        SendChallengeModeDeathCount(player);

        CastChallengePlayerSpell(player);
    }
}

void InstanceScript::OnPlayerExit(Player* player)
{
    player->RemoveAurasDueToSpell(SPELL_CHALLENGER_BURDEN);
}

void InstanceScript::OnPlayerDeath(Player* /*player*/)
{
    if (IsChallengeModeStarted())
    {
        ++_challengeModeDeathCount;

        SendChallengeModeElapsedTimer();
        SendChallengeModeDeathCount();
    }
}

void InstanceScript::SetHeaders(std::string const& dataHeaders)
{
    for (char header : dataHeaders)
        if (isalpha(header))
            headers.push_back(header);
}

void InstanceScript::LoadBossBoundaries(const BossBoundaryData& data)
{
    for (BossBoundaryEntry const& entry : data)
        if (entry.BossId < bosses.size())
            bosses[entry.BossId].boundary.insert(entry.Boundary);
}

void InstanceScript::LoadMinionData(const MinionData* data)
{
    while (data->entry)
    {
        if (data->bossId < bosses.size())
            minions.insert(std::make_pair(data->entry, MinionInfo(&bosses[data->bossId])));

        ++data;
    }
    TC_LOG_DEBUG("scripts", "InstanceScript::LoadMinionData: " UI64FMTD " minions loaded.", uint64(minions.size()));
}

void InstanceScript::LoadDoorData(const DoorData* data)
{
    while (data->entry)
    {
        if (data->bossId < bosses.size())
            doors.insert(std::make_pair(data->entry, DoorInfo(&bosses[data->bossId], data->type)));

        ++data;
    }
    TC_LOG_DEBUG("scripts", "InstanceScript::LoadDoorData: " UI64FMTD " doors loaded.", uint64(doors.size()));
}

void InstanceScript::LoadObjectData(ObjectData const* creatureData, ObjectData const* gameObjectData)
{
    if (creatureData)
        LoadObjectData(creatureData, _creatureInfo);

    if (gameObjectData)
        LoadObjectData(gameObjectData, _gameObjectInfo);

    TC_LOG_DEBUG("scripts", "InstanceScript::LoadObjectData: " SZFMTD " objects loaded.", _creatureInfo.size() + _gameObjectInfo.size());
}

void InstanceScript::LoadObjectData(ObjectData const* data, ObjectInfoMap& objectInfo)
{
    while (data->entry)
    {
        ASSERT(objectInfo.find(data->entry) == objectInfo.end());
        objectInfo[data->entry] = data->type;
        ++data;
    }
}

void InstanceScript::UpdateMinionState(Creature* minion, EncounterState state)
{
    switch (state)
    {
        case NOT_STARTED:
            if (!minion->IsAlive())
                minion->Respawn();
            else if (minion->IsInCombat())
                minion->AI()->EnterEvadeMode();
            break;
        case IN_PROGRESS:
            if (!minion->IsAlive())
                minion->Respawn();
            else if (!minion->GetVictim())
                minion->AI()->DoZoneInCombat();
            break;
        default:
            break;
    }
}

void InstanceScript::UpdateDoorState(GameObject* door)
{
    DoorInfoMapBounds range = doors.equal_range(door->GetEntry());
    if (range.first == range.second)
        return;

    bool open = true;
    for (; range.first != range.second && open; ++range.first)
    {
        DoorInfo const& info = range.first->second;
        switch (info.type)
        {
            case DOOR_TYPE_ROOM:
                open = (info.bossInfo->state != IN_PROGRESS);
                break;
            case DOOR_TYPE_PASSAGE:
                open = (info.bossInfo->state == DONE);
                break;
            case DOOR_TYPE_SPAWN_HOLE:
                open = (info.bossInfo->state == IN_PROGRESS);
                break;
            default:
                break;
        }
    }

    door->SetGoState(open ? GO_STATE_ACTIVE : GO_STATE_READY);
}

BossInfo* InstanceScript::GetBossInfo(uint32 id)
{
    ASSERT(id < bosses.size());
    return &bosses[id];
}

void InstanceScript::AddObject(Creature* obj, bool add)
{
    ObjectInfoMap::const_iterator j = _creatureInfo.find(obj->GetEntry());
    if (j != _creatureInfo.end())
        AddObject(obj, j->second, add);
    else
        AddObject(obj, obj->GetEntry(), add);
}

void InstanceScript::AddObject(GameObject* obj, bool add)
{
    ObjectInfoMap::const_iterator j = _gameObjectInfo.find(obj->GetEntry());
    if (j != _gameObjectInfo.end())
        AddObject(obj, j->second, add);
    else
        AddObject(obj, obj->GetEntry(), add);
}

void InstanceScript::AddObject(WorldObject* obj, uint32 type, bool add)
{
    if (add)
        _objectGuids[type] = obj->GetGUID();
    else
    {
        ObjectGuidMap::iterator i = _objectGuids.find(type);
        if (i != _objectGuids.end() && i->second == obj->GetGUID())
            _objectGuids.erase(i);
    }
}

void InstanceScript::AddDoor(GameObject* door, bool add)
{
    DoorInfoMapBounds range = doors.equal_range(door->GetEntry());
    if (range.first == range.second)
        return;

    for (; range.first != range.second; ++range.first)
    {
        DoorInfo const& data = range.first->second;

        if (add)
        {
            data.bossInfo->door[data.type].insert(door->GetGUID());
        }
        else
            data.bossInfo->door[data.type].erase(door->GetGUID());
    }

    if (add)
        UpdateDoorState(door);
}

void InstanceScript::AddMinion(Creature* minion, bool add)
{
    MinionInfoMap::iterator itr = minions.find(minion->GetEntry());
    if (itr == minions.end())
        return;

    if (add)
        itr->second.bossInfo->minion.insert(minion->GetGUID());
    else
        itr->second.bossInfo->minion.erase(minion->GetGUID());
}

bool InstanceScript::SetBossState(uint32 id, EncounterState state)
{
    if (id < bosses.size())
    {
        BossInfo* bossInfo = &bosses[id];
        if (bossInfo->state == TO_BE_DECIDED) // loading
        {
            bossInfo->state = state;
            //TC_LOG_ERROR("misc", "Inialize boss %u state as %u.", id, (uint32)state);
            return false;
        }
        else
        {
            if (bossInfo->state == state)
                return false;

            if (state == DONE)
                for (GuidSet::iterator i = bossInfo->minion.begin(); i != bossInfo->minion.end(); ++i)
                    if (Creature* minion = instance->GetCreature(*i))
                        if (minion->isWorldBoss() && minion->IsAlive())
                            return false;

            switch (state)
            {
                case NOT_STARTED:
                {
                    if (bossInfo->state == IN_PROGRESS)
                    {
                        ResetCombatResurrections();
                        SendEncounterEnd();
                    }
                    break;
                }
                case IN_PROGRESS:
                {
                    uint32 resInterval = GetCombatResurrectionChargeInterval();
                    InitializeCombatResurrections(1, resInterval);
                    SendEncounterStart(1, 9, resInterval, resInterval);

                    Map::PlayerList const &playerList = instance->GetPlayers();
                    if (!playerList.isEmpty())
                        for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
                            if (Player* player = i->GetSource())
                                if (player->IsAlive())
                                    player->ProcSkillsAndAuras(nullptr, PROC_FLAG_ENCOUNTER_START, PROC_FLAG_NONE, PROC_SPELL_TYPE_MASK_ALL, PROC_SPELL_PHASE_NONE, PROC_HIT_NONE, nullptr, nullptr, nullptr);
                    break;
                }
                case FAIL:
                case DONE:
                    ResetCombatResurrections();
                    SendEncounterEnd();
                    break;
                default:
                    break;
            }

            bossInfo->state = state;
            SaveToDB();
        }

        for (uint32 type = 0; type < MAX_DOOR_TYPES; ++type)
            for (GuidSet::iterator i = bossInfo->door[type].begin(); i != bossInfo->door[type].end(); ++i)
                if (GameObject* door = instance->GetGameObject(*i))
                    UpdateDoorState(door);

        GuidSet minions = bossInfo->minion; // Copy to prevent iterator invalidation (minion might be unsummoned in UpdateMinionState)
        for (GuidSet::iterator i = minions.begin(); i != minions.end(); ++i)
            if (Creature* minion = instance->GetCreature(*i))
                UpdateMinionState(minion, state);

        return true;
    }
    return false;
}

bool InstanceScript::_SkipCheckRequiredBosses(Player const* player /*= nullptr*/) const
{
    return player && player->GetSession()->HasPermission(rbac::RBAC_PERM_SKIP_CHECK_INSTANCE_REQUIRED_BOSSES);
}

void InstanceScript::Load(char const* data)
{
    if (!data)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(data);

    std::istringstream loadStream(data);

    if (ReadSaveDataHeaders(loadStream))
    {
        ReadSaveDataBossStates(loadStream);
        ReadSaveDataMore(loadStream);
    }
    else
        OUT_LOAD_INST_DATA_FAIL;

    OUT_LOAD_INST_DATA_COMPLETE;
}

bool InstanceScript::ReadSaveDataHeaders(std::istringstream& data)
{
    for (char header : headers)
    {
        char buff;
        data >> buff;

        if (header != buff)
            return false;
    }

    return true;
}

void InstanceScript::ReadSaveDataBossStates(std::istringstream& data)
{
    uint32 bossId = 0;
    for (std::vector<BossInfo>::iterator i = bosses.begin(); i != bosses.end(); ++i, ++bossId)
    {
        uint32 buff;
        data >> buff;
        if (buff == IN_PROGRESS || buff == FAIL || buff == SPECIAL)
            buff = NOT_STARTED;

        if (buff < TO_BE_DECIDED)
            SetBossState(bossId, EncounterState(buff));
    }
}

std::string InstanceScript::GetSaveData()
{
    OUT_SAVE_INST_DATA;

    std::ostringstream saveStream;

    WriteSaveDataHeaders(saveStream);
    WriteSaveDataBossStates(saveStream);
    WriteSaveDataMore(saveStream);

    OUT_SAVE_INST_DATA_COMPLETE;

    return saveStream.str();
}

void InstanceScript::WriteSaveDataHeaders(std::ostringstream& data)
{
    for (char header : headers)
        data << header << ' ';
}

void InstanceScript::WriteSaveDataBossStates(std::ostringstream& data)
{
    for (BossInfo const& bossInfo : bosses)
        data << uint32(bossInfo.state) << ' ';
}

void InstanceScript::HandleGameObject(ObjectGuid guid, bool open, GameObject* go /*= nullptr*/)
{
    if (!go)
        go = instance->GetGameObject(guid);
    if (go)
        go->SetGoState(open ? GO_STATE_ACTIVE : GO_STATE_READY);
    else
        TC_LOG_DEBUG("scripts", "InstanceScript: HandleGameObject failed");
}

void InstanceScript::UpdateOperations(uint32 const diff)
{
    for (auto itr = timedDelayedOperations.begin(); itr != timedDelayedOperations.end(); itr++)
    {
        itr->first -= diff;

        if (itr->first < 0)
        {
            itr->second();
            itr->second = nullptr;
        }
    }

    uint32 timedDelayedOperationCountToRemove = std::count_if(std::begin(timedDelayedOperations), std::end(timedDelayedOperations), [](const std::pair<int32, std::function<void()>> & pair) -> bool
    {
        return pair.second == nullptr;
    });

    for (uint32 i = 0; i < timedDelayedOperationCountToRemove; i++)
    {
        auto itr = std::find_if(std::begin(timedDelayedOperations), std::end(timedDelayedOperations), [](const std::pair<int32, std::function<void()>> & p_Pair) -> bool
        {
            return p_Pair.second == nullptr;
        });

        if (itr != std::end(timedDelayedOperations))
            timedDelayedOperations.erase(itr);
    }

    if (timedDelayedOperations.empty() && !emptyWarned)
    {
        emptyWarned = true;
        LastOperationCalled();
    }
}

void InstanceScript::DoUseDoorOrButton(ObjectGuid guid, uint32 withRestoreTime /*= 0*/, bool useAlternativeState /*= false*/)
{
    if (!guid)
        return;

    if (GameObject* go = instance->GetGameObject(guid))
    {
        if (go->GetGoType() == GAMEOBJECT_TYPE_DOOR || go->GetGoType() == GAMEOBJECT_TYPE_BUTTON)
        {
            if (go->getLootState() == GO_READY)
                go->UseDoorOrButton(withRestoreTime, useAlternativeState);
            else if (go->getLootState() == GO_ACTIVATED)
                go->ResetDoorOrButton();
        }
        else
            TC_LOG_ERROR("scripts", "InstanceScript: DoUseDoorOrButton can't use gameobject entry %u, because type is %u.", go->GetEntry(), go->GetGoType());
    }
    else
        TC_LOG_DEBUG("scripts", "InstanceScript: DoUseDoorOrButton failed");
}

void InstanceScript::DoCloseDoorOrButton(ObjectGuid guid)
{
    if (!guid)
        return;

    if (GameObject* go = instance->GetGameObject(guid))
    {
        if (go->GetGoType() == GAMEOBJECT_TYPE_DOOR || go->GetGoType() == GAMEOBJECT_TYPE_BUTTON)
        {
            if (go->getLootState() == GO_ACTIVATED)
                go->ResetDoorOrButton();
        }
        else
            TC_LOG_ERROR("scripts", "InstanceScript: DoCloseDoorOrButton can't use gameobject entry %u, because type is %u.", go->GetEntry(), go->GetGoType());
    }
    else
        TC_LOG_DEBUG("scripts", "InstanceScript: DoCloseDoorOrButton failed");
}

void InstanceScript::DoRespawnGameObject(ObjectGuid guid, uint32 timeToDespawn /*= MINUTE*/)
{
    if (GameObject* go = instance->GetGameObject(guid))
    {
        switch (go->GetGoType())
        {
            case GAMEOBJECT_TYPE_DOOR:
            case GAMEOBJECT_TYPE_BUTTON:
            case GAMEOBJECT_TYPE_TRAP:
            case GAMEOBJECT_TYPE_FISHINGNODE:
                // not expect any of these should ever be handled
                TC_LOG_ERROR("scripts", "InstanceScript: DoRespawnGameObject can't respawn gameobject entry %u, because type is %u.", go->GetEntry(), go->GetGoType());
                return;
            default:
                break;
        }

        if (go->isSpawned())
            return;

        go->SetRespawnTime(timeToDespawn);
    }
    else
        TC_LOG_DEBUG("scripts", "InstanceScript: DoRespawnGameObject failed");
}

void InstanceScript::DoUpdateWorldState(uint32 uiStateId, uint32 uiStateData)
{
    Map::PlayerList const& lPlayers = instance->GetPlayers();

    if (!lPlayers.isEmpty())
    {
        for (Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
            if (Player* player = itr->GetSource())
                player->SendUpdateWorldState(uiStateId, uiStateData);
    }
    else
        TC_LOG_DEBUG("scripts", "DoUpdateWorldState attempt send data but no players in map.");
}

// Send Notify to all players in instance
void InstanceScript::DoSendNotifyToInstance(char const* format, ...)
{
    InstanceMap::PlayerList const& players = instance->GetPlayers();

    if (!players.isEmpty())
    {
        va_list ap;
        va_start(ap, format);
        char buff[1024];
        vsnprintf(buff, 1024, format, ap);
        va_end(ap);
        for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
            if (Player* player = i->GetSource())
                if (WorldSession* session = player->GetSession())
                    session->SendNotification("%s", buff);
    }
}

// Update Achievement Criteria for all players in instance
void InstanceScript::DoUpdateCriteria(CriteriaTypes type, uint32 miscValue1 /*= 0*/, uint32 miscValue2 /*= 0*/, Unit* unit /*= NULL*/)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->GetSource())
                player->UpdateCriteria(type, miscValue1, miscValue2, 0, unit);
}

void InstanceScript::DoSendEventScenario(uint32 eventId /*= 0*/)
{
    DoUpdateCriteria(CRITERIA_TYPE_SEND_EVENT_SCENARIO, eventId, 0, nullptr);
}

// Start timed achievement for all players in instance
void InstanceScript::DoStartCriteriaTimer(CriteriaTimedTypes type, uint32 entry)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->GetSource())
                player->StartCriteriaTimer(type, entry);
}

// Stop timed achievement for all players in instance
void InstanceScript::DoStopCriteriaTimer(CriteriaTimedTypes type, uint32 entry)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->GetSource())
                player->RemoveCriteriaTimer(type, entry);
}

// Remove Auras due to Spell on all players in instance
void InstanceScript::DoRemoveAurasDueToSpellOnPlayers(uint32 spell)
{
    Map::PlayerList const& PlayerList = instance->GetPlayers();
    if (!PlayerList.isEmpty())
    {
        for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
        {
            if (Player* player = itr->GetSource())
            {
                player->RemoveAurasDueToSpell(spell);
                if (Pet* pet = player->GetPet())
                    pet->RemoveAurasDueToSpell(spell);
            }
        }
    }
}

// Kill all players with this aura in the instance
void InstanceScript::DoKillPlayersWithAura(uint32 spell)
{
    Map::PlayerList const& PlayerList = instance->GetPlayers();
    for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
        if (Player* player = itr->GetSource())
            if (player->HasAura(spell))
                player->Kill(player);
}

// Apply Dampening Spell for Creature
void InstanceScript::DoDampeningForCreatures(Creature* creature)
{
    if (!creature || !GetDampening() || !GetDampeningSpell())
        return;

    // Only normal/heroic mode
    if (instance->GetDifficultyID() == DIFFICULTY_LFR)
    {
        SetDampeningSpell(0);
        return;
    }

    if (Pet* pet = creature->ToPet())
        if (Player* player = pet->GetOwner())
            return;

    if (TempSummon* temp = creature->ToTempSummon())
        if (temp->GetSummonerGUID().IsPlayer())
            return;

    if (creature->HasAura(GetDampeningSpell()))
        return;

    creature->CastSpell(creature, GetDampeningSpell(), true);
    dampenedGUIDs.push_back(creature->GetGUID());
}

// Remove Specific Dampening Spell from affected creatures and prevent apply for future
void InstanceScript::DoRemoveDampeningFromCreatures()
{
    SetDampening(false);

    for (auto&& itr : dampenedGUIDs)
        if (Creature* creature = instance->GetCreature(itr))
            creature->RemoveAurasDueToSpell(GetDampeningSpell());

    // Not apply anymore in reset/evade
    SetDampeningSpell(0);
}

// Cast spell on all players in instance
void InstanceScript::DoCastSpellOnPlayers(uint32 spell, Unit* caster /*= nullptr*/, bool triggered /*= true*/)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
    {
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
        {
            if (Player* player = i->GetSource())
            {
                caster = caster ? caster : player;
                caster->CastSpell(player, spell, triggered);
            }
        }
    }
}

// Cast spell on all players in instance
void InstanceScript::DoPlayScenePackageIdOnPlayers(uint32 scenePackageId)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->GetSource())
                player->GetSceneMgr().PlaySceneByPackageId(scenePackageId);
}

void InstanceScript::DoPlaySceneOnPlayers(uint32 sceneId)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->GetSource())
                player->GetSceneMgr().PlayScene(sceneId);
}

void InstanceScript::DoRemoveForcedMovementsOnPlayers(ObjectGuid forceGuid)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->GetSource())
                player->RemoveMovementForce(forceGuid);
}

bool InstanceScript::ServerAllowsTwoSideGroups()
{
    return sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP);
}

void InstanceScript::DoOnPlayers(std::function<void(Player*)>&& function)
{
    Map::PlayerList const& plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* player = i->GetSource())
                function(player);
}

void InstanceScript::DoSetAlternatePowerOnPlayers(int32 value)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->GetSource())
                pPlayer->SetPower(POWER_ALTERNATE_POWER, value);
}

void InstanceScript::DoModifyPlayerCurrencies(uint32 id, int32 value)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->GetSource())
                pPlayer->ModifyCurrency(id, value);
}

void InstanceScript::RepopPlayersAtGraveyard()
{
    if (!instance)
        return;

    DoOnPlayers([](Player* player)
        {
            player->RepopAtGraveyard();
        });
}

void InstanceScript::DoNearTeleportPlayers(const Position pos, bool casting /*=false*/)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->GetSource())
                pPlayer->NearTeleportTo(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation(), casting);
}

void InstanceScript::DoTeleportPlayers(uint32 mapId, const Position pos)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->GetSource())
                pPlayer->TeleportTo(mapId, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation());
}

void InstanceScript::DoKilledMonsterKredit(uint32 questId, uint32 entry, ObjectGuid guid/* =0*/)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->GetSource())
                if (pPlayer->GetQuestStatus(questId) == QUEST_STATUS_INCOMPLETE)
                    pPlayer->KilledMonsterCredit(entry, guid);
}

// Complete Achievement for all players in instance
void InstanceScript::DoCompleteAchievement(uint32 achievement)
{
    AchievementEntry const* achievementEntry = sAchievementStore.LookupEntry(achievement);
    if (!achievementEntry)
    {
        TC_LOG_ERROR("scripts", "DoCompleteAchievement called for not existing achievement %u", achievement);
        return;
    }

    Map::PlayerList const& playerList = instance->GetPlayers();
    if (!playerList.isEmpty())
        for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
            if (Player* player = i->GetSource())
                player->CompletedAchievement(achievementEntry);
}

void InstanceScript::DoStartMovie(uint32 movieId)
{
    MovieEntry const* movieEntry = sMovieStore.LookupEntry(movieId);
    if (!movieEntry)
    {
        TC_LOG_ERROR("scripts", "DoStartMovie called for not existing movieId %u", movieId);
        return;
    }

    Map::PlayerList const& playerList = instance->GetPlayers();
    if (!playerList.isEmpty())
        for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
            if (Player* player = i->GetSource())
                player->SendMovieStart(movieId);
}

// Add phase on all players in instance
void InstanceScript::DoAddPhaseOnPlayers(uint32 phase)
{
    Map::PlayerList const& playerList = instance->GetPlayers();
    if (!playerList.isEmpty())
        for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
            if (Player* player = i->GetSource())
                PhasingHandler::AddPhase(player, phase, true);
}

// Remove phase on all players in instance
void InstanceScript::DoRemovePhaseOnPlayers(uint32 phase)
{
    Map::PlayerList const& playerList = instance->GetPlayers();
    if (!playerList.isEmpty())
        for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
            if (Player* player = i->GetSource())
                PhasingHandler::RemovePhase(player, phase, true);
}

// Update Achievement Criteria for all players in instance
void InstanceScript::DoUpdateAchievementCriteria(CriteriaTypes type, uint32 miscValue1 /*= 0*/, uint32 miscValue2 /*= 0*/, Unit* unit /*= NULL*/)
{
    Map::PlayerList const& playerList = instance->GetPlayers();

    if (!playerList.isEmpty())
        for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
            if (Player* player = i->GetSource())
                player->GetAchievementMgr()->UpdateCriteria(type, miscValue1, miscValue2, 0, unit);
}

// Add aura on all players in instance
void InstanceScript::DoAddAuraOnPlayers(uint32 spell)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->GetSource())
                player->AddAura(spell, player);
}

void InstanceScript::DoCombatStopOnPlayers()
{
    Map::PlayerList const& PlayerList = instance->GetPlayers();
    if (PlayerList.isEmpty())
        return;

    for (Map::PlayerList::const_iterator Iter = PlayerList.begin(); Iter != PlayerList.end(); ++Iter)
    {
        if (Player* Player = Iter->GetSource())
        {
            if (!Player->IsInCombat())
                continue;

            Player->CombatStop();
        }
    }
}

void InstanceScript::RespawnCreature(uint64 p_Guid /*= 0*/)
{
    Map::PlayerList const& playerList = instance->GetPlayers();
    if (!playerList.isEmpty())
    {
        Map::PlayerList::const_iterator i = playerList.begin();
        if (Player* player = i->GetSource()) {
            Trinity::RespawnDo u_do;
            Trinity::WorldObjectWorker<Trinity::RespawnDo> worker(player, u_do);
            Cell::VisitGridObjects(player, worker, player->GetGridActivationRange() * 1000);
        }      
    }
}

bool InstanceScript::CheckAchievementCriteriaMeet(uint32 criteria_id, Player const* /*source*/, Unit const* /*target*/ /*= NULL*/, uint32 /*miscvalue1*/ /*= 0*/)
{
    TC_LOG_ERROR("misc", "Achievement system call InstanceScript::CheckAchievementCriteriaMeet but instance script for map %u not have implementation for achievement criteria %u",
        instance->GetId(), criteria_id);
    return false;
}

void InstanceScript::SetEntranceLocation(uint32 worldSafeLocationId)
{
    _entranceId = worldSafeLocationId;
    if (_temporaryEntranceId)
        _temporaryEntranceId = 0;
}

void InstanceScript::SendEncounterUnit(uint32 type, Unit* unit /*= NULL*/, uint8 priority)
{
    switch (type)
    {
        case ENCOUNTER_FRAME_ENGAGE:                    // SMSG_INSTANCE_ENCOUNTER_ENGAGE_UNIT
        {
            if (!unit)
                return;

            WorldPackets::Instance::InstanceEncounterEngageUnit encounterEngageMessage;
            encounterEngageMessage.Unit = unit->GetGUID();
            encounterEngageMessage.TargetFramePriority = priority;
            instance->SendToPlayers(encounterEngageMessage.Write());
            break;
        }
        case ENCOUNTER_FRAME_DISENGAGE:                 // SMSG_INSTANCE_ENCOUNTER_DISENGAGE_UNIT
        {
            if (!unit)
                return;

            WorldPackets::Instance::InstanceEncounterDisengageUnit encounterDisengageMessage;
            encounterDisengageMessage.Unit = unit->GetGUID();
            instance->SendToPlayers(encounterDisengageMessage.Write());
            break;
        }
        case ENCOUNTER_FRAME_UPDATE_PRIORITY:           // SMSG_INSTANCE_ENCOUNTER_CHANGE_PRIORITY
        {
            if (!unit)
                return;

            WorldPackets::Instance::InstanceEncounterChangePriority encounterChangePriorityMessage;
            encounterChangePriorityMessage.Unit = unit->GetGUID();
            encounterChangePriorityMessage.TargetFramePriority = priority;
            instance->SendToPlayers(encounterChangePriorityMessage.Write());
            break;
        }
        default:
            break;
    }
}

void InstanceScript::SendEncounterStart(uint32 inCombatResCount /*= 0*/, uint32 maxInCombatResCount /*= 0*/, uint32 inCombatResChargeRecovery /*= 0*/, uint32 nextCombatResChargeTime /*= 0*/)
{
    WorldPackets::Instance::InstanceEncounterStart encounterStartMessage;
    encounterStartMessage.InCombatResCount = inCombatResCount;
    encounterStartMessage.MaxInCombatResCount = maxInCombatResCount;
    encounterStartMessage.CombatResChargeRecovery = inCombatResChargeRecovery;
    encounterStartMessage.NextCombatResChargeTime = nextCombatResChargeTime;

    instance->SendToPlayers(encounterStartMessage.Write());
}

void InstanceScript::SendEncounterEnd()
{
    WorldPackets::Instance::InstanceEncounterEnd encounterEndMessage;
    instance->SendToPlayers(encounterEndMessage.Write());
}

void InstanceScript::SendBossKillCredit(uint32 encounterId)
{
    WorldPackets::Instance::BossKillCredit bossKillCreditMessage;
    bossKillCreditMessage.DungeonEncounterID = encounterId;

    instance->SendToPlayers(bossKillCreditMessage.Write());
}

void InstanceScript::SendCompleteDungeonEncounter(uint32 encounterId)
{
    if (InstanceScenario* scenario = instance->ToInstanceMap()->GetInstanceScenario())
    {
        Map::PlayerList const& players = instance->GetPlayers();

        if (players.begin() != players.end())
            scenario->UpdateCriteria(CRITERIA_TYPE_COMPLETE_DUNGEON_ENCOUNTER, encounterId, 0, 0, nullptr, players.begin()->GetSource());
    }
}
void InstanceScript::CompleteScenario()
{
    if (InstanceScenario* inScenario = instance->GetInstanceScenario())

        inScenario->CompleteScenario();
    else
        TC_LOG_ERROR("scripts", "InstanceScript::CompleteScenario() fail", "");
}

void InstanceScript::CompleteCurrStep()
{
    if (InstanceScenario* inScenario = instance->GetInstanceScenario())
        inScenario->CompleteCurrStep();
    else
        TC_LOG_ERROR("scripts", "InstanceScript::CompleteCurrStep() fail", "");
}

void InstanceScript::GetScenarioData(Player* p_Player/*= nullptr*/)
{
    //m_ScenarioID, m_ScenarioStep
    TC_LOG_ERROR("scripts", "GetScenarioData  start %s", "");
    TeamId tid;
    uint32 zoneid;
    if (p_Player == nullptr)
    {
        tid = TEAM_ALLIANCE;
        zoneid = 0;
        TC_LOG_ERROR("scripts", "GetScenarioData  p_Player == nullptr %s", "");
    }
    else
    {
        tid  = (p_Player == nullptr) ? TEAM_ALLIANCE : p_Player->GetTeamId();
        zoneid = (p_Player == nullptr) ? 0 : p_Player->GetZoneId();
        TC_LOG_ERROR("scripts", "GetScenarioData  p_Player != nullptr %s", "");
    }
    InstanceMap* map = instance->ToInstanceMap();
    //map->SetDifficultyID(DIFFICULTY_MYTHIC_KEYSTONE);
    TC_LOG_ERROR("scripts", "GetScenarioData  map->SetDifficultyID %s", "");
    if (InstanceScenario* instanceScenario = sScenarioMgr->CreateInstanceScenario(map, tid, zoneid))
    {
        TC_LOG_ERROR("scripts", "GetScenarioData CreateInstanceScenario %s", "");
        m_ScenarioID = instanceScenario->GetStep()->ScenarioID;
        m_ScenarioStep = instanceScenario->GetStep()->ID;
        map->SetInstanceScenario(instanceScenario);

    }
    else

        TC_LOG_DEBUG("scripts", "InstanceScript: GetScenarioData failed");

    TC_LOG_ERROR("scripts", "GetScenarioData  end %s", "");

}

void InstanceScript::SendScenarioState(ScenarioData p_Data, Player* p_Player /*= nullptr*/)
{
    WorldPacket l_Data(SMSG_SCENARIO_STATE);

    l_Data << int32(p_Data.m_ScenarioID);
    l_Data << int32(p_Data.m_StepID);
    l_Data << uint32(instance->GetDifficultyID());
    l_Data << uint32(p_Data.m_WaveCurrent);
    l_Data << uint32(p_Data.m_WaveMax);
    l_Data << uint32(p_Data.m_TimerDuration);

    l_Data << uint32(p_Data.m_CriteriaCount);
    l_Data << uint32(p_Data.m_BonusCount);

    for (CriteriaProgressData l_ProgressData : p_Data.m_CriteriaProgress)
        BuildCriteriaProgressPacket(&l_Data, l_ProgressData);

    for (BonusObjectiveData l_BonusObjective : p_Data.m_BonusObjectives)
    {
        l_Data << int32(l_BonusObjective.m_ObjectiveID);
        l_Data.WriteBit(l_BonusObjective.m_ObjectiveComplete);
        l_Data.FlushBits();
    }

    l_Data.WriteBit(p_Data.m_ScenarioComplete);
    l_Data.FlushBits();

    if (p_Player == nullptr)
        instance->SendToPlayers(&l_Data);
    else
        p_Player->SendDirectMessage(&l_Data);
}

void InstanceScript::SendScenarioProgressUpdate(CriteriaProgressData p_Data, Player* p_Player /*= nullptr*/)
{
    WorldPacket l_Data(SMSG_SCENARIO_PROGRESS_UPDATE, 39);
    BuildCriteriaProgressPacket(&l_Data, p_Data);

    if (p_Player == nullptr)
        instance->SendToPlayers(&l_Data);
    else
        p_Player->SendDirectMessage(&l_Data);
}

void InstanceScript::BuildCriteriaProgressPacket(WorldPacket* p_Data, CriteriaProgressData p_CriteriaProgress)
{
    *p_Data << int32(p_CriteriaProgress.m_ID);
    *p_Data << uint64(p_CriteriaProgress.m_Quantity);

    *p_Data << uint64(p_CriteriaProgress.m_Guid);
    //p_Data->appendPackGUID(p_CriteriaProgress.m_Guid);
    *p_Data << uint32(secsToTimeBitFields(p_CriteriaProgress.m_Date));
    *p_Data << int32(p_CriteriaProgress.m_TimeFromStart);
    *p_Data << int32(p_CriteriaProgress.m_TimeFromCreate);

    p_Data->WriteBits(p_CriteriaProgress.m_Flags, 4);
    p_Data->FlushBits();
}

bool InstanceScript::IsWipe() const
{
    Map::PlayerList const& PlayerList = instance->GetPlayers();

    if (PlayerList.isEmpty())
        return true;

    for (Map::PlayerList::const_iterator Itr = PlayerList.begin(); Itr != PlayerList.end(); ++Itr)
    {
        Player* player = Itr->GetSource();

        if (!player)
            continue;

        if (player->IsAlive() && !player->IsGameMaster())
            return false;
    }

    return true;
}

void InstanceScript::UpdateEncounterState(EncounterCreditType type, uint32 creditEntry, Unit* /*source*/)
{
    DungeonEncounterList const* encounters = sObjectMgr->GetDungeonEncounterList(instance->GetId(), instance->GetDifficultyID());
    if (!encounters)
        return;

    uint32 dungeonId = 0;

    for (DungeonEncounterList::const_iterator itr = encounters->begin(); itr != encounters->end(); ++itr)
    {
        DungeonEncounter const* encounter = *itr;
        if (encounter->creditType == type && encounter->creditEntry == creditEntry)
        {
            completedEncounters |= 1 << encounter->dbcEntry->Bit;

            if (type == ENCOUNTER_CREDIT_KILL_CREATURE)
                SendBossKillCredit(encounter->dbcEntry->ID);

            SendCompleteDungeonEncounter(encounter->dbcEntry->ID);

            if (encounter->lastEncounterDungeon)
            {
                dungeonId = encounter->lastEncounterDungeon;
                TC_LOG_DEBUG("lfg", "UpdateEncounterState: Instance %s (instanceId %u) completed encounter %s. Credit Dungeon: %u",
                    instance->GetMapName(), instance->GetInstanceId(), encounter->dbcEntry->Name->Str[sWorld->GetDefaultDbcLocale()], dungeonId);
                break;
            }
        }
    }

    if (dungeonId)
    {
        Map::PlayerList const& players = instance->GetPlayers();
        for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
        {
            if (Player* player = i->GetSource())
                if (Group* grp = player->GetGroup())
                    if (grp->isLFGGroup())
                    {
                        sLFGMgr->FinishDungeon(grp->GetGUID(), dungeonId);
                        return;
                    }
        }
    }
}

void InstanceScript::UpdateEncounterStateForKilledCreature(uint32 creatureId, Unit* source)
{
    UpdateEncounterState(ENCOUNTER_CREDIT_KILL_CREATURE, creatureId, source);
}

void InstanceScript::UpdateEncounterStateForSpellCast(uint32 spellId, Unit* source)
{
    UpdateEncounterState(ENCOUNTER_CREDIT_CAST_SPELL, spellId, source);
}

void InstanceScript::UpdatePhasing()
{
    Map::PlayerList const& players = instance->GetPlayers();
    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        if (Player* player = itr->GetSource())
            PhasingHandler::SendToPlayer(player);
}

std::string InstanceScript::GetBossStateName(uint8 state)
{
    // See enum EncounterState in InstanceScript.h
    switch (state)
    {
        case NOT_STARTED:
            return "NOT_STARTED";
        case IN_PROGRESS:
            return "IN_PROGRESS";
        case FAIL:
            return "FAIL";
        case DONE:
            return "DONE";
        case SPECIAL:
            return "SPECIAL";
        case TO_BE_DECIDED:
            return "TO_BE_DECIDED";
        default:
            return "INVALID";
    }
}

void InstanceScript::UpdateCombatResurrection(uint32 diff)
{
    if (!_combatResurrectionTimerStarted)
        return;

    if (_combatResurrectionTimer <= diff)
        AddCombatResurrectionCharge();
    else
        _combatResurrectionTimer -= diff;
}

void InstanceScript::InitializeCombatResurrections(uint8 charges /*= 1*/, uint32 interval /*= 0*/)
{
    _combatResurrectionCharges = charges;
    if (!interval)
        return;

    _combatResurrectionTimer = interval;
    _combatResurrectionTimerStarted = true;
}

void InstanceScript::AddCombatResurrectionCharge()
{
    ++_combatResurrectionCharges;
    _combatResurrectionTimer = GetCombatResurrectionChargeInterval();

    WorldPackets::Instance::InstanceEncounterGainCombatResurrectionCharge gainCombatResurrectionCharge;
    gainCombatResurrectionCharge.InCombatResCount = _combatResurrectionCharges;
    gainCombatResurrectionCharge.CombatResChargeRecovery = _combatResurrectionTimer;
    instance->SendToPlayers(gainCombatResurrectionCharge.Write());
}

void InstanceScript::UseCombatResurrection()
{
    --_combatResurrectionCharges;

    instance->SendToPlayers(WorldPackets::Instance::InstanceEncounterInCombatResurrection().Write());
}

void InstanceScript::ResetCombatResurrections()
{
    _combatResurrectionCharges = 0;
    _combatResurrectionTimer = 0;
    _combatResurrectionTimerStarted = false;
}

uint32 InstanceScript::GetCombatResurrectionChargeInterval() const
{
    uint32 interval = 0;
    if (uint32 playerCount = instance->GetPlayers().getSize())
        interval = 90 * MINUTE * IN_MILLISECONDS / playerCount;

    return interval;
}

class ChallengeModeWorker
{
public:
    ChallengeModeWorker(InstanceScript* instance) : _instance(instance) { }

    void Visit(std::unordered_map<ObjectGuid, Creature*>& creatureMap)
    {
        for (auto const& p : creatureMap)
        {
            if (p.second->IsInWorld() && !p.second->IsPet())
            {
                if (!p.second->IsAlive())
                    p.second->Respawn();

                _instance->CastChallengeCreatureSpell(p.second);
            }
        }
    }

    template<class T>
    void Visit(std::unordered_map<ObjectGuid, T*>&) { }

private:
    InstanceScript* _instance;
};

void InstanceScript::StartChallengeMode(uint8 modeid, uint8 level, uint8 affix1, uint8 affix2, uint8 affix3)
{
    _challengeModeId = modeid;
    MapChallengeModeEntry const* mapChallengeModeEntry = sChallengeModeMgr->GetMapChallengeModeEntryByModeId(GetChallengeModeId());
    if (!mapChallengeModeEntry)
        return;

    if (IsChallengeModeStarted())
        return;

    if (GetCompletedEncounterMask() != 0)
        return;

    _affixes[0] = affix1;
    _affixes[1] = affix2;
    _affixes[2] = affix3;
    for (auto const& affix : _affixes)
        _affixesTest.set(affix);

    _challengeModeStarted = true;
    _challengeModeLevel = level;

    instance->SendToPlayers(WorldPackets::ChallengeMode::ChangePlayerDifficultyResult(5).Write());

    // Add the health/dmg modifier aura to all creatures
    ChallengeModeWorker worker(this);
    TypeContainerVisitor<ChallengeModeWorker, MapStoredObjectTypesContainer> visitor(worker);
    visitor.Visit(instance->GetObjectsStore());

    // Tp back all players to begin
    Position entranceLocation;
    if (WorldSafeLocsEntry const* entranceSafeLocEntry = sWorldSafeLocsStore.LookupEntry(GetEntranceLocation()))
        entranceLocation.Relocate(entranceSafeLocEntry->Loc.X, entranceSafeLocEntry->Loc.Y, entranceSafeLocEntry->Loc.Z, entranceSafeLocEntry->Facing);
    else if (AreaTriggerTeleportStruct const* areaTrigger = sObjectMgr->GetMapEntranceTrigger(instance->GetId()))
        entranceLocation.Relocate(areaTrigger->target_X, areaTrigger->target_Y, areaTrigger->target_Z, areaTrigger->target_Orientation);
    DoNearTeleportPlayers(entranceLocation);

    if (_challengeModeDoorPosition.has_value())
        instance->SummonGameObject(GOB_CHALLENGER_DOOR, *_challengeModeDoorPosition, QuaternionData(), WEEK);

    WorldPackets::ChallengeMode::ChangePlayerDifficultyResult changePlayerDifficultyResult(11);
    changePlayerDifficultyResult.InstanceDifficultyID = instance->GetId();
    changePlayerDifficultyResult.DifficultyRecID = DIFFICULTY_MYTHIC_KEYSTONE;
    instance->SendToPlayers(changePlayerDifficultyResult.Write());

    instance->SendToPlayers(WorldPackets::ChallengeMode::Reset(instance->GetId()).Write());

    WorldPackets::Misc::StartTimer startTimer;
    startTimer.Type = WorldPackets::Misc::StartTimer::TIMER_TYPE_CHALLENGE;
    startTimer.TotalTime = 10;
    startTimer.TimeLeft = 10;
    instance->SendToPlayers(startTimer.Write());

    SendChallengeModeStart();

    Map::PlayerList const &players = instance->GetPlayers();
    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
	{
        CastChallengePlayerSpell(itr->GetSource());
		// HOOK to PLAYERSCRIPT
        sScriptMgr->OnPlayerStartChallengeMode(itr->GetSource(), level, affix1, affix2, affix3);
    }

    AddTimedDelayedOperation(10000, [this]()
    {
        _challengeModeStartTime = getMSTime();

        SendChallengeModeElapsedTimer();

        if (GameObject* door = GetGameObject(GOB_CHALLENGER_DOOR))
            DoUseDoorOrButton(door->GetGUID(), WEEK);
		
		HideChallengeDoor();
    });
}

void InstanceScript::CompleteChallengeMode()
{
    MapChallengeModeEntry const* mapChallengeModeEntry = sChallengeModeMgr->GetMapChallengeModeEntryByModeId(GetChallengeModeId());
    if (!mapChallengeModeEntry)
        return;

    uint32 totalDuration = GetChallengeModeCurrentDuration();

    // Todo : Send stats

    uint8 mythicIncrement = 0;

    for (uint8 i = 0; i < 3; ++i)
        if (uint32(mapChallengeModeEntry->CriteriaCount[i]) > totalDuration)
            ++mythicIncrement;

    Map::PlayerList const &players = instance->GetPlayers();
    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        itr->GetSource()->AddChallengeKey(sChallengeModeMgr->GetRandomChallengeId(), std::max(_challengeModeLevel + mythicIncrement, 2));

    WorldPackets::ChallengeMode::Complete complete;
    complete.Duration = totalDuration;
    complete.MapId = instance->GetId();
    complete.ChallengeId = mapChallengeModeEntry->ID;
    complete.ChallengeLevel = _challengeModeLevel + mythicIncrement;
    instance->SendToPlayers(complete.Write());

    // Achievements only if timer respected
    if (mythicIncrement)
    {
        if (_challengeModeLevel >= 2)
            DoCompleteAchievement(11183);

        if (_challengeModeLevel >= 5)
            DoCompleteAchievement(11184);

        if (_challengeModeLevel >= 10)
            DoCompleteAchievement(11185);

        if (_challengeModeLevel >= 15)
        {
            DoCompleteAchievement(11162);
            DoCompleteAchievement(11224);
        }
    }

    SpawnChallengeModeRewardChest();

    //ChallengeNewPlayerRecord
    uint32 totalDurations = totalDuration * 1000;

    auto challengeData = new ChallengeData;

    challengeData->ID = instance->GetInstanceId(); //sObjectMgr->GetGenerator<HighGuid::Scenario>().Generate();
    challengeData->MapID = instance->GetId();
    challengeData->RecordTime = totalDurations;
    challengeData->Date = time(nullptr);
    challengeData->ChallengeLevel = _challengeModeLevel;
    challengeData->TimerLevel = std::max(_challengeModeLevel + mythicIncrement, 2);
    challengeData->ChallengeID = mapChallengeModeEntry ? mapChallengeModeEntry->ID : 0;
    challengeData->Affixes = _affixes;
    challengeData->GuildID = 0;
    //chest id
    if (_challengeChest.IsEmpty())
        challengeData->ChestID = sChallengeModeMgr->GetChest(challengeData->ChallengeID);
    else
        challengeData->ChestID = _challengeChest.GetEntry();

    std::map<ObjectGuid::LowType /*guild*/, uint32> guildCounter;

    DoOnPlayers([&](Player* player)
        {
            sChallengeModeMgr->Reward(player, _challengeModeLevel);

            ChallengeMember member;
            member.guid = player->GetGUID();
            member.specId = player->GetSpecializationId();// > GetUInt32Value(PLAYER_FIELD_CURRENT_SPEC_ID);
            member.Date = time(nullptr);
            member.ChallengeLevel = _challengeModeLevel;
            //chest id
            if (_challengeChest.IsEmpty())
                member.ChestID = sChallengeModeMgr->GetChest(challengeData->ChallengeID);
            else
                member.ChestID = _challengeChest.GetEntry();

            if (player->GetGuildId())
                guildCounter[player->GetGuildId()]++;

            challengeData->member.insert(member);
            if (sChallengeModeMgr->CheckBestMemberMapId(member.guid, challengeData))
            {
                WorldPackets::ChallengeMode::NewPlayerRecord newplayerrecord;
                newplayerrecord.Duration = totalDurations;
                newplayerrecord.MapId = instance->GetId();
                newplayerrecord.ChallengeLevel = _challengeModeLevel;
                player->GetSession()->SendPacket(newplayerrecord.Write());
            }

            SendChallengeModeMapStatsUpdate(player, challengeData->ChallengeLevel, challengeData->RecordTime);
            //player->GetSession()->SendChallengeModeMapStatsUpdate(mapChallengeModeEntry->ID);

            player->UpdateCriteria(CRITERIA_TYPE_COMPLETE_CHALLENGE_MODE, instance->GetId(), _challengeModeLevel);

            player->RemoveAura(ChallengersBurden);
        });

    for (auto const& v : guildCounter)
        if (v.second >= 3)
            challengeData->GuildID = v.first;

    sChallengeModeMgr->SetChallengeMapData(challengeData->ID, challengeData);
    sChallengeModeMgr->CheckBestMapId(challengeData);
    sChallengeModeMgr->CheckBestGuildMapId(challengeData);
    sChallengeModeMgr->SaveChallengeToDB(challengeData);
}

std::array<uint32, 3> InstanceScript::GetAffixes() const
{
    return _affixes;
}

bool InstanceScript::HasAffix(Affixes affix)
{
    return _affixesTest.test(size_t(affix));
}

uint32 InstanceScript::GetChallengeModeCurrentDuration() const
{
    return uint32(GetMSTimeDiffToNow(_challengeModeStartTime) / 1000) + (5 * _challengeModeDeathCount);
}

void InstanceScript::SendChallengeModeStart(Player* player/* = nullptr*/) const
{
    MapChallengeModeEntry const* mapChallengeModeEntry = sChallengeModeMgr->GetMapChallengeModeEntryByModeId(GetChallengeModeId());
    if (!mapChallengeModeEntry)
        return;

    WorldPackets::ChallengeMode::Start start;
    start.MapId = instance->GetId();
    start.ChallengeId = mapChallengeModeEntry->ID;
    start.ChallengeLevel = _challengeModeLevel;
    instance->SendToPlayers(start.Write());

    if (player)
        player->SendDirectMessage(start.Write());
    else
        instance->SendToPlayers(start.Write());
}

void InstanceScript::SendChallengeModeDeathCount(Player* player/* = nullptr*/) const
{
    WorldPackets::ChallengeMode::UpdateDeathCount updateDeathCount;
    updateDeathCount.DeathCount = _challengeModeDeathCount;

    if (player)
        player->SendDirectMessage(updateDeathCount.Write());
    else
        instance->SendToPlayers(updateDeathCount.Write());
}

void InstanceScript::SendChallengeModeElapsedTimer(Player* player/* = nullptr*/) const
{
    WorldPackets::Misc::StartElapsedTimer startElapsedTimer;
    startElapsedTimer.TimerID = 1;
    startElapsedTimer.CurrentDuration = GetChallengeModeCurrentDuration();

    if (player)
        player->SendDirectMessage(startElapsedTimer.Write());
    else
        instance->SendToPlayers(startElapsedTimer.Write());
}

void InstanceScript::SendChallengeModeMapStatsUpdate(Player* player, uint32 challengeId, uint32 recordTime) const
{
    ChallengeByMap* bestMap = sChallengeModeMgr->BestForMember(player->GetGUID());
    if (!bestMap)
        return;

    auto itr = bestMap->find(instance->GetId());
    if (itr == bestMap->end())
        return;

    ChallengeData* best = itr->second;
    if (!best)
        return;

    WorldPackets::ChallengeMode::NewPlayerRecord update;

    update.MapId = instance->GetId();
    update.Duration = best->RecordTime;
    update.ChallengeLevel = challengeId;

    if (player)
        player->SendDirectMessage(update.Write());
}

void InstanceScript::CastChallengeCreatureSpell(Creature* creature)
{
    if (!creature || creature->IsTrigger() || creature->IsControlledByPlayer() || creature->GetCreatureType() == CREATURE_TYPE_CRITTER)
        return;

    Unit* owner = creature->GetCharmerOrOwnerPlayerOrPlayerItself();
    if (owner && owner->IsPlayer())
        return;

    float modHealth = sChallengeModeMgr->GetHealthMultiplier(_challengeModeLevel);
    float modDamage = sChallengeModeMgr->GetDamageMultiplier(_challengeModeLevel);

    bool isDungeonBoss = false;
    if (creature->IsDungeonBoss())
        isDungeonBoss = true;

    if (isDungeonBoss)
    {
        // 9 Tyrannical ??
        if (HasAffix(Affixes::Tyrannical))
        {
            modHealth *= 1.4f;
            modDamage *= 1.15f;
        }
    }//10 Fortified ??
    else if (HasAffix(Affixes::Fortified))
    {
        modHealth *= 1.2f;
        modDamage *= 1.3f;
    }

    CustomSpellValues values;

    values.AddSpellMod(SPELLVALUE_BASE_POINT0, modHealth);
    values.AddSpellMod(SPELLVALUE_BASE_POINT1, modDamage);

    // Affixes
    values.AddSpellMod(SPELLVALUE_BASE_POINT2, (HasAffix(Affixes::Raging) && !isDungeonBoss) ? 1 : 0); // 6 Raging ??
    values.AddSpellMod(SPELLVALUE_BASE_POINT3, HasAffix(Affixes::Bolstering) ? 1 : 0); // 7 Bolstering ??
    values.AddSpellMod(SPELLVALUE_BASE_POINT4, (HasAffix(Affixes::Tyrannical) && isDungeonBoss) ? 1 : 0); // 9 Tyrannical ??
    values.AddSpellMod(SPELLVALUE_BASE_POINT5, 1); //
    values.AddSpellMod(SPELLVALUE_BASE_POINT6, 1); //
    values.AddSpellMod(SPELLVALUE_BASE_POINT7, HasAffix(Affixes::Volcanic) ? 1 : 0); // 3 Volcanic ??
    values.AddSpellMod(SPELLVALUE_BASE_POINT8, HasAffix(Affixes::Necrotic) ? 1 : 0); // 4 Necrotic ??
    values.AddSpellMod(SPELLVALUE_BASE_POINT9, (HasAffix(Affixes::Fortified) && !isDungeonBoss) ? 1 : 0); // 10 Fortified ??
    values.AddSpellMod(SPELLVALUE_BASE_POINT10, HasAffix(Affixes::Sanguine) ? 1 : 0); // 8 Sanguine ??
    values.AddSpellMod(SPELLVALUE_BASE_POINT11, HasAffix(Affixes::Quaking) ? 1 : 0); // 14 Quaking ??
    values.AddSpellMod(SPELLVALUE_BASE_POINT12, HasAffix(Affixes::FelExplosives) ? 1 : 0); // 13 Explosive ??
    values.AddSpellMod(SPELLVALUE_BASE_POINT13, HasAffix(Affixes::Bursting) ? 1 : 0); // 11 Bursting ??
    //5 15 ??
    //values.AddSpellMod(SPELLVALUE_BASE_POINT14, 0); //
    //values.AddSpellMod(SPELLVALUE_BASE_POINT15, 0); //
    creature->CastCustomSpell(SPELL_CHALLENGER_MIGHT, values, creature, TRIGGERED_FULL_MASK);
    //5 Teeming ??
    if (HasAffix(Affixes::Teeming) && !creature->IsDungeonBoss() && !creature->IsSummon() && !creature->IsAffixDisabled() && roll_chance_f(30.0f) && creature->GetSpawnId()) // Only for real creature summon copy
    {
        Position pos;
        pos = creature->GetNearPosition(6.0f, creature->GetOrientation());
        creature->SummonCreature(creature->GetEntry(), pos, TEMPSUMMON_DEAD_DESPAWN, 60000);
    }
    if (!creature->IsDungeonBoss() && HasAffix(Affixes::Relentless))//Relentless ??
    {
        creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
        creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);
        creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
        creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
        creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
        creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
        creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
        creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
        creature->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
    }
}

void InstanceScript::CastChallengePlayerSpell(Player* player)
{
    CustomSpellValues values;

    // Affixes
    values.AddSpellMod(SPELLVALUE_BASE_POINT1, HasAffix(Affixes::Overflowing) ? 1 : 0);// 1 Overflowing ??
    values.AddSpellMod(SPELLVALUE_BASE_POINT2, (HasAffix(Affixes::Skittish) && player->isInTankSpec()) ? 1 : 0); // 2 Skittish ??
    values.AddSpellMod(SPELLVALUE_BASE_POINT3, HasAffix(Affixes::Grievous) ? 1 : 0);  // 12 Grievous ??

    player->CastCustomSpell(SPELL_CHALLENGER_BURDEN, values, player, TRIGGERED_FULL_MASK);
}

void InstanceScript::AddChallengeModeChests(ObjectGuid chestGuid, uint8 chestLevel)
{
    _challengeChestGuids[chestLevel] = chestGuid;
}

ObjectGuid InstanceScript::GetChellngeModeChests(uint8 chestLevel)
{
    return _challengeChestGuids[chestLevel];
}

void InstanceScript::AddChallengeModeDoor(ObjectGuid doorGuid)
{
    _challengeDoorGuids.push_back(doorGuid);
}

void InstanceScript::AddChallengeModeOrb(ObjectGuid orbGuid)
{
    _challengeOrbGuid = orbGuid;
}

void InstanceScript::ResetChallengeMode()
{
    //if (_challenge)
    //    _challenge->ResetGo(); ToDo

    instance->m_respawnChallenge = time(nullptr); // For respawn all mobs
    RepopPlayersAtGraveyard();
    instance->SetDifficultyID(DIFFICULTY_MYTHIC);
}

bool InstanceHasScript(WorldObject const* obj, char const* scriptName)
{
    if (InstanceMap* instance = obj->GetMap()->ToInstanceMap())
        return instance->GetScriptName() == scriptName;

    return false;
}

// Complete Achievement for all players in instance
void InstanceScript::DoCompletedAchievement(AchievementEntry const* entry)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->GetSource())
                player->CompletedAchievement(entry);
}

/// Create Conversation for all players in instance
void InstanceScript::DoConversation(uint32 conversationId)
{
    if (!conversationId)
    {
        TC_LOG_ERROR("scripts", "DoConversation called for not existing conversationId %u", conversationId);
        return;
    }

    Map::PlayerList const& playerList = instance->GetPlayers();
    if (!playerList.isEmpty())
        for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
            if (Player* player = i->GetSource())
                Conversation::CreateConversation(conversationId, player, player->GetPosition(), { player->GetGUID() });
}

void InstanceScript::DoDelayedConversation(uint32 delay, uint32 conversationId)
{
    if (!conversationId)
    {
        TC_LOG_ERROR("scripts", "DoDelayedConversation called for not existing conversationId %u", conversationId);
        return;
    }

    Map::PlayerList const& playerList = instance->GetPlayers();
    if (!playerList.isEmpty())
        for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
            if (Player* player = i->GetSource())
                player->AddDelayedConversation(delay, conversationId);
}

// Add item on all players in instance
void InstanceScript::DoAddItemOnPlayers(uint32 itemId, uint32 count)
{
    Map::PlayerList const& playerList = instance->GetPlayers();
    if (!playerList.isEmpty())
        for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
            if (Player* player = i->GetSource())
                player->AddItem(itemId, count);
}

// Remove item on all players in instance
void InstanceScript::DoDestroyItemCountOnPlayers(uint32 item, uint32 count)
{
    Map::PlayerList const& playerList = instance->GetPlayers();
    if (!playerList.isEmpty())
        for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
            if (Player* player = i->GetSource())
                player->DestroyItemCount(item, count, true);
}
// Add item by class on all players in instance
void InstanceScript::DoAddItemByClassOnPlayers(uint8 classId, uint32 itemId, uint32 count)
{
    Map::PlayerList const& playerList = instance->GetPlayers();
    if (!playerList.isEmpty())
        for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
            if (Player* player = i->GetSource())
                if(player->getClass()== classId)
                    player->AddItem(itemId, count);
}

// Remove item by class on all players in instance
void InstanceScript::DoDestroyItemCountByClassOnPlayers(uint8 classId, uint32 item, uint32 count)
{
    Map::PlayerList const& playerList = instance->GetPlayers();
    if (!playerList.isEmpty())
        for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
            if (Player* player = i->GetSource())
                if (player->getClass() == classId)
                    player->DestroyItemCount(item, count, true);
}

// Resurrect all players in instance
void InstanceScript::DoResurrectPlayers(float restore_percent)
{
    Map::PlayerList const& playerList = instance->GetPlayers();
    if (!playerList.isEmpty())
        for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
            if (Player* player = i->GetSource())
                player->ResurrectPlayer(restore_percent);
}

void InstanceScript::SpawnFontOfPower()
{
    if (_challengeModeFontOfPowerPosition.has_value() && instance->IsMythic())
        instance->SummonGameObject(GO_FONT_OF_POWER, *_challengeModeFontOfPowerPosition, QuaternionData(), WEEK);
    if (_challengeModeFontOfPowerPosition2.has_value() && instance->IsMythic())
        instance->SummonGameObject(GO_FONT_OF_POWER, *_challengeModeFontOfPowerPosition2, QuaternionData(), WEEK);
}
