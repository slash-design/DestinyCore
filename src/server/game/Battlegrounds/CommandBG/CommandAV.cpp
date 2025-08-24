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

#include "CommandAV.h"
#include "BotAI.h"
#include "ReputationMgr.h"

CommandAV::CommandAV(Battleground* pBG, TeamId team) :
CommandBG(pBG, team),
lastUpdateTick(COMMANDBG_UPDATE_TICK / 5),
m_IsStartGeneral(false)
{
	if (team == TeamId::TEAM_ALLIANCE)
	{
		m_AIWPEntrys.push_back(37);
		m_AIWPEntrys.push_back(38);
		m_AIWPEntrys.push_back(39);
		m_AIWPEntrys.push_back(40);
		m_AIWPEntrys.push_back(41);
		m_AIWPEntrys.push_back(42);
		m_AIWPEntrys.push_back(43);
		m_AIWPEntrys.push_back(44);
		m_AIWPEntrys.push_back(45);

		m_AIWPEntrys.push_back(46);

		m_AIWPEntrys.push_back(48);
		m_AIWPEntrys.push_back(47);
		m_AIWPEntrys.push_back(49);
		m_AIWPEntrys.push_back(50);
		m_AIWPEntrys.push_back(51);
		m_AIWPEntrys.push_back(52);
		m_AIWPEntrys.push_back(53);
		m_AIWPEntrys.push_back(54);
		m_AIWPEntrys.push_back(55);

		m_AIWPEntrys.push_back(56);
		m_AIWPEntrys.push_back(57);
		m_AIWPEntrys.push_back(58);
	}
	else
	{
		m_AIWPEntrys.push_back(55);
		m_AIWPEntrys.push_back(54);
		m_AIWPEntrys.push_back(53);
		m_AIWPEntrys.push_back(52);
		m_AIWPEntrys.push_back(51);
		m_AIWPEntrys.push_back(50);
		m_AIWPEntrys.push_back(49);
		m_AIWPEntrys.push_back(48);
		m_AIWPEntrys.push_back(47);

		m_AIWPEntrys.push_back(46);

		m_AIWPEntrys.push_back(44);
		m_AIWPEntrys.push_back(45);
		m_AIWPEntrys.push_back(43);
		m_AIWPEntrys.push_back(42);
		m_AIWPEntrys.push_back(41);
		m_AIWPEntrys.push_back(40);
		m_AIWPEntrys.push_back(39);
		m_AIWPEntrys.push_back(38);
		m_AIWPEntrys.push_back(37);

		m_AIWPEntrys.push_back(56);
		m_AIWPEntrys.push_back(58);
		m_AIWPEntrys.push_back(57);
	}
}

CommandAV::~CommandAV()
{
}

void CommandAV::Initialize()
{
	CommandBG::Initialize();
}

void CommandAV::StartGame()
{
	CommandBG::StartGame();
	m_IsStartGeneral = false;
}

bool CommandAV::AddPlayerBot(Player* player, Battleground* pBG)
{
	//if (player->IsPlayerBot())
	//{
	//	FactionEntry const* factionEntry = sFactionStore.LookupEntry((player->GetTeamId() == TEAM_ALLIANCE) ? 730 : 729);
	//	if (factionEntry)
	//		player->GetReputationMgr().SetOneFactionReputation(factionEntry, 42000, false);
	//}
	return CommandBG::AddPlayerBot(player, pBG);
}

const Creature* CommandAV::GetMatchGraveyardNPC(const Player* player)
{
	if (!player || !player->InBattleground() || !m_pBattleground)
		return NULL;
	BattlegroundAV* m_pBattlegroundAV = dynamic_cast<BattlegroundAV*>(m_pBattleground);
	if (!m_pBattlegroundAV)
		return NULL;
	return m_pBattlegroundAV->GetClosestGraveCreature(player);
}

void CommandAV::Update(uint32 diff)
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
				ProcessGroupFocus(m_BGKeyWaypoints[AIWP_LM_STONE_GRAVE], m_BGKeyWaypoints[AIWP_BL_CENTER_POINT]);
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

AIWaypoint* CommandAV::GetReadyPosition()
{
	return sAIWPMgr->FindAIWaypoint(m_AIWPEntrys[AIWP_LM_AVSTART_POINT]);
}

bool CommandAV::CanDireFlee()
{
	return false;
}

void CommandAV::RndStartCommand()
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
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_LM_STONE_GRAVE];
			break;
		case 1:
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_LM_WOMEN_POINT];
			break;
		case 2:
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_SNOWFALL_GRAVE];
			break;
		case 3:
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_BL_MAN_POINT];
			break;
		case 4:
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_BL_ICEBLOOD_GRAVE];
			break;
		}
		pBotAI->GetAIMovement()->AcceptCommand(comInfo.targetAIWP);
	}
}

void CommandAV::ProcessRegulation()
{
	BattlegroundAV* pBattlegroundAV = dynamic_cast<BattlegroundAV*>(m_pBattleground);
	if (!pBattlegroundAV)
		return;
	PlayerGUIDs allPlayers;
	for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
	{
		allPlayers.insert(itGuid->first);
		TryOccupiedAVNode(pBattlegroundAV, itGuid->first);
	}
	ProcessAttackGeneral(pBattlegroundAV, allPlayers);
	ProcessDefense(pBattlegroundAV, allPlayers);
	ProcessAssault(pBattlegroundAV, allPlayers);
	for (PlayerGUIDs::iterator itGuid = allPlayers.begin(); itGuid != allPlayers.end(); itGuid++)
		AcceptCommandByPlayerGUID(*itGuid, m_BGKeyWaypoints[AIWP_BL_GENERAL_GRAVE]);
}

void CommandAV::ProcessAssault(BattlegroundAV* pBattlegroundAV, PlayerGUIDs& allPlayers)
{
	BG_AV_Nodes nodeType = (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_STONEHEART_GRAVE : BG_AV_NODES_ICEBLOOD_GRAVE;
	ProcessRequirementByNodeType(pBattlegroundAV, nodeType, m_BGKeyWaypoints[AIWP_LM_STONE_GRAVE], allPlayers);
	if (m_TeamID == TEAM_ALLIANCE)
	{
		if (!allPlayers.empty())
		{
			nodeType = BG_AV_NODES_SNOWFALL_GRAVE;
			ProcessRequirementByNodeType(pBattlegroundAV, nodeType, m_BGKeyWaypoints[AIWP_SNOWFALL_GRAVE], allPlayers);
		}
		if (!allPlayers.empty())
		{
			ProcessRequirementByCaptain(pBattlegroundAV, (m_TeamID == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE,
				m_BGKeyWaypoints[AIWP_BL_MAN_POINT], allPlayers, true);
		}
		if (!allPlayers.empty())
		{
			nodeType = (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_ICEBLOOD_TOWER : BG_AV_NODES_STONEHEART_BUNKER;
			ProcessRequirementByNodeType(pBattlegroundAV, nodeType, m_BGKeyWaypoints[AIWP_BL_ICEBLOOD_TOWER], allPlayers);
		}
		if (!allPlayers.empty())
		{
			nodeType = (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_ICEBLOOD_GRAVE : BG_AV_NODES_STONEHEART_GRAVE;
			ProcessRequirementByNodeType(pBattlegroundAV, nodeType, m_BGKeyWaypoints[AIWP_BL_ICEBLOOD_GRAVE], allPlayers);
		}
	}
	else
	{
		if (!allPlayers.empty())
		{
			nodeType = (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_ICEBLOOD_TOWER : BG_AV_NODES_STONEHEART_BUNKER;
			ProcessRequirementByNodeType(pBattlegroundAV, nodeType, m_BGKeyWaypoints[AIWP_BL_ICEBLOOD_TOWER], allPlayers);
		}
		if (!allPlayers.empty())
		{
			ProcessRequirementByCaptain(pBattlegroundAV, (m_TeamID == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE,
				m_BGKeyWaypoints[AIWP_BL_MAN_POINT], allPlayers, true);
		}
		if (!allPlayers.empty())
		{
			nodeType = (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_ICEBLOOD_GRAVE : BG_AV_NODES_STONEHEART_GRAVE;
			ProcessRequirementByNodeType(pBattlegroundAV, nodeType, m_BGKeyWaypoints[AIWP_BL_ICEBLOOD_GRAVE], allPlayers);
		}
		if (!allPlayers.empty())
		{
			nodeType = BG_AV_NODES_SNOWFALL_GRAVE;
			ProcessRequirementByNodeType(pBattlegroundAV, nodeType, m_BGKeyWaypoints[AIWP_SNOWFALL_GRAVE], allPlayers);
		}
	}
	if (!allPlayers.empty())
	{
		nodeType = (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_TOWER_POINT : BG_AV_NODES_ICEWING_BUNKER;
		ProcessRequirementByNodeType(pBattlegroundAV, nodeType, m_BGKeyWaypoints[AIWP_BL_HIGH_POWER], allPlayers);
	}
	//if (!pBattlegroundAV->NodeIsOccupyByTeamType(m_TeamID, (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_ICEBLOOD_GRAVE : BG_AV_NODES_STONEHEART_GRAVE)/* ||
	//	!pBattlegroundAV->NodeIsOccupyByTeamType(m_TeamID, (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_ICEBLOOD_TOWER : BG_AV_NODES_STONEHEART_BUNKER) ||
	//	!pBattlegroundAV->NodeIsOccupyByTeamType(m_TeamID, (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_TOWER_POINT : BG_AV_NODES_ICEWING_BUNKER)*/)
	//	return;

	if (!allPlayers.empty())
	{
		nodeType = (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_FROSTWOLF_GRAVE : BG_AV_NODES_STORMPIKE_GRAVE;
		ProcessRequirementByNodeType(pBattlegroundAV, nodeType, m_BGKeyWaypoints[AIWP_BL_FROSTWOLF_GRAVE], allPlayers);
	}
	if (!allPlayers.empty())
	{
		nodeType = (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_FROSTWOLF_ETOWER : BG_AV_NODES_DUNBALDAR_SOUTH;
		ProcessRequirementByNodeType(pBattlegroundAV, nodeType, m_BGKeyWaypoints[AIWP_BL_EAST_POWER], allPlayers);
	}
	if (!allPlayers.empty())
	{
		nodeType = (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_FROSTWOLF_WTOWER : BG_AV_NODES_DUNBALDAR_NORTH;
		ProcessRequirementByNodeType(pBattlegroundAV, nodeType, m_BGKeyWaypoints[AIWP_BL_WEST_POWER], allPlayers);
	}
	if (!allPlayers.empty())
	{
		nodeType = (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_FROSTWOLF_HUT : BG_AV_NODES_FIRSTAID_STATION;
		ProcessRequirementByNodeType(pBattlegroundAV, nodeType, m_BGKeyWaypoints[AIWP_BL_GENERAL_GRAVE], allPlayers);
	}
}

void CommandAV::ProcessDefense(BattlegroundAV* pBattlegroundAV, PlayerGUIDs& allPlayers)
{
	BG_AV_Nodes nodeType = (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_STONEHEART_BUNKER : BG_AV_NODES_ICEBLOOD_TOWER;
	ProcessRequirementByNodeType(pBattlegroundAV, nodeType, m_BGKeyWaypoints[AIWP_LM_STONE_TOWER], allPlayers);
	if (!allPlayers.empty())
	{
		ProcessRequirementByCaptain(pBattlegroundAV, (m_TeamID == TEAM_ALLIANCE) ? TEAM_ALLIANCE : TEAM_HORDE,
			m_BGKeyWaypoints[AIWP_LM_WOMEN_POINT], allPlayers, false);
	}
	if (!allPlayers.empty())
	{
		nodeType = (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_ICEWING_BUNKER : BG_AV_NODES_TOWER_POINT;
		ProcessRequirementByNodeType(pBattlegroundAV, nodeType, m_BGKeyWaypoints[AIWP_LM_ICEWING_TOWER], allPlayers);
	}
	if (!allPlayers.empty())
	{
		nodeType = (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_STORMPIKE_GRAVE : BG_AV_NODES_FROSTWOLF_GRAVE;
		ProcessRequirementByNodeType(pBattlegroundAV, nodeType, m_BGKeyWaypoints[AIWP_LM_PIKE_GRAVE], allPlayers);
	}
	if (!allPlayers.empty())
	{
		nodeType = (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_DUNBALDAR_SOUTH : BG_AV_NODES_FROSTWOLF_ETOWER;
		ProcessRequirementByNodeType(pBattlegroundAV, nodeType, m_BGKeyWaypoints[AIWP_LM_SOUTH_TOWER], allPlayers);
	}
	if (!allPlayers.empty())
	{
		nodeType = (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_DUNBALDAR_NORTH : BG_AV_NODES_FROSTWOLF_WTOWER;
		ProcessRequirementByNodeType(pBattlegroundAV, nodeType, m_BGKeyWaypoints[AIWP_LM_NORTH_TOWER], allPlayers);
	}
	if (!allPlayers.empty())
	{
		nodeType = (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_FIRSTAID_STATION : BG_AV_NODES_FROSTWOLF_HUT;
		ProcessRequirementByNodeType(pBattlegroundAV, nodeType, m_BGKeyWaypoints[AIWP_LM_GENERAL_GRAVE], allPlayers);
	}
}

void CommandAV::ProcessAttackGeneral(BattlegroundAV* pBattlegroundAV, PlayerGUIDs& allPlayers)
{
	BG_AV_Nodes nodeType = (m_TeamID == TEAM_ALLIANCE) ? BG_AV_NODES_FROSTWOLF_HUT : BG_AV_NODES_FIRSTAID_STATION;
	bool isOccupy = pBattlegroundAV->NodeIsOccupyByTeamType(m_TeamID, nodeType);
	if (!isOccupy)
		return;
	bool attackOrWait = false;
	PlayerGUIDs selfs = GetAVFlagRangePlayerByTeam(pBattlegroundAV, nodeType, (m_TeamID == TEAM_ALLIANCE) ? TEAM_ALLIANCE : TEAM_HORDE);
	if (selfs.size() < 28 && !m_IsStartGeneral)
		attackOrWait = false;
	else
	{
		m_IsStartGeneral = true;
		attackOrWait = true;
	}
	for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
	{
		bool isForce = attackOrWait ? false : true;
		if (isForce && (selfs.find(itGuid->first) != selfs.end()))
			isForce = false;
		AcceptCommandByPlayerGUID(itGuid->first, attackOrWait ? m_BGKeyWaypoints[AIWP_BL_GENERAL] : m_BGKeyWaypoints[AIWP_BL_GENERAL_GRAVE], isForce);
	}
	allPlayers.clear();
}

void CommandAV::ProcessRequirementByNodeType(BattlegroundAV* pBattlegroundAV, BG_AV_Nodes nodeType, AIWaypoint* waypoint, PlayerGUIDs& allPlayers)
{
    bool nodeState = pBattlegroundAV->NodeIsOccupyByTeamType(m_TeamID, nodeType);
    if (!nodeState)
    {
        uint32 enemyCount = GetAVFlagRangePlayerByTeam(pBattlegroundAV, nodeType, (m_TeamID == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE).size();
        PlayerGUIDs selfPlayers = GetAVFlagRangePlayerByTeam(pBattlegroundAV, nodeType, (m_TeamID == TEAM_ALLIANCE) ? TEAM_ALLIANCE : TEAM_HORDE);
        uint32 needCount = enemyCount + 5;
        if (needCount > 12)
            needCount = 12;
        uint32 canSteal = (enemyCount == 0) ? 3 : 0;
        for (PlayerGUIDs::iterator itGuid = selfPlayers.begin(); itGuid != selfPlayers.end(); itGuid++)
        {
            uint64 guid = *itGuid;
            if (canSteal > 0)
            {
                if (!AcceptCommandByPlayerGUID(guid, waypoint, true))
                    continue;
                --canSteal;
            }
            else
            {
                if (!AcceptCommandByPlayerGUID(guid, waypoint))
                    continue;
            }
            allPlayers.erase(guid);
            --needCount;
            if (needCount <= 0)
                break;
        }
        GameObject const* pBGNode = pBattlegroundAV->GetNodeObjectByPosType(nodeType);
        while (needCount > 0 && !allPlayers.empty() && pBGNode)
        {
            float minDistance = 99999;
            uint64 minGUID = 0;
            for (PlayerGUIDs::iterator itGuid = allPlayers.begin(); itGuid != allPlayers.end(); itGuid++)
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
                allPlayers.erase(minGUID);
                if (AcceptCommandByPlayerGUID(minGUID, waypoint))
                    --needCount;
            }
            else
                break;
        }
    }
}

void CommandAV::ProcessRequirementByCaptain(BattlegroundAV* pBattlegroundAV, TeamId team, AIWaypoint* waypoint, PlayerGUIDs& allPlayers, bool attOrDef)
{
	Creature const* pCaptain = pBattlegroundAV->GetAVAliveCaptainByTeam(team);
	if (!pCaptain)
		return;
	uint32 enemyCount = GetAVCaptainRangePlayerByTeam(pBattlegroundAV, pCaptain, (m_TeamID == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE).size();
	if (!attOrDef && enemyCount <= 3)
		return;
	PlayerGUIDs selfPlayers = GetAVCaptainRangePlayerByTeam(pBattlegroundAV, pCaptain, (m_TeamID == TEAM_ALLIANCE) ? TEAM_ALLIANCE : TEAM_HORDE);
	uint32 needCount = enemyCount + 10;
	for (PlayerGUIDs::iterator itGuid = selfPlayers.begin(); itGuid != selfPlayers.end(); itGuid++)
	{
		uint64 guid = *itGuid;
		if (!AcceptCommandByPlayerGUID(guid, waypoint))
			continue;
		allPlayers.erase(guid);
		--needCount;
		if (needCount <= 0)
			break;
	}
	while (needCount > 0 && !allPlayers.empty())
	{
		float minDistance = 99999;
		uint64 minGUID = 0;
		for (PlayerGUIDs::iterator itGuid = allPlayers.begin(); itGuid != allPlayers.end(); itGuid++)
		{
			Position pos = GetPositionByGuid(*itGuid);
			float posDis = pCaptain->GetDistance(pos);
			if (minGUID == 0 || posDis < minDistance)
			{
				minDistance = posDis;
				minGUID = *itGuid;
			}
		}
		if (minGUID != 0)
		{
			allPlayers.erase(minGUID);
			if (AcceptCommandByPlayerGUID(minGUID, waypoint))
				--needCount;
		}
		else
			break;
	}
}

PlayerGUIDs CommandAV::GetAVFlagRangePlayerByTeam(BattlegroundAV* pBattlegroundAV, BG_AV_Nodes nodeType, TeamId team)
{
    PlayerGUIDs existPlayers;
    GameObject const* pBGNode = pBattlegroundAV->GetNodeObjectByPosType(nodeType);
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

PlayerGUIDs CommandAV::GetAVCaptainRangePlayerByTeam(BattlegroundAV* pBattlegroundAV, Creature const* pCaptain, TeamId team)
{
	PlayerGUIDs existPlayers;
	NearPlayerList playersNearby;
	pCaptain->GetPlayerListInGrid(playersNearby, COMMAND_POINT_IFDISTANCE / 2);
	for (Player* player : playersNearby)
	{
		if (player->GetTeamId() != team)
			continue;
		existPlayers.insert(player->GetGUID());
	}
	return existPlayers;
}

bool CommandAV::AcceptCommandByPlayerGUID(uint64 guid, AIWaypoint* targetAIWP, bool isFlag /* = false */)
{
	if (!targetAIWP)
		return false;
	BotBGAI* pBotAI = GetBotBGAI(guid);
	if (!pBotAI)
		return false;
	pBotAI->GetAIMovement()->AcceptCommand(targetAIWP, isFlag);
	return true;
}

bool CommandAV::AcceptCommandByPlayerGUID(uint64 guid, ObjectGuid flagGuid, bool isFlag /* = false */)
{
	if (flagGuid.IsEmpty())
		return false;
	BotBGAI* pBotAI = GetBotBGAI(guid);
	if (!pBotAI)
		return false;
	pBotAI->GetAIMovement()->AcceptCommand(flagGuid, isFlag);
	return true;
}

void CommandAV::TryOccupiedAVNode(BattlegroundAV* pBattlegroundAV, uint64 guid)
{
	Player* player = GetBGPlayer(guid);
	if (!player || player->HasUnitState(UNIT_STATE_CASTING) || player->IsInCombat())
		return;
	GameObject const* pFlag = pBattlegroundAV->GetEnemyNodeObjectByRange(player, 8.0f);
	if (!pFlag)
		return;
	SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(21651);
	if (!spellInfo)
		return;
	BotBGAI* pBotAI = GetBotBGAI(guid);
	if (!pBotAI)
		return;

	pBotAI->Dismount();
	SpellCastTargets targets;
	targets.SetTargetMask(TARGET_FLAG_GAMEOBJECT);
	targets.SetGOTarget((GameObject*)pFlag);
    Spell* spell = new Spell(player, spellInfo, TRIGGERED_NONE, ObjectGuid::Empty, false);
	spell->prepare(&targets);
	//player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT, pFlag->GetEntry());
}
