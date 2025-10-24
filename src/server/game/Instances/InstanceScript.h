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

#ifndef INSTANCESCRIPT_H
#define INSTANCESCRIPT_H

#include "ZoneScript.h"
#include "Common.h"
#include "Optional.h"
#include "Position.h"
#include "DB2Structure.h"
#include <map>
#include <memory>
#include <set>
#include <bitset>  

#define OUT_SAVE_INST_DATA             TC_LOG_DEBUG("scripts", "Saving Instance Data for Instance %s (Map %d, Instance Id %d)", instance->GetMapName(), instance->GetId(), instance->GetInstanceId())
#define OUT_SAVE_INST_DATA_COMPLETE    TC_LOG_DEBUG("scripts", "Saving Instance Data for Instance %s (Map %d, Instance Id %d) completed.", instance->GetMapName(), instance->GetId(), instance->GetInstanceId())
#define OUT_LOAD_INST_DATA(a)          TC_LOG_DEBUG("scripts", "Loading Instance Data for Instance %s (Map %d, Instance Id %d). Input is '%s'", instance->GetMapName(), instance->GetId(), instance->GetInstanceId(), a)
#define OUT_LOAD_INST_DATA_COMPLETE    TC_LOG_DEBUG("scripts", "Instance Data Load for Instance %s (Map %d, Instance Id: %d) is complete.", instance->GetMapName(), instance->GetId(), instance->GetInstanceId())
#define OUT_LOAD_INST_DATA_FAIL        TC_LOG_ERROR("scripts", "Unable to load Instance Data for Instance %s (Map %d, Instance Id: %d).", instance->GetMapName(), instance->GetId(), instance->GetInstanceId())

class AreaBoundary;
class Creature;
class GameObject;
class InstanceMap;
class ModuleReference;
class Player;
class Unit;
struct Position;
enum CriteriaTypes : uint8;
enum CriteriaTimedTypes : uint8;
enum EncounterCreditType : uint8;
enum Affixes : uint32;

namespace WorldPackets
{
    namespace WorldState
    {
        class InitWorldStates;
    }
}

enum EncounterFrameType
{
    ENCOUNTER_FRAME_SET_COMBAT_RES_LIMIT    = 0,
    ENCOUNTER_FRAME_RESET_COMBAT_RES_LIMIT  = 1,
    ENCOUNTER_FRAME_ENGAGE                  = 2,
    ENCOUNTER_FRAME_DISENGAGE               = 3,
    ENCOUNTER_FRAME_UPDATE_PRIORITY         = 4,
    ENCOUNTER_FRAME_ADD_TIMER               = 5,
    ENCOUNTER_FRAME_ENABLE_OBJECTIVE        = 6,
    ENCOUNTER_FRAME_UPDATE_OBJECTIVE        = 7,
    ENCOUNTER_FRAME_DISABLE_OBJECTIVE       = 8,
    ENCOUNTER_FRAME_UNK7                    = 9,    // Seems to have something to do with sorting the encounter units
    ENCOUNTER_FRAME_ADD_COMBAT_RES_LIMIT    = 10
};

enum EncounterState
{
    NOT_STARTED   = 0,
    IN_PROGRESS   = 1,
    FAIL          = 2,
    DONE          = 3,
    SPECIAL       = 4,
    TO_BE_DECIDED = 5
};

enum BossTalk
{
    BOSS_TALK_ENTER_COMBAT  = 200,
    BOSS_TALK_KILL_PLAYER   = 201,
    BOSS_TALK_JUST_DIED     = 202,
};

enum DoorType
{
    DOOR_TYPE_ROOM          = 0,    // Door can open if encounter is not in progress
    DOOR_TYPE_PASSAGE       = 1,    // Door can open if encounter is done
    DOOR_TYPE_SPAWN_HOLE    = 2,    // Door can open if encounter is in progress, typically used for spawning places
    MAX_DOOR_TYPES
};

enum ChallengeMode
{
    GOB_CHALLENGER_DOOR     = 239408,
	GOB_CHALLENGER_DOOR_LINE235 = 239323,
    GO_FONT_OF_POWER        = 246779,

    SPELL_CHALLENGER_MIGHT  = 206150,
    SPELL_CHALLENGER_BURDEN = 206151,
    SPELL_BATTLE_HARDENED = 158082
};

enum ChallengeSpells : uint32
{
    ChallengersMight = 206150, /// generic creature aura
    ChallengersBurden = 206151, /// generic player aura
    ChallengerBolstering = 209859,
    ChallengerNecrotic = 209858,
    ChallengerOverflowing = 221772,
    ChallengerSanguine = 226489,
    ChallengerRaging = 228318,
    ChallengerSummonVolcanicPlume = 209861,
    ChallengerVolcanicPlume = 209862,
    ChallengerBursting = 240443,
    ChallengerQuake = 240447,

    //Explosive
    SPELL_FEL_EXPLOSIVES_SUMMON_1 = 240444, //Short dist
    SPELL_FEL_EXPLOSIVES_SUMMON_2 = 243110, //Long dist
    SPELL_FEL_EXPLOSIVES_VISUAL = 240445,
    SPELL_FEL_EXPLOSIVES_DMG = 240446,
};

enum ChallengeNpcs : uint32
{
    NpcVolcanicPlume = 105877,
    NPC_FEL_EXPLOSIVES = 120651,
};

static uint32 const ChallengeModeOrb = 246779;
static uint32 const ChallengeModeDoor = 239323;

struct DoorData
{
    uint32 entry, bossId;
    DoorType type;
};

struct BossBoundaryEntry
{
    uint32 BossId;
    AreaBoundary const* Boundary;
};

struct TC_GAME_API BossBoundaryData
{
    typedef std::vector<BossBoundaryEntry> StorageType;
    typedef StorageType::const_iterator const_iterator;

    BossBoundaryData(std::initializer_list<BossBoundaryEntry> data) : _data(data) { }
    ~BossBoundaryData();
    const_iterator begin() const { return _data.begin(); }
    const_iterator end() const { return _data.end(); }

    private:
        StorageType _data;
};

struct MinionData
{
    uint32 entry, bossId;
};

struct ObjectData
{
    uint32 entry;
    uint32 type;
};

typedef std::set<AreaBoundary const*> CreatureBoundary;

struct BossInfo
{
    BossInfo() : state(TO_BE_DECIDED) { }
    EncounterState state;
    GuidSet door[MAX_DOOR_TYPES];
    GuidSet minion;
    CreatureBoundary boundary;
};

struct DoorInfo
{
    explicit DoorInfo(BossInfo* _bossInfo, DoorType _type)
        : bossInfo(_bossInfo), type(_type) { }
    BossInfo* bossInfo;
    DoorType type;
};

struct MinionInfo
{
    explicit MinionInfo(BossInfo* _bossInfo) : bossInfo(_bossInfo) { }
    BossInfo* bossInfo;
};

typedef std::multimap<uint32 /*entry*/, DoorInfo> DoorInfoMap;
typedef std::pair<DoorInfoMap::const_iterator, DoorInfoMap::const_iterator> DoorInfoMapBounds;

typedef std::map<uint32 /*entry*/, MinionInfo> MinionInfoMap;
typedef std::map<uint32 /*type*/, ObjectGuid /*guid*/> ObjectGuidMap;
typedef std::map<uint32 /*entry*/, uint32 /*type*/> ObjectInfoMap;

class TC_GAME_API InstanceScript : public ZoneScript
{
    public:
        explicit InstanceScript(InstanceMap* map);

        virtual ~InstanceScript() { }

        InstanceMap* instance;

        // On creation, NOT load.
        // PLEASE INITIALIZE FIELDS IN THE CONSTRUCTOR INSTEAD !!!
        // KEEPING THIS METHOD ONLY FOR BACKWARD COMPATIBILITY !!!
        virtual void Initialize() { }

        // On load
        virtual void Load(char const* data);

        // When save is needed, this function generates the data
        virtual std::string GetSaveData();

        void SaveToDB();

        virtual void Update(uint32 /*diff*/) { }
        void UpdateOperations(uint32 const diff);
        void UpdateCombatResurrection(uint32 /*diff*/);

        // Used by the map's CannotEnter function.
        // This is to prevent players from entering during boss encounters.
        virtual bool IsEncounterInProgress() const;

        // Called when a creature/gameobject is added to map or removed from map.
        // Insert/Remove objectguid to dynamic guid store
        virtual void OnCreatureCreate(Creature* creature) override;
        virtual void OnCreatureRemove(Creature* creature) override;

        virtual void OnGameObjectCreate(GameObject* go) override;
        virtual void OnGameObjectRemove(GameObject* go) override;

        ObjectGuid GetObjectGuid(uint32 type) const;
        virtual ObjectGuid GetGuidData(uint32 type) const override;

        Creature* GetCreature(uint32 type);
        GameObject* GetGameObject(uint32 type);

        void OnPlayerEnter(Player*) override;
        void OnPlayerExit(Player*) override;
        void OnPlayerDeath(Player*) override;

        // Handle open / close objects
        // * use HandleGameObject(0, boolen, GO); in OnObjectCreate in instance scripts
        // * use HandleGameObject(GUID, boolen, NULL); in any other script
        void HandleGameObject(ObjectGuid guid, bool open, GameObject* go = nullptr);
		
		//DoAddItemOnPlayers
		void DoAddItemOnPlayers (uint32 entry, uint32 count);
		
		// Create Conversation for all players in instance
        void DoConversation(uint32 conversationId);

        void DoDelayedConversation(uint32 delay, uint32 conversationId);
		
		// Resurrect all players in instance
        void DoResurrectPlayers(float restore_percent);
		
		// Remove item on all players in instance
        void DoDestroyItemCountOnPlayers(uint32 item, uint32 count);
		
		// Add item by class on all players in instance
        void DoAddItemByClassOnPlayers(uint8 classId, uint32 itemId, uint32 count);

        // Remove item by class on all players in instance
        void DoDestroyItemCountByClassOnPlayers(uint8 classId, uint32 item, uint32 count);

        // Change active state of doors or buttons
        void DoUseDoorOrButton(ObjectGuid guid, uint32 withRestoreTime = 0, bool useAlternativeState = false);
        void DoCloseDoorOrButton(ObjectGuid guid);

        // Respawns a GO having negative spawntimesecs in gameobject-table
        void DoRespawnGameObject(ObjectGuid guid, uint32 timeToDespawn = MINUTE);

        // Sends world state update to all players in instance
        void DoUpdateWorldState(uint32 worldstateId, uint32 worldstateValue);

        // Send Notify to all players in instance
        void DoSendNotifyToInstance(char const* format, ...);

        // Update Achievement Criteria for all players in instance
        void DoUpdateCriteria(CriteriaTypes type, uint32 miscValue1 = 0, uint32 miscValue2 = 0, Unit* unit = NULL);

        // Start/Stop Timed Achievement Criteria for all players in instance
        void DoStartCriteriaTimer(CriteriaTimedTypes type, uint32 entry);
        void DoStopCriteriaTimer(CriteriaTimedTypes type, uint32 entry);

        // Remove Auras due to Spell on all players in instance
        void DoRemoveAurasDueToSpellOnPlayers(uint32 spell);

        // Kill all players with this aura in the instance
        void DoKillPlayersWithAura(uint32 spell);

        // Dampening creatures with specific aura
        void DoDampeningForCreatures(Creature* creature);
        void DoRemoveDampeningFromCreatures();

        // Cast spell on all players in instance
        void DoCastSpellOnPlayers(uint32 spell, Unit* caster = nullptr, bool triggered = true);

        // Play scene by Id on all players in instance
        void DoPlaySceneOnPlayers(uint32 sceneId);

        // Play scene by packageId on all players in instance
        void DoPlayScenePackageIdOnPlayers(uint32 scenePackageId);

        // Remove all movement forces related to forceGuid
        void DoRemoveForcedMovementsOnPlayers(ObjectGuid forceGuid);

        void DoSetAlternatePowerOnPlayers(int32 value);

        void DoModifyPlayerCurrencies(uint32 id, int32 value);

        // Teleport and resurrect all players in the instance at the nearest graveyard
        void RepopPlayersAtGraveyard();

        void DoNearTeleportPlayers(const Position pos, bool casting = false);
		
		void DoTeleportPlayers(uint32 mapId, const Position pos);

        void DoKilledMonsterKredit(uint32 questId, uint32 entry, ObjectGuid guid = ObjectGuid::Empty);

        // Complete Achievement for all players in instance
        void DoCompleteAchievement(uint32 achievement);

        // Update Achievement Criteria for all players in instance
        void DoUpdateAchievementCriteria(CriteriaTypes type, uint32 miscValue1 = 0, uint32 miscValue2 = 0, Unit* unit = nullptr);

        // Complete Achievement for all players in instance
        void DoCompletedAchievement(AchievementEntry const* entry);
		
	    // Send Event For Scenario
        void DoSendEventScenario(uint32 eventId /*= 0*/);
		
		// Add aura on all players in instance
        void DoAddAuraOnPlayers(uint32 spell);

        /// Do combat stop on all players in instance
        void DoCombatStopOnPlayers();

        // Start movie for all players in instance
        void DoStartMovie(uint32 movieId);
		
		// Add phase on all players in instance
        void DoAddPhaseOnPlayers(uint32 phase);

        // Remove phase on all players in instance
        void DoRemovePhaseOnPlayers(uint32 phase);

        // Return wether server allow two side groups or not
        bool ServerAllowsTwoSideGroups();

        virtual bool SetBossState(uint32 id, EncounterState state);
        EncounterState GetBossState(uint32 id) const { return id < bosses.size() ? bosses[id].state : TO_BE_DECIDED; }
        static std::string GetBossStateName(uint8 state);
        CreatureBoundary const* GetBossBoundary(uint32 id) const { return id < bosses.size() ? &bosses[id].boundary : NULL; }

        // Achievement criteria additional requirements check
        // NOTE: not use this if same can be checked existed requirement types from AchievementCriteriaRequirementType
        virtual bool CheckAchievementCriteriaMeet(uint32 /*criteria_id*/, Player const* /*source*/, Unit const* /*target*/ = NULL, uint32 /*miscvalue1*/ = 0);

        // Checks boss requirements (one boss required to kill other)
        virtual bool CheckRequiredBosses(uint32 /*bossId*/, Player const* /*player*/ = nullptr) const { return true; }

        // Checks encounter state at kill/spellcast
        void UpdateEncounterStateForKilledCreature(uint32 creatureId, Unit* source);
        void UpdateEncounterStateForSpellCast(uint32 spellId, Unit* source);

        // Used only during loading
        void SetCompletedEncountersMask(uint32 newMask) { completedEncounters = newMask; }

        // Returns completed encounters mask for packets
        uint32 GetCompletedEncounterMask() const { return completedEncounters; }

        // Sets the entrance location (WorldSafeLoc) id
        void SetEntranceLocation(uint32 worldSafeLocationId);

        // Sets a temporary entrance that does not get saved to db
        void SetTemporaryEntranceLocation(uint32 worldSafeLocationId) { _temporaryEntranceId = worldSafeLocationId; }
		
		// Get's the current entrance id
        uint32 GetEntranceLocation() const { uint32 locationId = _temporaryEntranceId ? _temporaryEntranceId : _entranceId; OnGetEntranceLocation(locationId); return locationId; }

        virtual void OnGetEntranceLocation(uint32& /*worldSafeLocationId*/) const { }

        /// Add timed delayed operation
        /// @p_Timeout  : Delay time
        /// @p_Function : Callback function
        void AddTimedDelayedOperation(uint32 timeout, std::function<void()> && function)
        {
            emptyWarned = false;
            timedDelayedOperations.push_back(std::pair<uint32, std::function<void()>>(timeout, function));
        }

        /// Called after last delayed operation was deleted
        /// Do whatever you want
        virtual void LastOperationCalled() { }

        std::vector<std::pair<int32, std::function<void()>>>    timedDelayedOperations;   ///< Delayed operations
        bool                                                    emptyWarned;              ///< Warning when there are no more delayed operations

        void SendEncounterUnit(uint32 type, Unit* unit = NULL, uint8 priority = 0);
        void SendEncounterStart(uint32 inCombatResCount = 0, uint32 maxInCombatResCount = 0, uint32 inCombatResChargeRecovery = 0, uint32 nextCombatResChargeTime = 0);
        void SendEncounterEnd();

        void SendBossKillCredit(uint32 encounterId);
		void SendCompleteDungeonEncounter(uint32 encounterId);

        virtual void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& /*packet*/) { }
		
		bool IsScenarioComplete()  { return m_IsScenarioComplete; }
        bool m_IsScenarioComplete;
        void CompleteScenario();
        void CompleteCurrStep();
        void RespawnCreature(uint64 p_Guid);

        struct CriteriaProgressData
        {
            CriteriaProgressData(uint32 p_ID, uint64 p_Quantity, uint64 p_Guid, uint32 p_Date, uint32 p_StartTime, uint8 p_Flags)
            {
                m_ID = p_ID;
                m_Quantity = p_Quantity;
                m_Guid = p_Guid;
                m_Date = p_Date;
                m_TimeFromStart = p_StartTime;
                m_TimeFromCreate = p_StartTime;
                m_Flags = p_Flags;
            }

            CriteriaProgressData()
            {
                m_ID = 0;
                m_Quantity = 0;
                m_Guid = 0;
                m_Date = 0;
                m_TimeFromStart = 0;
                m_TimeFromCreate = 0;
                m_Flags = 0;
            }

            uint32 m_ID;
            uint64 m_Quantity;
            uint64 m_Guid;
            uint32 m_Date;
            uint32 m_TimeFromStart;
            uint32 m_TimeFromCreate;
            uint8  m_Flags;
        };

        struct BonusObjectiveData
        {
            BonusObjectiveData(uint32 p_ID, bool p_Complete)
            {
                m_ObjectiveID = p_ID;
                m_ObjectiveComplete = p_Complete;
            }

            BonusObjectiveData()
            {
                m_ObjectiveID = 0;
                m_ObjectiveComplete = false;
            }

            uint32 m_ObjectiveID;
            bool m_ObjectiveComplete;
        };

        struct ScenarioData
        {
            ScenarioData(uint32 p_ScenarioID, uint32 p_StepID, uint32 p_CurrWave, uint32 p_MaxWave, uint32 p_Timer, uint32 p_CriteriaCount,
                uint32 p_BonusCount, bool p_Complete)
            {
                m_ScenarioID = p_ScenarioID;
                m_StepID = p_StepID;
                m_WaveCurrent = p_CurrWave;
                m_WaveMax = p_MaxWave;
                m_TimerDuration = p_Timer;
                m_CriteriaCount = p_CriteriaCount;
                m_BonusCount = p_BonusCount;
                m_ScenarioComplete = p_Complete;

                m_CriteriaProgress.resize(m_CriteriaCount);
                m_BonusObjectives.resize(m_BonusCount);
            }

            ScenarioData(uint32 p_ScenarioID, uint32 p_StepID)
            {
                m_ScenarioID = p_ScenarioID;
                m_StepID = p_StepID;
                m_WaveCurrent = 0;
                m_WaveMax = 0;
                m_TimerDuration = 0;
                m_CriteriaCount = 0;
                m_BonusCount = 0;
                m_ScenarioComplete = false;

                m_CriteriaProgress.clear();
                m_BonusObjectives.clear();
            }

            ScenarioData()
            {
                m_ScenarioID = 0;
                m_StepID = 0;
                m_WaveCurrent = 0;
                m_WaveMax = 0;
                m_TimerDuration = 0;
                m_CriteriaCount = 0;
                m_BonusCount = 0;
                m_ScenarioComplete = false;

                m_CriteriaProgress.clear();
                m_BonusObjectives.clear();
            }

            void AddBonusObjective(BonusObjectiveData p_Data)
            {
                m_BonusObjectives.push_back(p_Data);
            }

            void AddCriteriaProgress(CriteriaProgressData p_Data)
            {
                m_CriteriaProgress.push_back(p_Data);
            }

            uint32 m_ScenarioID;
            uint32 m_StepID;
            uint32 m_WaveCurrent;
            uint32 m_WaveMax;
            uint32 m_TimerDuration;
            uint32 m_CriteriaCount;
            uint32 m_BonusCount;
            bool m_ScenarioComplete;

            std::vector<CriteriaProgressData> m_CriteriaProgress;
            std::vector<BonusObjectiveData> m_BonusObjectives;
        };




		void GetScenarioData(Player* p_Player = nullptr);
        void SendScenarioState(ScenarioData p_Data, Player* p_Player = nullptr);
        void SendScenarioProgressUpdate(CriteriaProgressData p_Data, Player* p_Player = nullptr);
        void BuildCriteriaProgressPacket(WorldPacket* p_Data, CriteriaProgressData p_CriteriaProgress);


		///
		bool   m_ChallengeStarted;
        bool   m_ConditionCompleted;
        uint32 m_CreatureKilled;
        uint32 m_StartChallengeTime;
        std::vector<ObjectGuid> m_ChallengeDoorGuids;
        uint64 m_ChallengeOrbGuid;
        uint32 m_ChallengeTime;
        uint8  m_MedalType;
        uint64 m_InstanceGuid;
        uint32 m_BeginningTime;
        uint32 m_ScenarioID;
        uint8  m_ScenarioStep;
        uint32 m_LastResetTime;
        uint8  m_ChallengeLevel;
        uint8  m_DeathCount;

        // Check if all players are dead (except gamemasters)
        bool IsWipe() const;

        // ReCheck PhaseTemplate related conditions
        void UpdatePhasing();

        // Dampening condition
        void SetDampening(bool force = true) { hasDampening = force; }
        bool GetDampening() { return hasDampening; }
        void SetDampeningSpell(uint32 spellId) { dampeningSpellId = spellId; }
        uint32 GetDampeningSpell() { return dampeningSpellId; }

        uint32 GetEncounterCount() const { return uint32(bosses.size()); }

        void InitializeCombatResurrections(uint8 charges = 1, uint32 interval = 0);
        void AddCombatResurrectionCharge();
        void UseCombatResurrection();
        void ResetCombatResurrections();
        uint8 GetCombatResurrectionCharges() const { return _combatResurrectionCharges; }
        uint32 GetCombatResurrectionChargeInterval() const;

        // Challenge Modes
        void StartChallengeMode(uint8 modeid, uint8 level, uint8 affix1, uint8 affix2, uint8 affix3);
        void CompleteChallengeMode();

        bool IsChallengeModeStarted() const { return _challengeModeStarted; }
        uint8 GetChallengeModeId() const { return _challengeModeId; }
        uint8 GetChallengeModeLevel() const { return _challengeModeLevel; }
        std::array<uint32, 3> GetAffixes() const;
        bool HasAffix(Affixes affix);
        uint32 GetChallengeModeCurrentDuration() const;

        void SendChallengeModeStart(Player* player = nullptr) const;
        void SendChallengeModeDeathCount(Player* player = nullptr) const;
        void SendChallengeModeElapsedTimer(Player* player = nullptr) const;
        void SendChallengeModeMapStatsUpdate(Player* player, uint32 challengeId, uint32 recordTime) const;

        void CastChallengeCreatureSpell(Creature* creature);
        void CastChallengePlayerSpell(Player* player);

        void SetChallengeDoorPos(Position pos) { _challengeModeDoorPosition = pos; }
        virtual void SpawnChallengeModeRewardChest() { }

        void AddChallengeModeChests(ObjectGuid chestGuid, uint8 chestLevel);
        ObjectGuid GetChellngeModeChests(uint8 chestLevel);
        void AddChallengeModeDoor(ObjectGuid doorGuid);
        void AddChallengeModeOrb(ObjectGuid orbGuid);

        void ResetChallengeMode();

        std::vector<ObjectGuid> _challengeDoorGuids;
        std::vector<ObjectGuid> _challengeChestGuids;
        ObjectGuid _challengeOrbGuid;
        ObjectGuid _challengeChest;
		
		void SetFontOfPowerPos(Position pos) { _challengeModeFontOfPowerPosition = pos; }
        void SetFontOfPowerPos2(Position pos) { _challengeModeFontOfPowerPosition2 = pos; }
        void SpawnFontOfPower();
		
		virtual void ShowChallengeDoor() { }
        virtual void HideChallengeDoor() { }

        void DoOnPlayers(std::function<void(Player*)>&& function);

    protected:
        void SetHeaders(std::string const& dataHeaders);
        void SetBossNumber(uint32 number) { bosses.resize(number); }
        void LoadBossBoundaries(BossBoundaryData const& data);
        void LoadDoorData(DoorData const* data);
        void LoadMinionData(MinionData const* data);
        void LoadObjectData(ObjectData const* creatureData, ObjectData const* gameObjectData);

        void AddObject(Creature* obj, bool add);
        void AddObject(GameObject* obj, bool add);
        void AddObject(WorldObject* obj, uint32 type, bool add);

        virtual void AddDoor(GameObject* door, bool add);
        void AddMinion(Creature* minion, bool add);

        virtual void UpdateDoorState(GameObject* door);
        void UpdateMinionState(Creature* minion, EncounterState state);

        // Exposes private data that should never be modified unless exceptional cases.
        // Pay very much attention at how the returned BossInfo data is modified to avoid issues.
        BossInfo* GetBossInfo(uint32 id);

        // Instance Load and Save
        bool ReadSaveDataHeaders(std::istringstream& data);
        void ReadSaveDataBossStates(std::istringstream& data);
        virtual void ReadSaveDataMore(std::istringstream& /*data*/) { }
        void WriteSaveDataHeaders(std::ostringstream& data);
        void WriteSaveDataBossStates(std::ostringstream& data);
        virtual void WriteSaveDataMore(std::ostringstream& /*data*/) { }

        bool _SkipCheckRequiredBosses(Player const* player = nullptr) const;

    private:
        static void LoadObjectData(ObjectData const* creatureData, ObjectInfoMap& objectInfo);
        void UpdateEncounterState(EncounterCreditType type, uint32 creditEntry, Unit* source);

        std::vector<char> headers;
        std::vector<BossInfo> bosses;
        std::vector<ObjectGuid> dampenedGUIDs;
        DoorInfoMap doors;
        MinionInfoMap minions;
        ObjectInfoMap _creatureInfo;
        ObjectInfoMap _gameObjectInfo;
        ObjectGuidMap _objectGuids;
        uint32 completedEncounters; // completed encounter mask, bit indexes are DungeonEncounter.dbc boss numbers, used for packets
        uint32 _entranceId;
        uint32 _temporaryEntranceId;
        uint32 _combatResurrectionTimer;
        uint8 _combatResurrectionCharges; // the counter for available battle resurrections
        bool _combatResurrectionTimerStarted;

        bool _challengeModeStarted;
        uint8 _challengeModeId;
        uint8 _challengeModeLevel;
        uint32 _challengeModeStartTime;
        uint32 _challengeModeDeathCount;
        Optional<Position> _challengeModeDoorPosition;
        Optional<Position> _challengeModeFontOfPowerPosition;
        Optional<Position> _challengeModeFontOfPowerPosition2;
        std::array<uint32, 3> _affixes;
        std::bitset<size_t(121)> _affixesTest;//Affixes::MaxAffixes

        uint32 dampeningSpellId;
        bool hasDampening;

    #ifdef TRINITY_API_USE_DYNAMIC_LINKING
        // Strong reference to the associated script module
        std::shared_ptr<ModuleReference> module_reference;
    #endif // #ifndef TRINITY_API_USE_DYNAMIC_LINKING
};

#endif // TRINITY_INSTANCE_DATA_H
