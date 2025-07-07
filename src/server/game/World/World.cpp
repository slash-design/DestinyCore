/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

/** \file
    \ingroup world
*/

#include "Common.h"
#include "Memory.h"
#include "DatabaseEnv.h"
#include "Config.h"
#include "GitRevision.h"
#include "Log.h"
#include "Opcodes.h"
#include "WorldSession.h"
#include "WorldPacket.h"
#include "Player.h"
#include "Vehicle.h"
#include "SkillExtraItems.h"
#include "SkillDiscovery.h"
#include "World.h"
#include "AccountMgr.h"
#include "AchievementMgr.h"
#include "AuctionHouseMgr.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "GuildFinderMgr.h"
#include "TicketMgr.h"
#include "SpellMgr.h"
#include "GroupMgr.h"
#include "WordFilterMgr.h"
#include "Chat.h"
#include "DBCStores.h"
#include "DB2Stores.h"
#include "LootMgr.h"
#include "ItemEnchantmentMgr.h"
#include "MapManager.h"
#include "CreatureAIRegistry.h"
#include "BattlegroundMgr.h"
#include "OutdoorPvPMgr.h"
#include "TemporarySummon.h"
#include "WaypointMovementGenerator.h"
#include "VMapFactory.h"
#include "MMapFactory.h"
#include "VMapManager2.h"
#include "GameEventMgr.h"
#include "PoolMgr.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "InstanceSaveMgr.h"
#include "Util.h"
#include "Language.h"
#include "CreatureGroups.h"
#include "Transport.h"
#include "ScriptMgr.h"
#include "AddonMgr.h"
#include "LFGMgr.h"
#include "ConditionMgr.h"
#include "DisableMgr.h"
#include "CharacterDatabaseCleaner.h"
#include "ScriptMgr.h"
#include "WeatherMgr.h"
#include "CreatureTextMgr.h"
#include "SmartAI.h"
#include "Channel.h"
#include "WardenMgr.h"
#include "Warden.h"
#include "CalendarMgr.h"
#include "BattlefieldMgr.h"
#include "BracketMgr.h"
#include "PlayerDump.h"
#include "ChallengeMgr.h"
#include "ScenarioMgr.h"
#include "ThreadPoolMgr.hpp"

ACE_Atomic_Op<ACE_Thread_Mutex, bool> World::m_stopEvent = false;
uint8 World::m_ExitCode = SHUTDOWN_EXIT_CODE;
volatile uint32 World::m_worldLoopCounter = 0;

float World::m_MaxVisibleDistanceOnContinents = DEFAULT_VISIBILITY_DISTANCE;
float World::m_MaxVisibleDistanceInInstances = DEFAULT_VISIBILITY_DISTANCE;
float World::m_MaxVisibleDistanceInBG   = DEFAULT_VISIBILITY_BGARENAS;
float World::m_MaxVisibleDistanceInArenas   = DEFAULT_VISIBILITY_BGARENAS;

int32 World::m_visibility_notify_periodOnContinents = DEFAULT_VISIBILITY_NOTIFY_PERIOD;
int32 World::m_visibility_notify_periodInInstances  = DEFAULT_VISIBILITY_NOTIFY_PERIOD;
int32 World::m_visibility_notify_periodInBGArenas   = DEFAULT_VISIBILITY_NOTIFY_PERIOD;
int32 World::m_visibilityAINotifyDelay   = DEFAULT_VISIBILITY_NOTIFY_PERIOD;

float World::m_visibilityRelocationLowerLimit = 20.0f;
float World::ZoneUpdateDistanceRangeLimit = 5.0f;

// movement anticheat
bool World::m_EnableMvAnticheat = true;
bool World::m_EnableMvAnticheatDebug = false;
uint32 World::m_TeleportToPlaneAlarms = 50;
uint32 World::m_MistimingAlarms = 400;
uint32 World::m_MistimingDelta = 25000;

/// World constructor
World::World()
{
    m_playerLimit = 0;
    m_allowedSecurityLevel = SEC_PLAYER;
    m_allowMovement = true;
    m_ShutdownMask = 0;
    m_ShutdownTimer = 0;
    m_gameTime = time(NULL);
    m_startTime = m_gameTime;
    m_maxActiveSessionCount = 0;
    m_maxQueuedSessionCount = 0;
    m_PlayerCount = 0;
    m_MaxPlayerCount = 0;
    m_NextDailyQuestReset = 0;
    m_NextWeeklyQuestReset = 0;
    m_NextCurrencyReset = 0;
    m_NextInstanceDailyReset = 0;
    m_NextInstanceHalfWeekReset = 0;
    m_NextInstanceWeeklyReset = 0;

    m_defaultDbcLocale = LOCALE_enUS;
    m_availableDbcLocaleMask = 0;

    m_updateTimeSum = 0;
    m_updateTimeCount = 0;

//     for (uint8 i = 0; i < 10; i++)
//         m_averageLongUpdateTime[i] = 0;

    m_zoneDiffTimer = 0;

    m_isClosed = false;

    m_CleaningFlags = 0;
    m_CountQueueOnDM = 0;
}

/// World destructor
World::~World()
{
    ///- Empty the kicked session set
    while (!m_sessions.empty())
    {
        // not remove from queue, prevent loading new sessions
        delete m_sessions.begin()->second;
        m_sessions.erase(m_sessions.begin());
    }

    CliCommandHolder* command = NULL;
    while (cliCmdQueue.next(command))
        delete command;

    VMAP::VMapFactory::clear();
    MMAP::MMapFactory::clear();

    //TODO free addSessQueue
}

/// Find a player in a specified zone
Player* World::FindPlayerInZone(uint32 zone)
{
    ///- circle through active sessions and return the first player found in the zone
    SessionMap::const_iterator itr;
    for (itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    {
        if (!itr->second)
            continue;

        Player* player = itr->second->GetPlayer();
        if (!player)
            continue;

        if (player->IsInWorld() && player->getCurrentUpdateZoneID() == zone)
        {
            // Used by the weather system. We return the player to broadcast the change weather message to him and all players in the zone.
            return player;
        }
    }
    return NULL;
}

bool World::IsClosed() const
{
    return m_isClosed;
}

void World::SetClosed(bool val)
{
    m_isClosed = val;

    // Invert the value, for simplicity for scripters.
    sScriptMgr->OnOpenStateChange(!val);
}

void World::SetMotd(const std::string& motd)
{
    m_motd = motd;

    sScriptMgr->OnMotdChange(m_motd);
}

const char* World::GetMotd() const
{
    return m_motd.c_str();
}

/// Find a session by its id
WorldSession* World::FindSession(uint32 id) const
{
    SessionMap::const_iterator itr = m_sessions.find(id);

    if (itr != m_sessions.end())
        return itr->second;                                 // also can return NULL for kicked session
    else
        return NULL;
}

/// Remove a given session
bool World::RemoveSession(uint32 id)
{
    ///- Find the session, kick the user, but we can't delete session at this moment to prevent iterator invalidation
    SessionMap::const_iterator itr = m_sessions.find(id);

    if (itr != m_sessions.end() && itr->second)
    {
        if (itr->second->PlayerLoading())
            return false;

        itr->second->KickPlayer();
    }

    return true;
}

void World::AddSession(WorldSession* s)
{
    addSessQueue.add(s);
}

void World::AddSession_(WorldSession* s)
{
    ASSERT (s);

    //NOTE - Still there is race condition in WorldSession* being used in the Sockets

    ///- kick already loaded player with same account (if any) and remove session
    ///- if player is in loading and want to load again, return
    if (!RemoveSession (s->GetAccountId()))
    {
        s->KickPlayer();
        delete s;                                           // session not added yet in session list, so not listed in queue
        return;
    }

    // decrease session counts only at not reconnection case
    bool decrease_session = true;

    // if session already exist, prepare to it deleting at next world update
    // NOTE - KickPlayer() should be called on "old" in RemoveSession()
    {
        SessionMap::const_iterator old = m_sessions.find(s->GetAccountId());

        if (old != m_sessions.end())
        {
            // prevent decrease sessions count if session queued
            if (RemoveQueuedPlayer(old->second))
                decrease_session = false;
            // not remove replaced session form queue if listed
            delete old->second;
        }
    }

    m_sessions[s->GetAccountId()] = s;

    uint32 Sessions = GetActiveAndQueuedSessionCount();
    uint32 pLimit = GetPlayerAmountLimit();
    uint32 QueueSize = GetQueuedSessionCount();             //number of players in the queue

    //so we don't count the user trying to
    //login as a session and queue the socket that we are using
    if (decrease_session)
        --Sessions;

    if (pLimit > 0 && Sessions >= pLimit && AccountMgr::IsPlayerAccount(s->GetSecurity()) && !HasRecentlyDisconnected(s))
    {
        AddQueuedPlayer (s);
        UpdateMaxSessionCounters();
        TC_LOG_DEBUG("server", "PlayerQueue: Account id %u is in Queue Position (%u).", s->GetAccountId(), ++QueueSize);
        return;
    }

    s->SendAuthResponse(AUTH_OK);
    //s->SendAddonsInfo();
    s->SendClientCacheVersion(sWorld->getIntConfig(CONFIG_CLIENTCACHE_VERSION));
    s->SendBattlePay();
    s->SendTutorialsData();
    //s->SendDisplayPromo(0);
    s->SendTimezoneInformation();

    UpdateMaxSessionCounters();

    // Updates the population
    if (pLimit > 0)
    {
        float popu = (float)GetActiveSessionCount();              // updated number of users on the server
        popu /= pLimit;
        popu *= 2;
        TC_LOG_DEBUG("server", "Server Population (%f).", popu);
    }
}

bool World::HasRecentlyDisconnected(WorldSession* session)
{
    if (!session)
        return false;

    if (uint32 tolerance = getIntConfig(CONFIG_INTERVAL_DISCONNECT_TOLERANCE))
    {
        for (DisconnectMap::iterator i = m_disconnects.begin(); i != m_disconnects.end();)
        {
            if (difftime(i->second, time(NULL)) < tolerance)
            {
                if (i->first == session->GetAccountId())
                    return true;
                ++i;
            }
            else
                m_disconnects.erase(i++);
        }
    }
    return false;
 }

int32 World::GetQueuePos(WorldSession* sess)
{
    uint32 position = 1;

    for (Queue::const_iterator iter = m_QueuedPlayer.begin(); iter != m_QueuedPlayer.end(); ++iter, ++position)
        if ((*iter) == sess)
            return position;

    return 0;
}

void World::AddQueuedPlayer(WorldSession* sess)
{
    sess->SetInQueue(true);
    m_QueuedPlayer.push_back(sess);

    // The 1st SMSG_AUTH_RESPONSE needs to contain other info too.
    sess->SendAuthResponse(AUTH_OK, true, GetQueuePos(sess));
}

bool World::RemoveQueuedPlayer(WorldSession* sess)
{
    // sessions count including queued to remove (if removed_session set)
    uint32 sessions = GetActiveSessionCount();

    uint32 position = 1;
    Queue::iterator iter = m_QueuedPlayer.begin();

    // search to remove and count skipped positions
    bool found = false;

    for (; iter != m_QueuedPlayer.end(); ++iter, ++position)
    {
        if (*iter == sess)
        {
            sess->SetInQueue(false);
            sess->ResetTimeOutTime();
            iter = m_QueuedPlayer.erase(iter);
            found = true;                                   // removing queued session
            break;
        }
    }

    // iter point to next socked after removed or end()
    // position store position of removed socket and then new position next socket after removed

    // if session not queued then we need decrease sessions count
    if (!found && sessions)
        --sessions;

    // accept first in queue
    if ((!m_playerLimit || sessions < m_playerLimit) && !m_QueuedPlayer.empty())
    {
        WorldSession* pop_sess = m_QueuedPlayer.front();
        pop_sess->SetInQueue(false);
        pop_sess->ResetTimeOutTime();
        pop_sess->SendAuthWaitQue(0);
        pop_sess->SendAddonsInfo();

        pop_sess->SendClientCacheVersion(sWorld->getIntConfig(CONFIG_CLIENTCACHE_VERSION));
        pop_sess->SendAccountDataTimes(GLOBAL_CACHE_MASK);
        pop_sess->SendTutorialsData();

        m_QueuedPlayer.pop_front();

        // update iter to point first queued socket or end() if queue is empty now
        iter = m_QueuedPlayer.begin();
        position = 1;
    }

    // update position from iter to end()
    // iter point to first not updated socket, position store new position
    for (; iter != m_QueuedPlayer.end(); ++iter, ++position)
        (*iter)->SendAuthWaitQue(position);

    return found;
}

/// Initialize config values
void World::LoadConfigSettings(bool reload)
{
    if (reload)
    {
        if (!ConfigMgr::Load())
        {
            TC_LOG_ERROR("server", "World settings reload fail: can't read settings from %s.", ConfigMgr::GetFilename().c_str());
            return;
        }
        sLog->LoadFromConfig();
    }

    ///- Read the player limit and the Message of the day from the config file
    SetPlayerAmountLimit(ConfigMgr::GetIntDefault("PlayerLimit", 100));
    SetMotd(ConfigMgr::GetStringDefault("Motd", "Welcome to a Trinity Core Server."));

    ///- Read ticket system setting from the config file
    m_bool_configs[CONFIG_ALLOW_TICKETS] = ConfigMgr::GetBoolDefault("AllowTickets", true);

    ///- Get string for new logins (newly created characters)
    SetNewCharString(ConfigMgr::GetStringDefault("PlayerStart.String", ""));

    ///- Send server info on login?
    m_int_configs[CONFIG_ENABLE_SINFO_LOGIN] = ConfigMgr::GetIntDefault("Server.LoginInfo", 0);

    ///- Read all rates from the config file
    rate_values[RATE_HEALTH]      = ConfigMgr::GetFloatDefault("Rate.Health", 1);
    if (rate_values[RATE_HEALTH] < 0)
    {
        TC_LOG_ERROR("server", "Rate.Health (%f) must be > 0. Using 1 instead.", rate_values[RATE_HEALTH]);
        rate_values[RATE_HEALTH] = 1;
    }
    rate_values[RATE_POWER_MANA]  = ConfigMgr::GetFloatDefault("Rate.Mana", 1);
    if (rate_values[RATE_POWER_MANA] < 0)
    {
        TC_LOG_ERROR("server", "Rate.Mana (%f) must be > 0. Using 1 instead.", rate_values[RATE_POWER_MANA]);
        rate_values[RATE_POWER_MANA] = 1;
    }
    rate_values[RATE_POWER_RAGE_INCOME] = ConfigMgr::GetFloatDefault("Rate.Rage.Income", 1);
    rate_values[RATE_POWER_RAGE_LOSS]   = ConfigMgr::GetFloatDefault("Rate.Rage.Loss", 1);
    if (rate_values[RATE_POWER_RAGE_LOSS] < 0)
    {
        TC_LOG_ERROR("server", "Rate.Rage.Loss (%f) must be > 0. Using 1 instead.", rate_values[RATE_POWER_RAGE_LOSS]);
        rate_values[RATE_POWER_RAGE_LOSS] = 1;
    }
    rate_values[RATE_POWER_RUNICPOWER_INCOME] = ConfigMgr::GetFloatDefault("Rate.RunicPower.Income", 1);
    rate_values[RATE_POWER_RUNICPOWER_LOSS]   = ConfigMgr::GetFloatDefault("Rate.RunicPower.Loss", 1);
    if (rate_values[RATE_POWER_RUNICPOWER_LOSS] < 0)
    {
        TC_LOG_ERROR("server", "Rate.RunicPower.Loss (%f) must be > 0. Using 1 instead.", rate_values[RATE_POWER_RUNICPOWER_LOSS]);
        rate_values[RATE_POWER_RUNICPOWER_LOSS] = 1;
    }
    rate_values[RATE_POWER_FOCUS]  = ConfigMgr::GetFloatDefault("Rate.Focus", 1.0f);
    rate_values[RATE_POWER_ENERGY] = ConfigMgr::GetFloatDefault("Rate.Energy", 1.0f);

    rate_values[RATE_SKILL_DISCOVERY]      = ConfigMgr::GetFloatDefault("Rate.Skill.Discovery", 1.0f);

    rate_values[RATE_DROP_ITEM_POOR]       = ConfigMgr::GetFloatDefault("Rate.Drop.Item.Poor", 1.0f);
    rate_values[RATE_DROP_ITEM_NORMAL]     = ConfigMgr::GetFloatDefault("Rate.Drop.Item.Normal", 1.0f);
    rate_values[RATE_DROP_ITEM_UNCOMMON]   = ConfigMgr::GetFloatDefault("Rate.Drop.Item.Uncommon", 1.0f);
    rate_values[RATE_DROP_ITEM_RARE]       = ConfigMgr::GetFloatDefault("Rate.Drop.Item.Rare", 1.0f);
    rate_values[RATE_DROP_ITEM_EPIC]       = ConfigMgr::GetFloatDefault("Rate.Drop.Item.Epic", 1.0f);
    rate_values[RATE_DROP_ITEM_LEGENDARY]  = ConfigMgr::GetFloatDefault("Rate.Drop.Item.Legendary", 1.0f);
    rate_values[RATE_DROP_ITEM_ARTIFACT]   = ConfigMgr::GetFloatDefault("Rate.Drop.Item.Artifact", 1.0f);
    rate_values[RATE_DROP_ITEM_REFERENCED] = ConfigMgr::GetFloatDefault("Rate.Drop.Item.Referenced", 1.0f);
    rate_values[RATE_DROP_ITEM_REFERENCED_AMOUNT] = ConfigMgr::GetFloatDefault("Rate.Drop.Item.ReferencedAmount", 1.0f);
    rate_values[RATE_DROP_CURRENCY]        =  ConfigMgr::GetFloatDefault("Rate.Drop.Currency", 1.0f);
    rate_values[RATE_DROP_CURRENCY_AMOUNT] =  ConfigMgr::GetFloatDefault("Rate.Drop.Currency.Amount", 1.0f);
    rate_values[RATE_DROP_MONEY]    = ConfigMgr::GetFloatDefault("Rate.Drop.Money", 1.0f);
    rate_values[RATE_XP_KILL]       = ConfigMgr::GetFloatDefault("Rate.XP.Kill", 1.0f);
    rate_values[RATE_XP_QUEST]      = ConfigMgr::GetFloatDefault("Rate.XP.Quest", 1.0f);
    rate_values[RATE_XP_EXPLORE]    = ConfigMgr::GetFloatDefault("Rate.XP.Explore", 1.0f);
    rate_values[RATE_XP_GATHERING]  = ConfigMgr::GetFloatDefault("Rate.XP.Gathering", 1.0f);
    rate_values[RATE_REPAIRCOST]    = ConfigMgr::GetFloatDefault("Rate.RepairCost", 1.0f);
    if (rate_values[RATE_REPAIRCOST] < 0.0f)
    {
        TC_LOG_ERROR("server", "Rate.RepairCost (%f) must be >=0. Using 0.0 instead.", rate_values[RATE_REPAIRCOST]);
        rate_values[RATE_REPAIRCOST] = 0.0f;
    }
    rate_values[RATE_REPUTATION_GAIN]  = ConfigMgr::GetFloatDefault("Rate.Reputation.Gain", 1.0f);
    rate_values[RATE_REPUTATION_LOWLEVEL_KILL]  = ConfigMgr::GetFloatDefault("Rate.Reputation.LowLevel.Kill", 1.0f);
    rate_values[RATE_REPUTATION_LOWLEVEL_QUEST]  = ConfigMgr::GetFloatDefault("Rate.Reputation.LowLevel.Quest", 1.0f);
    rate_values[RATE_REPUTATION_RECRUIT_A_FRIEND_BONUS] = ConfigMgr::GetFloatDefault("Rate.Reputation.RecruitAFriendBonus", 0.1f);
    rate_values[RATE_CREATURE_NORMAL_DAMAGE]          = ConfigMgr::GetFloatDefault("Rate.Creature.Normal.Damage", 1.0f);
    rate_values[RATE_CREATURE_ELITE_ELITE_DAMAGE]     = ConfigMgr::GetFloatDefault("Rate.Creature.Elite.Elite.Damage", 1.0f);
    rate_values[RATE_CREATURE_ELITE_RAREELITE_DAMAGE] = ConfigMgr::GetFloatDefault("Rate.Creature.Elite.RAREELITE.Damage", 1.0f);
    rate_values[RATE_CREATURE_ELITE_WORLDBOSS_DAMAGE] = ConfigMgr::GetFloatDefault("Rate.Creature.Elite.WORLDBOSS.Damage", 1.0f);
    rate_values[RATE_CREATURE_ELITE_RARE_DAMAGE]      = ConfigMgr::GetFloatDefault("Rate.Creature.Elite.RARE.Damage", 1.0f);
    rate_values[RATE_CREATURE_NORMAL_HP]          = ConfigMgr::GetFloatDefault("Rate.Creature.Normal.HP", 1.0f);
    rate_values[RATE_CREATURE_ELITE_ELITE_HP]     = ConfigMgr::GetFloatDefault("Rate.Creature.Elite.Elite.HP", 1.0f);
    rate_values[RATE_CREATURE_ELITE_RAREELITE_HP] = ConfigMgr::GetFloatDefault("Rate.Creature.Elite.RAREELITE.HP", 1.0f);
    rate_values[RATE_CREATURE_ELITE_WORLDBOSS_HP] = ConfigMgr::GetFloatDefault("Rate.Creature.Elite.WORLDBOSS.HP", 1.0f);
    rate_values[RATE_CREATURE_ELITE_RARE_HP]      = ConfigMgr::GetFloatDefault("Rate.Creature.Elite.RARE.HP", 1.0f);
    rate_values[RATE_CREATURE_NORMAL_SPELLDAMAGE]          = ConfigMgr::GetFloatDefault("Rate.Creature.Normal.SpellDamage", 1.0f);
    rate_values[RATE_CREATURE_ELITE_ELITE_SPELLDAMAGE]     = ConfigMgr::GetFloatDefault("Rate.Creature.Elite.Elite.SpellDamage", 1.0f);
    rate_values[RATE_CREATURE_ELITE_RAREELITE_SPELLDAMAGE] = ConfigMgr::GetFloatDefault("Rate.Creature.Elite.RAREELITE.SpellDamage", 1.0f);
    rate_values[RATE_CREATURE_ELITE_WORLDBOSS_SPELLDAMAGE] = ConfigMgr::GetFloatDefault("Rate.Creature.Elite.WORLDBOSS.SpellDamage", 1.0f);
    rate_values[RATE_CREATURE_ELITE_RARE_SPELLDAMAGE]      = ConfigMgr::GetFloatDefault("Rate.Creature.Elite.RARE.SpellDamage", 1.0f);
    rate_values[RATE_CREATURE_AGGRO]  = ConfigMgr::GetFloatDefault("Rate.Creature.Aggro", 1.0f);
    rate_values[RATE_REST_INGAME]                    = ConfigMgr::GetFloatDefault("Rate.Rest.InGame", 1.0f);
    rate_values[RATE_REST_OFFLINE_IN_TAVERN_OR_CITY] = ConfigMgr::GetFloatDefault("Rate.Rest.Offline.InTavernOrCity", 1.0f);
    rate_values[RATE_REST_OFFLINE_IN_WILDERNESS]     = ConfigMgr::GetFloatDefault("Rate.Rest.Offline.InWilderness", 1.0f);
    rate_values[RATE_DAMAGE_FALL]  = ConfigMgr::GetFloatDefault("Rate.Damage.Fall", 1.0f);
    rate_values[RATE_AUCTION_TIME]  = ConfigMgr::GetFloatDefault("Rate.Auction.Time", 1.0f);
    rate_values[RATE_AUCTION_DEPOSIT] = ConfigMgr::GetFloatDefault("Rate.Auction.Deposit", 1.0f);
    rate_values[RATE_AUCTION_CUT] = ConfigMgr::GetFloatDefault("Rate.Auction.Cut", 1.0f);
    rate_values[RATE_HONOR] = ConfigMgr::GetFloatDefault("Rate.Honor", 1.0f);
    rate_values[RATE_HONOR_QB] = ConfigMgr::GetFloatDefault("Rate.Honor.QuestBg", 1.0f);
    rate_values[RATE_MINING_AMOUNT] = ConfigMgr::GetFloatDefault("Rate.Mining.Amount", 1.0f);
    rate_values[RATE_MINING_NEXT]   = ConfigMgr::GetFloatDefault("Rate.Mining.Next", 1.0f);
    rate_values[RATE_TALENT] = ConfigMgr::GetFloatDefault("Rate.Talent", 1.0f);
    if (rate_values[RATE_TALENT] < 0.0f)
    {
        TC_LOG_ERROR("server", "Rate.Talent (%f) must be > 0. Using 1 instead.", rate_values[RATE_TALENT]);
        rate_values[RATE_TALENT] = 1.0f;
    }
    rate_values[RATE_MOVESPEED] = ConfigMgr::GetFloatDefault("Rate.MoveSpeed", 1.0f);
    rate_values[RATE_ONLINE] = ConfigMgr::GetFloatDefault("Rate.Online", 1.0f);
    rate_values[RATE_DONATE] = ConfigMgr::GetFloatDefault("Rate.Donate", 1.0f);
    if (rate_values[RATE_MOVESPEED] < 0)
    {
        TC_LOG_ERROR("server", "Rate.MoveSpeed (%f) must be > 0. Using 1 instead.", rate_values[RATE_MOVESPEED]);
        rate_values[RATE_MOVESPEED] = 1.0f;
    }
    for (uint8 i = 0; i < MAX_MOVE_TYPE; ++i) playerBaseMoveSpeed[i] = baseMoveSpeed[i] * rate_values[RATE_MOVESPEED];
    rate_values[RATE_CORPSE_DECAY_LOOTED] = ConfigMgr::GetFloatDefault("Rate.Corpse.Decay.Looted", 0.5f);

    rate_values[RATE_TARGET_POS_RECALCULATION_RANGE] = ConfigMgr::GetFloatDefault("TargetPosRecalculateRange", 1.5f);
    if (rate_values[RATE_TARGET_POS_RECALCULATION_RANGE] < CONTACT_DISTANCE)
    {
        TC_LOG_ERROR("server", "TargetPosRecalculateRange (%f) must be >= %f. Using %f instead.", rate_values[RATE_TARGET_POS_RECALCULATION_RANGE], CONTACT_DISTANCE, CONTACT_DISTANCE);
        rate_values[RATE_TARGET_POS_RECALCULATION_RANGE] = CONTACT_DISTANCE;
    }
    else if (rate_values[RATE_TARGET_POS_RECALCULATION_RANGE] > NOMINAL_MELEE_RANGE)
    {
        TC_LOG_ERROR("server", "TargetPosRecalculateRange (%f) must be <= %f. Using %f instead.",
            rate_values[RATE_TARGET_POS_RECALCULATION_RANGE], NOMINAL_MELEE_RANGE, NOMINAL_MELEE_RANGE);
        rate_values[RATE_TARGET_POS_RECALCULATION_RANGE] = NOMINAL_MELEE_RANGE;
    }

    rate_values[RATE_DURABILITY_LOSS_ON_DEATH]  = ConfigMgr::GetFloatDefault("DurabilityLoss.OnDeath", 10.0f);
    if (rate_values[RATE_DURABILITY_LOSS_ON_DEATH] < 0.0f)
    {
        TC_LOG_ERROR("server", "DurabilityLoss.OnDeath (%f) must be >=0. Using 0.0 instead.", rate_values[RATE_DURABILITY_LOSS_ON_DEATH]);
        rate_values[RATE_DURABILITY_LOSS_ON_DEATH] = 0.0f;
    }
    if (rate_values[RATE_DURABILITY_LOSS_ON_DEATH] > 100.0f)
    {
        TC_LOG_ERROR("server", "DurabilityLoss.OnDeath (%f) must be <= 100. Using 100.0 instead.", rate_values[RATE_DURABILITY_LOSS_ON_DEATH]);
        rate_values[RATE_DURABILITY_LOSS_ON_DEATH] = 0.0f;
    }
    rate_values[RATE_DURABILITY_LOSS_ON_DEATH] = rate_values[RATE_DURABILITY_LOSS_ON_DEATH] / 100.0f;

    rate_values[RATE_DURABILITY_LOSS_DAMAGE] = ConfigMgr::GetFloatDefault("DurabilityLossChance.Damage", 0.5f);
    if (rate_values[RATE_DURABILITY_LOSS_DAMAGE] < 0.0f)
    {
        TC_LOG_ERROR("server", "DurabilityLossChance.Damage (%f) must be >=0. Using 0.0 instead.", rate_values[RATE_DURABILITY_LOSS_DAMAGE]);
        rate_values[RATE_DURABILITY_LOSS_DAMAGE] = 0.0f;
    }
    rate_values[RATE_DURABILITY_LOSS_ABSORB] = ConfigMgr::GetFloatDefault("DurabilityLossChance.Absorb", 0.5f);
    if (rate_values[RATE_DURABILITY_LOSS_ABSORB] < 0.0f)
    {
        TC_LOG_ERROR("server", "DurabilityLossChance.Absorb (%f) must be >=0. Using 0.0 instead.", rate_values[RATE_DURABILITY_LOSS_ABSORB]);
        rate_values[RATE_DURABILITY_LOSS_ABSORB] = 0.0f;
    }
    rate_values[RATE_DURABILITY_LOSS_PARRY] = ConfigMgr::GetFloatDefault("DurabilityLossChance.Parry", 0.05f);
    if (rate_values[RATE_DURABILITY_LOSS_PARRY] < 0.0f)
    {
        TC_LOG_ERROR("server", "DurabilityLossChance.Parry (%f) must be >=0. Using 0.0 instead.", rate_values[RATE_DURABILITY_LOSS_PARRY]);
        rate_values[RATE_DURABILITY_LOSS_PARRY] = 0.0f;
    }
    rate_values[RATE_DURABILITY_LOSS_BLOCK] = ConfigMgr::GetFloatDefault("DurabilityLossChance.Block", 0.05f);
    if (rate_values[RATE_DURABILITY_LOSS_BLOCK] < 0.0f)
    {
        TC_LOG_ERROR("server", "DurabilityLossChance.Block (%f) must be >=0. Using 0.0 instead.", rate_values[RATE_DURABILITY_LOSS_BLOCK]);
        rate_values[RATE_DURABILITY_LOSS_BLOCK] = 0.0f;
    }

    // movement anticheat
    m_EnableMvAnticheat = ConfigMgr::GetBoolDefault("Anticheat.Movement.Enable", true);
    m_EnableMvAnticheatDebug = ConfigMgr::GetBoolDefault("Anticheat.MovementDubug.Enable", false);
    m_TeleportToPlaneAlarms = ConfigMgr::GetIntDefault("Anticheat.Movement.TeleportToPlaneAlarms", 50);
    if (m_TeleportToPlaneAlarms < 20)
    {
        TC_LOG_ERROR("server", "Anticheat.Movement.TeleportToPlaneAlarms (%d) must be >= 20. Using 20 instead.", m_TeleportToPlaneAlarms);
        m_TeleportToPlaneAlarms = 20;
    }
    if (m_TeleportToPlaneAlarms > 100)
    {
        TC_LOG_ERROR("server", "Anticheat.Movement.TeleportToPlaneAlarms (%d) must be <= 100. Using 100 instead.", m_TeleportToPlaneAlarms);
        m_TeleportToPlaneAlarms = 100;
    }
    m_MistimingDelta = ConfigMgr::GetIntDefault("Anticheat.Movement.MistimingDelta", 15000);
    if (m_MistimingDelta < 5000)
    {
        TC_LOG_ERROR("server", "Anticheat.Movement.m_MistimingDelta (%d) must be >= 5000ms. Using 5000ms instead.", m_MistimingDelta);
        m_MistimingDelta = 5000;
    }
    if (m_MistimingDelta > 50000)
    {
        TC_LOG_ERROR("server", "Anticheat.Movement.m_MistimingDelta (%d) must be <= 50000ms. Using 50000ms instead.", m_MistimingDelta);
        m_MistimingDelta = 50000;
    }
    m_MistimingAlarms = ConfigMgr::GetIntDefault("Anticheat.Movement.MistimingAlarms", 200);
    if (m_MistimingAlarms < 100)
    {
        TC_LOG_ERROR("server", "Anticheat.Movement.MistimingAlarms (%d) must be >= 100. Using 100 instead.", m_MistimingAlarms);
        m_MistimingAlarms = 100;
    }
    if (m_MistimingAlarms > 500)
    {
        TC_LOG_ERROR("server", "Anticheat.Movement.m_MistimingAlarms (%d) must be <= 500. Using 500 instead.", m_MistimingAlarms);
        m_MistimingAlarms = 500;
    }
    ///- Read other configuration items from the config file

    m_bool_configs[CONFIG_DURABILITY_LOSS_IN_PVP] = ConfigMgr::GetBoolDefault("DurabilityLoss.InPvP", false);

    m_int_configs[CONFIG_COMPRESSION] = ConfigMgr::GetIntDefault("Compression", 1);
    if (m_int_configs[CONFIG_COMPRESSION] < 1 || m_int_configs[CONFIG_COMPRESSION] > 9)
    {
        TC_LOG_ERROR("server", "Compression level (%i) must be in range 1..9. Using default compression level (1).", m_int_configs[CONFIG_COMPRESSION]);
        m_int_configs[CONFIG_COMPRESSION] = 1;
    }
    m_bool_configs[CONFIG_ADDON_CHANNEL] = ConfigMgr::GetBoolDefault("AddonChannel", true);
    m_bool_configs[CONFIG_CLEAN_CHARACTER_DB] = ConfigMgr::GetBoolDefault("CleanCharacterDB", false);
    m_int_configs[CONFIG_PERSISTENT_CHARACTER_CLEAN_FLAGS] = ConfigMgr::GetIntDefault("PersistentCharacterCleanFlags", 0);
    m_int_configs[CONFIG_CHAT_CHANNEL_LEVEL_REQ] = ConfigMgr::GetIntDefault("ChatLevelReq.Channel", 1);
    m_int_configs[CONFIG_CHAT_WHISPER_LEVEL_REQ] = ConfigMgr::GetIntDefault("ChatLevelReq.Whisper", 1);
    m_int_configs[CONFIG_CHAT_SAY_LEVEL_REQ] = ConfigMgr::GetIntDefault("ChatLevelReq.Say", 1);
    m_int_configs[CONFIG_TRADE_LEVEL_REQ] = ConfigMgr::GetIntDefault("LevelReq.Trade", 1);
    m_int_configs[CONFIG_TICKET_LEVEL_REQ] = ConfigMgr::GetIntDefault("LevelReq.Ticket", 1);
    m_int_configs[CONFIG_AUCTION_LEVEL_REQ] = ConfigMgr::GetIntDefault("LevelReq.Auction", 1);
    m_int_configs[CONFIG_MAIL_LEVEL_REQ] = ConfigMgr::GetIntDefault("LevelReq.Mail", 1);
    m_bool_configs[CONFIG_ALLOW_PLAYER_COMMANDS] = ConfigMgr::GetBoolDefault("AllowPlayerCommands", 1);
    m_bool_configs[CONFIG_PRESERVE_CUSTOM_CHANNELS] = ConfigMgr::GetBoolDefault("PreserveCustomChannels", false);
    m_int_configs[CONFIG_PRESERVE_CUSTOM_CHANNEL_DURATION] = ConfigMgr::GetIntDefault("PreserveCustomChannelDuration", 14);
    m_bool_configs[CONFIG_GRID_UNLOAD] = ConfigMgr::GetBoolDefault("GridUnload", true);
    m_int_configs[CONFIG_INTERVAL_SAVE] = ConfigMgr::GetIntDefault("PlayerSaveInterval", 15 * MINUTE * IN_MILLISECONDS);
    m_int_configs[CONFIG_INTERVAL_DISCONNECT_TOLERANCE] = ConfigMgr::GetIntDefault("DisconnectToleranceInterval", 0);
    m_bool_configs[CONFIG_STATS_SAVE_ONLY_ON_LOGOUT] = ConfigMgr::GetBoolDefault("PlayerSave.Stats.SaveOnlyOnLogout", true);

    m_int_configs[CONFIG_MIN_LEVEL_STAT_SAVE] = ConfigMgr::GetIntDefault("PlayerSave.Stats.MinLevel", 0);
    if (m_int_configs[CONFIG_MIN_LEVEL_STAT_SAVE] > MAX_LEVEL)
    {
        TC_LOG_ERROR("server", "PlayerSave.Stats.MinLevel (%i) must be in range 0..80. Using default, do not save character stats (0).", m_int_configs[CONFIG_MIN_LEVEL_STAT_SAVE]);
        m_int_configs[CONFIG_MIN_LEVEL_STAT_SAVE] = 0;
    }

    m_int_configs[CONFIG_INTERVAL_GRIDCLEAN] = ConfigMgr::GetIntDefault("GridCleanUpDelay", 5 * MINUTE * IN_MILLISECONDS);
    if (m_int_configs[CONFIG_INTERVAL_GRIDCLEAN] < MIN_GRID_DELAY)
    {
        TC_LOG_ERROR("server", "GridCleanUpDelay (%i) must be greater %u. Use this minimal value.", m_int_configs[CONFIG_INTERVAL_GRIDCLEAN], MIN_GRID_DELAY);
        m_int_configs[CONFIG_INTERVAL_GRIDCLEAN] = MIN_GRID_DELAY;
    }
    if (reload)
        sMapMgr->SetGridCleanUpDelay(m_int_configs[CONFIG_INTERVAL_GRIDCLEAN]);

    m_int_configs[CONFIG_INTERVAL_MAPUPDATE] = ConfigMgr::GetIntDefault("MapUpdateInterval", 100);
    if (m_int_configs[CONFIG_INTERVAL_MAPUPDATE] < MIN_MAP_UPDATE_DELAY)
    {
        TC_LOG_ERROR("server", "MapUpdateInterval (%i) must be greater %u. Use this minimal value.", m_int_configs[CONFIG_INTERVAL_MAPUPDATE], MIN_MAP_UPDATE_DELAY);
        m_int_configs[CONFIG_INTERVAL_MAPUPDATE] = MIN_MAP_UPDATE_DELAY;
    }
    if (reload)
        sMapMgr->SetMapUpdateInterval(m_int_configs[CONFIG_INTERVAL_MAPUPDATE]);

    m_int_configs[CONFIG_INTERVAL_CHANGEWEATHER] = ConfigMgr::GetIntDefault("ChangeWeatherInterval", 10 * MINUTE * IN_MILLISECONDS);

    if (reload)
    {
        uint32 val = ConfigMgr::GetIntDefault("WorldServerPort", 8085);
        if (val != m_int_configs[CONFIG_PORT_WORLD])
            TC_LOG_ERROR("server", "WorldServerPort option can't be changed at worldserver.conf reload, using current value (%u).", m_int_configs[CONFIG_PORT_WORLD]);
    }
    else
        m_int_configs[CONFIG_PORT_WORLD] = ConfigMgr::GetIntDefault("WorldServerPort", 8085);

    m_int_configs[CONFIG_SOCKET_TIMEOUTTIME] = ConfigMgr::GetIntDefault("SocketTimeOutTime", 900000);
    m_int_configs[CONFIG_SESSION_ADD_DELAY] = ConfigMgr::GetIntDefault("SessionAddDelay", 10000);

    m_float_configs[CONFIG_GROUP_XP_DISTANCE] = ConfigMgr::GetFloatDefault("MaxGroupXPDistance", 74.0f);
    m_float_configs[CONFIG_MAX_RECRUIT_A_FRIEND_DISTANCE] = ConfigMgr::GetFloatDefault("MaxRecruitAFriendBonusDistance", 100.0f);

    /// \todo Add MonsterSight and GuarderSight (with meaning) in worldserver.conf or put them as define
    m_float_configs[CONFIG_SIGHT_MONSTER] = ConfigMgr::GetFloatDefault("MonsterSight", 50);
    m_float_configs[CONFIG_SIGHT_GUARDER] = ConfigMgr::GetFloatDefault("GuarderSight", 50);

    if (reload)
    {
        uint32 val = ConfigMgr::GetIntDefault("GameType", 0);
        if (val != m_int_configs[CONFIG_GAME_TYPE])
            TC_LOG_ERROR("server", "GameType option can't be changed at worldserver.conf reload, using current value (%u).", m_int_configs[CONFIG_GAME_TYPE]);
    }
    else
        m_int_configs[CONFIG_GAME_TYPE] = ConfigMgr::GetIntDefault("GameType", 0);

    if (reload)
    {
        uint32 val = ConfigMgr::GetIntDefault("RealmZone", REALM_ZONE_DEVELOPMENT);
        if (val != m_int_configs[CONFIG_REALM_ZONE])
            TC_LOG_ERROR("server", "RealmZone option can't be changed at worldserver.conf reload, using current value (%u).", m_int_configs[CONFIG_REALM_ZONE]);
    }
    else
        m_int_configs[CONFIG_REALM_ZONE] = ConfigMgr::GetIntDefault("RealmZone", REALM_ZONE_DEVELOPMENT);

    m_bool_configs[CONFIG_ALLOW_TWO_SIDE_ACCOUNTS]            = ConfigMgr::GetBoolDefault("AllowTwoSide.Accounts", true);
    m_bool_configs[CONFIG_ALLOW_TWO_SIDE_INTERACTION_CALENDAR]= ConfigMgr::GetBoolDefault("AllowTwoSide.Interaction.Calendar", false);
    m_bool_configs[CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHAT]    = ConfigMgr::GetBoolDefault("AllowTwoSide.Interaction.Chat", false);
    m_bool_configs[CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHANNEL] = ConfigMgr::GetBoolDefault("AllowTwoSide.Interaction.Channel", false);
    m_bool_configs[CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP]   = ConfigMgr::GetBoolDefault("AllowTwoSide.Interaction.Group", false);
    m_bool_configs[CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD]   = ConfigMgr::GetBoolDefault("AllowTwoSide.Interaction.Guild", false);
    m_bool_configs[CONFIG_ALLOW_TWO_SIDE_INTERACTION_AUCTION] = ConfigMgr::GetBoolDefault("AllowTwoSide.Interaction.Auction", false);
    m_bool_configs[CONFIG_ALLOW_TWO_SIDE_INTERACTION_MAIL]    = ConfigMgr::GetBoolDefault("AllowTwoSide.Interaction.Mail", false);
    m_bool_configs[CONFIG_ALLOW_TWO_SIDE_WHO_LIST]            = ConfigMgr::GetBoolDefault("AllowTwoSide.WhoList", false);
    m_bool_configs[CONFIG_ALLOW_TWO_SIDE_ADD_FRIEND]          = ConfigMgr::GetBoolDefault("AllowTwoSide.AddFriend", false);
    m_bool_configs[CONFIG_ALLOW_TWO_SIDE_TRADE]               = ConfigMgr::GetBoolDefault("AllowTwoSide.trade", false);
    m_int_configs[CONFIG_STRICT_PLAYER_NAMES]                 = ConfigMgr::GetIntDefault ("StrictPlayerNames",  0);
    m_int_configs[CONFIG_STRICT_CHARTER_NAMES]                = ConfigMgr::GetIntDefault ("StrictCharterNames", 0);
    m_int_configs[CONFIG_STRICT_PET_NAMES]                    = ConfigMgr::GetIntDefault ("StrictPetNames",     0);

    m_int_configs[CONFIG_MIN_PLAYER_NAME]                     = ConfigMgr::GetIntDefault ("MinPlayerName",  2);
    if (m_int_configs[CONFIG_MIN_PLAYER_NAME] < 1 || m_int_configs[CONFIG_MIN_PLAYER_NAME] > MAX_PLAYER_NAME)
    {
        TC_LOG_ERROR("server", "MinPlayerName (%i) must be in range 1..%u. Set to 2.", m_int_configs[CONFIG_MIN_PLAYER_NAME], MAX_PLAYER_NAME);
        m_int_configs[CONFIG_MIN_PLAYER_NAME] = 2;
    }

    m_int_configs[CONFIG_MIN_CHARTER_NAME]                    = ConfigMgr::GetIntDefault ("MinCharterName", 2);
    if (m_int_configs[CONFIG_MIN_CHARTER_NAME] < 1 || m_int_configs[CONFIG_MIN_CHARTER_NAME] > MAX_CHARTER_NAME)
    {
        TC_LOG_ERROR("server", "MinCharterName (%i) must be in range 1..%u. Set to 2.", m_int_configs[CONFIG_MIN_CHARTER_NAME], MAX_CHARTER_NAME);
        m_int_configs[CONFIG_MIN_CHARTER_NAME] = 2;
    }

    m_int_configs[CONFIG_MIN_PET_NAME]                        = ConfigMgr::GetIntDefault ("MinPetName",     2);
    if (m_int_configs[CONFIG_MIN_PET_NAME] < 1 || m_int_configs[CONFIG_MIN_PET_NAME] > MAX_PET_NAME)
    {
        TC_LOG_ERROR("server", "MinPetName (%i) must be in range 1..%u. Set to 2.", m_int_configs[CONFIG_MIN_PET_NAME], MAX_PET_NAME);
        m_int_configs[CONFIG_MIN_PET_NAME] = 2;
    }

    m_int_configs[CONFIG_CHARACTER_CREATING_DISABLED] = ConfigMgr::GetIntDefault("CharacterCreating.Disabled", 0);
    m_int_configs[CONFIG_CHARACTER_CREATING_DISABLED_RACEMASK] = ConfigMgr::GetIntDefault("CharacterCreating.Disabled.RaceMask", 0);
    m_int_configs[CONFIG_CHARACTER_CREATING_DISABLED_CLASSMASK] = ConfigMgr::GetIntDefault("CharacterCreating.Disabled.ClassMask", 0);

    m_int_configs[CONFIG_CHARACTERS_PER_REALM] = ConfigMgr::GetIntDefault("CharactersPerRealm", 11);
    if (m_int_configs[CONFIG_CHARACTERS_PER_REALM] < 1 || m_int_configs[CONFIG_CHARACTERS_PER_REALM] > 11)
    {
        TC_LOG_ERROR("server", "CharactersPerRealm (%i) must be in range 1..11. Set to 11.", m_int_configs[CONFIG_CHARACTERS_PER_REALM]);
        m_int_configs[CONFIG_CHARACTERS_PER_REALM] = 11;
    }

    // must be after CONFIG_CHARACTERS_PER_REALM
    m_int_configs[CONFIG_CHARACTERS_PER_ACCOUNT] = ConfigMgr::GetIntDefault("CharactersPerAccount", 50);
    if (m_int_configs[CONFIG_CHARACTERS_PER_ACCOUNT] < m_int_configs[CONFIG_CHARACTERS_PER_REALM])
    {
        TC_LOG_ERROR("server", "CharactersPerAccount (%i) can't be less than CharactersPerRealm (%i).", m_int_configs[CONFIG_CHARACTERS_PER_ACCOUNT], m_int_configs[CONFIG_CHARACTERS_PER_REALM]);
        m_int_configs[CONFIG_CHARACTERS_PER_ACCOUNT] = m_int_configs[CONFIG_CHARACTERS_PER_REALM];
    }

    m_int_configs[CONFIG_HEROIC_CHARACTERS_PER_REALM] = ConfigMgr::GetIntDefault("HeroicCharactersPerRealm", 1);
    if (int32(m_int_configs[CONFIG_HEROIC_CHARACTERS_PER_REALM]) < 0 || m_int_configs[CONFIG_HEROIC_CHARACTERS_PER_REALM] > 10)
    {
        TC_LOG_ERROR("server", "HeroicCharactersPerRealm (%i) must be in range 0..10. Set to 1.", m_int_configs[CONFIG_HEROIC_CHARACTERS_PER_REALM]);
        m_int_configs[CONFIG_HEROIC_CHARACTERS_PER_REALM] = 1;
    }

    m_int_configs[CONFIG_CHARACTER_CREATING_MIN_LEVEL_FOR_HEROIC_CHARACTER] = ConfigMgr::GetIntDefault("CharacterCreating.MinLevelForHeroicCharacter", 55);

    m_int_configs[CONFIG_SKIP_CINEMATICS] = ConfigMgr::GetIntDefault("SkipCinematics", 0);
    if (int32(m_int_configs[CONFIG_SKIP_CINEMATICS]) < 0 || m_int_configs[CONFIG_SKIP_CINEMATICS] > 2)
    {
        TC_LOG_ERROR("server", "SkipCinematics (%i) must be in range 0..2. Set to 0.", m_int_configs[CONFIG_SKIP_CINEMATICS]);
        m_int_configs[CONFIG_SKIP_CINEMATICS] = 0;
    }

    if (reload)
    {
        uint32 val = ConfigMgr::GetIntDefault("MaxPlayerLevel", DEFAULT_MAX_LEVEL);
        if (val != m_int_configs[CONFIG_MAX_PLAYER_LEVEL])
            TC_LOG_ERROR("server", "MaxPlayerLevel option can't be changed at config reload, using current value (%u).", m_int_configs[CONFIG_MAX_PLAYER_LEVEL]);
    }
    else
        m_int_configs[CONFIG_MAX_PLAYER_LEVEL] = ConfigMgr::GetIntDefault("MaxPlayerLevel", DEFAULT_MAX_LEVEL);

    m_int_configs[CONFIG_MAX_SKILL_VALUE] = ConfigMgr::GetIntDefault("MaxSkillValue", DEFAULT_MAX_SKILL_VALUE);
    m_int_configs[CONFIG_TRANSFER_GOLD_LIMIT] = ConfigMgr::GetIntDefault("TransferGoldLimit", 100000000); // 10k gold for default limit transfer

    if (m_int_configs[CONFIG_MAX_PLAYER_LEVEL] > MAX_LEVEL)
    {
        TC_LOG_ERROR("server", "MaxPlayerLevel (%i) must be in range 1..%u. Set to %u.", m_int_configs[CONFIG_MAX_PLAYER_LEVEL], MAX_LEVEL, MAX_LEVEL);
        m_int_configs[CONFIG_MAX_PLAYER_LEVEL] = MAX_LEVEL;
    }

    m_int_configs[CONFIG_MIN_DUALSPEC_LEVEL] = ConfigMgr::GetIntDefault("MinDualSpecLevel", 40);

    m_int_configs[CONFIG_START_PLAYER_LEVEL] = ConfigMgr::GetIntDefault("StartPlayerLevel", 1);
    if (m_int_configs[CONFIG_START_PLAYER_LEVEL] < 1)
    {
        TC_LOG_ERROR("server", "StartPlayerLevel (%i) must be in range 1..MaxPlayerLevel(%u). Set to 1.", m_int_configs[CONFIG_START_PLAYER_LEVEL], m_int_configs[CONFIG_MAX_PLAYER_LEVEL]);
        m_int_configs[CONFIG_START_PLAYER_LEVEL] = 1;
    }
    else if (m_int_configs[CONFIG_START_PLAYER_LEVEL] > m_int_configs[CONFIG_MAX_PLAYER_LEVEL])
    {
        TC_LOG_ERROR("server", "StartPlayerLevel (%i) must be in range 1..MaxPlayerLevel(%u). Set to %u.", m_int_configs[CONFIG_START_PLAYER_LEVEL], m_int_configs[CONFIG_MAX_PLAYER_LEVEL], m_int_configs[CONFIG_MAX_PLAYER_LEVEL]);
        m_int_configs[CONFIG_START_PLAYER_LEVEL] = m_int_configs[CONFIG_MAX_PLAYER_LEVEL];
    }

    m_int_configs[CONFIG_START_HEROIC_PLAYER_LEVEL] = ConfigMgr::GetIntDefault("StartHeroicPlayerLevel", 55);
    if (m_int_configs[CONFIG_START_HEROIC_PLAYER_LEVEL] < 1)
    {
        TC_LOG_ERROR("server", "StartHeroicPlayerLevel (%i) must be in range 1..MaxPlayerLevel(%u). Set to 55.",
            m_int_configs[CONFIG_START_HEROIC_PLAYER_LEVEL], m_int_configs[CONFIG_MAX_PLAYER_LEVEL]);
        m_int_configs[CONFIG_START_HEROIC_PLAYER_LEVEL] = 55;
    }
    else if (m_int_configs[CONFIG_START_HEROIC_PLAYER_LEVEL] > m_int_configs[CONFIG_MAX_PLAYER_LEVEL])
    {
        TC_LOG_ERROR("server", "StartHeroicPlayerLevel (%i) must be in range 1..MaxPlayerLevel(%u). Set to %u.",
            m_int_configs[CONFIG_START_HEROIC_PLAYER_LEVEL], m_int_configs[CONFIG_MAX_PLAYER_LEVEL], m_int_configs[CONFIG_MAX_PLAYER_LEVEL]);
        m_int_configs[CONFIG_START_HEROIC_PLAYER_LEVEL] = m_int_configs[CONFIG_MAX_PLAYER_LEVEL];
    }

    m_int_configs[CONFIG_START_PLAYER_MONEY] = ConfigMgr::GetIntDefault("StartPlayerMoney", 0);
    if (int32(m_int_configs[CONFIG_START_PLAYER_MONEY]) < 0)
    {
        TC_LOG_ERROR("server", "StartPlayerMoney (%i) must be in range 0.." UI64FMTD ". Set to %u.", m_int_configs[CONFIG_START_PLAYER_MONEY], MAX_MONEY_AMOUNT, 0);
        m_int_configs[CONFIG_START_PLAYER_MONEY] = 0;
    }
    else if (m_int_configs[CONFIG_START_PLAYER_MONEY] > 0x7FFFFFFF-1) // TODO: (See MAX_MONEY_AMOUNT)
    {
        TC_LOG_ERROR("server", "StartPlayerMoney (%i) must be in range 0..%u. Set to %u.",
            m_int_configs[CONFIG_START_PLAYER_MONEY], 0x7FFFFFFF-1, 0x7FFFFFFF-1);
        m_int_configs[CONFIG_START_PLAYER_MONEY] = 0x7FFFFFFF-1;
    }

    m_int_configs[CONFIG_CURRENCY_RESET_HOUR] = ConfigMgr::GetIntDefault("Currency.ResetHour", 3);
    if (m_int_configs[CONFIG_CURRENCY_RESET_HOUR] > 23)
    {
        TC_LOG_ERROR("server", "Currency.ResetHour (%i) can't be load. Set to 6.", m_int_configs[CONFIG_CURRENCY_RESET_HOUR]);
        m_int_configs[CONFIG_CURRENCY_RESET_HOUR] = 3;
    }
    m_int_configs[CONFIG_CURRENCY_RESET_DAY] = ConfigMgr::GetIntDefault("Currency.ResetDay", 3);
    if (m_int_configs[CONFIG_CURRENCY_RESET_DAY] > 6)
    {
        TC_LOG_ERROR("server", "Currency.ResetDay (%i) can't be load. Set to 3.", m_int_configs[CONFIG_CURRENCY_RESET_DAY]);
        m_int_configs[CONFIG_CURRENCY_RESET_DAY] = 3;
    }
    m_int_configs[CONFIG_CURRENCY_RESET_INTERVAL] = ConfigMgr::GetIntDefault("Currency.ResetInterval", 7);
    if (int32(m_int_configs[CONFIG_CURRENCY_RESET_INTERVAL]) <= 0)
    {
        TC_LOG_ERROR("server", "Currency.ResetInterval (%i) must be > 0, set to default 7.", m_int_configs[CONFIG_CURRENCY_RESET_INTERVAL]);
        m_int_configs[CONFIG_CURRENCY_RESET_INTERVAL] = 7;
    }

    m_int_configs[CONFIG_CURRENCY_START_HONOR_POINTS] = ConfigMgr::GetIntDefault("Currency.StartHonorPoints", 0);
    if (int32(m_int_configs[CONFIG_CURRENCY_START_HONOR_POINTS]) < 0)
    {
        TC_LOG_ERROR("server", "Currency.StartHonorPoints (%i) must be >= 0, set to default 0.", m_int_configs[CONFIG_CURRENCY_START_HONOR_POINTS]);
        m_int_configs[CONFIG_CURRENCY_START_HONOR_POINTS] = 0;
    }
    m_int_configs[CONFIG_CURRENCY_MAX_HONOR_POINTS] = ConfigMgr::GetIntDefault("Currency.MaxHonorPoints", 4000);
    if (int32(m_int_configs[CONFIG_CURRENCY_MAX_HONOR_POINTS]) < 0)
    {
        TC_LOG_ERROR("server", "Currency.MaxHonorPoints (%i) can't be negative. Set to default 4000.", m_int_configs[CONFIG_CURRENCY_MAX_HONOR_POINTS]);
        m_int_configs[CONFIG_CURRENCY_MAX_HONOR_POINTS] = 4000;
    }
    m_int_configs[CONFIG_CURRENCY_MAX_HONOR_POINTS] *= 100; //precision

    m_int_configs[CONFIG_CURRENCY_START_JUSTICE_POINTS] = ConfigMgr::GetIntDefault("Currency.StartJusticePoints", 0);
    if (int32(m_int_configs[CONFIG_CURRENCY_START_JUSTICE_POINTS]) < 0)
    {
        TC_LOG_ERROR("server", "Currency.StartJusticePoints (%i) must be >= 0, set to default 0.", m_int_configs[CONFIG_CURRENCY_START_JUSTICE_POINTS]);
        m_int_configs[CONFIG_CURRENCY_START_JUSTICE_POINTS] = 0;
    }
    m_int_configs[CONFIG_CURRENCY_MAX_JUSTICE_POINTS] = ConfigMgr::GetIntDefault("Currency.MaxJusticePoints", 4000);
    if (int32(m_int_configs[CONFIG_CURRENCY_MAX_JUSTICE_POINTS]) < 0)
    {
        TC_LOG_ERROR("server", "Currency.MaxJusticePoints (%i) can't be negative. Set to default 4000.", m_int_configs[CONFIG_CURRENCY_MAX_JUSTICE_POINTS]);
        m_int_configs[CONFIG_CURRENCY_MAX_JUSTICE_POINTS] = 4000;
    }
    m_int_configs[CONFIG_CURRENCY_MAX_JUSTICE_POINTS] *= 100; //precision

    m_int_configs[CONFIG_CURRENCY_START_CONQUEST_POINTS] = ConfigMgr::GetIntDefault("Currency.StartConquestPoints", 0);
    if (int32(m_int_configs[CONFIG_CURRENCY_START_CONQUEST_POINTS]) < 0)
    {
        TC_LOG_ERROR("server", "Currency.StartConquestPoints (%i) must be >= 0, set to default 0.", m_int_configs[CONFIG_CURRENCY_START_CONQUEST_POINTS]);
        m_int_configs[CONFIG_CURRENCY_START_CONQUEST_POINTS] = 0;
    }
    m_int_configs[CONFIG_CURRENCY_START_CONQUEST_POINTS] *= 100; //precision

    m_int_configs[CONFIG_CURRENCY_CONQUEST_POINTS_WEEK_CAP] = ConfigMgr::GetIntDefault("Currency.ConquestPointsWeekCap", 1800);
    if (int32(m_int_configs[CONFIG_CURRENCY_CONQUEST_POINTS_WEEK_CAP]) <= 0)
    {
        TC_LOG_ERROR("server", "Currency.ConquestPointsWeekCap (%i) must be > 0, set to default 1800.", m_int_configs[CONFIG_CURRENCY_CONQUEST_POINTS_WEEK_CAP]);
        m_int_configs[CONFIG_CURRENCY_CONQUEST_POINTS_WEEK_CAP] = 1800;
    }
    m_int_configs[CONFIG_CURRENCY_CONQUEST_POINTS_WEEK_CAP] *= GetCurrencyPrecision(CURRENCY_TYPE_CONQUEST_POINTS); //precision

    m_int_configs[CONFIG_CURRENCY_CONQUEST_POINTS_ARENA_REWARD] = ConfigMgr::GetIntDefault("Currency.ConquestPointsArenaReward", 180 * 100);
    if (int32(m_int_configs[CONFIG_CURRENCY_CONQUEST_POINTS_ARENA_REWARD]) <= 0)
    {
        TC_LOG_ERROR("server", "Currency.ConquestPointsArenaReward (%i) must be > 0, set to default 180.", m_int_configs[CURRENCY_TYPE_CONQUEST_META_ARENA]);
        m_int_configs[CONFIG_CURRENCY_CONQUEST_POINTS_ARENA_REWARD] = 180;
    }
    m_int_configs[CONFIG_CURRENCY_CONQUEST_POINTS_ARENA_REWARD] *= 100; //precision

    m_int_configs[CONFIG_CURRENCY_CONQUEST_POINTS_RBG_REWARD] = ConfigMgr::GetIntDefault("Currency.ConquestPointsRbgReward", 400);
    if (int32(m_int_configs[CONFIG_CURRENCY_CONQUEST_POINTS_RBG_REWARD]) <= 0)
    {
        TC_LOG_ERROR("server", "Currency.ConquestPointsRbgReward (%i) must be > 0, set to default 400.", m_int_configs[CONFIG_CURRENCY_CONQUEST_POINTS_RBG_REWARD]);
        m_int_configs[CONFIG_CURRENCY_CONQUEST_POINTS_RBG_REWARD] = 400;
    }
    m_int_configs[CONFIG_CURRENCY_CONQUEST_POINTS_RBG_REWARD] *= 100; //precision

    m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_NO_MEDAL] = ConfigMgr::GetIntDefault("Currency.ValorForChallengeNoMedal", 0);
    if (int32(m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_NO_MEDAL]) < 0)
        m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_NO_MEDAL] = 0;
    m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_NO_MEDAL] *= 100; //precision

    m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_BRONZ] = ConfigMgr::GetIntDefault("Currency.ValorForChallengeBronz", 125);
    if (int32(m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_BRONZ]) < 0)
        m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_BRONZ] = 0;
    m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_BRONZ] *= 100; //precision

    m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_SILVER] = ConfigMgr::GetIntDefault("Currency.ValorForChallengeSilver", 145);
    if (int32(m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_SILVER]) < 0)
        m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_SILVER] = 0;
    m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_SILVER] *= 100; //precision

    m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_GOLD] = ConfigMgr::GetIntDefault("Currency.ValorForChallengeSilver", 165);
    if (int32(m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_GOLD]) < 0)
        m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_GOLD] = 0;
    m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_GOLD] *= 100; //precision

    m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_PLATINUM] = ConfigMgr::GetIntDefault("Currency.ValorForChallengePlatinum", 185);
    if (int32(m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_PLATINUM]) < 0)
        m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_PLATINUM] = 0;
    m_int_configs[CONFIG_CURRENCY_VALOR_FOR_CHALLENGE_PLATINUM] *= 100; //precision

    m_int_configs[CONFIG_MAX_RECRUIT_A_FRIEND_BONUS_PLAYER_LEVEL] = ConfigMgr::GetIntDefault("RecruitAFriend.MaxLevel", 85);
    if (m_int_configs[CONFIG_MAX_RECRUIT_A_FRIEND_BONUS_PLAYER_LEVEL] > m_int_configs[CONFIG_MAX_PLAYER_LEVEL])
    {
        TC_LOG_ERROR("server", "RecruitAFriend.MaxLevel (%i) must be in the range 0..MaxLevel(%u). Set to %u.",
            m_int_configs[CONFIG_MAX_RECRUIT_A_FRIEND_BONUS_PLAYER_LEVEL], m_int_configs[CONFIG_MAX_PLAYER_LEVEL], 85);
        m_int_configs[CONFIG_MAX_RECRUIT_A_FRIEND_BONUS_PLAYER_LEVEL] = 85;
    }

    m_int_configs[CONFIG_MAX_RECRUIT_A_FRIEND_BONUS_PLAYER_LEVEL_DIFFERENCE] = ConfigMgr::GetIntDefault("RecruitAFriend.MaxDifference", 4);
    m_bool_configs[CONFIG_ALL_TAXI_PATHS] = ConfigMgr::GetBoolDefault("AllFlightPaths", false);
    m_bool_configs[CONFIG_INSTANT_TAXI] = ConfigMgr::GetBoolDefault("InstantFlightPaths", false);

    m_bool_configs[CONFIG_INSTANCE_IGNORE_LEVEL] = ConfigMgr::GetBoolDefault("Instance.IgnoreLevel", false);
    m_bool_configs[CONFIG_INSTANCE_IGNORE_RAID]  = ConfigMgr::GetBoolDefault("Instance.IgnoreRaid", false);

    m_bool_configs[CONFIG_CAST_UNSTUCK] = ConfigMgr::GetBoolDefault("CastUnstuck", true);
    m_int_configs[CONFIG_MAX_SPELL_CASTS_IN_CHAIN]  = ConfigMgr::GetIntDefault("MaxSpellCastsInChain", 10);
    m_int_configs[CONFIG_INSTANCE_RESET_TIME_HOUR]  = ConfigMgr::GetIntDefault("Instance.ResetTimeHour", 6);
    m_int_configs[CONFIG_INSTANCE_DAILY_RESET]  = ConfigMgr::GetIntDefault("Instance.DailyReset", 1);
    m_int_configs[CONFIG_INSTANCE_HALF_WEEK_RESET]  = ConfigMgr::GetIntDefault("Instance.HalfWeekReset", 3);
    m_int_configs[CONFIG_INSTANCE_WEEKLY_RESET]  = ConfigMgr::GetIntDefault("Instance.WeeklyReset", 7);
    m_int_configs[CONFIG_INSTANCE_UNLOAD_DELAY] = ConfigMgr::GetIntDefault("Instance.UnloadDelay", 30 * MINUTE * IN_MILLISECONDS);

    m_int_configs[CONFIG_MAX_PRIMARY_TRADE_SKILL] = ConfigMgr::GetIntDefault("MaxPrimaryTradeSkill", 2);
    m_int_configs[CONFIG_MIN_PETITION_SIGNS] = ConfigMgr::GetIntDefault("MinPetitionSigns", 9);
    if (m_int_configs[CONFIG_MIN_PETITION_SIGNS] > 9)
    {
        TC_LOG_ERROR("server", "MinPetitionSigns (%i) must be in range 0..9. Set to 9.", m_int_configs[CONFIG_MIN_PETITION_SIGNS]);
        m_int_configs[CONFIG_MIN_PETITION_SIGNS] = 9;
    }

    m_int_configs[CONFIG_GM_LOGIN_STATE]        = ConfigMgr::GetIntDefault("GM.LoginState", 2);
    m_int_configs[CONFIG_GM_VISIBLE_STATE]      = ConfigMgr::GetIntDefault("GM.Visible", 2);
    m_int_configs[CONFIG_GM_CHAT]               = ConfigMgr::GetIntDefault("GM.Chat", 2);
    m_int_configs[CONFIG_GM_WHISPERING_TO]      = ConfigMgr::GetIntDefault("GM.WhisperingTo", 2);

    m_int_configs[CONFIG_GM_LEVEL_IN_GM_LIST]   = ConfigMgr::GetIntDefault("GM.InGMList.Level", SEC_ADMINISTRATOR);
    m_int_configs[CONFIG_GM_LEVEL_IN_WHO_LIST]  = ConfigMgr::GetIntDefault("GM.InWhoList.Level", SEC_ADMINISTRATOR);
    m_bool_configs[CONFIG_GM_LOG_TRADE]         = ConfigMgr::GetBoolDefault("GM.LogTrade", false);
    m_int_configs[CONFIG_START_GM_LEVEL]        = ConfigMgr::GetIntDefault("GM.StartLevel", 1);
    if (m_int_configs[CONFIG_START_GM_LEVEL] < m_int_configs[CONFIG_START_PLAYER_LEVEL])
    {
        TC_LOG_ERROR("server", "GM.StartLevel (%i) must be in range StartPlayerLevel(%u)..%u. Set to %u.",
            m_int_configs[CONFIG_START_GM_LEVEL], m_int_configs[CONFIG_START_PLAYER_LEVEL], MAX_LEVEL, m_int_configs[CONFIG_START_PLAYER_LEVEL]);
        m_int_configs[CONFIG_START_GM_LEVEL] = m_int_configs[CONFIG_START_PLAYER_LEVEL];
    }
    else if (m_int_configs[CONFIG_START_GM_LEVEL] > MAX_LEVEL)
    {
        TC_LOG_ERROR("server", "GM.StartLevel (%i) must be in range 1..%u. Set to %u.", m_int_configs[CONFIG_START_GM_LEVEL], MAX_LEVEL, MAX_LEVEL);
        m_int_configs[CONFIG_START_GM_LEVEL] = MAX_LEVEL;
    }
    m_bool_configs[CONFIG_ALLOW_GM_GROUP]       = ConfigMgr::GetBoolDefault("GM.AllowInvite", false);
    m_bool_configs[CONFIG_ALLOW_GM_FRIEND]      = ConfigMgr::GetBoolDefault("GM.AllowFriend", false);
    m_bool_configs[CONFIG_GM_LOWER_SECURITY] = ConfigMgr::GetBoolDefault("GM.LowerSecurity", false);
    m_float_configs[CONFIG_CHANCE_OF_GM_SURVEY] = ConfigMgr::GetFloatDefault("GM.TicketSystem.ChanceOfGMSurvey", 50.0f);

    m_int_configs[CONFIG_GROUP_VISIBILITY] = ConfigMgr::GetIntDefault("Visibility.GroupMode", 1);

    m_int_configs[CONFIG_MAIL_DELIVERY_DELAY] = ConfigMgr::GetIntDefault("MailDeliveryDelay", HOUR);

    m_int_configs[CONFIG_UPTIME_UPDATE] = ConfigMgr::GetIntDefault("UpdateUptimeInterval", 10);
    if (int32(m_int_configs[CONFIG_UPTIME_UPDATE]) <= 0)
    {
        TC_LOG_ERROR("server", "UpdateUptimeInterval (%i) must be > 0, set to default 10.", m_int_configs[CONFIG_UPTIME_UPDATE]);
        m_int_configs[CONFIG_UPTIME_UPDATE] = 10;
    }
    if (reload)
    {
        m_timers[WUPDATE_UPTIME].SetInterval(m_int_configs[CONFIG_UPTIME_UPDATE]*MINUTE*IN_MILLISECONDS);
        m_timers[WUPDATE_UPTIME].Reset();
    }

    // log db cleanup interval
    m_int_configs[CONFIG_LOGDB_CLEARINTERVAL] = ConfigMgr::GetIntDefault("LogDB.Opt.ClearInterval", 10);
    if (int32(m_int_configs[CONFIG_LOGDB_CLEARINTERVAL]) <= 0)
    {
        TC_LOG_ERROR("server", "LogDB.Opt.ClearInterval (%i) must be > 0, set to default 10.", m_int_configs[CONFIG_LOGDB_CLEARINTERVAL]);
        m_int_configs[CONFIG_LOGDB_CLEARINTERVAL] = 10;
    }
    if (reload)
    {
        m_timers[WUPDATE_CLEANDB].SetInterval(m_int_configs[CONFIG_LOGDB_CLEARINTERVAL] * MINUTE * IN_MILLISECONDS);
        m_timers[WUPDATE_CLEANDB].Reset();
    }
    m_int_configs[CONFIG_LOGDB_CLEARTIME] = ConfigMgr::GetIntDefault("LogDB.Opt.ClearTime", 1209600); // 14 days default
    TC_LOG_INFO("server", "Will clear `logs` table of entries older than %i seconds every %u minutes.",
        m_int_configs[CONFIG_LOGDB_CLEARTIME], m_int_configs[CONFIG_LOGDB_CLEARINTERVAL]);

    m_int_configs[CONFIG_SKILL_CHANCE_ORANGE] = ConfigMgr::GetIntDefault("SkillChance.Orange", 100);
    m_int_configs[CONFIG_SKILL_CHANCE_YELLOW] = ConfigMgr::GetIntDefault("SkillChance.Yellow", 75);
    m_int_configs[CONFIG_SKILL_CHANCE_GREEN]  = ConfigMgr::GetIntDefault("SkillChance.Green", 25);
    m_int_configs[CONFIG_SKILL_CHANCE_GREY]   = ConfigMgr::GetIntDefault("SkillChance.Grey", 0);

    m_int_configs[CONFIG_SKILL_CHANCE_MINING_STEPS]  = ConfigMgr::GetIntDefault("SkillChance.MiningSteps", 75);
    m_int_configs[CONFIG_SKILL_CHANCE_SKINNING_STEPS]   = ConfigMgr::GetIntDefault("SkillChance.SkinningSteps", 75);

    m_bool_configs[CONFIG_SKILL_PROSPECTING] = ConfigMgr::GetBoolDefault("SkillChance.Prospecting", false);
    m_bool_configs[CONFIG_SKILL_MILLING] = ConfigMgr::GetBoolDefault("SkillChance.Milling", false);

    m_int_configs[CONFIG_SKILL_GAIN_CRAFTING]  = ConfigMgr::GetIntDefault("SkillGain.Crafting", 1);

    m_int_configs[CONFIG_SKILL_GAIN_GATHERING]  = ConfigMgr::GetIntDefault("SkillGain.Gathering", 1);

    m_int_configs[CONFIG_MAX_OVERSPEED_PINGS] = ConfigMgr::GetIntDefault("MaxOverspeedPings", 2);
    if (m_int_configs[CONFIG_MAX_OVERSPEED_PINGS] != 0 && m_int_configs[CONFIG_MAX_OVERSPEED_PINGS] < 2)
    {
        TC_LOG_ERROR("server", "MaxOverspeedPings (%i) must be in range 2..infinity (or 0 to disable check). Set to 2.", m_int_configs[CONFIG_MAX_OVERSPEED_PINGS]);
        m_int_configs[CONFIG_MAX_OVERSPEED_PINGS] = 2;
    }

    m_bool_configs[CONFIG_SAVE_RESPAWN_TIME_IMMEDIATELY] = ConfigMgr::GetBoolDefault("SaveRespawnTimeImmediately", true);
    m_bool_configs[CONFIG_WEATHER] = ConfigMgr::GetBoolDefault("ActivateWeather", true);

    m_int_configs[CONFIG_DISABLE_BREATHING] = ConfigMgr::GetIntDefault("DisableWaterBreath", SEC_CONSOLE);

    if (reload)
    {
        uint32 val = ConfigMgr::GetIntDefault("Expansion", 1);
        if (val != m_int_configs[CONFIG_EXPANSION])
            TC_LOG_ERROR("server", "Expansion option can't be changed at worldserver.conf reload, using current value (%u).", m_int_configs[CONFIG_EXPANSION]);
    }
    else
        m_int_configs[CONFIG_EXPANSION] = ConfigMgr::GetIntDefault("Expansion", 1);

    m_int_configs[CONFIG_CHATFLOOD_MESSAGE_COUNT] = ConfigMgr::GetIntDefault("ChatFlood.MessageCount", 10);
    m_int_configs[CONFIG_CHATFLOOD_MESSAGE_DELAY] = ConfigMgr::GetIntDefault("ChatFlood.MessageDelay", 1);
    m_int_configs[CONFIG_CHATFLOOD_MUTE_TIME]     = ConfigMgr::GetIntDefault("ChatFlood.MuteTime", 10);

    m_int_configs[CONFIG_WORD_FILTER_MUTE_DURATION] = ConfigMgr::GetIntDefault("WordFilter.MuteDuration", 30000);
    m_bool_configs[CONFIG_WORD_FILTER_ENABLE]       = ConfigMgr::GetBoolDefault("WordFilter.Enable", true);
    m_bool_configs[CONFIG_SHARE_ENABLE]             = ConfigMgr::GetBoolDefault("Share.Enable", false);
    m_bool_configs[CONFIG_IPSET_ENABLE]             = ConfigMgr::GetBoolDefault("Ipset.Enable", false);

    m_int_configs[CONFIG_EVENT_ANNOUNCE] = ConfigMgr::GetIntDefault("Event.Announce", 0);

    m_float_configs[CONFIG_CREATURE_FAMILY_FLEE_ASSISTANCE_RADIUS] = ConfigMgr::GetFloatDefault("CreatureFamilyFleeAssistanceRadius", 30.0f);
    m_float_configs[CONFIG_CREATURE_FAMILY_ASSISTANCE_RADIUS] = ConfigMgr::GetFloatDefault("CreatureFamilyAssistanceRadius", 10.0f);
    m_int_configs[CONFIG_CREATURE_FAMILY_ASSISTANCE_DELAY]  = ConfigMgr::GetIntDefault("CreatureFamilyAssistanceDelay", 1500);
    m_int_configs[CONFIG_CREATURE_FAMILY_FLEE_DELAY]        = ConfigMgr::GetIntDefault("CreatureFamilyFleeDelay", 7000);

    m_int_configs[CONFIG_WORLD_BOSS_LEVEL_DIFF] = ConfigMgr::GetIntDefault("WorldBossLevelDiff", 3);

    // note: disable value (-1) will assigned as 0xFFFFFFF, to prevent overflow at calculations limit it to max possible player level MAX_LEVEL(100)
    m_int_configs[CONFIG_QUEST_LOW_LEVEL_HIDE_DIFF] = ConfigMgr::GetIntDefault("Quests.LowLevelHideDiff", 4);
    if (m_int_configs[CONFIG_QUEST_LOW_LEVEL_HIDE_DIFF] > MAX_LEVEL)
        m_int_configs[CONFIG_QUEST_LOW_LEVEL_HIDE_DIFF] = MAX_LEVEL;
    m_int_configs[CONFIG_QUEST_HIGH_LEVEL_HIDE_DIFF] = ConfigMgr::GetIntDefault("Quests.HighLevelHideDiff", 7);
    if (m_int_configs[CONFIG_QUEST_HIGH_LEVEL_HIDE_DIFF] > MAX_LEVEL)
        m_int_configs[CONFIG_QUEST_HIGH_LEVEL_HIDE_DIFF] = MAX_LEVEL;
    m_bool_configs[CONFIG_QUEST_IGNORE_RAID] = ConfigMgr::GetBoolDefault("Quests.IgnoreRaid", false);
    m_bool_configs[CONFIG_QUEST_IGNORE_AUTO_ACCEPT] = ConfigMgr::GetBoolDefault("Quests.IgnoreAutoAccept", false);
    m_bool_configs[CONFIG_QUEST_IGNORE_AUTO_COMPLETE] = ConfigMgr::GetBoolDefault("Quests.IgnoreAutoComplete", false);

    m_int_configs[CONFIG_RANDOM_BG_RESET_HOUR] = ConfigMgr::GetIntDefault("Battleground.Random.ResetHour", 6);
    if (m_int_configs[CONFIG_RANDOM_BG_RESET_HOUR] > 23)
    {
        TC_LOG_ERROR("server", "Battleground.Random.ResetHour (%i) can't be load. Set to 6.", m_int_configs[CONFIG_RANDOM_BG_RESET_HOUR]);
        m_int_configs[CONFIG_RANDOM_BG_RESET_HOUR] = 6;
    }

    m_bool_configs[CONFIG_DETECT_POS_COLLISION] = ConfigMgr::GetBoolDefault("DetectPosCollision", true);

    m_bool_configs[CONFIG_RESTRICTED_LFG_CHANNEL]      = ConfigMgr::GetBoolDefault("Channel.RestrictedLfg", true);
    m_bool_configs[CONFIG_SILENTLY_GM_JOIN_TO_CHANNEL] = ConfigMgr::GetBoolDefault("Channel.SilentlyGMJoin", false);

    m_bool_configs[CONFIG_TALENTS_INSPECTING]           = ConfigMgr::GetBoolDefault("TalentsInspecting", true);
    m_bool_configs[CONFIG_CHAT_FAKE_MESSAGE_PREVENTING] = ConfigMgr::GetBoolDefault("ChatFakeMessagePreventing", false);
    m_int_configs[CONFIG_CHAT_STRICT_LINK_CHECKING_SEVERITY] = ConfigMgr::GetIntDefault("ChatStrictLinkChecking.Severity", 0);
    m_int_configs[CONFIG_CHAT_STRICT_LINK_CHECKING_KICK] = ConfigMgr::GetIntDefault("ChatStrictLinkChecking.Kick", 0);

    m_int_configs[CONFIG_CORPSE_DECAY_NORMAL]    = ConfigMgr::GetIntDefault("Corpse.Decay.NORMAL", 60);
    m_int_configs[CONFIG_CORPSE_DECAY_RARE]      = ConfigMgr::GetIntDefault("Corpse.Decay.RARE", 300);
    m_int_configs[CONFIG_CORPSE_DECAY_ELITE]     = ConfigMgr::GetIntDefault("Corpse.Decay.ELITE", 300);
    m_int_configs[CONFIG_CORPSE_DECAY_RAREELITE] = ConfigMgr::GetIntDefault("Corpse.Decay.RAREELITE", 300);
    m_int_configs[CONFIG_CORPSE_DECAY_WORLDBOSS] = ConfigMgr::GetIntDefault("Corpse.Decay.WORLDBOSS", 3600);

    m_int_configs[CONFIG_DEATH_SICKNESS_LEVEL]           = ConfigMgr::GetIntDefault ("Death.SicknessLevel", 11);
    m_bool_configs[CONFIG_DEATH_CORPSE_RECLAIM_DELAY_PVP] = ConfigMgr::GetBoolDefault("Death.CorpseReclaimDelay.PvP", true);
    m_bool_configs[CONFIG_DEATH_CORPSE_RECLAIM_DELAY_PVE] = ConfigMgr::GetBoolDefault("Death.CorpseReclaimDelay.PvE", true);
    m_bool_configs[CONFIG_DEATH_BONES_WORLD]              = ConfigMgr::GetBoolDefault("Death.Bones.World", true);
    m_bool_configs[CONFIG_DEATH_BONES_BG_OR_ARENA]        = ConfigMgr::GetBoolDefault("Death.Bones.BattlegroundOrArena", true);

    m_bool_configs[CONFIG_DIE_COMMAND_MODE] = ConfigMgr::GetBoolDefault("Die.Command.Mode", true);

    m_float_configs[CONFIG_THREAT_RADIUS] = ConfigMgr::GetFloatDefault("ThreatRadius", 60.0f);

    // always use declined names in the russian client
    m_bool_configs[CONFIG_DECLINED_NAMES_USED] = ConfigMgr::GetBoolDefault("DeclinedNames", false);

    m_float_configs[CONFIG_LISTEN_RANGE_SAY]       = ConfigMgr::GetFloatDefault("ListenRange.Say", 25.0f);
    m_float_configs[CONFIG_LISTEN_RANGE_TEXTEMOTE] = ConfigMgr::GetFloatDefault("ListenRange.TextEmote", 25.0f);
    m_float_configs[CONFIG_LISTEN_RANGE_YELL]      = ConfigMgr::GetFloatDefault("ListenRange.Yell", 300.0f);

    m_bool_configs[CONFIG_BATTLEGROUND_CAST_DESERTER]                = ConfigMgr::GetBoolDefault("Battleground.CastDeserter", true);
    m_bool_configs[CONFIG_BATTLEGROUND_QUEUE_ANNOUNCER_ENABLE]       = ConfigMgr::GetBoolDefault("Battleground.QueueAnnouncer.Enable", false);
    m_bool_configs[CONFIG_BATTLEGROUND_QUEUE_ANNOUNCER_PLAYERONLY]   = ConfigMgr::GetBoolDefault("Battleground.QueueAnnouncer.PlayerOnly", false);
    m_int_configs[CONFIG_BATTLEGROUND_INVITATION_TYPE]               = ConfigMgr::GetIntDefault ("Battleground.InvitationType", 0);
    m_int_configs[CONFIG_BATTLEGROUND_PREMATURE_FINISH_TIMER]        = ConfigMgr::GetIntDefault ("Battleground.PrematureFinishTimer", 5 * MINUTE * IN_MILLISECONDS);
    m_int_configs[CONFIG_BATTLEGROUND_PREMADE_GROUP_WAIT_FOR_MATCH]  = ConfigMgr::GetIntDefault ("Battleground.PremadeGroupWaitForMatch", 30 * MINUTE * IN_MILLISECONDS);
    m_bool_configs[CONFIG_CROSSFACTIONBG]                            = ConfigMgr::GetBoolDefault("MixedBGs", false);
    m_bool_configs[CONFIG_BG_XP_FOR_KILL]                            = ConfigMgr::GetBoolDefault("Battleground.GiveXPForKills", false);
    m_int_configs[CONFIG_ARENA_MAX_RATING_DIFFERENCE]                = ConfigMgr::GetIntDefault ("Arena.MaxRatingDifference", 150);
    m_int_configs[CONFIG_ARENA_RATING_DISCARD_TIMER]                 = ConfigMgr::GetIntDefault ("Arena.RatingDiscardTimer", 10 * MINUTE * IN_MILLISECONDS);
    m_int_configs[CONFIG_ARENA_RATED_UPDATE_TIMER]                   = ConfigMgr::GetIntDefault ("Arena.RatedUpdateTimer", 5 * IN_MILLISECONDS);
    m_bool_configs[CONFIG_ARENA_AUTO_DISTRIBUTE_POINTS]              = ConfigMgr::GetBoolDefault("Arena.AutoDistributePoints", false);
    m_int_configs[CONFIG_ARENA_AUTO_DISTRIBUTE_INTERVAL_DAYS]        = ConfigMgr::GetIntDefault ("Arena.AutoDistributeInterval", 7);
    m_bool_configs[CONFIG_ARENA_QUEUE_ANNOUNCER_ENABLE]              = ConfigMgr::GetBoolDefault("Arena.QueueAnnouncer.Enable", false);
    m_bool_configs[CONFIG_ARENA_QUEUE_ANNOUNCER_PLAYERONLY]          = ConfigMgr::GetBoolDefault("Arena.QueueAnnouncer.PlayerOnly", false);
    m_int_configs[CONFIG_ARENA_SEASON_ID]                            = ConfigMgr::GetIntDefault ("Arena.ArenaSeason.ID", 14);
    m_int_configs[CONFIG_ARENA_START_RATING]                         = ConfigMgr::GetIntDefault ("Arena.ArenaStartRating", 0);
    m_int_configs[CONFIG_ARENA_START_PERSONAL_RATING]                = ConfigMgr::GetIntDefault ("Arena.ArenaStartPersonalRating", 1000);
    m_int_configs[CONFIG_ARENA_START_MATCHMAKER_RATING]              = ConfigMgr::GetIntDefault ("Arena.ArenaStartMatchmakerRating", 1500);
    m_bool_configs[CONFIG_ARENA_SEASON_IN_PROGRESS]                  = ConfigMgr::GetBoolDefault("Arena.ArenaSeason.InProgress", true);
    m_bool_configs[CONFIG_ARENA_LOG_EXTENDED_INFO]                   = ConfigMgr::GetBoolDefault("ArenaLog.ExtendedInfo", false);
    m_int_configs[CONFIG_PVP_ITEM_LEVEL_CAP]                         = ConfigMgr::GetIntDefault ("PVP.ItemLevelCap", 540);

    m_bool_configs[CONFIG_OFFHAND_CHECK_AT_SPELL_UNLEARN]            = ConfigMgr::GetBoolDefault("OffhandCheckAtSpellUnlearn", true);

    if (int32 clientCacheId = ConfigMgr::GetIntDefault("ClientCacheVersion", 0))
    {
        // overwrite DB/old value
        if (clientCacheId > 0)
        {
            m_int_configs[CONFIG_CLIENTCACHE_VERSION] = clientCacheId;
            TC_LOG_INFO("server", "Client cache version set to: %u", clientCacheId);
        }
        else
            TC_LOG_ERROR("server", "ClientCacheVersion can't be negative %d, ignored.", clientCacheId);
    }

    m_int_configs[CONFIG_INSTANT_LOGOUT] = ConfigMgr::GetIntDefault("InstantLogout", SEC_MODERATOR);

    m_int_configs[CONFIG_GUILD_EVENT_LOG_COUNT] = ConfigMgr::GetIntDefault("Guild.EventLogRecordsCount", GUILD_EVENTLOG_MAX_RECORDS);
    if (m_int_configs[CONFIG_GUILD_EVENT_LOG_COUNT] > GUILD_EVENTLOG_MAX_RECORDS)
        m_int_configs[CONFIG_GUILD_EVENT_LOG_COUNT] = GUILD_EVENTLOG_MAX_RECORDS;
    m_int_configs[CONFIG_GUILD_BANK_EVENT_LOG_COUNT] = ConfigMgr::GetIntDefault("Guild.BankEventLogRecordsCount", GUILD_BANKLOG_MAX_RECORDS);
    if (m_int_configs[CONFIG_GUILD_BANK_EVENT_LOG_COUNT] > GUILD_BANKLOG_MAX_RECORDS)
        m_int_configs[CONFIG_GUILD_BANK_EVENT_LOG_COUNT] = GUILD_BANKLOG_MAX_RECORDS;

    //visibility on continents
    m_MaxVisibleDistanceOnContinents = ConfigMgr::GetFloatDefault("Visibility.Distance.Continents", DEFAULT_VISIBILITY_DISTANCE);
    if (m_MaxVisibleDistanceOnContinents < DEFAULT_VISIBILITY_DISTANCE*sWorld->getRate(RATE_CREATURE_AGGRO))
    {
        TC_LOG_ERROR("server", "Visibility.Distance.Continents can't be less max aggro radius %f", DEFAULT_VISIBILITY_DISTANCE*sWorld->getRate(RATE_CREATURE_AGGRO));
        m_MaxVisibleDistanceOnContinents = DEFAULT_VISIBILITY_DISTANCE*sWorld->getRate(RATE_CREATURE_AGGRO);
    }
    else if (m_MaxVisibleDistanceOnContinents > MAX_VISIBILITY_DISTANCE)
    {
        TC_LOG_ERROR("server", "Visibility.Distance.Continents can't be greater %f", MAX_VISIBILITY_DISTANCE);
        m_MaxVisibleDistanceOnContinents = MAX_VISIBILITY_DISTANCE;
    }

    m_visibilityRelocationLowerLimit = ConfigMgr::GetFloatDefault("Visibility.RelocationLowerLimit", 10.f);
    m_visibilityAINotifyDelay = ConfigMgr::GetIntDefault("Visibility.AINotifyDelay", DEFAULT_VISIBILITY_NOTIFY_PERIOD);

    //visibility in instances
    m_MaxVisibleDistanceInInstances = ConfigMgr::GetFloatDefault("Visibility.Distance.Instances", DEFAULT_VISIBILITY_DISTANCE);
    if (m_MaxVisibleDistanceInInstances < DEFAULT_VISIBILITY_DISTANCE*sWorld->getRate(RATE_CREATURE_AGGRO))
    {
        TC_LOG_ERROR("server", "Visibility.Distance.Instances can't be less max aggro radius %f", DEFAULT_VISIBILITY_DISTANCE*sWorld->getRate(RATE_CREATURE_AGGRO));
        m_MaxVisibleDistanceInInstances = DEFAULT_VISIBILITY_DISTANCE*sWorld->getRate(RATE_CREATURE_AGGRO);
    }
    else if (m_MaxVisibleDistanceInInstances > MAX_VISIBILITY_DISTANCE)
    {
        TC_LOG_ERROR("server", "Visibility.Distance.Instances can't be greater %f", MAX_VISIBILITY_DISTANCE);
        m_MaxVisibleDistanceInInstances = MAX_VISIBILITY_DISTANCE;
    }

    //visibility in BG
    m_MaxVisibleDistanceInBG = ConfigMgr::GetFloatDefault("Visibility.Distance.BG", DEFAULT_VISIBILITY_BGARENAS);
    if (m_MaxVisibleDistanceInBG < 45*sWorld->getRate(RATE_CREATURE_AGGRO))
    {
        TC_LOG_ERROR("server", "Visibility.Distance.BG can't be less max aggro radius %f", 45*sWorld->getRate(RATE_CREATURE_AGGRO));
        m_MaxVisibleDistanceInBG = 45*sWorld->getRate(RATE_CREATURE_AGGRO);
    }
    else if (m_MaxVisibleDistanceInBG > MAX_VISIBILITY_DISTANCE)
    {
        TC_LOG_ERROR("server", "Visibility.Distance.BG can't be greater %f", MAX_VISIBILITY_DISTANCE);
        m_MaxVisibleDistanceInBG = MAX_VISIBILITY_DISTANCE;
    }

    //visibility in Arenas
    m_MaxVisibleDistanceInArenas = ConfigMgr::GetFloatDefault("Visibility.Distance.Arenas", DEFAULT_VISIBILITY_BGARENAS);
    if (m_MaxVisibleDistanceInArenas < 45*sWorld->getRate(RATE_CREATURE_AGGRO))
    {
        TC_LOG_ERROR("server", "Visibility.Distance.Arenas can't be less max aggro radius %f", 45*sWorld->getRate(RATE_CREATURE_AGGRO));
        m_MaxVisibleDistanceInArenas = 45*sWorld->getRate(RATE_CREATURE_AGGRO);
    }
    else if (m_MaxVisibleDistanceInArenas > MAX_VISIBILITY_DISTANCE)
    {
        TC_LOG_ERROR("server", "Visibility.Distance.Arenas can't be greater %f", MAX_VISIBILITY_DISTANCE);
        m_MaxVisibleDistanceInArenas = MAX_VISIBILITY_DISTANCE;
    }

    m_visibility_notify_periodOnContinents = ConfigMgr::GetIntDefault("Visibility.Notify.Period.OnContinents", DEFAULT_VISIBILITY_NOTIFY_PERIOD);
    m_visibility_notify_periodInInstances = ConfigMgr::GetIntDefault("Visibility.Notify.Period.InInstances",   DEFAULT_VISIBILITY_NOTIFY_PERIOD);
    m_visibility_notify_periodInBGArenas = ConfigMgr::GetIntDefault("Visibility.Notify.Period.InBGArenas",    DEFAULT_VISIBILITY_NOTIFY_PERIOD);

    ZoneUpdateDistanceRangeLimit = ConfigMgr::GetFloatDefault("Zone.UpdateDistanceRage", 5.f);

    //dynamic visibility
    m_int_configs[CONFIG_MAX_ZONES_DIFF] = ConfigMgr::GetIntDefault("DynamicVisibility.MaxZonesDiff", 30);
    m_int_configs[CONFIG_CRITICAL_ZONES_DIFF] = ConfigMgr::GetIntDefault("DynamicVisibility.CriticalZonesDiff", 150);
    m_int_configs[CONFIG_MAX_POSSIBLE_VISIBILITY_RANGE] = ConfigMgr::GetIntDefault("DynamicVisibility.MaxPossibleVisibilityRange", 100);
    m_int_configs[CONFIG_HANDLE_VISIBILITY_TIMER] = ConfigMgr::GetIntDefault("DynamicVisibility.HandleVisibilityTimer", 20000);

    ///- Load the CharDelete related config options
    m_int_configs[CONFIG_CHARDELETE_METHOD] = ConfigMgr::GetIntDefault("CharDelete.Method", 0);
    m_int_configs[CONFIG_CHARDELETE_MIN_LEVEL] = ConfigMgr::GetIntDefault("CharDelete.MinLevel", 0);
    m_int_configs[CONFIG_CHARDELETE_KEEP_DAYS] = ConfigMgr::GetIntDefault("CharDelete.KeepDays", 30);

    ///- Read the "Data" directory from the config file
    std::string dataPath = ConfigMgr::GetStringDefault("DataDir", "./");
    if (dataPath.at(dataPath.length()-1) != '/' && dataPath.at(dataPath.length()-1) != '\\')
        dataPath.push_back('/');

    if (reload)
    {
        if (dataPath != m_dataPath)
            TC_LOG_ERROR("server", "DataDir option can't be changed at worldserver.conf reload, using current value (%s).", m_dataPath.c_str());
    }
    else
    {
        m_dataPath = dataPath;
        TC_LOG_INFO("server", "Using DataDir %s", m_dataPath.c_str());
    }

    // MMap related
    m_bool_configs[CONFIG_ENABLE_MMAPS] = ConfigMgr::GetBoolDefault("mmap.enablePathFinding", true);
    TC_LOG_INFO("server", "WORLD: MMap data directory is: %smmaps", m_dataPath.c_str());

    m_bool_configs[CONFIG_VMAP_INDOOR_CHECK] = ConfigMgr::GetBoolDefault("vmap.enableIndoorCheck", 0);
    bool enableIndoor = ConfigMgr::GetBoolDefault("vmap.enableIndoorCheck", true);
    bool enableLOS = ConfigMgr::GetBoolDefault("vmap.enableLOS", true);
    bool enableHeight = ConfigMgr::GetBoolDefault("vmap.enableHeight", true);
    bool enablePetLOS = ConfigMgr::GetBoolDefault("vmap.petLOS", true);

    if (!enableHeight)
        TC_LOG_ERROR("server", "VMap height checking disabled! Creatures movements and other various things WILL be broken! Expect no support.");

    VMAP::VMapFactory::createOrGetVMapManager()->setEnableLineOfSightCalc(enableLOS);
    VMAP::VMapFactory::createOrGetVMapManager()->setEnableHeightCalc(enableHeight);
    TC_LOG_INFO("server", "VMap support included. LineOfSight:%i, getHeight:%i, indoorCheck:%i PetLOS:%i", enableLOS, enableHeight, enableIndoor, enablePetLOS);
    TC_LOG_INFO("server", "VMap data directory is: %svmaps", m_dataPath.c_str());

    m_int_configs[CONFIG_MAX_WHO] = ConfigMgr::GetIntDefault("MaxWhoListReturns", 49);
    m_bool_configs[CONFIG_LIMIT_WHO_ONLINE] = ConfigMgr::GetBoolDefault("LimitWhoOnline", true);
    m_bool_configs[CONFIG_PET_LOS] = ConfigMgr::GetBoolDefault("vmap.petLOS", true);
    m_bool_configs[CONFIG_START_ALL_SPELLS] = ConfigMgr::GetBoolDefault("PlayerStart.AllSpells", false);
    if (m_bool_configs[CONFIG_START_ALL_SPELLS])
        TC_LOG_WARN("server", "PlayerStart.AllSpells enabled - may not function as intended!");
    m_int_configs[CONFIG_HONOR_AFTER_DUEL] = ConfigMgr::GetIntDefault("HonorPointsAfterDuel", 0);
    m_bool_configs[CONFIG_START_ALL_EXPLORED] = ConfigMgr::GetBoolDefault("PlayerStart.MapsExplored", false);
    m_bool_configs[CONFIG_START_ALL_REP] = ConfigMgr::GetBoolDefault("PlayerStart.AllReputation", false);
    m_bool_configs[CONFIG_ALWAYS_MAXSKILL] = ConfigMgr::GetBoolDefault("AlwaysMaxWeaponSkill", false);
    m_bool_configs[CONFIG_PVP_TOKEN_ENABLE] = ConfigMgr::GetBoolDefault("PvPToken.Enable", false);
    m_int_configs[CONFIG_PVP_TOKEN_MAP_TYPE] = ConfigMgr::GetIntDefault("PvPToken.MapAllowType", 4);
    m_int_configs[CONFIG_PVP_TOKEN_ID] = ConfigMgr::GetIntDefault("PvPToken.ItemID", 29434);
    m_int_configs[CONFIG_PVP_TOKEN_COUNT] = ConfigMgr::GetIntDefault("PvPToken.ItemCount", 1);
    if (m_int_configs[CONFIG_PVP_TOKEN_COUNT] < 1)
        m_int_configs[CONFIG_PVP_TOKEN_COUNT] = 1;

    m_bool_configs[CONFIG_NO_RESET_TALENT_COST] = ConfigMgr::GetBoolDefault("NoResetTalentsCost", false);
    m_bool_configs[CONFIG_SHOW_KICK_IN_WORLD] = ConfigMgr::GetBoolDefault("ShowKickInWorld", false);
    m_int_configs[CONFIG_INTERVAL_LOG_UPDATE] = ConfigMgr::GetIntDefault("RecordUpdateTimeDiffInterval", 60000);
    m_int_configs[CONFIG_MIN_LOG_UPDATE] = ConfigMgr::GetIntDefault("MinRecordUpdateTimeDiff", 100);
    m_int_configs[CONFIG_NUMTHREADS] = ConfigMgr::GetIntDefault("MapUpdate.Threads", 1);
    m_int_configs[CONFIG_MAX_RESULTS_LOOKUP_COMMANDS] = ConfigMgr::GetIntDefault("Command.LookupMaxResults", 0);

    // chat logging
    m_bool_configs[CONFIG_CHATLOG_CHANNEL] = ConfigMgr::GetBoolDefault("ChatLogs.Channel", false);
    m_bool_configs[CONFIG_CHATLOG_WHISPER] = ConfigMgr::GetBoolDefault("ChatLogs.Whisper", false);
    m_bool_configs[CONFIG_CHATLOG_SYSCHAN] = ConfigMgr::GetBoolDefault("ChatLogs.SysChan", false);
    m_bool_configs[CONFIG_CHATLOG_PARTY] = ConfigMgr::GetBoolDefault("ChatLogs.Party", false);
    m_bool_configs[CONFIG_CHATLOG_RAID] = ConfigMgr::GetBoolDefault("ChatLogs.Raid", false);
    m_bool_configs[CONFIG_CHATLOG_GUILD] = ConfigMgr::GetBoolDefault("ChatLogs.Guild", false);
    m_bool_configs[CONFIG_CHATLOG_PUBLIC] = ConfigMgr::GetBoolDefault("ChatLogs.Public", false);
    m_bool_configs[CONFIG_CHATLOG_ADDON] = ConfigMgr::GetBoolDefault("ChatLogs.Addon", false);
    m_bool_configs[CONFIG_CHATLOG_BGROUND] = ConfigMgr::GetBoolDefault("ChatLogs.Battleground", false);

    // Warden
    m_bool_configs[CONFIG_WARDEN_ENABLED] = ConfigMgr::GetBoolDefault("Warden.Enabled", false);
    m_bool_configs[CONFIG_WARDEN_ENABLED_DYNAMIC_CHECKS] = ConfigMgr::GetBoolDefault("Warden.EnabledDynamicChecks", false);
    m_bool_configs[CONFIG_WARDEN_ENABLE_CHECK_BAD_VERSION] = ConfigMgr::GetBoolDefault("Warden.EnableCheckBadVersion", false);
    m_int_configs[CONFIG_WARDEN_NUM_MEM_CHECKS] = ConfigMgr::GetIntDefault("Warden.NumMemChecks", 10);
    m_int_configs[CONFIG_WARDEN_NUM_OTHER_CHECKS] = ConfigMgr::GetIntDefault("Warden.NumOtherChecks", 10);
    m_int_configs[CONFIG_WARDEN_CLIENT_CHECK_HOLDOFF] = ConfigMgr::GetIntDefault("Warden.ClientCheckHoldOff", 20000);
    m_int_configs[CONFIG_WARDEN_CLIENT_RESPONSE_DELAY] = ConfigMgr::GetIntDefault("Warden.ClientResponseDelay", 60000);
    m_int_configs[CONFIG_WARDEN_CLIENT_BAN_DURATION] = ConfigMgr::GetIntDefault("Warden.BanDuration", 86400);
    m_int_configs[CONFIG_WARDEN_CLIENT_FAIL_ACTION] = ConfigMgr::GetIntDefault("Warden.ClientCheckFailAction", 0);
    m_int_configs[CONFIG_WARDEN_CLIENT_RESPONSE_DELAY_ALERTS] = ConfigMgr::GetIntDefault("Warden.ClientResponseDelayAlerts", 2);
    m_int_configs[CONFIG_WARDEN_NUM_SPEED_ALERTS] = ConfigMgr::GetIntDefault("Warden.NumSpeedAlerts", 5);
    m_int_configs[CONFIG_WARDEN_NUM_SPEED_EXT_ALERTS] = ConfigMgr::GetIntDefault("Warden.NumSpeedExtAlerts", 5);
    m_int_configs[CONFIG_WARDEN_NUM_MOVEFLAGS_ALERTS] = ConfigMgr::GetIntDefault("Warden.NumMoveFlagsAlerts", 5);
    m_int_configs[CONFIG_WARDEN_NUM_FALIED_COORDS_ALERTS] = ConfigMgr::GetIntDefault("Warden.NumFaliedCoordsAlerts", 5);

    m_float_configs[CONFIG_WARDEN_Z_AXIS_DELTA] = ConfigMgr::GetFloatDefault("Warden.ZAxisDelta", 5.0f);
    m_float_configs[CONFIG_WARDEN_MAX_TP_DIST] = ConfigMgr::GetFloatDefault("Warden.MaxTeleportDist", 35.0f);

    m_bool_configs[CONFIG_WARDEN_USES_EXP_SYSTEM] = ConfigMgr::GetBoolDefault("Warden.UseExpSystemMovementChecks", false);
    m_bool_configs[CONFIG_WARDEN_USES_Z_AXIS_CHECK] = ConfigMgr::GetBoolDefault("Warden.UseZAxisMovementChecks", false);

    // Dungeon finder
    m_int_configs[CONFIG_LFG_OPTIONSMASK] = ConfigMgr::GetIntDefault("DungeonFinder.OptionsMask", 1);
    m_bool_configs[CONFIG_LFG_DEBUG_JOIN] = ConfigMgr::GetBoolDefault("DungeonFinder.DebugJoin", false);
    m_bool_configs[CONFIG_LFG_FORCE_MINPLAYERS] = ConfigMgr::GetBoolDefault("DungeonFinder.ForceMinplayers", false);

    m_bool_configs[CONFIG_GROUP_BONUS_XP] = ConfigMgr::GetBoolDefault("Group.BonusXp", false);
    m_bool_configs[CONFIG_FUN_OPTION_ENABLED] = ConfigMgr::GetIntDefault("FunOption", false);

    // DBC_ItemAttributes
    m_bool_configs[CONFIG_DBC_ENFORCE_ITEM_ATTRIBUTES] = ConfigMgr::GetBoolDefault("DBC.EnforceItemAttributes", true);

    // Max instances per hour
    m_int_configs[CONFIG_MAX_INSTANCES_PER_HOUR] = ConfigMgr::GetIntDefault("AccountInstancesPerHour", 50);

    // AutoBroadcast
    m_bool_configs[CONFIG_AUTOBROADCAST] = ConfigMgr::GetBoolDefault("AutoBroadcast.On", false);
    m_int_configs[CONFIG_AUTOBROADCAST_CENTER] = ConfigMgr::GetIntDefault("AutoBroadcast.Center", 0);
    m_int_configs[CONFIG_AUTOBROADCAST_INTERVAL] = ConfigMgr::GetIntDefault("AutoBroadcast.Timer", 60000);

    if (reload)
    {
        m_timers[WUPDATE_AUTOBROADCAST].SetInterval(m_int_configs[CONFIG_AUTOBROADCAST_INTERVAL]);
        m_timers[WUPDATE_AUTOBROADCAST].Reset();
    }

    // MySQL ping time interval
    m_int_configs[CONFIG_DB_PING_INTERVAL] = ConfigMgr::GetIntDefault("MaxPingTime", 30);

    // Guild save interval
    m_bool_configs[CONFIG_GUILD_LEVELING_ENABLED] = ConfigMgr::GetBoolDefault("Guild.LevelingEnabled", true);
    m_int_configs[CONFIG_GUILD_SAVE_INTERVAL] = ConfigMgr::GetIntDefault("Guild.SaveInterval", 15);
    m_int_configs[CONFIG_GUILD_MAX_LEVEL] = ConfigMgr::GetIntDefault("Guild.MaxLevel", 25);
    m_int_configs[CONFIG_GUILD_UNDELETABLE_LEVEL] = ConfigMgr::GetIntDefault("Guild.UndeletableLevel", 4);
    rate_values[RATE_XP_GUILD_MODIFIER] = ConfigMgr::GetFloatDefault("Guild.XPModifier", 0.25f);
    m_int_configs[CONFIG_GUILD_DAILY_XP_CAP] = ConfigMgr::GetIntDefault("Guild.DailyXPCap", 7807500);
    m_int_configs[CONFIG_GUILD_WEEKLY_REP_CAP] = ConfigMgr::GetIntDefault("Guild.WeeklyReputationCap", 4375);

    // misc
    m_bool_configs[CONFIG_PDUMP_NO_PATHS] = ConfigMgr::GetBoolDefault("PlayerDump.DisallowPaths", true);
    m_bool_configs[CONFIG_PDUMP_NO_OVERWRITE] = ConfigMgr::GetBoolDefault("PlayerDump.DisallowOverwrite", true);

    //Auto restart system
    m_int_configs[CONFIG_AUTO_SERVER_RESTART_HOUR] = ConfigMgr::GetIntDefault("Server.Auto.RestartHour", 5);
    if (m_int_configs[CONFIG_AUTO_SERVER_RESTART_HOUR] > 23)
    {
        m_int_configs[CONFIG_AUTO_SERVER_RESTART_HOUR] = 5;
    }
    m_bool_configs[CONFIG_DISABLE_RESTART] = ConfigMgr::GetBoolDefault("DisableRestart", true);
    m_bool_configs[CONFIG_DISABLE_NEW_ONLINE] = ConfigMgr::GetBoolDefault("DisableUpdateOnlineTable", false);

    // call ScriptMgr if we're reloading the configuration
    m_bool_configs[CONFIG_WINTERGRASP_ENABLE] = ConfigMgr::GetBoolDefault("Wintergrasp.Enable", false);
    m_int_configs[CONFIG_WINTERGRASP_PLR_MAX] = ConfigMgr::GetIntDefault("Wintergrasp.PlayerMax", 100);
    m_int_configs[CONFIG_WINTERGRASP_PLR_MIN] = ConfigMgr::GetIntDefault("Wintergrasp.PlayerMin", 0);
    m_int_configs[CONFIG_WINTERGRASP_PLR_MIN_LVL] = ConfigMgr::GetIntDefault("Wintergrasp.PlayerMinLvl", 77);
    m_int_configs[CONFIG_WINTERGRASP_BATTLETIME] = ConfigMgr::GetIntDefault("Wintergrasp.BattleTimer", 30);
    m_int_configs[CONFIG_WINTERGRASP_NOBATTLETIME] = ConfigMgr::GetIntDefault("Wintergrasp.NoBattleTimer", 150);
    m_int_configs[CONFIG_WINTERGRASP_RESTART_AFTER_CRASH] = ConfigMgr::GetIntDefault("Wintergrasp.CrashRestartTimer", 10);

    // Tol'Barad
    m_bool_configs[CONFIG_TOL_BARAD_ENABLE] = ConfigMgr::GetBoolDefault("Tol Barad.Enable", false);
    m_int_configs[CONFIG_TOL_BARAD_PLR_MAX] = ConfigMgr::GetIntDefault("Tol Barad.PlayerMax", 100);
    m_int_configs[CONFIG_TOL_BARAD_PLR_MIN] = ConfigMgr::GetIntDefault("Tol Barad.PlayerMin", 0);
    m_int_configs[CONFIG_TOL_BARAD_PLR_MIN_LVL] = ConfigMgr::GetIntDefault("Tol Barad.PlayerMinLvl", 80);
    m_int_configs[CONFIG_TOL_BARAD_BATTLETIME] = ConfigMgr::GetIntDefault("Tol Barad.BattleTimer", 30);
    m_int_configs[CONFIG_TOL_BARAD_NOBATTLETIME] = ConfigMgr::GetIntDefault("Tol Barad.NoBattleTimer", 150);

    //Announce server for a ban    
    m_bool_configs[CONFIG_ANNOUNCE_BAN] = ConfigMgr::GetBoolDefault("AnnounceBan", false);
    m_bool_configs[CONFIG_ANNOUNCE_MUTE] = ConfigMgr::GetBoolDefault("AnnounceMute", false);
    m_bool_configs[CONFIG_SPELL_FORBIDDEN] = ConfigMgr::GetBoolDefault("SpellForbidden", false);

    // Archaeology
    m_bool_configs[CONFIG_ARCHAEOLOGY_ENABLED] = ConfigMgr::GetBoolDefault("Archaeology.Enabled", true);
    m_float_configs[CONFIG_ARCHAEOLOGY_RARE_BASE_CHANCE] = ConfigMgr::GetFloatDefault("Archaeology.RareBaseChance", 10.0f);
    m_float_configs[CONFIG_ARCHAEOLOGY_RARE_MAXLEVEL_CHANCE] = ConfigMgr::GetFloatDefault("Archaeology.RareMaxLevelChance", 10.0f);

    // Blizzard Shop
    m_bool_configs[CONFIG_PURCHASE_SHOP_ENABLED] = ConfigMgr::GetBoolDefault("Purchase.Shop.Enabled", true);

    // Battle Pets
    m_bool_configs[CONFIG_PET_BATTLES_ENABLED] = ConfigMgr::GetBoolDefault("PetBattles.Enabled", true);

    // Custom content
    m_bool_configs[CONFIG_CUSTOM_CONTENT_ENABLED] = ConfigMgr::GetBoolDefault("Custom.Content.Enabled", false);
    m_int_configs[CONFIG_CUSTOM_CONTENT_VERSION] = ConfigMgr::GetIntDefault("Custom.Content.Version", 548);

    // Test new damage from dbc
    m_bool_configs[CONFIG_DAMAGE_ENABLE_FROM_DBC] = ConfigMgr::GetBoolDefault("Damage.Dbc.Enabled", false);

    // Faster for loading via disable some functions like GOs, some spawns, etc
    // only for debugging
    m_bool_configs[CONFIG_FASTER_LOADING] = ConfigMgr::GetBoolDefault("FasterLoading.Enabled", false);

    //m_bool_configs[CONFIG_ENABLE_FLYING_CHECK] = ConfigMgr::GetBoolDefault("MovementSync.FlyingCheckEnabled", false);
    //m_bool_configs[CONFIG_ENABLE_WATERWALK_CHECK] = ConfigMgr::GetBoolDefault("MovementSync.WaterwalkCheckEnabled", false);
    //m_bool_configs[CONFIG_ENABLE_HOVER_CHECK] = ConfigMgr::GetBoolDefault("MovementSync.HoverCheckEnabled", false);
    //m_bool_configs[CONFIG_ENABLE_TELEPORT_CHECK] = ConfigMgr::GetBoolDefault("MovementSync.TeleportCheckEnabled", false);
    
    // custom events
    m_bool_configs[CONFIG_CUSTOM_BATTLEGROUND] = ConfigMgr::GetBoolDefault("Custom.BattleGround.event", false);
    m_bool_configs[CONFIG_CUSTOM_X20] = ConfigMgr::GetBoolDefault("Custom.Achievement.x20", false);
    m_bool_configs[CONFIG_CUSTOM_FOOTBALL] = ConfigMgr::GetBoolDefault("Custom.Football", false);
    
    m_bool_configs[CONFIG_ANTI_FLOOD_LFG] = ConfigMgr::GetBoolDefault("Anti.Flood.on.LFG", false);

    if (reload)
        sScriptMgr->OnConfigLoad(reload);
}

/// Initialize the World
void World::SetInitialWorldSettings()
{
    ///- Server startup begin
    uint32 startupBegin = getMSTime();

    ///- Initialize the random number generator
    srand((unsigned int)time(NULL));

    ///- Initialize detour memory management
    dtAllocSetCustom(dtCustomAlloc, dtCustomFree);

    ///- Initialize config settings
    LoadConfigSettings();

    ///- Initialize Allowed Security Level
    LoadDBAllowedSecurityLevel();

    ///- Initialize VMapManager function pointers (to untangle game/collision circular deps)
    if (VMAP::VMapManager2* vmmgr2 = dynamic_cast<VMAP::VMapManager2*>(VMAP::VMapFactory::createOrGetVMapManager()))
    {
        vmmgr2->GetLiquidFlagsPtr = &GetLiquidFlags;
        vmmgr2->IsVMAPDisabledForPtr = &DisableMgr::IsVMAPDisabledFor;
    }

    ///- Init highest guids before any table loading to prevent using not initialized guids in some code.
    sObjectMgr->SetHighestGuids();

    ///- Check the existence of the map files for all races' startup areas.
    if (!MapManager::ExistMapAndVMap(0, -6240.32f, 331.033f)
        || !MapManager::ExistMapAndVMap(0, -8949.95f, -132.493f)
        || !MapManager::ExistMapAndVMap(1, -618.518f, -4251.67f)
        || !MapManager::ExistMapAndVMap(0, 1676.35f, 1677.45f)
        || !MapManager::ExistMapAndVMap(1, 10311.3f, 832.463f)
        || !MapManager::ExistMapAndVMap(1, -2917.58f, -257.98f)
        || (m_int_configs[CONFIG_EXPANSION] && (
            !MapManager::ExistMapAndVMap(530, 10349.6f, -6357.29f) ||
            !MapManager::ExistMapAndVMap(530, -3961.64f, -13931.2f))))
    {
        TC_LOG_ERROR("server", "Correct *.map files not found in path '%smaps' or *.vmtree/*.vmtile files in '%svmaps'. Please place *.map/*.vmtree/*.vmtile files in appropriate directories or correct the DataDir value in the worldserver.conf file.", m_dataPath.c_str(), m_dataPath.c_str());
        exit(1);
    }

    ///- Initialize pool manager
    sPoolMgr->Initialize();

    ///- Initialize game event manager
    sGameEventMgr->Initialize();

    ///- Initialize warden and anticheat systems
    if (m_bool_configs[CONFIG_WARDEN_ENABLED])
    {
        TC_LOG_INFO("server", "---INITIALIZE WARDEN SYSTEM---");
        TC_LOG_INFO("server", "Loading Warden Modules...");
        _wardenMgr->LoadWardenModules("Win");
        _wardenMgr->LoadWardenModules("OSX");

        TC_LOG_INFO("server", "Loading Warden Checks...");
        _wardenMgr->LoadWardenChecks();

        TC_LOG_INFO("server", "Loading Warden Action Overrides...");
        _wardenMgr->LoadWardenOverrides();

        TC_LOG_INFO("server", "Loading Warden Custom MPQ Data...");
        _wardenMgr->LoadWardenCustomMPQData();
    }

    TC_LOG_INFO("server", "---INITIALIZE ANTICHEAT SYSTEM---");
    TC_LOG_INFO("server", "Load anticheat data...");

    ///- Loading strings. Getting no records means core load has to be canceled because no error message can be output.

    TC_LOG_INFO("server", "Loading Trinity strings...");
    if (!sObjectMgr->LoadTrinityStrings())
        exit(1);                                            // Error message displayed in function already

    ///- Update the realm entry in the database with the realm type from the config file
    //No SQL injection as values are treated as integers

    // not send custom type REALM_FFA_PVP to realm list
    uint32 server_type = IsFFAPvPRealm() ? uint32(REALM_TYPE_PVP) : getIntConfig(CONFIG_GAME_TYPE);
    uint32 realm_zone = getIntConfig(CONFIG_REALM_ZONE);

    LoginDatabase.PExecute("UPDATE realmlist SET icon = %u, timezone = %u WHERE id = '%d'", server_type, realm_zone, realmID);      // One-time query

    ///- Remove the bones (they should not exist in DB though) and old corpses after a restart
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_OLD_CORPSES);
    stmt->setUInt32(0, 3 * DAY);
    CharacterDatabase.Execute(stmt);

    TC_LOG_INFO("server", "Starting thread pool manager");
    sThreadPoolMgr->start(getIntConfig(CONFIG_NUMTHREADS));

    ///- Load the DBC files
    TC_LOG_INFO("server", "Initialize data stores...");
    LoadDBCStores(m_dataPath);
    LoadDB2Stores(m_dataPath);
    DetectDBCLang();

    std::vector<uint32> mapIds;
    for (uint32 mapId = 0; mapId < sMapStore.GetNumRows(); mapId++)
        if (sMapStore.LookupEntry(mapId))
            mapIds.push_back(mapId);

    if (VMAP::VMapManager2* vmmgr2 = dynamic_cast<VMAP::VMapManager2*>(VMAP::VMapFactory::createOrGetVMapManager()))
        vmmgr2->InitializeThreadUnsafe(mapIds);

    MMAP::MMapManager* mmmgr = MMAP::MMapFactory::createOrGetMMapManager();
    mmmgr->InitializeThreadUnsafe(mapIds);

    TC_LOG_INFO("server", "Loading SpellInfo store...");
    sSpellMgr->LoadSpellInfoStore();

    TC_LOG_INFO("server", "Loading TalentSpellInfo store....");
    sSpellMgr->LoadTalentSpellInfo();

    TC_LOG_INFO("server", "Loading SpellPowerInfo store....");
    sSpellMgr->LoadSpellPowerInfo();

    TC_LOG_INFO("server", "Loading SkillLineAbilityMultiMap Data...");
    sSpellMgr->LoadSkillLineAbilityMap();

    TC_LOG_INFO("server", "Loading spell custom attributes...");
    sSpellMgr->LoadSpellCustomAttr();

    TC_LOG_INFO("server", "Loading GameObject models...");
    LoadGameObjectModelList(m_dataPath);

    TC_LOG_INFO("server", "Loading World Visible Distance...");
    sObjectMgr->LoadWorldVisibleDistance();

    TC_LOG_INFO("server", "Loading Script Names...");
    sObjectMgr->LoadScriptNames();

    TC_LOG_INFO("server", "Loading Instance Template...");
    sObjectMgr->LoadInstanceTemplate();

    // Must be called before `creature_respawn`/`gameobject_respawn` tables
    TC_LOG_INFO("server", "Loading instances...");
    sInstanceSaveMgr->LoadInstances();

    TC_LOG_INFO("server", "Restructuring Creatures GUIDs...");
    //sObjectMgr->RestructCreatureGUID(10000);

    TC_LOG_INFO("server", "Restructuring Gameobjects GUIDs...");
    //sObjectMgr->RestructGameObjectGUID(10000);

    TC_LOG_INFO("server", "Loading Localization strings...");
    uint32 oldMSTime = getMSTime();
    
    if (!getBoolConfig(CONFIG_FASTER_LOADING))
    {
        sObjectMgr->LoadCreatureLocales();
        sObjectMgr->LoadGameObjectLocales();
        sObjectMgr->LoadItemLocales();
        sObjectMgr->LoadQuestLocales();
        sObjectMgr->LoadNpcTextLocales();
        sObjectMgr->LoadPageTextLocales();
        sObjectMgr->LoadGossipMenuItemsLocales();
        sObjectMgr->LoadPointOfInterestLocales();
    }

    sObjectMgr->SetDBCLocaleIndex(GetDefaultDbcLocale());        // Get once for all the locale index of DBC language (console/broadcasts)
    TC_LOG_INFO("server", ">> Localization strings loaded in %u ms", GetMSTimeDiffToNow(oldMSTime));

    TC_LOG_INFO("server","Loading Letter Analogs...");
    sWordFilterMgr->LoadLetterAnalogs();

    TC_LOG_INFO("server","Loading Bad Words...");
    sWordFilterMgr->LoadBadWords();

    if (!getBoolConfig(CONFIG_FASTER_LOADING))
    {
        TC_LOG_INFO("server", "Loading Page Texts...");
        sObjectMgr->LoadPageTexts();
    }

    TC_LOG_INFO("server", "Loading Game Object Templates...");         // must be after LoadPageTexts
    sObjectMgr->LoadGameObjectTemplate();

    TC_LOG_INFO("server", "Loading Spell Rank Data...");
    sSpellMgr->LoadSpellRanks();

    TC_LOG_INFO("server", "Loading Spell Required Data...");
    sSpellMgr->LoadSpellRequired();

    TC_LOG_INFO("server", "Loading Spell Group types...");
    sSpellMgr->LoadSpellGroups();

    TC_LOG_INFO("server", "Loading Spell Learn Skills...");
    sSpellMgr->LoadSpellLearnSkills();                           // must be after LoadSpellRanks

    TC_LOG_INFO("server", "Loading Spell Learn Spells...");
    sSpellMgr->LoadSpellLearnSpells();

    TC_LOG_INFO("server", "Loading Spell Proc Event conditions...");
    sSpellMgr->LoadSpellProcEvents();

    TC_LOG_INFO("server", "Loading Spell Proc conditions and data...");
    sSpellMgr->LoadSpellProcs();

    TC_LOG_INFO("server", "Loading Spell Bonus Data...");
    sSpellMgr->LoadSpellBonusess();

    TC_LOG_INFO("server", "Loading Aggro Spells Definitions...");
    sSpellMgr->LoadSpellThreats();

    TC_LOG_INFO("server", "Loading Spell Group Stack Rules...");
    sSpellMgr->LoadSpellGroupStackRules();

    TC_LOG_INFO("server", "Loading forbidden spells...");
    sSpellMgr->LoadForbiddenSpells();

    TC_LOG_INFO("server", "Loading Spell Phase Dbc Info...");
    sObjectMgr->LoadSpellPhaseInfo();

    TC_LOG_INFO("server", "Loading NPC Texts...");
    sObjectMgr->LoadGossipText();

    TC_LOG_INFO("server", "Loading Enchant Spells Proc datas...");
    sSpellMgr->LoadSpellEnchantProcData();

    TC_LOG_INFO("server", "Loading Item Random Enchantments Table...");
    LoadRandomEnchantmentsTable();

    TC_LOG_INFO("server", "Loading Disables");
    DisableMgr::LoadDisables();                                 // must be before loading quests and items

    TC_LOG_INFO("server", "Loading Items...");                         // must be after LoadRandomEnchantmentsTable and LoadPageTexts
    sObjectMgr->LoadItemTemplates();

    if (!getBoolConfig(CONFIG_FASTER_LOADING))
    {
        TC_LOG_INFO("server", "Loading Item set names...");                // must be after LoadItemPrototypes
        sObjectMgr->LoadItemTemplateAddon();
    }

    TC_LOG_INFO("server", "Loading Item Scripts...");                 // must be after LoadItemPrototypes
    sObjectMgr->LoadItemScriptNames();

    TC_LOG_INFO("server", "Loading Creature Model Based Info Data...");
    sObjectMgr->LoadCreatureModelInfo();

    TC_LOG_INFO("server", "Loading Equipment templates...");
    sObjectMgr->LoadEquipmentTemplates();

    TC_LOG_INFO("server", "Loading Creature templates...");
    sObjectMgr->LoadCreatureTemplates();

    if (!getBoolConfig(CONFIG_FASTER_LOADING))
    {
        TC_LOG_INFO("server", "Loading Creature template addons...");
        sObjectMgr->LoadCreatureTemplateAddons();
    }

    TC_LOG_INFO("server", "Loading Creature difficulty stat...");
    sObjectMgr->LoadCreatureDifficultyStat();

    TC_LOG_INFO("server", "Loading Reputation Reward Rates...");
    sObjectMgr->LoadReputationRewardRate();

    TC_LOG_INFO("server", "Loading Creature Reputation OnKill Data...");
    sObjectMgr->LoadReputationOnKill();

    TC_LOG_INFO("server", "Loading Reputation Spillover Data...");
    sObjectMgr->LoadReputationSpilloverTemplate();

    TC_LOG_INFO("server", "Loading Points Of Interest Data...");
    sObjectMgr->LoadPointsOfInterest();

    TC_LOG_INFO("server", "Loading Creature Base Stats...");
    sObjectMgr->LoadCreatureClassLevelStats();

    TC_LOG_INFO("server", "Loading Creature Data...");
    sObjectMgr->LoadCreatures();

    TC_LOG_INFO("server", "Loading pet levelup spells...");
    sSpellMgr->LoadPetLevelupSpellMap();

    TC_LOG_INFO("server", "Loading pet default spells additional to levelup spells...");
    sSpellMgr->LoadPetDefaultSpells();

    if (!getBoolConfig(CONFIG_FASTER_LOADING))
    {
        TC_LOG_INFO("server", "Loading Creature Addon Data...");
        sObjectMgr->LoadCreatureAddons();                            // must be after LoadCreatureTemplates() and LoadCreatures()

        TC_LOG_INFO("server", "Loading Treasure Data...");
        sObjectMgr->LoadPersonalLootTemplate();

        TC_LOG_INFO("server", "Loading Gameobject Data...");
        sObjectMgr->LoadGameobjects();

        TC_LOG_INFO("server", "Loading Creature Linked Respawn...");
        sObjectMgr->LoadLinkedRespawn();                             // must be after LoadCreatures(), LoadGameObjects()

        TC_LOG_INFO("server", "Loading Weather Data...");
        WeatherMgr::LoadWeatherData();

        TC_LOG_INFO("server", "Loading Quests...");
        sObjectMgr->LoadQuests();                                    // must be loaded after DBCs, creature_template, item_template, gameobject tables

        TC_LOG_INFO("server", "Checking Quest Disables");
        DisableMgr::CheckQuestDisables();                           // must be after loading quests

        TC_LOG_INFO("server", "Loading Quest POI");
        sObjectMgr->LoadQuestPOI();

        TC_LOG_INFO("server", "Loading Quests Relations...");
        sObjectMgr->LoadQuestRelations();                            // must be after quest load
    }

    TC_LOG_INFO("server", "Loading Scenario POI");
    sObjectMgr->LoadScenarioPOI();

    TC_LOG_INFO("server", "Loading Objects Pooling Data...");
    sPoolMgr->LoadFromDB();

    TC_LOG_INFO("server", "Loading Game Event Data...");               // must be after loading pools fully
    sGameEventMgr->LoadFromDB();

    TC_LOG_INFO("server", "Loading UNIT_NPC_FLAG_SPELLCLICK Data..."); // must be after LoadQuests
    sObjectMgr->LoadNPCSpellClickSpells();

    TC_LOG_INFO("server", "Loading Vehicle Template Accessories...");
    sObjectMgr->LoadVehicleTemplateAccessories();                // must be after LoadCreatureTemplates() and LoadNPCSpellClickSpells()

    TC_LOG_INFO("server", "Loading Vehicle Accessories...");
    sObjectMgr->LoadVehicleAccessories();                       // must be after LoadCreatureTemplates() and LoadNPCSpellClickSpells()

    TC_LOG_INFO("server", "Loading SpellArea Data...");                // must be after quest load
    sSpellMgr->LoadSpellAreas();

    TC_LOG_INFO("server", "Loading Spell Classes Info...");
    sSpellMgr->LoadSpellClassInfo();

    TC_LOG_INFO("server", "Loading AreaTrigger definitions...");
    sObjectMgr->LoadAreaTriggerTeleports();

    TC_LOG_INFO("server", "Loading Access Requirements...");
    sObjectMgr->LoadAccessRequirements();                        // must be after item template load

    TC_LOG_INFO("server", "Loading Quest Area Triggers...");
    sObjectMgr->LoadQuestAreaTriggers();                         // must be after LoadQuests

    TC_LOG_INFO("server", "Loading Tavern Area Triggers...");
    sObjectMgr->LoadTavernAreaTriggers();

    TC_LOG_INFO("server", "Loading AreaTrigger script names...");
    sObjectMgr->LoadAreaTriggerScripts();

    TC_LOG_INFO("server", "Loading LFG entrance positions..."); // Must be after areatriggers
    sLFGMgr->LoadLFGDungeons(true);

    TC_LOG_INFO("server", "Loading Dungeon boss data...");
    sObjectMgr->LoadInstanceEncounters();

    TC_LOG_INFO("server", "Loading LFG rewards...");
    sLFGMgr->LoadRewards();

    TC_LOG_INFO("server", "Loading Graveyard-zone links...");
    sObjectMgr->LoadGraveyardZones();

    TC_LOG_INFO("server", "Loading spell pet auras...");
    sSpellMgr->LoadSpellPetAuras();

    TC_LOG_INFO("server", "Loading Spell target coordinates...");
    sSpellMgr->LoadSpellTargetPositions();

    TC_LOG_INFO("server", "Loading enchant custom attributes...");
    sSpellMgr->LoadEnchantCustomAttr();

    TC_LOG_INFO("server", "Loading linked spells...");
    sSpellMgr->LoadSpellLinked();
    sSpellMgr->LoadTalentSpellLinked();

    TC_LOG_INFO("server", "Loading Prco Check spells...");
    sSpellMgr->LoadSpellPrcoCheck();

    TC_LOG_INFO("server", "Loading triggered spells...");
    sSpellMgr->LoadSpellTriggered();
    sSpellMgr->LoadSpellVisual();
    sSpellMgr->LoadSpellPendingCast();
    sSpellMgr->LoadSpellScene();

    TC_LOG_INFO("server", "Loading mount replace list...");
    sSpellMgr->LoadmSpellMountList();

    TC_LOG_INFO("server", "Loading Player Create Data...");
    sObjectMgr->LoadPlayerInfo();

    TC_LOG_INFO("server", "Loading Exploration BaseXP Data...");
    sObjectMgr->LoadExplorationBaseXP();

    TC_LOG_INFO("server", "Loading Pet Name Parts...");
    sObjectMgr->LoadPetNames();

    CharacterDatabaseCleaner::CleanDatabase();

    TC_LOG_INFO("server", "Loading the max pet number...");
    sObjectMgr->LoadPetNumber();

    TC_LOG_INFO("server", "Loading the max batlpet number...");
    sObjectMgr->LoadBattlePetGuid();

    TC_LOG_INFO("server", "Loading pet level stats...");
    sObjectMgr->LoadPetStats();

    if (!getBoolConfig(CONFIG_FASTER_LOADING)) 
    {
        TC_LOG_INFO("server", "Loading Player Corpses...");
        sObjectMgr->LoadCorpses();

        TC_LOG_INFO("server", "Loading Player level dependent mail rewards...");
        sObjectMgr->LoadMailLevelRewards();

        // Loot tables
        LoadLootTables();

        TC_LOG_INFO("server", "Loading Skill Discovery Table...");
        LoadSkillDiscoveryTable();

        TC_LOG_INFO("server", "Loading Skill Extra Item Table...");
        LoadSkillExtraItemTable();

        TC_LOG_INFO("server", "Loading Skill Fishing base level requirements...");
        sObjectMgr->LoadFishingBaseSkillLevel();
    }

    TC_LOG_INFO("server", "Loading Achievements...");
    sAchievementMgr->LoadAchievementReferenceList();
    TC_LOG_INFO("server", "Loading Criteria Lists...");
    sAchievementMgr->LoadCriteriaList();
    TC_LOG_INFO("server", "Loading Achievement Criteria Data...");
    sAchievementMgr->LoadAchievementCriteriaData();
    TC_LOG_INFO("server", "Loading Achievement Rewards...");
    sAchievementMgr->LoadRewards();
    TC_LOG_INFO("server", "Loading Achievement Reward Locales...");
    sAchievementMgr->LoadRewardLocales();
    TC_LOG_INFO("server", "Loading Completed Achievements...");
    sAchievementMgr->LoadCompletedAchievements();

    // Delete expired auctions before loading
    TC_LOG_INFO("server", "Deleting expired auctions...");
    sAuctionMgr->DeleteExpiredAuctionsAtStartup();

    ///- Load dynamic data tables from the database
    TC_LOG_INFO("server", "Loading Item Auctions...");
    sAuctionMgr->LoadAuctionItems();
    TC_LOG_INFO("server", "Loading Auctions...");
    sAuctionMgr->LoadAuctions();

    TC_LOG_INFO("server", "Loading Currencys Loot...");
    sObjectMgr->LoadCurrencysLoot();

    TC_LOG_INFO("server", "Loading Guild XP for level...");
    sGuildMgr->LoadGuildXpForLevel();

    TC_LOG_INFO("server", "Loading Guild rewards...");
    sGuildMgr->LoadGuildRewards();

    sGuildMgr->LoadGuilds();

    sGuildFinderMgr->LoadFromDB();

    sBracketMgr->LoadCharacterBrackets();

    TC_LOG_INFO("server", "Loading Groups...");
    sGroupMgr->LoadGroups();

    TC_LOG_INFO("server", "Loading ReservedNames...");
    sObjectMgr->LoadReservedPlayersNames();

    TC_LOG_INFO("server", "Loading GameObjects for quests...");
    sObjectMgr->LoadGameObjectForQuests();

    TC_LOG_INFO("server", "Loading BattleMasters...");
    sBattlegroundMgr->LoadBattleMastersEntry();

    TC_LOG_INFO("server", "Loading GameTeleports...");
    sObjectMgr->LoadGameTele();

    TC_LOG_INFO("server", "Loading Gossip menu...");
    sObjectMgr->LoadGossipMenu();

    TC_LOG_INFO("server", "Loading Gossip menu options...");
    sObjectMgr->LoadGossipMenuItems();

    TC_LOG_INFO("server", "Loading Vendors...");
    sObjectMgr->LoadVendors();                                   // must be after load CreatureTemplate and ItemTemplate

    TC_LOG_INFO("server", "Loading Trainers...");
    sObjectMgr->LoadTrainerSpell();                              // must be after load CreatureTemplate

    TC_LOG_INFO("server", "Loading Waypoints...");
    sWaypointMgr->Load();

    TC_LOG_INFO("server", "Loading SmartAI Waypoints...");
    sSmartWaypointMgr->LoadFromDB();

    TC_LOG_INFO("server", "Loading Creature Formations...");
    sFormationMgr->LoadCreatureFormations();

    TC_LOG_INFO("server", "Loading World States...");              // must be loaded before battleground, outdoor PvP and conditions
    LoadWorldStates();

    TC_LOG_INFO("server", "Loading Anti Spamm...");
    LoadWorldAntispamm();

    TC_LOG_INFO("server", "Loading Phase definitions...");
    sObjectMgr->LoadPhaseDefinitions();

    TC_LOG_INFO("server", "Loading AreaTrigger data...");
    sObjectMgr->LoadAreaTriggerActionsAndData();

    TC_LOG_INFO("server", "Loading Conditions...");
    sConditionMgr->LoadConditions();

    TC_LOG_INFO("server", "Loading faction change achievement pairs...");
    sObjectMgr->LoadFactionChangeAchievements();

    TC_LOG_INFO("server", "Loading faction change spell pairs...");
    sObjectMgr->LoadFactionChangeSpells();

    TC_LOG_INFO("server", "Loading faction change item pairs...");
    sObjectMgr->LoadFactionChangeItems();

    TC_LOG_INFO("server", "Loading faction change reputation pairs...");
    sObjectMgr->LoadFactionChangeReputations();

    TC_LOG_INFO("server", "Loading faction change title pairs...");
    sObjectMgr->LoadFactionChangeTitles();

    TC_LOG_INFO("server", "Loading GM tickets...");
    sTicketMgr->LoadTickets();

    TC_LOG_INFO("server", "Loading GM surveys...");
    sTicketMgr->LoadSurveys();

    TC_LOG_INFO("server", "Loading client addons...");
    AddonMgr::LoadFromDB();

    ///- Handle outdated emails (delete/return)
    TC_LOG_INFO("server", "Returning old mails...");
    sObjectMgr->ReturnOrDeleteOldMails(false);

    TC_LOG_INFO("server", "Loading Autobroadcasts...");
    LoadAutobroadcasts();

    TC_LOG_INFO("server", "Loading challenge save...");
    sChallengeMgr->LoadFromDB();

    ///- Load and initialize scripts
    sObjectMgr->LoadQuestStartScripts();                         // must be after load Creature/Gameobject(Template/Data) and QuestTemplate
    sObjectMgr->LoadQuestEndScripts();                           // must be after load Creature/Gameobject(Template/Data) and QuestTemplate
    sObjectMgr->LoadSpellScripts();                              // must be after load Creature/Gameobject(Template/Data)
    sObjectMgr->LoadGameObjectScripts();                         // must be after load Creature/Gameobject(Template/Data)
    sObjectMgr->LoadEventScripts();                              // must be after load Creature/Gameobject(Template/Data)
    sObjectMgr->LoadWaypointScripts();

    TC_LOG_INFO("server", "Loading Scripts text locales...");      // must be after Load*Scripts calls
    sObjectMgr->LoadDbScriptStrings();

    TC_LOG_INFO("server", "Loading spell script names...");
    sObjectMgr->LoadSpellScriptNames();

    TC_LOG_INFO("server", "Loading Achievement script names...");
    sObjectMgr->LoadAchievementScriptNames();

    TC_LOG_INFO("server", "Loading Creature Texts...");
    sCreatureTextMgr->LoadCreatureTexts();

    TC_LOG_INFO("server", "Loading Creature Text Locales...");
    sCreatureTextMgr->LoadCreatureTextLocales();

    TC_LOG_INFO("server", "Initializing Scripts...");
    sScriptMgr->Initialize();
    sScriptMgr->OnConfigLoad(false);                                // must be done after the ScriptMgr has been properly initialized

    TC_LOG_INFO("server", "Validating spell scripts...");
    sObjectMgr->ValidateSpellScripts();

    TC_LOG_INFO("server", "Loading SmartAI scripts...");
    sSmartScriptMgr->LoadSmartAIFromDB();

    TC_LOG_INFO("server", "Loading Calendar data...");
    sCalendarMgr->LoadFromDB();

    ///- Initialize game time and timers
    TC_LOG_INFO("server", "Initialize game time and timers");
    m_gameTime = time(NULL);
    m_startTime = m_gameTime;

    LoginDatabase.PExecute("INSERT INTO uptime (realmid, starttime, uptime, revision) VALUES(%u, %u, 0, '%s')",
                            realmID, uint32(m_startTime), GitRevision::GetFullVersion());       // One-time query

    m_timers[WUPDATE_WEATHERS].SetInterval(1*IN_MILLISECONDS);
    m_timers[WUPDATE_AUCTIONS].SetInterval(MINUTE*IN_MILLISECONDS);
    m_timers[WUPDATE_UPTIME].SetInterval(m_int_configs[CONFIG_UPTIME_UPDATE]*MINUTE*IN_MILLISECONDS);
                                                            //Update "uptime" table based on configuration entry in minutes.
    m_timers[WUPDATE_CORPSES].SetInterval(20 * MINUTE * IN_MILLISECONDS);
                                                            //erase corpses every 20 minutes
    m_timers[WUPDATE_CLEANDB].SetInterval(m_int_configs[CONFIG_LOGDB_CLEARINTERVAL]*MINUTE*IN_MILLISECONDS);
                                                            // clean logs table every 14 days by default
    m_timers[WUPDATE_AUTOBROADCAST].SetInterval(getIntConfig(CONFIG_AUTOBROADCAST_INTERVAL));
    m_timers[WUPDATE_DELETECHARS].SetInterval(DAY*IN_MILLISECONDS); // check for chars to delete every day

    m_timers[WUPDATE_PINGDB].SetInterval(getIntConfig(CONFIG_DB_PING_INTERVAL)*MINUTE*IN_MILLISECONDS);    // Mysql ping time in minutes

    m_timers[WUPDATE_GUILDSAVE].SetInterval(getIntConfig(CONFIG_GUILD_SAVE_INTERVAL) * MINUTE * IN_MILLISECONDS);

    //to set mailtimer to return mails every day between 4 and 5 am
    //mailtimer is increased when updating auctions
    //one second is 1000 -(tested on win system)
    //TODO: Get rid of magic numbers
    mail_timer = ((((localtime(&m_gameTime)->tm_hour + 20) % 24)* HOUR * IN_MILLISECONDS) / m_timers[WUPDATE_AUCTIONS].GetInterval());
                                                            //1440
    mail_timer_expires = ((DAY * IN_MILLISECONDS) / (m_timers[WUPDATE_AUCTIONS].GetInterval()));
    TC_LOG_INFO("server", "Mail timer set to: " UI64FMTD ", mail return is called every " UI64FMTD " minutes", uint64(mail_timer), uint64(mail_timer_expires));

    ///- Initilize static helper structures
    AIRegistry::Initialize();

    ///- Initialize MapManager
    TC_LOG_INFO("server", "Starting Map System");
    sMapMgr->Initialize();

    TC_LOG_INFO("server", "Starting Game Event system...");
    uint32 nextGameEvent = sGameEventMgr->StartSystem();
    m_timers[WUPDATE_EVENTS].SetInterval(nextGameEvent);    //depend on next event

    // Delete all characters which have been deleted X days before
    Player::DeleteOldCharacters();

    // Delete all custom channels which haven't been used for PreserveCustomChannelDuration days.
    Channel::CleanOldChannelsInDB();

    TC_LOG_INFO("server", "Starting Arena Season...");
    sGameEventMgr->StartArenaSeason();

    sTicketMgr->Initialize();

    TC_LOG_INFO("server","Initializing Mailbox queue system...");
    m_timers[WUPDATE_MAILBOXQUEUE].SetInterval( 2 * MINUTE * 1000 );

    ///- Initialize Battlegrounds
    TC_LOG_INFO("server", "Starting Battleground System");
    sBattlegroundMgr->CreateInitialBattlegrounds();
    //sBattlegroundMgr->InitAutomaticArenaPointDistribution();
    CharacterDatabase.Execute("UPDATE `character_battleground_data` SET `team` = 0"); // Need update if crash server

    ///- Initialize outdoor pvp
    TC_LOG_INFO("server", "Starting Outdoor PvP System");
    sOutdoorPvPMgr->InitOutdoorPvP();

    ///- Initialize Battlefield
    TC_LOG_INFO("server", "Starting Battlefield System");
    sBattlefieldMgr->InitBattlefield();

    TC_LOG_INFO("server", "Loading Transports...");
    sMapMgr->LoadTransports();

    TC_LOG_INFO("server", "Loading Transport NPCs...");
    sMapMgr->LoadTransportNPCs();

    // Banned addons
    TC_LOG_INFO("server", "Loading Banned Addons...");
    sObjectMgr->LoadBannedAddons();

    TC_LOG_INFO("server", "Deleting expired bans...");
    LoginDatabase.Execute("DELETE FROM ip_banned WHERE unbandate <= UNIX_TIMESTAMP() AND unbandate<>bandate");      // One-time query

    TC_LOG_INFO("server", "Calculate next daily quest reset time...");
    InitDailyQuestResetTime();

    TC_LOG_INFO("server", "Calculate next weekly quest reset time...");
    InitWeeklyQuestResetTime();

    TC_LOG_INFO("server", "Calculate random battleground reset time...");
    InitRandomBGResetTime();

    TC_LOG_INFO("server", "Calculate next currency reset time...");
    InitCurrencyResetTime();

    TC_LOG_INFO("server", "Calculate next instanse reset time...");
    InitInstanceDailyResetTime();
    InitInstanceHalfWeekResetTime();
    InitInstanceWeeklyResetTime();

    TC_LOG_INFO("server","Init Auto Restart time..." );
    InitServerAutoRestartTime();

    LoadCharacterNameData();

    TC_LOG_INFO("server", "Initializing Opcodes...");
    InitOpcodes();

    TC_LOG_INFO("server", "Loading hotfix info...");
    sObjectMgr->LoadHotfixData();

    TC_LOG_INFO("server", "Loading research site to zone mapping data...");
    sObjectMgr->LoadResearchSiteToZoneData();

    TC_LOG_INFO("server", "Loading archaeology digsite positions...");
    sObjectMgr->LoadDigSitePositions();

    // Battle Pets some data
    sObjectMgr->LoadBattlePetBreedsToSpecies();

    TC_LOG_INFO("server", "Loading realm name...");

    m_realmName = "Mist of Pandaria servers";
    QueryResult realmResult = LoginDatabase.PQuery("SELECT name FROM realmlist WHERE id = %u", realmID);
    if (realmResult)
        m_realmName = (*realmResult)[0].GetString();

    for (uint8 i = 0; i < m_realmName.size(); ++i)
        if (m_realmName[i] != ' ')
            m_trimmedRealmName += m_realmName[i];

    for (uint16 i = 0; i < 6864; i++)
    {
        m_zonesDiff[i][0] = 0;
        m_zonesDiff[i][1] = 0;
        m_zonesDiff[i][2] = m_int_configs[CONFIG_MAX_POSSIBLE_VISIBILITY_RANGE];
    }

    uint32 startupDuration = GetMSTimeDiffToNow(startupBegin);

    TC_LOG_INFO("server", "World initialized in %u minutes %u seconds", (startupDuration / 60000), ((startupDuration % 60000) / 1000));

    sLog->SetRealmId(realmID);
}

void World::DetectDBCLang()
{
    uint8 m_lang_confid = ConfigMgr::GetIntDefault("DBC.Locale", 0);

    if (m_lang_confid >= TOTAL_LOCALES)
    {
        TC_LOG_ERROR("server", "Incorrect DBC.Locale! Must be >= 0 and < %d (set to 0)", TOTAL_LOCALES);
        m_lang_confid = LOCALE_enUS;
    }

    /*ChrRacesEntry const* race = sChrRacesStore.LookupEntry(1);

    std::string availableLocalsStr;

    uint8 default_locale = TOTAL_LOCALES;
    for (uint8 i = default_locale-1; i < TOTAL_LOCALES; --i)  // -1 will be 255 due to uint8
    {
        if (race->name[i][0] != '\0')                     // check by race names
        {
            default_locale = i;
            m_availableDbcLocaleMask |= (1 << i);
            availableLocalsStr += localeNames[i];
            availableLocalsStr += " ";
        }
    }

    if (default_locale != m_lang_confid && m_lang_confid < TOTAL_LOCALES &&
        (m_availableDbcLocaleMask & (1 << m_lang_confid)))
    {
        default_locale = m_lang_confid;
    }

    if (default_locale >= TOTAL_LOCALES)
    {
        TC_LOG_ERROR("server", "Unable to determine your DBC Locale! (corrupt DBC?)");
        exit(1);
    }*/

    m_defaultDbcLocale = LocaleConstant(m_lang_confid);

    TC_LOG_INFO("server", "Using %s DBC Locale", localeNames[m_defaultDbcLocale]);
}

void World::RecordTimeDiff(const char *text, ...)
{
    if (m_updateTimeCount != 1)
        return;
    if (!text)
    {
        m_currentTime = getMSTime();
        return;
    }

    uint32 thisTime = getMSTime();
    uint32 diff = getMSTimeDiff(m_currentTime, thisTime);

    if (diff > m_int_configs[CONFIG_MIN_LOG_UPDATE])
    {
        va_list ap;
        char str[256];
        va_start(ap, text);
        vsnprintf(str, 256, text, ap);
        va_end(ap);
        TC_LOG_DEBUG("diff","Difftime %s: %u.", str, diff);
    }

    m_currentTime = thisTime;
}

void World::LoadAutobroadcasts()
{
    uint32 oldMSTime = getMSTime();

    m_Autobroadcasts.clear();

    QueryResult result = WorldDatabase.Query("SELECT text FROM autobroadcast");

    if (!result)
    {
        TC_LOG_INFO("server", ">> Loaded 0 autobroadcasts definitions. DB table `autobroadcast` is empty!");

        return;
    }

    uint32 count = 0;

    do
    {

        Field* fields = result->Fetch();
        std::string message = fields[0].GetString();

        m_Autobroadcasts.push_back(message);

        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server", ">> Loaded %u autobroadcasts definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

}

/// Update the World !
void World::Update(uint32 diff)
{
    m_updateTime = diff;

    if (m_int_configs[CONFIG_INTERVAL_LOG_UPDATE])
    {
        if (m_updateTimeSum > m_int_configs[CONFIG_INTERVAL_LOG_UPDATE])
        {
//             uint32 averageDiff = 0;
// 
//             for (uint8 i = 0; i < 9; i++)
//             {
//                 m_averageLongUpdateTime[i] = m_averageLongUpdateTime[i + 1];
//                 averageDiff += m_averageLongUpdateTime[i];
//             }
//             m_averageLongUpdateTime[9] = m_updateTimeSum / m_updateTimeCount;
//             averageDiff += m_averageLongUpdateTime[9];
//             averageDiff /= 10;
// 
//             TC_LOG_DEBUG("diff","Update time diff for 1min: %u. Average diff of last 10min: %u. Players online: %u.", m_averageLongUpdateTime[9], averageDiff, GetActiveSessionCount());
            TC_LOG_DEBUG("diff","Update time diff: %u. Players online: %u.", m_updateTimeSum / m_updateTimeCount, GetActiveSessionCount());
            m_updateTimeSum = m_updateTime;
            m_updateTimeCount = 1;
        }
        else
        {
            m_updateTimeSum += m_updateTime;
            ++m_updateTimeCount;
        }
    }

    m_zoneDiffTimer += diff;
    if (m_zoneDiffTimer > m_int_configs[CONFIG_HANDLE_VISIBILITY_TIMER])
    {
        HandleZoneDiff();
        m_zoneDiffTimer = 0;
    }

    ///- Update the different timers
    for (int i = 0; i < WUPDATE_COUNT; ++i)
    {
        if (m_timers[i].GetCurrent() >= 0)
            m_timers[i].Update(diff);
        else
            m_timers[i].SetCurrent(0);
    }

    ///- Update the game time and check for shutdown time
    _UpdateGameTime();

    /// Handle daily quests reset time
    if (m_gameTime > m_NextDailyQuestReset)
    {
        ResetDailyQuests();
        m_NextDailyQuestReset += DAY;
        sGuildMgr->ResetExperienceCaps();
    }

    if (m_gameTime > m_NextWeeklyQuestReset)
    {
        ResetWeeklyQuests();
        sGuildMgr->ResetReputationCaps();
    }

    if (m_gameTime > m_NextRandomBGReset)
        ResetRandomBG();

    if (m_gameTime > m_NextCurrencyReset)
        ResetCurrencyWeekCap();

    if (m_gameTime > m_NextInstanceDailyReset)
        InstanceDailyResetTime();

    if (m_gameTime > m_NextInstanceHalfWeekReset)
        InstanceHalfWeekResetTime();

    if (m_gameTime > m_NextInstanceWeeklyReset)
    {
        InstanceWeeklyResetTime();
        ResetLootCooldown();
    }

    if (m_gameTime > m_NextServerRestart)
        AutoRestartServer();

    /// <ul><li> Handle auctions when the timer has passed
    if (m_timers[WUPDATE_AUCTIONS].Passed())
    {
        m_timers[WUPDATE_AUCTIONS].Reset();

        ///- Update mails (return old mails with item, or delete them)
        //(tested... works on win)
        if (++mail_timer > mail_timer_expires)
        {
            mail_timer = 0;
            sObjectMgr->ReturnOrDeleteOldMails(true);
        }

        ///- Handle expired auctions
        sAuctionMgr->Update();
    }

    /// <li> Handle session updates when the timer has passed
    RecordTimeDiff(NULL);
    UpdateSessions(diff);
    RecordTimeDiff("UpdateSessions");

    /// <li> Handle weather updates when the timer has passed
    if (m_timers[WUPDATE_WEATHERS].Passed())
    {
        m_timers[WUPDATE_WEATHERS].Reset();
        WeatherMgr::Update(uint32(m_timers[WUPDATE_WEATHERS].GetInterval()));
    }

    /// <li> Update uptime table
    if (m_timers[WUPDATE_UPTIME].Passed())
    {
        uint32 tmpDiff = uint32(m_gameTime - m_startTime);
        uint32 maxOnlinePlayers = GetMaxPlayerCount();

        m_timers[WUPDATE_UPTIME].Reset();

        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_UPTIME_PLAYERS);

        stmt->setUInt32(0, tmpDiff);
        stmt->setUInt16(1, uint16(maxOnlinePlayers));
        stmt->setUInt32(2, realmID);
        stmt->setUInt32(3, uint32(m_startTime));

        LoginDatabase.Execute(stmt);

        if (!m_bool_configs[CONFIG_DISABLE_NEW_ONLINE])
            LoginDatabase.PQuery("REPLACE INTO `online` (`realmID`, `online`, `diff`, `uptime`) VALUES ('%u', '%u', '%u', '%u')", realmID, GetActiveSessionCount(), GetUpdateTime(), GetUptime());
    }

    /// <li> Clean logs table
    if (sWorld->getIntConfig(CONFIG_LOGDB_CLEARTIME) > 0) // if not enabled, ignore the timer
    {
        if (m_timers[WUPDATE_CLEANDB].Passed())
        {
            m_timers[WUPDATE_CLEANDB].Reset();

            PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_OLD_LOGS);

            stmt->setUInt32(0, sWorld->getIntConfig(CONFIG_LOGDB_CLEARTIME));
            stmt->setUInt32(1, uint32(time(0)));

            LoginDatabase.Execute(stmt);
        }
    }

    /// <li> Handle all other objects
    ///- Update objects when the timer has passed (maps, transport, creatures, ...)
    RecordTimeDiff(NULL);
    sMapMgr->Update(diff);
    RecordTimeDiff("UpdateMapMgr");

    if (sWorld->getBoolConfig(CONFIG_AUTOBROADCAST))
    {
        if (m_timers[WUPDATE_AUTOBROADCAST].Passed())
        {
            m_timers[WUPDATE_AUTOBROADCAST].Reset();
            SendAutoBroadcast();
        }
    }

    sBattlegroundMgr->Update(diff);
    RecordTimeDiff("UpdateBattlegroundMgr");

    sOutdoorPvPMgr->Update(diff);
    RecordTimeDiff("UpdateOutdoorPvPMgr");

    sBattlefieldMgr->Update(diff);
    RecordTimeDiff("BattlefieldMgr");

    ///- Delete all characters which have been deleted X days before
    if (m_timers[WUPDATE_DELETECHARS].Passed())
    {
        m_timers[WUPDATE_DELETECHARS].Reset();
        Player::DeleteOldCharacters();
    }

    sScenarioMgr->Update(diff);
    RecordTimeDiff("UpdateScenarioMgr");

    sLFGMgr->Update(diff);
    RecordTimeDiff("UpdateLFGMgr");

    // execute callbacks from sql queries that were queued recently
    ProcessQueryCallbacks();
    RecordTimeDiff("ProcessQueryCallbacks");

    ///- Erase corpses once every 20 minutes
    if (m_timers[WUPDATE_CORPSES].Passed())
    {
        m_timers[WUPDATE_CORPSES].Reset();
        sObjectAccessor->RemoveOldCorpses();
    }

    ///- Process Game events when necessary
    if (m_timers[WUPDATE_EVENTS].Passed())
    {
        m_timers[WUPDATE_EVENTS].Reset();                   // to give time for Update() to be processed
        uint32 nextGameEvent = sGameEventMgr->Update();
        m_timers[WUPDATE_EVENTS].SetInterval(nextGameEvent);
        m_timers[WUPDATE_EVENTS].Reset();
    }

    if (m_timers[WUPDATE_MAILBOXQUEUE].Passed())
    {
        m_timers[WUPDATE_MAILBOXQUEUE].Reset();
        ProcessMailboxQueue();
        Transfer();
        GetPoolGuids();
    }

    ///- Ping to keep MySQL connections alive
    if (m_timers[WUPDATE_PINGDB].Passed())
    {
        m_timers[WUPDATE_PINGDB].Reset();
        TC_LOG_DEBUG("server", "Ping MySQL to keep connection alive");
        CharacterDatabase.KeepAlive();
        LoginDatabase.KeepAlive();
        WorldDatabase.KeepAlive();
    }

    if (m_timers[WUPDATE_GUILDSAVE].Passed())
    {
        m_timers[WUPDATE_GUILDSAVE].Reset();
        sGuildMgr->SaveGuilds();
    }

    // update the instance reset times
    sInstanceSaveMgr->Update();

    // And last, but not least handle the issued cli commands
    ProcessCliCommands();

    sScriptMgr->OnWorldUpdate(diff);
}

void World::ForceGameEventUpdate()
{
    m_timers[WUPDATE_EVENTS].Reset();                   // to give time for Update() to be processed
    uint32 nextGameEvent = sGameEventMgr->Update();
    m_timers[WUPDATE_EVENTS].SetInterval(nextGameEvent);
    m_timers[WUPDATE_EVENTS].Reset();
}

/// Send a packet to all players (except self if mentioned)
void World::SendGlobalMessage(WorldPacket* packet, WorldSession* self, uint32 team)
{
    SessionMap::const_iterator itr;
    for (itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    {
        if (itr->second &&
            itr->second->GetPlayer() &&
            itr->second->GetPlayer()->IsInWorld() &&
            itr->second != self &&
            (team == 0 || itr->second->GetPlayer()->GetTeam() == team))
        {
            itr->second->SendPacket(packet);
        }
    }
}

/// Send a packet to all GMs (except self if mentioned)
void World::SendGlobalGMMessage(WorldPacket* packet, WorldSession* self, uint32 team)
{
    SessionMap::iterator itr;
    for (itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    {
        if (itr->second &&
            itr->second->GetPlayer() &&
            itr->second->GetPlayer()->IsInWorld() &&
            itr->second != self &&
            !AccountMgr::IsPlayerAccount(itr->second->GetSecurity()) &&
            (team == 0 || itr->second->GetPlayer()->GetTeam() == team))
        {
            itr->second->SendPacket(packet);
        }
    }
}

namespace Trinity
{
    class WorldWorldTextBuilder
    {
        public:
            typedef std::vector<WorldPacket*> WorldPacketList;
            explicit WorldWorldTextBuilder(int32 textId, va_list* args = NULL) : i_textId(textId), i_args(args) {}
            void operator()(WorldPacketList& data_list, LocaleConstant loc_idx)
            {
                char const* text = sObjectMgr->GetTrinityString(i_textId, loc_idx);

                if (i_args)
                {
                    // we need copy va_list before use or original va_list will corrupted
                    va_list ap;
                    va_copy(ap, *i_args);

                    char str[2048];
                    vsnprintf(str, 2048, text, ap);
                    va_end(ap);

                    do_helper(data_list, &str[0]);
                }
                else
                    do_helper(data_list, (char*)text);
            }
        private:
            char* lineFromMessage(char*& pos) { char* start = strtok(pos, "\n"); pos = NULL; return start; }
            void do_helper(WorldPacketList& data_list, char* text)
            {
                char* pos = text;

                while (char* line = lineFromMessage(pos))
                {
                    Trinity::ChatData c;
                    c.message = line ? line : "";
                    c.chatType = CHAT_MSG_SYSTEM;

                    WorldPacket* data = new WorldPacket();
                    Trinity::BuildChatPacket(*data, c);

                    data_list.push_back(data);
                }
            }

            int32 i_textId;
            va_list* i_args;
    };
}                                                           // namespace Trinity

/// Send a System Message to all players (except self if mentioned)
void World::SendWorldText(int32 string_id, ...)
{
    va_list ap;
    va_start(ap, string_id);

    Trinity::WorldWorldTextBuilder wt_builder(string_id, &ap);
    Trinity::LocalizedPacketListDo<Trinity::WorldWorldTextBuilder> wt_do(wt_builder);
    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    {
        if (!itr->second || !itr->second->GetPlayer() || !itr->second->GetPlayer()->IsInWorld())
            continue;

        wt_do(itr->second->GetPlayer());
    }

    va_end(ap);
}

/// Send a System Message to all GMs (except self if mentioned)
void World::SendGMText(int32 string_id, ...)
{
    va_list ap;
    va_start(ap, string_id);

    Trinity::WorldWorldTextBuilder wt_builder(string_id, &ap);
    Trinity::LocalizedPacketListDo<Trinity::WorldWorldTextBuilder> wt_do(wt_builder);
    for (SessionMap::iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    {
        if (!itr->second || !itr->second->GetPlayer() || !itr->second->GetPlayer()->IsInWorld())
            continue;

        if (AccountMgr::IsPlayerAccount(itr->second->GetSecurity()))
            continue;

        wt_do(itr->second->GetPlayer());
    }

    va_end(ap);
}

/// DEPRECATED, only for debug purpose. Send a System Message to all players (except self if mentioned)
void World::SendGlobalText(const char* text, WorldSession* self)
{
    WorldPacket data;

    // need copy to prevent corruption by strtok call in LineFromMessage original string
    char* buf = strdup(text);
    char* pos = buf;

    while (char* line = ChatHandler::LineFromMessage(pos))
    {
        ChatHandler::FillMessageData(&data, NULL, CHAT_MSG_SYSTEM, LANG_UNIVERSAL, NULL, 0, line, NULL);
        SendGlobalMessage(&data, self);
    }

    free(buf);
}

/// Send a packet to all players (or players selected team) in the zone (except self if mentioned)
void World::SendZoneMessage(uint32 zone, WorldPacket* packet, WorldSession* self, uint32 team)
{
    SessionMap::const_iterator itr;
    for (itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    {
        if (itr->second &&
            itr->second->GetPlayer() &&
            itr->second->GetPlayer()->IsInWorld() &&
            itr->second->GetPlayer()->getCurrentUpdateZoneID() == zone &&
            itr->second != self &&
            (team == 0 || itr->second->GetPlayer()->GetTeam() == team))
        {
            itr->second->SendPacket(packet);
        }
    }
}

/// Send a System Message to all players in the zone (except self if mentioned)
void World::SendZoneText(uint32 zone, const char* text, WorldSession* self, uint32 team)
{
    WorldPacket data;
    ChatHandler::FillMessageData(&data, NULL, CHAT_MSG_SYSTEM, LANG_UNIVERSAL, NULL, 0, text, NULL);
    SendZoneMessage(zone, &data, self, team);
}

/// Kick (and save) all players
void World::KickAll()
{
    m_QueuedPlayer.clear();                                 // prevent send queue update packet and login queued sessions

    // session not removed at kick and will removed in next update tick
    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        itr->second->KickPlayer();
}

/// Kick (and save) all players with security level less `sec`
void World::KickAllLess(AccountTypes sec)
{
    // session not removed at kick and will removed in next update tick
    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        if (itr->second->GetSecurity() < sec)
            itr->second->KickPlayer();
}

/// Ban an account or ban an IP address, duration will be parsed using TimeStringToSecs if it is positive, otherwise permban
BanReturn World::BanAccount(BanMode mode, std::string nameOrIP, std::string duration, std::string reason, std::string author)
{
    uint32 duration_secs = TimeStringToSecs(duration);
    PreparedQueryResult resultAccounts = PreparedQueryResult(NULL); //used for kicking
    PreparedStatement* stmt = NULL;

    ///- Update the database with ban information
    switch (mode)
    {
        case BAN_IP:
            // No SQL injection with prepared statements
            stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_BY_IP);
            stmt->setString(0, nameOrIP);
            resultAccounts = LoginDatabase.Query(stmt);
            stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_IP_BANNED);
            stmt->setString(0, nameOrIP);
            stmt->setUInt32(1, duration_secs);
            stmt->setString(2, author);
            stmt->setString(3, reason);
            LoginDatabase.Execute(stmt);
            break;
        case BAN_ACCOUNT:
            // No SQL injection with prepared statements
            stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_ID_BY_NAME);
            stmt->setString(0, nameOrIP);
            resultAccounts = LoginDatabase.Query(stmt);
            break;
        case BAN_CHARACTER:
            // No SQL injection with prepared statements
            stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_BY_NAME);
            stmt->setString(0, nameOrIP);
            resultAccounts = CharacterDatabase.Query(stmt);
            break;
        default:
            return BAN_SYNTAX_ERROR;
    }

    if (!resultAccounts)
    {
        if (mode == BAN_IP)
            return BAN_SUCCESS;                             // ip correctly banned but nobody affected (yet)
        else
            return BAN_NOTFOUND;                            // Nobody to ban
    }

    ///- Disconnect all affected players (for IP it can be several)
    SQLTransaction trans = LoginDatabase.BeginTransaction();
    do
    {
        Field* fieldsAccount = resultAccounts->Fetch();
        uint32 account = fieldsAccount[0].GetUInt32();

        if (mode != BAN_IP)
        {
            // make sure there is only one active ban
            stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_BANNED);
            stmt->setUInt32(0, account);
            PreparedQueryResult banresult = LoginDatabase.Query(stmt);
            if (banresult)
            {
                stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_ACCOUNT_BANNED);
                stmt->setUInt32(0, duration_secs);
                stmt->setUInt32(1, account);
                trans->Append(stmt);
            }
            else
            {
                // No SQL injection with prepared statements
                stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_ACCOUNT_BANNED);
                stmt->setUInt32(0, account);
                stmt->setUInt32(1, duration_secs);
                stmt->setString(2, author);
                stmt->setString(3, reason);
                trans->Append(stmt);
            }
        }

        if (WorldSession* sess = FindSession(account))
            if (std::string(sess->GetPlayerName()) != author)
                sess->KickPlayer();
    } while (resultAccounts->NextRow());

    LoginDatabase.CommitTransaction(trans);

    return BAN_SUCCESS;
}

/// Remove a ban from an account or IP address
bool World::RemoveBanAccount(BanMode mode, std::string nameOrIP)
{
    PreparedStatement* stmt = NULL;
    if (mode == BAN_IP)
    {
        stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_IP_NOT_BANNED);
        stmt->setString(0, nameOrIP);
        LoginDatabase.Execute(stmt);
    }
    else
    {
        uint32 account = 0;
        if (mode == BAN_ACCOUNT)
            account = AccountMgr::GetId(nameOrIP);
        else if (mode == BAN_CHARACTER)
            account = ObjectMgr::GetPlayerAccountIdByPlayerName(nameOrIP);

        if (!account)
            return false;

        //NO SQL injection as account is uint32
        stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_ACCOUNT_NOT_BANNED);
        stmt->setUInt32(0, account);
        LoginDatabase.Execute(stmt);
    }
    return true;
}

/// Ban an account or ban an IP address, duration will be parsed using TimeStringToSecs if it is positive, otherwise permban
BanReturn World::BanCharacter(std::string name, std::string duration, std::string reason, std::string author)
{
    Player* pBanned = sObjectAccessor->FindPlayerByName(name);
    uint32 guid = 0;

    uint32 duration_secs = TimeStringToSecs(duration);

    /// Pick a player to ban if not online
    if (!pBanned)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_BY_NAME);
        stmt->setString(0, name);
        PreparedQueryResult resultCharacter = CharacterDatabase.Query(stmt);

        if (!resultCharacter)
            return BAN_NOTFOUND;                                    // Nobody to ban

        guid = (*resultCharacter)[0].GetUInt32();
    }
    else
        guid = pBanned->GetGUIDLow();

    // make sure there is only one active ban
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHARACTER_BAN);
    stmt->setUInt32(0, guid);
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHARACTER_BAN);
    stmt->setUInt32(0, guid);
    stmt->setUInt32(1, duration_secs);
    stmt->setString(2, author);
    stmt->setString(3, reason);
    CharacterDatabase.Execute(stmt);

    if (pBanned)
        pBanned->GetSession()->KickPlayer();

    return BAN_SUCCESS;
}

/// Remove a ban from a character
bool World::RemoveBanCharacter(std::string name)
{
    Player* pBanned = sObjectAccessor->FindPlayerByName(name);
    uint32 guid = 0;

    /// Pick a player to ban if not online
    if (!pBanned)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_BY_NAME);
        stmt->setString(0, name);
        PreparedQueryResult resultCharacter = CharacterDatabase.Query(stmt);

        if (!resultCharacter)
            return false;

        guid = (*resultCharacter)[0].GetUInt32();
    }
    else
        guid = pBanned->GetGUIDLow();

    if (!guid)
        return false;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHARACTER_BAN);
    stmt->setUInt32(0, guid);
    CharacterDatabase.Execute(stmt);
    return true;
}

/// Update the game time
void World::_UpdateGameTime()
{
    ///- update the time
    time_t thisTime = time(NULL);
    uint32 elapsed = uint32(thisTime - m_gameTime);
    m_gameTime = thisTime;

    ///- if there is a shutdown timer
    if (!IsStopped() && m_ShutdownTimer > 0 && elapsed > 0)
    {
        ///- ... and it is overdue, stop the world (set m_stopEvent)
        if (m_ShutdownTimer <= elapsed)
        {
            if (!(m_ShutdownMask & SHUTDOWN_MASK_IDLE) || GetActiveAndQueuedSessionCount() == 0)
                m_stopEvent = true;                         // exist code already set
            else
                m_ShutdownTimer = 1;                        // minimum timer value to wait idle state
        }
        ///- ... else decrease it and if necessary display a shutdown countdown to the users
        else
        {
            m_ShutdownTimer -= elapsed;

            ShutdownMsg();
        }
    }
}

/// Shutdown the server
void World::ShutdownServ(uint32 time, uint32 options, uint8 exitcode)
{
    // ignore if server shutdown at next tick
    if (IsStopped())
        return;

    m_ShutdownMask = options;
    m_ExitCode = exitcode;

    ///- If the shutdown time is 0, set m_stopEvent (except if shutdown is 'idle' with remaining sessions)
    if (time == 0)
    {
        if (!(options & SHUTDOWN_MASK_IDLE) || GetActiveAndQueuedSessionCount() == 0)
            m_stopEvent = true;                             // exist code already set
        else
            m_ShutdownTimer = 1;                            //So that the session count is re-evaluated at next world tick
    }
    ///- Else set the shutdown timer and warn users
    else
    {
        m_ShutdownTimer = time;
        ShutdownMsg(true);
    }

    sScriptMgr->OnShutdownInitiate(ShutdownExitCode(exitcode), ShutdownMask(options));
}

/// Display a shutdown message to the user(s)
void World::ShutdownMsg(bool show, Player* player)
{
    // not show messages for idle shutdown mode
    if (m_ShutdownMask & SHUTDOWN_MASK_IDLE)
        return;

    ///- Display a message every 12 hours, hours, 5 minutes, minute, 5 seconds and finally seconds
    if (show ||
        (m_ShutdownTimer < 5* MINUTE && (m_ShutdownTimer % 15) == 0) || // < 5 min; every 15 sec
        (m_ShutdownTimer < 15 * MINUTE && (m_ShutdownTimer % MINUTE) == 0) || // < 15 min ; every 1 min
        (m_ShutdownTimer < 30 * MINUTE && (m_ShutdownTimer % (5 * MINUTE)) == 0) || // < 30 min ; every 5 min
        (m_ShutdownTimer < 12 * HOUR && (m_ShutdownTimer % HOUR) == 0) || // < 12 h ; every 1 h
        (m_ShutdownTimer > 12 * HOUR && (m_ShutdownTimer % (12 * HOUR)) == 0)) // > 12 h ; every 12 h
    {
        std::string str = secsToTimeString(m_ShutdownTimer);

        ServerMessageType msgid = (m_ShutdownMask & SHUTDOWN_MASK_RESTART) ? SERVER_MSG_RESTART_TIME : SERVER_MSG_SHUTDOWN_TIME;

        SendServerMessage(msgid, str.c_str(), player);
        TC_LOG_DEBUG("server", "Server is %s in %s", (m_ShutdownMask & SHUTDOWN_MASK_RESTART ? "restart" : "shuttingdown"), str.c_str());
    }
}

/// Cancel a planned server shutdown
void World::ShutdownCancel()
{
    // nothing cancel or too later
    if (!m_ShutdownTimer || m_stopEvent.value())
        return;

    ServerMessageType msgid = (m_ShutdownMask & SHUTDOWN_MASK_RESTART) ? SERVER_MSG_RESTART_CANCELLED : SERVER_MSG_SHUTDOWN_CANCELLED;

    m_ShutdownMask = 0;
    m_ShutdownTimer = 0;
    m_ExitCode = SHUTDOWN_EXIT_CODE;                       // to default value
    SendServerMessage(msgid);

    TC_LOG_DEBUG("server", "Server %s cancelled.", (m_ShutdownMask & SHUTDOWN_MASK_RESTART ? "restart" : "shuttingdown"));

    sScriptMgr->OnShutdownCancel();
}

/// Send a server message to the user(s)
void World::SendServerMessage(ServerMessageType type, const char *text, Player* player)
{
    WorldPacket data(SMSG_SERVER_MESSAGE, 50);              // guess size
    data << uint32(type);
    if (type <= SERVER_MSG_STRING)
        data << text;

    if (player)
        player->GetSession()->SendPacket(&data);
    else
        SendGlobalMessage(&data);
}

void World::UpdateSessions(uint32 diff)
{
    ///- Add new sessions
    WorldSession* sess = NULL;
    while (addSessQueue.next(sess))
        AddSession_ (sess);

    ///- Then send an update signal to remaining ones
    for (SessionMap::iterator itr = m_sessions.begin(), next; itr != m_sessions.end(); itr = next)
    {
        next = itr;
        ++next;

        ///- and remove not active sessions from the list
        WorldSession* pSession = itr->second;
        WorldSessionFilter updater(pSession);

        if (!pSession->Update(diff, updater))    // As interval = 0
        {
            if (!RemoveQueuedPlayer(itr->second) && itr->second && getIntConfig(CONFIG_INTERVAL_DISCONNECT_TOLERANCE))
                m_disconnects[itr->second->GetAccountId()] = time(NULL);
            RemoveQueuedPlayer(pSession);
            m_sessions.erase(itr);
            delete pSession;

        }
    }
}

// This handles the issued and queued CLI commands
void World::ProcessCliCommands()
{
    CliCommandHolder::Print* zprint = NULL;
    void* callbackArg = NULL;
    CliCommandHolder* command = NULL;
    while (cliCmdQueue.next(command))
    {
        TC_LOG_INFO("server", "CLI command under processing...");
        zprint = command->m_print;
        callbackArg = command->m_callbackArg;
        CliHandler handler(callbackArg, zprint);
        handler.ParseCommands(command->m_command);
        if (command->m_commandFinished)
            command->m_commandFinished(callbackArg, !handler.HasSentErrorMessage());
        delete command;
    }
}

void World::SendAutoBroadcast()
{
    if (m_Autobroadcasts.empty())
        return;

    std::string msg;

    msg = Trinity::Containers::SelectRandomContainerElement(m_Autobroadcasts);

    uint32 abcenter = sWorld->getIntConfig(CONFIG_AUTOBROADCAST_CENTER);

    if (abcenter == 0)
        sWorld->SendWorldText(LANG_AUTO_BROADCAST, msg.c_str());

    else if (abcenter == 1)
    {
        WorldPacket data(SMSG_NOTIFICATION, 2 + msg.length());
        data.WriteBits(msg.length(), 12);
        data.FlushBits();
        data.WriteString(msg);
        sWorld->SendGlobalMessage(&data);
    }

    else if (abcenter == 2)
    {
        sWorld->SendWorldText(LANG_AUTO_BROADCAST, msg.c_str());

        WorldPacket data(SMSG_NOTIFICATION, 2 + msg.length());
        data.WriteBits(msg.length(), 12);
        data.FlushBits();
        data.WriteString(msg);
        sWorld->SendGlobalMessage(&data);
    }

    TC_LOG_DEBUG("server", "AutoBroadcast: '%s'", msg.c_str());
}

void World::UpdateRealmCharCount(uint32 accountId)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_COUNT);
    stmt->setUInt32(0, accountId);
    PreparedQueryResultFuture result = CharacterDatabase.AsyncQuery(stmt);
    m_realmCharCallbacks.insert(result);
}

void World::_UpdateRealmCharCount(PreparedQueryResult resultCharCount)
{
    if (resultCharCount)
    {
        Field* fields = resultCharCount->Fetch();
        uint32 accountId = fields[0].GetUInt32();
        uint8 charCount = uint8(fields[1].GetUInt64());

        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_REALM_CHARACTERS_BY_REALM);
        stmt->setUInt32(0, accountId);
        stmt->setUInt32(1, realmID);
        LoginDatabase.Execute(stmt);

        stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_REALM_CHARACTERS);
        stmt->setUInt8(0, charCount);
        stmt->setUInt32(1, accountId);
        stmt->setUInt32(2, realmID);
        LoginDatabase.Execute(stmt);
    }
}

void World::InitWeeklyQuestResetTime()
{
    time_t wstime = sWorld->getWorldState(WS_WEEKLY_QUEST_RESET_TIME);
    time_t curtime = time(NULL);
    m_NextWeeklyQuestReset = wstime < curtime ? curtime : time_t(wstime);
}

void World::InitDailyQuestResetTime()
{
    time_t mostRecentQuestTime;

    QueryResult result = CharacterDatabase.Query("SELECT MAX(time) FROM character_queststatus_daily");
    if (result)
    {
        Field* fields = result->Fetch();
        mostRecentQuestTime = time_t(fields[0].GetUInt32());
    }
    else
        mostRecentQuestTime = 0;

    // client built-in time for reset is 6:00 AM
    // FIX ME: client not show day start time
    time_t curTime = time(NULL);
    tm localTm = *localtime(&curTime);
    localTm.tm_hour = 6;
    localTm.tm_min  = 0;
    localTm.tm_sec  = 0;

    // current day reset time
    time_t curDayResetTime = mktime(&localTm);

    // last reset time before current moment
    time_t resetTime = (curTime < curDayResetTime) ? curDayResetTime - DAY : curDayResetTime;

    // need reset (if we have quest time before last reset time (not processed by some reason)
    if (mostRecentQuestTime && mostRecentQuestTime <= resetTime)
        m_NextDailyQuestReset = mostRecentQuestTime;
    else // plan next reset time
        m_NextDailyQuestReset = (curTime >= curDayResetTime) ? curDayResetTime + DAY : curDayResetTime;
}

void World::InitRandomBGResetTime()
{
    time_t bgtime = sWorld->getWorldState(WS_BG_DAILY_RESET_TIME);
    // generate time by config
    time_t curTime = time(NULL);
    tm localTm = *localtime(&curTime);
    localTm.tm_hour = getIntConfig(CONFIG_RANDOM_BG_RESET_HOUR);
    localTm.tm_min = 0;
    localTm.tm_sec = 0;

    // current day reset time
    time_t nextDayResetTime = mktime(&localTm);

    // next reset time before current moment
    if (curTime >= nextDayResetTime)
        nextDayResetTime += DAY;

    // normalize reset time
    m_NextRandomBGReset = bgtime ? bgtime : nextDayResetTime;

    if (!bgtime)
        sWorld->setWorldState(WS_BG_DAILY_RESET_TIME, m_NextRandomBGReset);
}

void World::InitCurrencyResetTime()
{
    time_t currencytime = sWorld->getWorldState(WS_CURRENCY_RESET_TIME);
    // generate time by config
    time_t curTime = time(NULL);
    tm localTm = *localtime(&curTime);

    localTm.tm_wday = getIntConfig(CONFIG_CURRENCY_RESET_DAY);
    localTm.tm_hour = getIntConfig(CONFIG_CURRENCY_RESET_HOUR);
    localTm.tm_min = 0;
    localTm.tm_sec = 0;

    // current week reset time
    time_t nextWeekResetTime = mktime(&localTm);

    // next reset time before current moment
    if (curTime >= nextWeekResetTime)
        nextWeekResetTime += getIntConfig(CONFIG_CURRENCY_RESET_INTERVAL) * DAY;

    // normalize reset time
    m_NextCurrencyReset = currencytime ? currencytime : nextWeekResetTime;

    if (!currencytime)
        sWorld->setWorldState(WS_CURRENCY_RESET_TIME, m_NextCurrencyReset);
}

void World::InitInstanceDailyResetTime()
{
    time_t insttime = sWorld->getWorldState(WS_INSTANCE_DAILY_RESET_TIME);
    // generate time by config
    time_t curTime = time(NULL);
    tm localTm = *localtime(&curTime);
    localTm.tm_hour = getIntConfig(CONFIG_INSTANCE_RESET_TIME_HOUR);
    localTm.tm_min = 0;
    localTm.tm_sec = 0;

    // Daily reset time
    time_t nextResetTime = mktime(&localTm);

    // next reset time before current moment
    if (curTime >= nextResetTime)
        nextResetTime += getIntConfig(CONFIG_INSTANCE_DAILY_RESET) * DAY;

    // normalize reset time
    m_NextInstanceDailyReset = insttime ? insttime : nextResetTime;

    if (!insttime)
        sWorld->setWorldState(WS_INSTANCE_DAILY_RESET_TIME, m_NextInstanceDailyReset);
}

void World::InitInstanceHalfWeekResetTime()
{
    time_t insttime = sWorld->getWorldState(WS_INSTANCE_HALF_WEEK_RESET_TIME);
    // generate time by config
    time_t curTime = time(NULL);
    tm localTm = *localtime(&curTime);
    localTm.tm_hour = getIntConfig(CONFIG_INSTANCE_RESET_TIME_HOUR);
    localTm.tm_min = 0;
    localTm.tm_sec = 0;

    // Daily reset time
    time_t nextResetTime = mktime(&localTm);

    // next reset time before current moment
    if (curTime >= nextResetTime)
        nextResetTime += getIntConfig(CONFIG_INSTANCE_HALF_WEEK_RESET) * DAY;

    // normalize reset time
    m_NextInstanceHalfWeekReset = insttime ? insttime : nextResetTime;

    if (!insttime)
        sWorld->setWorldState(WS_INSTANCE_HALF_WEEK_RESET_TIME, m_NextInstanceHalfWeekReset);
}

void World::InitInstanceWeeklyResetTime()
{
    time_t insttime = sWorld->getWorldState(WS_INSTANCE_WEEKLY_RESET_TIME);

    // generate time by config
    time_t curTime = time(NULL);
    tm localTm = *localtime(&curTime);
    localTm.tm_wday = 3;
    localTm.tm_hour = getIntConfig(CONFIG_INSTANCE_RESET_TIME_HOUR);
    localTm.tm_min = 0;
    localTm.tm_sec = 0;

    // Daily reset time
    time_t nextResetTime = mktime(&localTm);

    // next reset time before current moment
    if (curTime >= nextResetTime)
        nextResetTime += getIntConfig(CONFIG_INSTANCE_WEEKLY_RESET) * DAY;

    // normalize reset time
    m_NextInstanceWeeklyReset = insttime ? insttime : nextResetTime;

    if (!insttime)
        sWorld->setWorldState(WS_INSTANCE_WEEKLY_RESET_TIME, m_NextInstanceWeeklyReset);
}

time_t World::getInstanceResetTime(uint32 resetTime)
{
    switch(resetTime)
    {
        case 86400:
            return sWorld->getWorldState(WS_INSTANCE_DAILY_RESET_TIME);
        case 259200:
            return sWorld->getWorldState(WS_INSTANCE_HALF_WEEK_RESET_TIME);
        case 604800:
            return sWorld->getWorldState(WS_INSTANCE_WEEKLY_RESET_TIME);
        default:
            return time(NULL);
    }

    return time(NULL);
}

void World::ResetDailyQuests()
{
    TC_LOG_INFO("server", "Daily quests reset for all characters.");

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_QUEST_STATUS_DAILY);
    CharacterDatabase.Execute(stmt);

    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        if (itr->second->GetPlayer())
            itr->second->GetPlayer()->ResetDailyQuestStatus();

    // change available dailies
    sPoolMgr->ChangeDailyQuests();
}

void World::ResetCurrencyWeekCap()
{
    sThreadPoolMgr->schedule([] {
        CharacterDatabase.Execute("UPDATE `character_currency` SET `week_count` = 0, `curentcap` = 0");
    });

    sThreadPoolMgr->wait();

    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        if (itr->second->GetPlayer())
            itr->second->GetPlayer()->ResetCurrencyWeekCap();

    m_NextCurrencyReset = time_t(m_NextCurrencyReset + DAY * getIntConfig(CONFIG_CURRENCY_RESET_INTERVAL));
    sWorld->setWorldState(WS_CURRENCY_RESET_TIME, m_NextCurrencyReset);
}

void World::InstanceDailyResetTime()
{
    for (MapDifficultyMap::const_iterator itr = sMapDifficultyMap.begin(); itr != sMapDifficultyMap.end(); ++itr)
    {
        uint32 mapid = PAIR32_LOPART(itr->first);
        Difficulty difficulty = Difficulty(PAIR32_HIPART(itr->first));

        if (itr->second.resetTime <= 86400)
            sInstanceSaveMgr->ResetOrWarnAll(mapid, difficulty);
    }

    m_NextInstanceDailyReset = time_t(m_NextInstanceDailyReset + DAY * getIntConfig(CONFIG_INSTANCE_DAILY_RESET));
    sWorld->setWorldState(WS_INSTANCE_DAILY_RESET_TIME, m_NextInstanceDailyReset);
}

void World::InstanceHalfWeekResetTime()
{
    for (MapDifficultyMap::const_iterator itr = sMapDifficultyMap.begin(); itr != sMapDifficultyMap.end(); ++itr)
    {
        uint32 mapid = PAIR32_LOPART(itr->first);
        Difficulty difficulty = Difficulty(PAIR32_HIPART(itr->first));

        if (itr->second.resetTime == 259200)
            sInstanceSaveMgr->ResetOrWarnAll(mapid, difficulty);
    }

    m_NextInstanceHalfWeekReset = time_t(m_NextInstanceHalfWeekReset + DAY * getIntConfig(CONFIG_INSTANCE_HALF_WEEK_RESET));
    sWorld->setWorldState(WS_INSTANCE_HALF_WEEK_RESET_TIME, m_NextInstanceHalfWeekReset);
}

void World::InstanceWeeklyResetTime()
{
    for (MapDifficultyMap::const_iterator itr = sMapDifficultyMap.begin(); itr != sMapDifficultyMap.end(); ++itr)
    {
        uint32 mapid = PAIR32_LOPART(itr->first);
        Difficulty difficulty = Difficulty(PAIR32_HIPART(itr->first));

        if (itr->second.resetTime == 604800)
            sInstanceSaveMgr->ResetOrWarnAll(mapid, difficulty);
    }

    m_NextInstanceWeeklyReset = time_t(m_NextInstanceWeeklyReset + DAY * getIntConfig(CONFIG_INSTANCE_WEEKLY_RESET));
    sWorld->setWorldState(WS_INSTANCE_WEEKLY_RESET_TIME, m_NextInstanceWeeklyReset);
}

void World::ResetLootCooldown()
{
    CharacterDatabase.Execute("DELETE FROM character_loot_cooldown WHERE respawnTime != 0");

    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        if (itr->second->GetPlayer())
            itr->second->GetPlayer()->ResetLootCooldown();
}

void World::LoadDBAllowedSecurityLevel()
{
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_REALMLIST_SECURITY_LEVEL);
    stmt->setInt32(0, int32(realmID));
    PreparedQueryResult result = LoginDatabase.Query(stmt);

    if (result)
        SetPlayerSecurityLimit(AccountTypes(uint8(result->Fetch()->GetFloat())));
}

void World::SetPlayerSecurityLimit(AccountTypes _sec)
{
    AccountTypes sec = _sec < SEC_CONSOLE ? _sec : SEC_PLAYER;
    bool update = sec > m_allowedSecurityLevel;
    m_allowedSecurityLevel = sec;
    if (update)
        KickAllLess(m_allowedSecurityLevel);
}

void World::ResetWeeklyQuests()
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_QUEST_STATUS_WEEKLY);
    CharacterDatabase.Execute(stmt);

    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        if (itr->second->GetPlayer())
            itr->second->GetPlayer()->ResetWeeklyQuestStatus();

    m_NextWeeklyQuestReset = time_t(m_NextWeeklyQuestReset + WEEK);
    sWorld->setWorldState(WS_WEEKLY_QUEST_RESET_TIME, m_NextWeeklyQuestReset);

    // change available weeklies
    sPoolMgr->ChangeWeeklyQuests();
}

void World::ResetEventSeasonalQuests(uint16 event_id)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_QUEST_STATUS_SEASONAL);
    stmt->setUInt16(0,event_id);
    CharacterDatabase.Execute(stmt);

    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        if (itr->second->GetPlayer())
            itr->second->GetPlayer()->ResetSeasonalQuestStatus(event_id);
}

void World::ResetRandomBG()
{
    TC_LOG_INFO("server", "Random BG status reset for all characters.");

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_BATTLEGROUND_RANDOM);
    CharacterDatabase.Execute(stmt);

    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        if (itr->second->GetPlayer())
            itr->second->GetPlayer()->SetRandomWinner(false);

    m_NextRandomBGReset = time_t(m_NextRandomBGReset + DAY);
    sWorld->setWorldState(WS_BG_DAILY_RESET_TIME, m_NextRandomBGReset);
}

void World::UpdateMaxSessionCounters()
{
    m_maxActiveSessionCount = std::max(m_maxActiveSessionCount, uint32(m_sessions.size()-m_QueuedPlayer.size()));
    m_maxQueuedSessionCount = std::max(m_maxQueuedSessionCount, uint32(m_QueuedPlayer.size()));
}

void World::LoadDBVersion()
{
    QueryResult result = WorldDatabase.Query("SELECT db_version, cache_id FROM version LIMIT 1");
    if (result)
    {
        Field* fields = result->Fetch();

        m_DBVersion = fields[0].GetString();
        // will be overwrite by config values if different and non-0
        m_int_configs[CONFIG_CLIENTCACHE_VERSION] = fields[1].GetUInt32();
    }

    if (m_DBVersion.empty())
        m_DBVersion = "Unknown world database.";
}

void World::ProcessStartEvent()
{
    isEventKillStart = true;
}

void World::ProcessStopEvent()
{
    isEventKillStart = false;
}

void World::UpdateAreaDependentAuras()
{
    SessionMap::const_iterator itr;
    for (itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        if (itr->second && itr->second->GetPlayer() && itr->second->GetPlayer()->IsInWorld())
        {
            itr->second->GetPlayer()->UpdateAreaDependentAuras(itr->second->GetPlayer()->GetAreaId());
            itr->second->GetPlayer()->UpdateZoneDependentAuras(itr->second->GetPlayer()->getCurrentUpdateZoneID());
        }
}

void World::LoadWorldStates()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = CharacterDatabase.Query("SELECT entry, value FROM worldstates");

    if (!result)
    {
        TC_LOG_INFO("server", ">> Loaded 0 world states. DB table `worldstates` is empty!");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        m_worldstates[fields[0].GetUInt32()] = fields[1].GetUInt32();
        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server", ">> Loaded %u world states in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

}

void World::LoadWorldAntispamm()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT id, maxcount, second FROM package_antispamm");

    if (!result)
    {
        TC_LOG_INFO("server", ">> Loaded 0 package count. DB table `package_antispamm` is empty!");
        return;
    }

    for (uint32 i = 0; i < PACKETS_COUNT; ++i)
    {
       loadantispamm[i][0] = 0;
       loadantispamm[i][1] = 0;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        loadantispamm[fields[0].GetUInt32()][0] = fields[1].GetUInt32();
        loadantispamm[fields[0].GetUInt32()][1] = fields[2].GetUInt32();
        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server", ">> Loaded %u package count in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

// Setting a worldstate will save it to DB
void World::setWorldState(uint32 index, uint32 value)
{
    WorldStatesMap::const_iterator it = m_worldstates.find(index);
    if (it != m_worldstates.end())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_WORLDSTATE);

        stmt->setUInt32(0, uint32(value));
        stmt->setUInt32(1, index);

        CharacterDatabase.Execute(stmt);
    }
    else
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_WORLDSTATE);

        stmt->setUInt32(0, index);
        stmt->setUInt32(1, uint32(value));

        CharacterDatabase.Execute(stmt);
    }
    m_worldstates[index] = value;
}

uint32 World::getWorldState(uint32 index) const
{
    WorldStatesMap::const_iterator it = m_worldstates.find(index);
    return it != m_worldstates.end() ? it->second : 0;
}

void World::ProcessQueryCallbacks()
{
    PreparedQueryResult result;

    while (!m_realmCharCallbacks.is_empty())
    {
        ACE_Future<PreparedQueryResult> lResult;
        ACE_Time_Value timeout = ACE_Time_Value::zero;
        if (m_realmCharCallbacks.next_readable(lResult, &timeout) != 1)
            break;

        if (lResult.ready())
        {
            lResult.get(result);
            _UpdateRealmCharCount(result);
            lResult.cancel();
        }
    }
}

void World::LoadCharacterNameData()
{
    TC_LOG_INFO("server", "Loading character name data");

    QueryResult result = CharacterDatabase.Query("SELECT guid, name, race, gender, class, level FROM characters WHERE deleteDate IS NULL OR deleteDate = 0");
    if (!result)
    {
        TC_LOG_INFO("server", "No character name data loaded, empty query");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        AddCharacterNameData(fields[0].GetUInt32(), fields[1].GetString(),
            fields[3].GetUInt8() /*gender*/, fields[2].GetUInt8() /*race*/, fields[4].GetUInt8() /*class*/, fields[5].GetUInt8() /*level*/);
        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server", "Loaded name data for %u characters", count);
}

void World::AddCharacterNameData(uint32 guid, std::string const& name, uint8 gender, uint8 race, uint8 playerClass, uint8 level)
{
    CharacterNameData& data = _characterNameDataMap[guid];
    data.m_name = name;
    data.m_race = race;
    data.m_gender = gender;
    data.m_class = playerClass;
    data.m_level = level;
}

void World::UpdateCharacterNameData(uint32 guid, std::string const& name, uint8 gender /*= GENDER_NONE*/, uint8 race /*= RACE_NONE*/)
{
    std::map<uint32, CharacterNameData>::iterator itr = _characterNameDataMap.find(guid);
    if (itr == _characterNameDataMap.end())
        return;

    itr->second.m_name = name;

    if (gender != GENDER_NONE)
        itr->second.m_gender = gender;

    if (race != RACE_NONE)
        itr->second.m_race = race;
}

void World::UpdateCharacterNameDataLevel(uint32 guid, uint8 level)
{
    std::map<uint32, CharacterNameData>::iterator itr = _characterNameDataMap.find(guid);
    if (itr == _characterNameDataMap.end())
        return;

    itr->second.m_level = level;
}

CharacterNameData const* World::GetCharacterNameData(uint32 guid) const
{
    std::map<uint32, CharacterNameData>::const_iterator itr = _characterNameDataMap.find(guid);
    if (itr != _characterNameDataMap.end())
        return &itr->second;
    else
        return NULL;
}

void World::UpdatePhaseDefinitions()
{	
    SessionMap::const_iterator itr;	
    for (itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)	
        if (itr->second && itr->second->GetPlayer() && itr->second->GetPlayer()->IsInWorld())	
            itr->second->GetPlayer()->GetPhaseMgr().NotifyStoresReloaded();	
}

void World::ProcessMailboxQueue()
{
    Field * f;
    Field * fetch;

    QueryResult result = CharacterDatabase.Query("SELECT id FROM mailbox_queue LIMIT 10000");

    if(result)
    {
        do
        {
            fetch = result->Fetch();
            uint32 id = fetch[0].GetUInt32();

            QueryResult select = CharacterDatabase.PQuery("SELECT * FROM mailbox_queue WHERE id = '%u'", id);
            if(select)
            {
                do
                {
                    f = select->Fetch();

                    time_t deliver_time = m_gameTime;
                    time_t expire_time = deliver_time + 30 * DAY;

                    uint32 i = 1; //don't need the temp index
                    uint32 messageType = f[i++].GetUInt32();
                    uint32 stationery = f[i++].GetUInt32();
                    uint32 sender_guid = f[i++].GetUInt32();
                    uint32 receiver_guid = f[i++].GetUInt32();
                    std::string subject = f[i++].GetCString();
                    std::string body = f[i++].GetCString();
                    uint32 money = f[i++].GetUInt32();
                    uint32 itemid = f[i++].GetUInt32();
                    uint32 itemcount = f[i++].GetUInt32();

                    uint32 forefir = 0;

                    if(itemid != 0 && itemid != 49426 && itemid != 38186 && itemid != 62062 && itemid != 47241 && money)
                    {
                        forefir = money / 10000;
                        money = 0;
                    }

                    ItemTemplate const *pProto = sObjectMgr->GetItemTemplate(itemid);
                    if(itemid && !pProto)
                        continue;

                    uint8 countmails = 1;
                    uint32 stack = pProto ? pProto->GetMaxStackSize() : uint32(0x7FFFFFFF-1);
                    if (itemid != 0)
                    {
                        if (itemcount > MAX_MAIL_ITEMS*stack)
                        {
                            uint32 tempitemcount = itemcount;
                            while (tempitemcount > MAX_MAIL_ITEMS*stack)
                            {
                                countmails++;
                                tempitemcount -= MAX_MAIL_ITEMS*stack;
                            }
                        }
                    }
                    for (uint8 j = 0; j < countmails; ++j)
                    {
                        uint32 mailId = sObjectMgr->GenerateMailID();
                        Player * receiver = ObjectAccessor::FindPlayer(receiver_guid);
                        
                        std::vector<Item*> ItemsOnMail;
                        ItemsOnMail.clear();
                        
                        uint32 countitems = 0;
                        if (itemid != 0)
                        {
                            if (itemcount <= MAX_MAIL_ITEMS*stack)
                                countitems = itemcount;
                            else
                            {
                                countitems = MAX_MAIL_ITEMS*stack;
                                itemcount -= MAX_MAIL_ITEMS*stack;
                            }
                        }
                        
                        SQLTransaction trans = CharacterDatabase.BeginTransaction();

                        uint32 tempcountitems = countitems;
                        while (countitems && itemid)
                        {
                            Item *attachment = NULL;

                            if (itemid != 0)
                            {
                                if (countitems > stack)
                                {
                                    attachment = Item::CreateItem ( itemid, stack, receiver );
                                    countitems -= stack;
                                }
                                else
                                {
                                    attachment = Item::CreateItem ( itemid, countitems, receiver );
                                    countitems = 0;
                                }
                                if (attachment)
                                {
                                    attachment->SaveToDB(trans);
                                    ItemsOnMail.push_back(attachment);
                                }
                            }
                            if(attachment != NULL)
                            {
                                trans->PAppend("REPLACE INTO mail_items (mail_id,item_guid,receiver) VALUES ('%u', '%u', '%u')", mailId, attachment->GetGUIDLow(), receiver_guid);
                                if(forefir)
                                    trans->PAppend("REPLACE INTO character_donate (`owner_guid`, `itemguid`, `itemEntry`, `efircount`, `count`) VALUES ('%u', '%u', '%u', '%u', '%u')", receiver_guid, attachment->GetGUIDLow(), itemid, forefir, tempcountitems);
                            }
                        }

                        // Add to DB
                        CharacterDatabase.EscapeString(subject);
                        CharacterDatabase.EscapeString(body);
                        trans->PAppend("REPLACE INTO mail (id,messageType,stationery,mailTemplateId,sender,receiver,subject,body,has_items,expire_time,deliver_time,money,cod,checked) VALUES ('%u', '%u', '%u', '%u', '%u', '%u', '%s', '%s', '%u', '" UI64FMTD "','" UI64FMTD "', '%u', '%u', '%d')",
                            mailId, messageType, stationery, 0, sender_guid, receiver_guid, subject.c_str(), body.c_str(), (tempcountitems != 0 ? 1 : 0), (uint64)expire_time, (uint64)deliver_time, money, 0, 0);
             
                        TC_LOG_ERROR("remote","Sending mail to %u, Item:%u , ItemCount:%u, %s", receiver_guid, itemid,tempcountitems,body.c_str());

                        CharacterDatabase.CommitTransaction(trans);

                        // For online receiver update in game mail status and data
                        if (receiver)
                        {
                            receiver->AddNewMailDeliverTime(deliver_time);

                            if (receiver->IsMailsLoaded())
                            {
                                Mail *m = new Mail;
                                m->messageID = mailId;
                                m->messageType = messageType;
                                m->stationery = stationery;
                                m->mailTemplateId = 0;
                                m->sender = sender_guid;
                                m->receiver = receiver->GetGUIDLow();
                                m->subject = subject;
                                m->body = body;

                                if(!ItemsOnMail.empty())
                                    for (Item* pitem : ItemsOnMail)
                                        m->AddItem(pitem->GetGUIDLow(), pitem->GetEntry());

                                m->expire_time = expire_time;
                                m->deliver_time = deliver_time;
                                m->money = money;
                                m->COD = 0;
                                m->checked = 0;
                                m->state = MAIL_STATE_UNCHANGED;

                                receiver->AddMail(m);                           // to insert new mail to beginning of maillist

                                if(!ItemsOnMail.empty())
                                    for (Item* pitem : ItemsOnMail)
                                        receiver->AddMItem(pitem);
                            }
                            else if(!ItemsOnMail.empty())
                                for (Item* pitem : ItemsOnMail)
                                    delete pitem;
                        }
                        else if(!ItemsOnMail.empty())
                            for (Item* pitem : ItemsOnMail)
                                delete pitem;

                    }
                    CharacterDatabase.PExecute("DELETE FROM mailbox_queue WHERE id = '%u'", id);
                } while (select->NextRow());
            }
		} while ( result->NextRow() );
        TC_LOG_ERROR("server", "MailboxQueue: Complete");
    }
    else
        TC_LOG_ERROR("server", "MailboxQueue: No pending mail");
}

void World::InitServerAutoRestartTime()
{
    time_t serverRestartTime = sWorld->getWorldState(WS_AUTO_SERVER_RESTART_TIME);
    if (!serverRestartTime)
        m_NextServerRestart = time_t(time(NULL));         // game time not yet init

    // generate time by config
    time_t curTime = time(NULL);
    tm localTm = *localtime(&curTime);
    localTm.tm_hour = getIntConfig(CONFIG_AUTO_SERVER_RESTART_HOUR);
    localTm.tm_min = 0;
    localTm.tm_sec = 0;

    // current day reset time
    time_t nextDayRestartTime = mktime(&localTm);

    // next reset time before current moment
    if (curTime >= nextDayRestartTime)
        nextDayRestartTime += DAY;

    // normalize reset time
    m_NextServerRestart = serverRestartTime < curTime ? nextDayRestartTime - DAY : nextDayRestartTime;

    if (!serverRestartTime)
        sWorld->setWorldState(WS_AUTO_SERVER_RESTART_TIME, m_NextServerRestart);

    if (m_bool_configs[CONFIG_DISABLE_RESTART])
        m_NextServerRestart += DAY*1;
}

void World::AutoRestartServer()
{
    TC_LOG_INFO("server","Automatic server restart 60s.");

    sWorld->ShutdownServ(60, SHUTDOWN_MASK_RESTART, RESTART_EXIT_CODE);

    m_NextServerRestart = time_t(m_NextServerRestart + DAY*1);
    sWorld->setWorldState(WS_AUTO_SERVER_RESTART_TIME, m_NextServerRestart);
}

void World::Transfer()
{
    //state 0 waiting to dump on source server
    //state 1 load dump char to dest server
    //state 2 error to many char on acc
    //state 3 dump char to waiting buy transfer
    //state 4 waiting buy transfer
    QueryResult toDump = LoginDatabase.PQuery("SELECT `id`, `account`, `perso_guid`, `to`, `state` FROM transferts WHERE `from` = %u AND state IN (0,3)", realmID);
    QueryResult toLoad = LoginDatabase.PQuery("SELECT `id`, `account`, `perso_guid`, `from`, `dump`, `toacc`, `transferId` FROM transferts WHERE `to` = %u AND state = 1", realmID);

    TC_LOG_DEBUG("network", "World::Transfer()");

    if(toDump)
    {
        do
        {
            Field* field = toDump->Fetch();
            uint32 transaction = field[0].GetUInt32();
            uint32 account = field[1].GetUInt32();
            uint32 guid = field[2].GetUInt32();
            uint32 to = field[3].GetUInt32();
            uint32 state = field[4].GetUInt32();

            if (Player * pPlayer = sObjectMgr->GetPlayerByLowGUID(guid))
            {
                pPlayer->GetSession()->SendNotification("You must logout to transfer your char");
                continue;
            }

            CharacterDatabase.PExecute("DELETE FROM `group_member` WHERE `memberGuid` = '%u'",  guid);
            CharacterDatabase.PExecute("DELETE FROM `guild_member` WHERE `guid` = '%u'",        guid);

            std::string dump;
            DumpReturn dumpState = PlayerDumpWriter().WriteDump(guid, dump);

            TC_LOG_DEBUG("network", "Transfer toDump guid %u, dump %u", guid, dumpState);

            if (dumpState == DUMP_SUCCESS)
            {
                CharacterDatabase.PExecute("UPDATE `characters` SET `at_login` = '512', `deleteInfos_Name` = `name`, `deleteInfos_Account` = `account`, `deleteDate` ='" UI64FMTD "', `name` = '', `account` = 0, `transfer` = '%u' WHERE `guid` = '%u'", uint64(time(NULL)), to, guid);
                PreparedStatement * stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_DUMP);
                if(stmt)
                {
                    stmt->setString(0, dump);
                    if(state == 3)
                        stmt->setUInt32(1, 4);
                    else
                        stmt->setUInt32(1, 1);
                    stmt->setUInt32(2, transaction);
                    LoginDatabase.Execute(stmt);
                }
            }
            else
            {
                LoginDatabase.PQuery("UPDATE `transferts` SET `error` = %u WHERE `id` = '%u'", dumpState, transaction);
                continue;
            }
        }
        while(toDump->NextRow());
    }

    if(toLoad)
    {
        do
        {
            Field* field = toLoad->Fetch();
            uint32 transaction = field[0].GetUInt32();
            uint32 account = field[1].GetUInt32();
            uint32 guid = field[2].GetUInt32();
            uint32 from = field[3].GetUInt32();
            std::string dump = field[4].GetString();
            uint32 toacc = field[5].GetUInt32();
            uint32 transferId = field[6].GetUInt32();
            uint32 newguid = 0;
            if(!toacc)
                toacc = account;

            DumpReturn dumpState = PlayerDumpReader().LoadDump(toacc, dump, "", newguid);

            TC_LOG_DEBUG("network", "Transfer toLoad guid %u, dump %u", guid, dumpState);

            if (dumpState == DUMP_SUCCESS)
            {
                LoginDatabase.PQuery("DELETE FROM `transferts` WHERE `id` = %u", transaction);
                PreparedStatement * stmt = LoginDatabase.GetPreparedStatement(LOGIN_ADD_TRANSFERTS_LOGS);
                if(stmt)
                {
                    stmt->setUInt32(0, transaction);
                    stmt->setUInt32(1, account);
                    stmt->setUInt32(2, guid);
                    stmt->setUInt32(3, from);
                    stmt->setUInt32(4, realmID);
                    stmt->setString(5, dump);
                    stmt->setUInt32(6, toacc);
                    stmt->setUInt32(7, newguid);
                    stmt->setUInt32(8, transferId);
                    LoginDatabase.Execute(stmt);
                    if(transferId)
                    {
                        if(realmID == 59)
                            LoginDatabase.PQuery("UPDATE `transfer_requests` SET `guid` = '%u' WHERE `id` = '%u'", newguid, transferId);
                        else
                            LoginDatabase.PQuery("UPDATE `transfer_requests` SET `status` = '%u', `guid` = '%u' WHERE `id` = '%u'", dumpState, newguid, transferId);
                    }
                }
            }
            else
            {
                LoginDatabase.PQuery("UPDATE `transferts` SET `error` = '%u', `nb_attempt` = `nb_attempt` + 1 WHERE `id` = '%u'", dumpState, transaction);
                if(transferId && realmID != 59)
                    LoginDatabase.PQuery("UPDATE `transfer_requests` SET `status` = '%u' WHERE `id` = '%u'", dumpState, transferId);
                continue;
            }
        }
        while (toLoad->NextRow());
    }
}

void World::GetPoolGuids()
{
    if(QueryResult queryPool = CharacterDatabase.Query("SELECT `id`, `type`, `count` FROM character_pool_guid WHERE `start` = 0;"))
    {
        do
        {
            Field* field = queryPool->Fetch();
            uint32 idPool = field[0].GetUInt32();
            uint8 typePool = field[1].GetUInt8();
            uint32 count = field[2].GetUInt32();

            GuidDiapason diapason = sObjectMgr->GenerateNewDiapasonFor(typePool, count);
            CharacterDatabase.PQuery("UPDATE `character_pool_guid` SET `start` = %u, `end` = %u WHERE `id` = '%u' AND `type` = '%u'", diapason.m_begin, diapason.m_end, idPool, typePool);
        }
        while(queryPool->NextRow());
    }
}

void World::AddZoneDiff(uint16 *zoneDiffs)
{
    for (uint16 i = 0; i < 6864; i++)
        if (zoneDiffs[i])
        {
            m_zonesDiff[i][0] += zoneDiffs[i];
            m_zonesDiff[i][1] += 1;
        }
}

void World::HandleZoneDiff()
{
    uint16 dist = m_int_configs[CONFIG_MAX_POSSIBLE_VISIBILITY_RANGE];
    uint16 maxDiff = m_int_configs[CONFIG_MAX_ZONES_DIFF];
    uint16 critDiff = m_int_configs[CONFIG_CRITICAL_ZONES_DIFF];

    for (uint16 i = 0; i < 6864; i++)
        if (uint16 diff = m_zonesDiff[i][0])
        {
            uint16 count = m_zonesDiff[i][1];

            diff /= count;

            if (diff > maxDiff)
            {
                if (diff < critDiff)
                {
                    diff -= maxDiff;
                    m_zonesDiff[i][2] = (dist * (dist - ((diff * 100) / critDiff))) / 100;
                }
                else
                    m_zonesDiff[i][2] = 10;
            }
            else
                m_zonesDiff[i][2] = dist;

            m_zonesDiff[i][0] = 0;
            m_zonesDiff[i][1] = 0;
        }
}
