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

#include "CommandAB.h"
#include "BotAI.h"
#include <BattlegroundAB.h>

CommandAB::CommandAB(Battleground* pBG, TeamId team) :
CommandBG(pBG, team),
lastUpdateTick(COMMANDBG_UPDATE_TICK / 5)
{
	if (team == TeamId::TEAM_ALLIANCE)
	{
		m_AIWPEntrys.push_back(17);
		m_AIWPEntrys.push_back(18);
		m_AIWPEntrys.push_back(19);
		m_AIWPEntrys.push_back(20);
		m_AIWPEntrys.push_back(21);

		m_AIWPEntrys.push_back(21);
		m_AIWPEntrys.push_back(20);
		m_AIWPEntrys.push_back(19);
		m_AIWPEntrys.push_back(18);
		m_AIWPEntrys.push_back(17);

		m_AIWPEntrys.push_back(22);
		m_AIWPEntrys.push_back(23);
		m_AIWPEntrys.push_back(24);
		m_AIWPEntrys.push_back(25);
	}
	else
	{
		m_AIWPEntrys.push_back(21);
		m_AIWPEntrys.push_back(20);
		m_AIWPEntrys.push_back(19);
		m_AIWPEntrys.push_back(18);
		m_AIWPEntrys.push_back(17);

		m_AIWPEntrys.push_back(17);
		m_AIWPEntrys.push_back(18);
		m_AIWPEntrys.push_back(19);
		m_AIWPEntrys.push_back(20);
		m_AIWPEntrys.push_back(21);

		m_AIWPEntrys.push_back(23);
		m_AIWPEntrys.push_back(22);
		m_AIWPEntrys.push_back(24);
		m_AIWPEntrys.push_back(25);
	}
}

CommandAB::~CommandAB()
{
}

void CommandAB::Initialize()
{
	CommandBG::Initialize();
}

const Creature* CommandAB::GetMatchGraveyardNPC(const Player* player)
{
	if (!player || !player->InBattleground() || !m_pBattleground)
		return NULL;
	BattlegroundAB* pBGAB = dynamic_cast<BattlegroundAB*>(m_pBattleground);
	if (!pBGAB)
		return NULL;
	return pBGAB->GetClosestGraveCreature(player);
}

void CommandAB::Update(uint32 diff)
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
				ProcessGroupFocus(m_BGKeyWaypoints[AIWP_LM_BRIDGE], m_BGKeyWaypoints[AIWP_BL_BRIDGE]);
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

AIWaypoint* CommandAB::GetReadyPosition()
{
	if (m_TeamID == TEAM_ALLIANCE)
	{
		return sAIWPMgr->FindAIWaypoint(m_AIWPEntrys[AIWP_LM_START]);
	}
	else
	{
		return sAIWPMgr->FindAIWaypoint(m_AIWPEntrys[AIWP_BL_START]);
	}
	return NULL;
}

void CommandAB::RndStartCommand()
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
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_SELF_STABLES];
			break;
		case 1:
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_SELF_LUMBER_MILL];
			break;
		case 2:
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_SELF_BLACKSMITH];
			break;
		case 3:
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_SELF_GOLD_MINE];
			break;
		case 4:
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_SELF_FARM];
			break;
		}
		pBotAI->GetAIMovement()->AcceptCommand(comInfo.targetAIWP);
	}
}

void CommandAB::ProcessRegulation()
{
	PlayerGUIDs allPlayers;
	for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
	{
		allPlayers.insert(itGuid->first);
		TryOccupiedABNode(itGuid->first);
	}
	ProcessABNodeRequirement(GetABNodeIndexByTeam(AIWP_SELF_STABLES, m_TeamID), m_BGKeyWaypoints[AIWP_SELF_STABLES], allPlayers);
	if (!allPlayers.empty())
		ProcessABNodeRequirement(GetABNodeIndexByTeam(AIWP_SELF_LUMBER_MILL, m_TeamID), m_BGKeyWaypoints[AIWP_SELF_LUMBER_MILL], allPlayers);
	if (!allPlayers.empty())
		ProcessABNodeRequirement(GetABNodeIndexByTeam(AIWP_SELF_BLACKSMITH, m_TeamID), m_BGKeyWaypoints[AIWP_SELF_BLACKSMITH], allPlayers);
	if (!allPlayers.empty())
		ProcessABNodeRequirement(GetABNodeIndexByTeam(AIWP_SELF_GOLD_MINE, m_TeamID), m_BGKeyWaypoints[AIWP_SELF_GOLD_MINE], allPlayers);
	if (!allPlayers.empty())
		ProcessABNodeRequirement(GetABNodeIndexByTeam(AIWP_SELF_FARM, m_TeamID), m_BGKeyWaypoints[AIWP_SELF_FARM], allPlayers);
}

void CommandAB::ProcessABNodeRequirement(uint32 abNode, AIWaypoint* waypoint, PlayerGUIDs& players)
{
	if (!waypoint)
		return;
	GameObject* pBGNode = m_pBattleground->GetBGObject(abNode * 8);
	if (!pBGNode)
		return;
	PlayerGUIDs nodeNearPlayers = GetABFlagRangePlayerByTeam(abNode, m_TeamID);
	uint32 enemyCount = GetABFlagRangePlayerByTeam(abNode, (m_TeamID == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE).size();
	bool flagIsOvvupied = ABFlagIsOccupied(abNode, m_TeamID);
	bool canStealFlag = (enemyCount == 0 && !flagIsOvvupied);
	int32 needCount = int32(enemyCount) + (flagIsOvvupied ? 2 : 4);
	for (PlayerGUIDs::iterator itGuid = nodeNearPlayers.begin(); itGuid != nodeNearPlayers.end(); itGuid++)
	{
		uint64 guid = *itGuid;
		if (canStealFlag)
		{
			if (!AcceptCommandByPlayerGUID(guid, pBGNode->GetGUID()))
				continue;
			canStealFlag = false;
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
			if (canStealFlag)
			{
				if (!AcceptCommandByPlayerGUID(minGUID, pBGNode->GetGUID()))
					continue;
				canStealFlag = false;
			}
			else if (AcceptCommandByPlayerGUID(minGUID, waypoint))
				--needCount;
		}
		else
			break;
	}
}

bool CommandAB::ABFlagIsOccupied(uint32 abNode, TeamId team)
{
    if (!m_pBattleground || abNode >= BG_AB_BattlegroundNodes::BG_AB_DYNAMIC_NODES_COUNT)
        return false;
    BattlegroundAB* pBattlegroundAB = dynamic_cast<BattlegroundAB*>(m_pBattleground);
    if (!pBattlegroundAB)
        return false;
    uint8 nodeState = pBattlegroundAB->GetABNodeState(abNode);
    if (team == TEAM_ALLIANCE)
    {
        if (nodeState == BG_AB_NodeStatus::BG_AB_NODE_STATUS_ALLY_CONTESTED ||
            nodeState == BG_AB_NodeStatus::BG_AB_NODE_STATUS_ALLY_OCCUPIED)
            return true;
    }
    else
    {
        if (nodeState == BG_AB_NodeStatus::BG_AB_NODE_STATUS_HORDE_CONTESTED ||
            nodeState == BG_AB_NodeStatus::BG_AB_NODE_STATUS_HORDE_OCCUPIED)
            return true;
    }
    return false;
}

PlayerGUIDs CommandAB::GetABFlagRangePlayerByTeam(uint32 abNode, TeamId team)
{
	PlayerGUIDs existPlayers;
	if (!m_pBattleground || abNode >= BG_AB_BattlegroundNodes::BG_AB_DYNAMIC_NODES_COUNT)
		return existPlayers;
	GameObject* pBGNode = m_pBattleground->GetBGObject(abNode * 8);
	if (!pBGNode)
		return existPlayers;
	NearPlayerList playersNearby;
	pBGNode->GetPlayerListInGrid(playersNearby, COMMAND_POINT_IFDISTANCE / 2);
	for (Player* player : playersNearby)
	{
		if (player->GetTeamId() != team)
			continue;
		existPlayers.insert(player->GetGUID());
	}
	return existPlayers;
}

bool CommandAB::AcceptCommandByPlayerGUID(uint64 guid, AIWaypoint* targetAIWP, bool isFlag /* = false */)
{
	if (!targetAIWP)
		return false;
	BotBGAI* pBotAI = GetBotBGAI(guid);
	if (!pBotAI)
		return false;
	pBotAI->GetAIMovement()->AcceptCommand(targetAIWP, isFlag);
	return true;
}

bool CommandAB::AcceptCommandByPlayerGUID(uint64 guid, ObjectGuid flagGuid, bool isFlag /* = false */)
{
	if (flagGuid.IsEmpty())
		return false;
	BotBGAI* pBotAI = GetBotBGAI(guid);
	if (!pBotAI)
		return false;
	pBotAI->GetAIMovement()->AcceptCommand(flagGuid, isFlag);
	return true;
}

void CommandAB::TryOccupiedABNode(uint64 guid)
{
	Player* player = GetBGPlayer(guid);
	if (!player || player->HasUnitState(UNIT_STATE_CASTING) || player->IsInCombat())
		return;
	BotBGAI* pBotAI = GetBotBGAI(guid);
	if (!pBotAI)
		return;
	BattlegroundAB* pBattlegroundAB = dynamic_cast<BattlegroundAB*>(m_pBattleground);
	if (!pBattlegroundAB)
		return;
	GameObject const* pFlag = pBattlegroundAB->GetNearGameObjectFlag(player);
	if (!pFlag)
		return;
	SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(21651);
	if (!spellInfo)
		return;

	pBotAI->Dismount();
	SpellCastTargets targets;
	targets.SetTargetMask(TARGET_FLAG_GAMEOBJECT);
	targets.SetGOTarget((GameObject*)pFlag);
    Spell* spell = new Spell(player, spellInfo, TRIGGERED_NONE, ObjectGuid::Empty, false);
	spell->prepare(&targets);
	//player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT, pFlag->GetEntry());
}

uint32 CommandAB::GetABNodeIndexByTeam(uint32 index, TeamId team)
{
	switch (index)
	{
	case AIWP_SELF_STABLES:
		return (team == TEAM_ALLIANCE) ? BG_AB_BattlegroundNodes::BG_AB_NODE_STABLES : BG_AB_BattlegroundNodes::BG_AB_NODE_FARM;
	case AIWP_SELF_LUMBER_MILL:
		return (team == TEAM_ALLIANCE) ? BG_AB_BattlegroundNodes::BG_AB_NODE_LUMBER_MILL : BG_AB_BattlegroundNodes::BG_AB_NODE_GOLD_MINE;
	case AIWP_SELF_BLACKSMITH:
		return BG_AB_BattlegroundNodes::BG_AB_NODE_BLACKSMITH;
	case AIWP_SELF_GOLD_MINE:
		return (team == TEAM_ALLIANCE) ? BG_AB_BattlegroundNodes::BG_AB_NODE_GOLD_MINE : BG_AB_BattlegroundNodes::BG_AB_NODE_LUMBER_MILL;
	case AIWP_SELF_FARM:
		return (team == TEAM_ALLIANCE) ? BG_AB_BattlegroundNodes::BG_AB_NODE_FARM : BG_AB_BattlegroundNodes::BG_AB_NODE_STABLES;
	}
	return (team == TEAM_ALLIANCE) ? BG_AB_BattlegroundNodes::BG_AB_NODE_STABLES : BG_AB_BattlegroundNodes::BG_AB_NODE_FARM;
}
