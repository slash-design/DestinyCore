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

#include "CommandIC.h"
#include "BotAI.h"
#include <BattlegroundIC.h>

CommandIC::CommandIC(Battleground* pBG, TeamId team) :
CommandBG(pBG, team),
lastUpdateTick(COMMANDBG_UPDATE_TICK / 5)
{
	if (team == TeamId::TEAM_ALLIANCE)
	{
		m_AIWPEntrys.push_back(68);
		m_AIWPEntrys.push_back(59);
		m_AIWPEntrys.push_back(60);
		m_AIWPEntrys.push_back(61);

		m_AIWPEntrys.push_back(62);
		m_AIWPEntrys.push_back(63);
		m_AIWPEntrys.push_back(64);

		m_AIWPEntrys.push_back(65);
		m_AIWPEntrys.push_back(66);
		m_AIWPEntrys.push_back(67);

		m_AIWPEntrys.push_back(70);
		m_AIWPEntrys.push_back(71);
	}
	else
	{
		m_AIWPEntrys.push_back(69);
		m_AIWPEntrys.push_back(67);
		m_AIWPEntrys.push_back(66);
		m_AIWPEntrys.push_back(65);

		m_AIWPEntrys.push_back(62);
		m_AIWPEntrys.push_back(63);
		m_AIWPEntrys.push_back(64);

		m_AIWPEntrys.push_back(61);
		m_AIWPEntrys.push_back(60);
		m_AIWPEntrys.push_back(59);

		m_AIWPEntrys.push_back(71);
		m_AIWPEntrys.push_back(70);
	}
}

CommandIC::~CommandIC()
{
}

void CommandIC::Initialize()
{
	CommandBG::Initialize();
}

const Creature* CommandIC::GetMatchGraveyardNPC(const Player* player)
{
	if (!player || !player->InBattleground() || !m_pBattleground)
		return NULL;
	BattlegroundIC* pBGIC = dynamic_cast<BattlegroundIC*>(m_pBattleground);
	if (!pBGIC)
		return NULL;
	return pBGIC->GetClosestGraveCreature(player);
}

void CommandIC::Update(uint32 diff)
{
	CommandBG::Update(diff);

	if (m_pBattleground && m_pBattleground->GetStatus() == BattlegroundStatus::STATUS_IN_PROGRESS)
	{
		lastUpdateTick -= int32(diff);
		if (lastUpdateTick <= 0)
		{
			switch (CommandBG::g_CommandModelType)
			{
			case CommandModel::CM_Dispersibility:
				RndStartCommand();
				lastUpdateTick = COMMANDBG_UPDATE_TICK * 15;
				break;
			case CommandModel::CM_GroupFocus:
				ProcessGroupFocus(m_BGKeyWaypoints[AIWP_IC_GROUPFOCUS], m_BGKeyWaypoints[AIWP_IC_WORKSHOP]);
				lastUpdateTick = COMMANDBG_UPDATE_TICK;
				break;
			case CommandModel::CM_Regulation:
				ProcessRegulation();
				lastUpdateTick = COMMANDBG_UPDATE_TICK / 10;
				break;
			default:
				lastUpdateTick = COMMANDBG_UPDATE_TICK;
				break;
			}
		}
	}
}

AIWaypoint* CommandIC::GetReadyPosition()
{
	return sAIWPMgr->FindAIWaypoint(m_AIWPEntrys[AIWP_IC_SELF_READY]);
}

void CommandIC::RndStartCommand()
{
	for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
	{
		BotBGAI* pBotAI = GetBotBGAI(itGuid->first);
		if (!pBotAI)
			continue;
		BGCommandInfo comInfo(BGCommandType::BGCT_GuardPoint);
		comInfo.rndOffset = 5.0f;
		uint32 rnd = irand(0, 4);
		switch (rnd)
		{
		case 0:
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_IC_SELF_CITYOUT];
			break;
		case 1:
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_IC_WORKSHOP];
			break;
		case 2:
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_IC_DOCK];
			break;
		case 3:
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_IC_AIRSHIP];
			break;
		case 4:
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_IC_ENEMY_CITYOUT];
			break;
		}
		pBotAI->GetAIMovement()->AcceptCommand(comInfo.targetAIWP);
	}
}

void CommandIC::ProcessRegulation()
{
	PlayerGUIDs allPlayers;
	for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
	{
		allPlayers.insert(itGuid->first);
		TryOccupiedICNode(itGuid->first);
	}
	if (!allPlayers.empty())
		ProcessICNodeRequirement(GetICNodeObjectType(AIWP_IC_WORKSHOP, m_TeamID), m_BGKeyWaypoints[AIWP_IC_WORKSHOP], 5, allPlayers);
	if (!allPlayers.empty())
		ProcessICNodeRequirement(GetICNodeObjectType(AIWP_IC_SELF_CITY, m_TeamID), m_BGKeyWaypoints[AIWP_IC_SELF_READY], 0, allPlayers);
	if (!allPlayers.empty())
		ProcessICNodeRequirement(GetICNodeObjectType(AIWP_IC_DOCK, m_TeamID), m_BGKeyWaypoints[AIWP_IC_DOCK], 2, allPlayers);

	if (!allPlayers.empty())
	{
		for (PlayerGUIDs::iterator itGuid = allPlayers.begin(); itGuid != allPlayers.end(); itGuid++)
			AcceptCommandByPlayerGUID(*itGuid, m_BGKeyWaypoints[AIWP_IC_WORKSHOP]);
	}
}

void CommandIC::ProcessICNodeRequirement(uint32 nodeType, AIWaypoint* waypoint, uint32 baseCount, PlayerGUIDs& players)
{
	if (!waypoint || nodeType == 0)
		return;
	GameObject* pBGNode = m_pBattleground->GetBGObject(nodeType);
	if (!pBGNode)
		return;
	BattlegroundIC* pBGIC = dynamic_cast<BattlegroundIC*>(m_pBattleground);
	if (!pBGIC)
		return;
	bool flagIsOcupied = pBGIC->GetNodeState(nodeType) >= NODE_STATE_CONTROLLED_A;

	PlayerGUIDs nodeNearPlayers = GetICFlagRangePlayerByTeam(nodeType, m_TeamID);
	uint32 enemyCount = GetICFlagRangePlayerByTeam(nodeType, (m_TeamID == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE).size();
	uint32 nearEnemyCount = GetICFlagRangePlayerByTeam(nodeType, (m_TeamID == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE, 10.0f).size();
	int32 maxNeed = m_PlayerGUIDs.size() / 3 * 2;
	int32 needCount = int32(enemyCount) + int32(baseCount);
	if (needCount > maxNeed)
		needCount = maxNeed;
	if (needCount == 0)
		return;
	int32 canStealFlag = (nearEnemyCount == 0 && !flagIsOcupied) ? 1 : 0;
	for (PlayerGUIDs::iterator itGuid = nodeNearPlayers.begin(); itGuid != nodeNearPlayers.end(); itGuid++)
	{
		uint64 guid = *itGuid;
		if (canStealFlag > 0)
		{
			if (!AcceptCommandByPlayerGUID(guid, pBGNode->GetGUID()))
				continue;
			--canStealFlag;
		}
		else if (!AcceptCommandByPlayerGUID(guid, waypoint))
			continue;
		players.erase(guid);
		--needCount;
		if (needCount <= 0)
			break;
	}
	while (needCount > 0 && !players.empty())
	{
		float minDistance = 99999;
		uint64 minGUID = 0;
		for (PlayerGUIDs::iterator itGuid = players.begin(); itGuid != players.end(); itGuid++)
		{
			Position pos = GetPositionByGuid(*itGuid);
			float posDis = pBGNode->GetDistance(pos);
			if (minGUID == 0 || posDis < minDistance)
			{
				minDistance = posDis;
				minGUID = *itGuid;
			}
		}
		if (minGUID != 0)
		{
			players.erase(minGUID);
			if (canStealFlag > 0)
			{
				if (!AcceptCommandByPlayerGUID(minGUID, pBGNode->GetGUID()))
					continue;
				--canStealFlag;
			}
			else if (AcceptCommandByPlayerGUID(minGUID, waypoint))
				--needCount;
		}
		else
			break;
	}
}

PlayerGUIDs CommandIC::GetICFlagRangePlayerByTeam(uint32 nodeType, TeamId team, float range)
{
	PlayerGUIDs existPlayers;
	if (!m_pBattleground)
		return existPlayers;
	GameObject* pBGNode = m_pBattleground->GetBGObject(nodeType);
	if (!pBGNode)
		return existPlayers;
	NearPlayerList playersNearby;
	pBGNode->GetPlayerListInGrid(playersNearby, range);
	for (Player* player : playersNearby)
	{
		if (player->GetTeamId() != team)
			continue;
		existPlayers.insert(player->GetGUID());
	}
	return existPlayers;
}

bool CommandIC::AcceptCommandByPlayerGUID(uint64 guid, AIWaypoint* targetAIWP, bool isFlag /* = false */)
{
	if (!targetAIWP)
		return false;
	BotBGAI* pBotAI = GetBotBGAI(guid);
	if (!pBotAI)
		return false;
	pBotAI->GetAIMovement()->AcceptCommand(targetAIWP, isFlag);
	return true;
}

bool CommandIC::AcceptCommandByPlayerGUID(uint64 guid, ObjectGuid flagGuid, bool isFlag /* = false */)
{
	if (flagGuid.IsEmpty())
		return false;
	BotBGAI* pBotAI = GetBotBGAI(guid);
	if (!pBotAI)
		return false;
	pBotAI->GetAIMovement()->AcceptCommand(flagGuid, isFlag);
	return true;
}

void CommandIC::TryOccupiedICNode(uint64 guid)
{
	Player* player = GetBGPlayer(guid);
	if (!player || player->HasUnitState(UNIT_STATE_CASTING))// || player->IsInCombat())
		return;
	BotBGAI* pBotAI = GetBotBGAI(guid);
	if (!pBotAI || pBotAI->NearHasEnemy(20.0f))
		return;
	BattlegroundIC* pBattlegroundIC = dynamic_cast<BattlegroundIC*>(m_pBattleground);
	if (!pBattlegroundIC)
		return;
	GameObject const* pFlag = pBattlegroundIC->GetClosestEnemyFlagByRange(player, 6);
	if (!pFlag)
		return;
	SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(21651);
	if (!spellInfo)
		return;

	pBotAI->GetAIMovement()->ClearMovement();
	pBotAI->Dismount();
	SpellCastTargets targets;
	targets.SetTargetMask(TARGET_FLAG_GAMEOBJECT);
	targets.SetGOTarget((GameObject*)pFlag);
    Spell* spell = new Spell(player, spellInfo, TRIGGERED_NONE, ObjectGuid::Empty, false);
	spell->prepare(&targets);
	//player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT, pFlag->GetEntry());
}

uint32 CommandIC::GetICNodeObjectType(uint32 index, TeamId team)
{
	BattlegroundIC* pBattlegroundIC = dynamic_cast<BattlegroundIC*>(m_pBattleground);
	if (!pBattlegroundIC)
		return 0;
	if (index < AIWP_IC_WORKSHOP)
	{
		return pBattlegroundIC->GetNodeObjectType((team == TEAM_ALLIANCE) ? ICNodePointType::NODE_TYPE_GRAVEYARD_A : ICNodePointType::NODE_TYPE_GRAVEYARD_H);
	}
	else if (index == AIWP_IC_WORKSHOP)
	{
		return pBattlegroundIC->GetNodeObjectType(ICNodePointType::NODE_TYPE_WORKSHOP);
	}
	else if (index == AIWP_IC_DOCK)
	{
		return pBattlegroundIC->GetNodeObjectType(ICNodePointType::NODE_TYPE_DOCKS);
	}
	else if (index == AIWP_IC_AIRSHIP)
	{
		return pBattlegroundIC->GetNodeObjectType(ICNodePointType::NODE_TYPE_HANGAR);
	}
	else if (index > AIWP_IC_AIRSHIP)
	{
		return pBattlegroundIC->GetNodeObjectType((team == TEAM_ALLIANCE) ? ICNodePointType::NODE_TYPE_GRAVEYARD_H : ICNodePointType::NODE_TYPE_GRAVEYARD_A);
	}
	return 0;
}
