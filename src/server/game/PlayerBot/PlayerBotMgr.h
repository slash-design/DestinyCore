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

#ifndef __PLAYERBOTMGR_H__
#define __PLAYERBOTMGR_H__

//#include <chrono>

#include "Log.h"
#include "Common.h"
#include "SharedDefines.h"
#include "DatabaseEnv.h"
#include "Timer.h"
//#include "Callback.h"
#include "PlayerBotSession.h"
#include "BotAITool.h"
#include "LFGMgr.h"
//#include "ArenaTeamMgr.h"
#include "Containers.h"

#define CONVERT_ARENAAI_TOBG

class ObjectGuid;
struct LFGBotRequirement;
struct BotGlobleSchedule;
class PlayerBotSession;

enum PlayerBotAIType
{
	PBAIT_FIELD,
	PBAIT_BG,
	PBAIT_GROUP,
	PBAIT_DUNGEON,
	PBAIT_DUEL,
	PBAIT_ARENA,
	PBAIT_OVER
};

struct PlayerBotCharBaseInfo
{
	uint64 guid;
	uint32 account;
	std::string name;
	uint16 race;
	uint16 profession;
	uint16 gender;
	uint16 level;

	PlayerBotCharBaseInfo()
	{
		guid = 0;
		account = 0;
		race = profession = gender = level = 0;
	}
	PlayerBotCharBaseInfo(uint64 id, uint32 acc, const std::string &na, uint16 ra, uint16 pro, uint16 gen, uint16 lv) :
		guid(id), account(acc), name(na), race(ra), profession(pro), gender(gen), level(lv)
	{
	}

	std::string GetNameANDClassesText();

	TeamId GetCamp()
	{
		if (race == 1 || race == 3 || race == 4 || race == 7 || race == 11)
		{
			return TeamId::TEAM_ALLIANCE;
		}
		if (race == 2 || race == 5 || race == 6 || race == 8 || race == 10)
		{
			return TeamId::TEAM_HORDE;
		}
		return TeamId::TEAM_NEUTRAL;
	}
};

struct PlayerBotBaseInfo
{
	static PlayerBotCharBaseInfo empty;
	bool isAccountInfo;
	uint32 id;
	std::string username;
	std::string pass;
	using CharInfoMap = std::map<uint64, PlayerBotCharBaseInfo>;
	CharInfoMap characters;
	std::queue<WorldPacket> needCreateBots;

	PlayerBotBaseInfo(uint32 uid, const char *name, std::string &pa, bool isAcc) :
		isAccountInfo(isAcc), id(uid), pass(pa)
	{
		characters.clear();
		username = name;
	}

	bool MatchRaceByFuction(bool fuction, uint16 race)
	{
		if (fuction)
		{
			if (race == 1 || race == 3 || race == 4 || race == 7 || race == 11)
			{
				return true;
			}
		}
		else
		{
			if (race == 2 || race == 5 || race == 6 || race == 8 || race == 10)
			{
				return true;
			}
		}
		return false;
	}
	bool ExistClass(bool fuction, uint16 prof)
	{
#ifdef INCOMPLETE_BOT
		if (prof != 1 && prof != 5 && prof != 9)
			return false;
#endif
		for (CharInfoMap::iterator it = characters.begin();
			it != characters.end();
			it++)
		{
			if (it->second.profession == prof)
			{
				uint16 race = it->second.race;
				if (MatchRaceByFuction(fuction, race))
					return true;
			}
		}
		return false;
	}

	PlayerBotCharBaseInfo& GetRandomCharacterByFuction(bool faction)
	{
		if (characters.size() <= 0)
		{
			return empty;
		}
#ifdef INCOMPLETE_BOT
		for (int i = 0; i < 20; i++)
#else
		for (int i = 0; i < 5; i++)
#endif
		{
			int16 select = irand(0, characters.size() / 2 - 1);
			for (CharInfoMap::iterator it = characters.begin();
				it != characters.end();
				it++)
			{
#ifdef INCOMPLETE_BOT
				if (it->second.profession != 1 && it->second.profession != 5 && it->second.profession != 9)
					continue;
#endif
				if (MatchRaceByFuction(faction, it->second.race))
				{
					if (select <= 0)
						return it->second;
					else
						--select;
				}
			}
		}
		return characters.begin()->second;
	}

	std::vector<uint32> GetNoArenaTeamCharacterIDsByFuction(bool faction, ArenaGroupTypes type)
	{
		std::vector<uint32> outIDs;
		if (characters.size() <= 0)
		{
			return outIDs;
		}
		for (CharInfoMap::iterator it = characters.begin();
			it != characters.end();
			it++)
		{
#ifdef INCOMPLETE_BOT
			if (it->second.profession != 1 && it->second.profession != 5 && it->second.profession != 9)
				continue;
#endif
			if (MatchRaceByFuction(faction, it->second.race))
			{
				//if (sArenaTeamMgr->ExistArenaTeamByType(ObjectGuid(uint64(it->second.guid)), type))
				//	continue;
				outIDs.push_back(it->second.guid);
			}
		}
		Trinity::Containers::RandomShuffle(outIDs);
		//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		//std::shuffle(outIDs.begin(), outIDs.end(), std::default_random_engine(time(NULL)));
		return outIDs;
	}

	PlayerBotCharBaseInfo& GetCharacter(bool faction, uint32 prof)
	{
		if (characters.size() <= 0)
			return empty;
		for (CharInfoMap::iterator it = characters.begin();
			it != characters.end();
			it++)
		{
			if (!MatchRaceByFuction(faction, it->second.race))
				continue;
			if (it->second.profession == prof)
				return it->second;
		}
		return empty;
	}

	uint32 GetCharIDByNoArenaType(bool faction, uint32 prof, uint32 arenaType, std::vector<ObjectGuid>& fliters);

	bool ExistCharacterByGUID(ObjectGuid& guid)
	{
		for (CharInfoMap::iterator it = characters.begin();
			it != characters.end();
			it++)
		{
			if (it->second.guid == guid.GetCounter())
				return true;
		}
		return false;
	}

	TeamId GetTeamIDByChar(ObjectGuid& guid)
	{
		uint32 id = guid.GetCounter();
		for (CharInfoMap::iterator it = characters.begin();
			it != characters.end();
			it++)
		{
			if (it->second.guid != id)
				continue;
			return it->second.GetCamp();
		}
		return TeamId::TEAM_NEUTRAL;
	}

	std::string GetCharNameANDClassesText(ObjectGuid& guid)
	{
		uint32 id = guid.GetCounter();
		for (CharInfoMap::iterator it = characters.begin();
			it != characters.end();
			it++)
		{
			if (it->second.guid != id)
				continue;
			return it->second.GetNameANDClassesText();
		}
		return "";
	}

	bool RemoveCharacterByGUID(ObjectGuid& guid)
	{
		uint32 id = guid.GetCounter();
		for (CharInfoMap::iterator it = characters.begin();
			it != characters.end();
			it++)
		{
			if (it->second.guid != id)
				continue;
			characters.erase(it);
			return true;
		}
		return false;
	}
};

//using BattlegroundTypeId = uint16;
class TC_GAME_API PlayerBotMgr
{
	typedef std::vector<BattlegroundTypeId> BattleGroundTypes;

private:
	PlayerBotMgr();
	~PlayerBotMgr();

public:
	PlayerBotMgr(PlayerBotMgr const&) = delete;
	PlayerBotMgr(PlayerBotMgr&&) = delete;

	PlayerBotMgr& operator= (PlayerBotMgr const&) = delete;
	PlayerBotMgr& operator= (PlayerBotMgr&&) = delete;

	static PlayerBotMgr* instance();
	static void SwitchPlayerBotAI(Player* player, PlayerBotAIType aiType, bool force);

	std::string GetPlayerLinkText(Player const* player) const;

	std::string RandomArenaName();
	PlayerBotSession* GetBotSessionByCharGUID(ObjectGuid& guid);
	TeamId GetTeamIDByPlayerBotGUID(ObjectGuid& guid);
	bool IsPlayerBot(WorldSession *pSession);
	bool IsBotAccuntName(std::string name);
	bool IsIDLEPlayerBot(Player* player);
	void DestroyBotMail(uint32 guid);
	void LoadPlayerBotBaseInfo();
	void AddNewAccountBotBaseInfo(std::string name);
	std::set<uint32> GetArenaTeamPlayerBotIDCountByTeam(TeamId team, int32 count, ArenaGroupTypes type);
	PlayerBotBaseInfo* GetPlayerBotAccountInfo(uint32 guid);
	PlayerBotBaseInfo* GetAccountBotAccountInfo(uint32 guid);

	void UpdateLastAccountIndex(std::string& username);
	void UpAllPlayerBotSession();

	void OnPlayerBotCreate(ObjectGuid const& guid, uint32 accountId, std::string const& name, uint8 gender, uint8 race, uint8 playerClass, uint8 level);
	void OnAccountBotCreate(ObjectGuid const& guid, uint32 accountId, std::string const& name, uint8 gender, uint8 race, uint8 playerClass, uint8 level);
	void OnAccountBotDelete(ObjectGuid& guid, uint32 accountId);
	void OnPlayerBotLogin(WorldSession* pSession, Player* pPlayer);
	void OnPlayerBotLogout(WorldSession* pSession);
	void OnPlayerBotLeaveOriginalGroup(Player* pPlayer);
	void LoginGroupBotByPlayer(Player* pPlayer);
	void LoginFriendBotByPlayer(Player* pPlayer);
	void LogoutAllGroupPlayerBot(Group* pGroup, bool force);

	void AllPlayerBotRandomLogin(const char* name = "");
	void AllPlayerBotLogout();
	bool PlayerBotLogout(uint32 account);
	bool AllPlayerLeaveBG(uint32 account);
	void SupplementPlayerBot();

	void OnRealPlayerJoinBattlegroundQueue(uint32 bgTypeId, uint32 level);
	void OnRealPlayerLeaveBattlegroundQueue(uint32 bgTypeId, uint32 level);
	void OnRealPlayerLeaveArenaQueue(uint32 bgTypeId, uint32 level, uint32 aaType);
	void OnRealPlayerEnterBattleground(uint32 bgTypeId, uint32 level);
	void OnRealPlayerLeaveBattleground(const Player* player);

	void Update();
	uint32 GetOnlineBotCount(TeamId team, bool hasReal);
	uint32 GetOnlineBotCount2(TeamId team, bool hasReal);

	bool LoginBotByAccountIndex(uint32 account, uint32 index);
	void DelayLoginPlayerBotByGUID(ObjectGuid& guid) { m_DelayOnlineBots.push(guid); }
	bool AddNewPlayerBotByGUID(ObjectGuid& guid);
	bool AddNewPlayerBotByGUID2(ObjectGuid& guid);
	void AddNewPlayerBot(bool faction, Classes prof, uint32 count);
	void AddNewAccountBot(bool faction, Classes prof);
	void AddNewPlayerBotByClass(uint32 count, Classes prof);
	bool ChangePlayerBotSetting(uint32 account, uint32 minLV, uint32 maxLV, uint32 talent);
	lfg::LfgRoles GetPlayerBotCurrentLFGRoles(Player* player);
	ObjectGuid GetNoArenaMatchCharacter(TeamId team, uint32 arenaType, Classes cls, std::vector<ObjectGuid>& fliters);
	std::string GetNameANDClassesText(ObjectGuid& guid);
	bool CanReadyArenaByArenaTeamID(uint32 arenaTeamId);
    void SetMax(int max) { m_MaxOnlineBot = max; }
    int32 m_MaxOnlineBot;
    int32 m_BotOnlineCount;

private:
	bool ExistClassByRace(uint8 race, uint8 prof);
	void InitializeCreatePlayerBotName();
	std::string RandomName();
	uint8 RandomRace(bool group, uint8 prof);
	uint8 RandomSkinColor(uint8 race, uint8 gender, uint8 prof);
	uint8 RandomFace(uint8 race, uint8 gender, uint8 skinColor, uint8 prof);
	uint8 RandomHair(uint8 race, uint8 gender, uint8 prof);
	uint8 RandomHairColor(uint8 race, uint8 gender, uint8 hairID, uint8 prof);
	uint8 RandomFacialHair(uint8 race, uint8 gender, uint8 hairColor, uint8 prof);
	PlayerBotSession* UpPlayerBotSessionByBaseInfo(PlayerBotBaseInfo *pAcc, bool accountInfo);
	WorldPacket BuildCreatePlayerData(bool group, uint8 prof);
	void CreateOncePlayerBot();

	void ClearBaseInfo();
	void SupplementAccount();
	void LoadCharBaseInfo();
	void LoadSessionPermissionsCallback(PreparedQueryResult result);

	void ClearEmptyNeedPlayer();
	void ClearNeedPlayer(uint32 bgTypeID, uint32 bracketID);
	void AddNewPlayerBotToBG(TeamId team, uint32 minLV, uint32 maxLV, BattlegroundTypeId bgTypeID);
	//void AddNewPlayerBotToLFG(lfg::LFGBotRequirement* botRequirement);
	void AddNewPlayerBotToAA(TeamId team, BattlegroundTypeId bgTypeID, uint32 bracketID, uint32 aaType);
	void AddTeamBotToRatedArena(uint32 arenaTeamId);
	//bool FillOnlineBotScheduleByLFGRequirement(lfg::LFGBotRequirement* botRequirement, BotGlobleSchedule* botSchedule);
	uint32 GetScheduleTalentByLFGRequirement(lfg::LfgRoles roles, uint32 botCls);
	void QueryBattlegroundRequirement();
	void QueryRatedArenaRequirement();
	void QueryNonRatedArenaRequirement();
	void OnlinePlayerBotByGUIDQueue();
	bool ExistUnBGPlayerBot();
	PVPDifficultyEntry const* FindBGBracketEntry(Battleground* bg_template, uint32 level);

private:
	uint32 m_BotAccountAmount;
	uint32 m_LastBotAccountIndex;
	uint32 m_LFGSearchTick;
	uint32 m_ArenaSearchTick;
	std::map<uint32, PlayerBotBaseInfo*> m_idPlayerBotBase;
	std::map<uint32, PlayerBotBaseInfo*> m_idAccountBotBase;

	BattleGroundTypes m_BGTypes;
	std::vector<std::string> allName;
	std::vector<std::string> allArenaName;
	std::queue<ObjectGuid> m_DelayOnlineBots;

public:
	static std::map<uint32, std::list<UnitAI*> > m_DelayDestroyAIs;
	static std::mutex g_uniqueLock;
};

#define sPlayerBotMgr PlayerBotMgr::instance()

#endif // __PLAYERBOTMGR_H__
