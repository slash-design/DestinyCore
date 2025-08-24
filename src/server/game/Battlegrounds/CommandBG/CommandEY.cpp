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

#include "CommandEY.h"
#include "BotAI.h"
#include <BattlegroundEY.h>


CommandEY::CommandEY(Battleground* pBG, TeamId team) :
CommandBG(pBG, team),
lastUpdateTick(COMMANDBG_UPDATE_TICK / 5)
{
	if (team == TeamId::TEAM_ALLIANCE)
	{
		m_AIWPEntrys.push_back(26);
		m_AIWPEntrys.push_back(28);
		m_AIWPEntrys.push_back(29);
		m_AIWPEntrys.push_back(30);
		m_AIWPEntrys.push_back(31);
		m_AIWPEntrys.push_back(27);
		m_AIWPEntrys.push_back(32);
	}
	else
	{
		m_AIWPEntrys.push_back(27);
		m_AIWPEntrys.push_back(30);
		m_AIWPEntrys.push_back(31);
		m_AIWPEntrys.push_back(28);
		m_AIWPEntrys.push_back(29);
		m_AIWPEntrys.push_back(26);
		m_AIWPEntrys.push_back(32);
	}
}

CommandEY::~CommandEY()
{
}

void CommandEY::Initialize()
{
	CommandBG::Initialize();
}

void CommandEY::StartGame()
{
    AIWaypoint* pWaypoint = sAIWPMgr->FindAIWaypoint(m_AIWPEntrys[AIWP_SELF_START]);
    if (!pWaypoint)
        return;
    for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
    {
        Position pos = Position(pWaypoint->posX, pWaypoint->posY, pWaypoint->posZ, 0);
        BotBGAI* pBotAI = GetBotBGAI(itGuid->first);
        if (pBotAI)
            pBotAI->SetTeleport(pos);
		//Player* player = GetBGPlayer(itGuid->first);
		//if (!player)
		//	continue;
		////player->StopMoving();
		//player->TeleportTo(player->GetMapId(), pWaypoint->posX, pWaypoint->posY, pWaypoint->posZ, player->GetOrientation());
		////player->UpdatePosition(pWaypoint->posX, pWaypoint->posY, pWaypoint->posZ, player->GetOrientation(), true);
		//WorldPacket opcode(2);
		//opcode.appendPackGUID(player->GetGUID().GetRawValue());
		//opcode << uint32(0);
		//opcode << uint32(100);
		//player->GetSession()->HandleMoveTeleportAck(opcode);
	}
	CommandBG::StartGame();
}

bool CommandEY::AddPlayerBot(Player* player, Battleground* pBG)
{
	if (!CommandBG::AddPlayerBot(player, pBG))
		return false;
	if (pBG->GetStatus() != BattlegroundStatus::STATUS_IN_PROGRESS)
		return true;
	if (!player->IsPlayerBot())
		return false;
	AIWaypoint* pWaypoint = sAIWPMgr->FindAIWaypoint(m_AIWPEntrys[AIWP_SELF_START]);
	if (!pWaypoint)
		return false;
    Position pos = Position(pWaypoint->posX, pWaypoint->posY, pWaypoint->posZ, 0);
	BotBGAI* pBotAI = GetBotBGAI(player->GetGUID());
	if (pBotAI)
		pBotAI->SetTeleport(pos);
	////player->StopMoving();
	//player->TeleportTo(player->GetMapId(), pWaypoint->posX, pWaypoint->posY, pWaypoint->posZ, player->GetOrientation());
	////player->UpdatePosition(pWaypoint->posX, pWaypoint->posY, pWaypoint->posZ, player->GetOrientation(), true);
	//WorldPacket opcode(2);
	//opcode.appendPackGUID(player->GetGUID().GetRawValue());
	//opcode << uint32(0);
	//opcode << uint32(100);
	//player->GetSession()->HandleMoveTeleportAck(opcode);

	return true;
}

const Creature* CommandEY::GetMatchGraveyardNPC(const Player* player)
{
	if (!player || !player->InBattleground() || !m_pBattleground)
		return NULL;
	BattlegroundEY* pBGEY = dynamic_cast<BattlegroundEY*>(m_pBattleground);
	if (!pBGEY)
		return NULL;
	return pBGEY->GetClosestGraveCreature(player);
}

void CommandEY::Update(uint32 diff)
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
				ProcessGroupFocus(m_BGKeyWaypoints[AIWP_SELF_START], m_BGKeyWaypoints[AIWP_ENEMY_START]);
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

void CommandEY::OnPlayerRevive(Player* player)
{
	if (!player)
		return;
	AIWaypoint* pWaypoint = sAIWPMgr->FindAIWaypoint(m_AIWPEntrys[AIWP_SELF_START]);
	if (!pWaypoint)
		return;
	Position pointPos = pWaypoint->GetPosition();
	if (player->GetPosition().IsInDist(&pointPos, 105))
	{
		player->TeleportTo(player->GetMapId(), pWaypoint->posX, pWaypoint->posY, pWaypoint->posZ, player->GetOrientation());
		player->UpdatePosition(pWaypoint->posX, pWaypoint->posY, pWaypoint->posZ, player->GetOrientation(), true);
	}
}

bool CommandEY::CanUpMount(Player* player)
{
	BattlegroundEY* pBattlegroundEY = dynamic_cast<BattlegroundEY*>(m_pBattleground);
	if (pBattlegroundEY)
	{
		if (pBattlegroundEY->GetFlagPickerGUID() == player->GetGUID())
			return false;
	}
	return true;
}

//AIWaypoint* CommandEY::GetReadyPosition()
//{
//	if (m_TeamID == TEAM_ALLIANCE)
//	{
//		return sAIWPMgr->FindAIWaypoint(m_AIWPEntrys[AIWP_SELF_START]);
//	}
//	else
//	{
//		return sAIWPMgr->FindAIWaypoint(m_AIWPEntrys[AIWP_SELF_START]);
//	}
//	return NULL;
//}

void CommandEY::RndStartCommand()
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
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_SELF_TOWER1];
			break;
		case 1:
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_SELF_TOWER2];
			break;
		case 2:
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_SELF_TOWER3];
			break;
		case 3:
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_SELF_TOWER4];
			break;
		case 4:
			comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_EY_FLAG];
			break;
		}
		pBotAI->GetAIMovement()->AcceptCommand(comInfo.targetAIWP);
	}
}

void CommandEY::ProcessRegulation()
{
	PlayerGUIDs allPlayers;
	for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
	{
		allPlayers.insert(itGuid->first);
	}
	ProcessEYPointRequirement(GetEYPointIndexByTeam(AIWP_SELF_TOWER1, m_TeamID), m_BGKeyWaypoints[AIWP_SELF_TOWER1], allPlayers);
	if (!allPlayers.empty())
		ProcessEYPointRequirement(GetEYPointIndexByTeam(AIWP_SELF_TOWER2, m_TeamID), m_BGKeyWaypoints[AIWP_SELF_TOWER2], allPlayers);
	if (!allPlayers.empty())
		ProcessEYPointRequirement(GetEYPointIndexByTeam(AIWP_SELF_TOWER3, m_TeamID), m_BGKeyWaypoints[AIWP_SELF_TOWER3], allPlayers);
	//if (!allPlayers.empty())
	//	ProcessEYPointRequirement(GetEYPointIndexByTeam(AIWP_SELF_TOWER4, m_TeamID), m_BGKeyWaypoints[AIWP_SELF_TOWER4], allPlayers);
	for (PlayerGUIDs::iterator itGuid = allPlayers.begin(); itGuid != allPlayers.end(); itGuid++)
		AcceptCommandByPlayerGUID(*itGuid, m_BGKeyWaypoints[AIWP_EY_FLAG]);
	for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
		TryPickStormFlag(itGuid->first);

	BattlegroundEY* pBattlegroundEY = dynamic_cast<BattlegroundEY*>(m_pBattleground);
	if (pBattlegroundEY && pBattlegroundEY->GetFlagState() == BG_EY_FlagState::BG_EY_FLAG_STATE_ON_PLAYER)
	{
		Player* flagPlayer = GetBGPlayer(pBattlegroundEY->GetFlagPickerGUID());
		if (flagPlayer)
			ProcessFlagPicker(flagPlayer);
	}
}

void CommandEY::ProcessEYPointRequirement(uint32 point, AIWaypoint* waypoint, PlayerGUIDs& players)
{
	if (!waypoint || point >= EYBattlegroundPoints::EY_PLAYERS_OUT_OF_POINTS)
		return;
	GameObject* pBGNode = m_pBattleground->GetBGObject(point + EYBattlegroundObjectTypes::BG_EY_OBJECT_TOWER_CAP_FEL_REAVER);
	if (!pBGNode)
		return;
	PlayerGUIDs nodeNearPlayers = GetEYPointRangePlayerByTeam(point, m_TeamID);
	uint32 enemyCount = GetEYPointRangePlayerByTeam(point, (m_TeamID == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE).size();
	bool flagIsOvvupied = EYPointIsOccupied(point, m_TeamID);
	int32 needCount = int32(enemyCount) + (flagIsOvvupied ? 1 : 3);
	for (PlayerGUIDs::iterator itGuid = nodeNearPlayers.begin(); itGuid != nodeNearPlayers.end(); itGuid++)
	{
		uint64 guid = *itGuid;
		if (!AcceptCommandByPlayerGUID(guid, waypoint))
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
			if (AcceptCommandByPlayerGUID(minGUID, waypoint))
				--needCount;
		}
		else
			break;
	}
}

bool CommandEY::EYPointIsOccupied(uint32 point, TeamId team)
{
	if (!m_pBattleground || point >= EYBattlegroundPoints::EY_PLAYERS_OUT_OF_POINTS)
		return false;
	BattlegroundEY* pBattlegroundEY = dynamic_cast<BattlegroundEY*>(m_pBattleground);
	if (!pBattlegroundEY)
		return false;
	return pBattlegroundEY->EYPointIsControl(((team == TEAM_ALLIANCE) ? Team::ALLIANCE : Team::HORDE), point);
}

PlayerGUIDs CommandEY::GetEYPointRangePlayerByTeam(uint32 point, TeamId team)
{
	PlayerGUIDs existPlayers;
	if (!m_pBattleground || point >= EYBattlegroundPoints::EY_PLAYERS_OUT_OF_POINTS)
		return existPlayers;
	GameObject* pBGNode = m_pBattleground->GetBGObject(point + EYBattlegroundObjectTypes::BG_EY_OBJECT_TOWER_CAP_FEL_REAVER);
	if (!pBGNode)
		return existPlayers;
	NearPlayerList playersNearby;
	pBGNode->GetPlayerListInGrid(playersNearby, BG_EY_ProgressBarConsts::BG_EY_POINT_RADIUS);
	for (Player* player : playersNearby)
	{
		if (player->GetTeamId() != team)
			continue;
		existPlayers.insert(player->GetGUID());
	}
	return existPlayers;
}

bool CommandEY::AcceptCommandByPlayerGUID(uint64 guid, AIWaypoint* targetAIWP, bool isFlag /* = false */)
{
	if (!targetAIWP)
		return false;
	BotBGAI* pBotAI = GetBotBGAI(guid);
	if (!pBotAI)
		return false;
	pBotAI->GetAIMovement()->AcceptCommand(targetAIWP, isFlag);
	return true;
}

bool CommandEY::AcceptCommandByPlayerGUID(uint64 guid, ObjectGuid flagGuid, bool isFlag /* = false */)
{
	if (flagGuid.IsEmpty())
		return false;
	BotBGAI* pBotAI = GetBotBGAI(guid);
	if (!pBotAI)
		return false;
	pBotAI->GetAIMovement()->AcceptCommand(flagGuid, isFlag);
	return true;
}

uint32 CommandEY::GetEYPointIndexByTeam(uint32 index, TeamId team)
{
	switch (index)
	{
	case AIWP_SELF_TOWER1:
		return (team == TEAM_ALLIANCE) ? EYBattlegroundPoints::MAGE_TOWER : EYBattlegroundPoints::BLOOD_ELF;
	case AIWP_SELF_TOWER2:
		return (team == TEAM_ALLIANCE) ? EYBattlegroundPoints::DRAENEI_RUINS : EYBattlegroundPoints::FEL_REAVER;
	case AIWP_SELF_TOWER3:
		return (team == TEAM_ALLIANCE) ? EYBattlegroundPoints::BLOOD_ELF : EYBattlegroundPoints::MAGE_TOWER;
	case AIWP_SELF_TOWER4:
		return (team == TEAM_ALLIANCE) ? EYBattlegroundPoints::FEL_REAVER : EYBattlegroundPoints::DRAENEI_RUINS;
	}
	return (team == TEAM_ALLIANCE) ? EYBattlegroundPoints::MAGE_TOWER : EYBattlegroundPoints::BLOOD_ELF;
}

void CommandEY::TryPickStormFlag(uint64 guid)
{
	if (!m_pBattleground)
		return;
	BattlegroundEY* pBattlegroundEY = dynamic_cast<BattlegroundEY*>(m_pBattleground);
	if (!pBattlegroundEY)
		return;
	uint8 flagState = pBattlegroundEY->GetFlagState();
	if (flagState == BG_EY_FlagState::BG_EY_FLAG_STATE_WAIT_RESPAWN)
		return;
	if (flagState == BG_EY_FlagState::BG_EY_FLAG_STATE_ON_PLAYER)
		return;
	Player* player = GetBGPlayer(guid);
	if (!player || !player->IsAlive())
		return;
	if (player->HasUnitState(UNIT_STATE_CASTING))
		return;
	if (flagState == BG_EY_FlagState::BG_EY_FLAG_STATE_ON_BASE)
	{
		GameObject* pObject = pBattlegroundEY->GetBGObject(EYBattlegroundObjectTypes::BG_EY_OBJECT_FLAG_NETHERSTORM);
		if (!pObject || !pObject->isSpawned())
			return;
		float distance = player->GetDistance(pObject->GetPosition());
		if (distance > 5)
		{
			if (distance < 60)
			{
				AcceptCommandByPlayerGUID(guid, m_BGKeyWaypoints[AIWP_EY_FLAG]);
				return;
			}
			else
				return;
		}
		if (BotBGAI* pAI = GetBotBGAI(guid))
		{
			if (pAI->CanUseBGObject())
			{
				SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(21651);
				if (!spellInfo)
					return;
				pAI->Dismount();
				SpellCastTargets targets;
				targets.SetTargetMask(TARGET_FLAG_GAMEOBJECT);
				targets.SetGOTarget(pObject);
                Spell* spell = new Spell(player, spellInfo, TRIGGERED_NONE, ObjectGuid::Empty, false);
				spell->prepare(&targets);
				//player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT, pObject->GetEntry());
				//pObject->Use(player);
			}
		}
	}
	else
	{
		GameObject* pObject = pBattlegroundEY->GetBgMap()->GetGameObject(pBattlegroundEY->GetDroppedFlagGUID());
		if (!pObject)
			return;
		float pickedDistance = player->GetDistance(pObject->GetPosition());
		if (pickedDistance <= 5.0f)
		{
			if (BotBGAI* pAI = GetBotBGAI(guid))
			{
				if (pAI->CanUseBGObject())
				{
					pAI->Dismount();
					pObject->Use(player);
				}
			}
		}
		else if (pickedDistance < 60)
		{
			AcceptCommandByPlayerGUID(guid, pObject->GetGUID());
		}
	}
}

void CommandEY::TryCaptureFlag(Player* player, AIWaypoint* pAIWP, uint32 point)
{
	if (!player || !pAIWP || !m_pBattleground)
		return;
	if (player->GetDistance(pAIWP->GetPosition()) > 5)
		return;
	BattlegroundEY* pBattlegroundEY = dynamic_cast<BattlegroundEY*>(m_pBattleground);
	if (!pBattlegroundEY || pBattlegroundEY->GetFlagPickerGUID() != player->GetGUID())
		return;
	pBattlegroundEY->HandleAreaTrigger(player, GetCaptureFlagPoint(point, m_TeamID), true);
}

uint32 CommandEY::GetCaptureFlagPoint(uint32 index, TeamId team)
{
	switch (index)
	{
	case AIWP_SELF_TOWER1:
		return (team == TEAM_ALLIANCE) ? TR_MAGE_TOWER_POINT : TR_BLOOD_ELF_POINT;
	case AIWP_SELF_TOWER2:
		return (team == TEAM_ALLIANCE) ? TR_DRAENEI_RUINS_POINT : TR_FEL_REAVER_POINT;
	case AIWP_SELF_TOWER3:
		return (team == TEAM_ALLIANCE) ? TR_BLOOD_ELF_POINT : TR_MAGE_TOWER_POINT;
	case AIWP_SELF_TOWER4:
		return (team == TEAM_ALLIANCE) ? TR_FEL_REAVER_POINT : TR_DRAENEI_RUINS_POINT;
	}
	return (team == TEAM_ALLIANCE) ? TR_MAGE_TOWER_POINT : TR_BLOOD_ELF_POINT;
}

void CommandEY::ProcessFlagPicker(Player* player)
{
	if (!player || !player->IsAlive())
		return;
	NearPlayerList playersNearby;
	player->GetPlayerListInGrid(playersNearby, 40);
	for (Player* pVisionPlayer : playersNearby)
	{
		if (pVisionPlayer->IsAlive() && pVisionPlayer != player)
		{
			if (UnitAI* pAI = pVisionPlayer->GetAI())
			{
				if (BotBGAI* pBotAI = dynamic_cast<BotBGAI*>(pAI))
				{
					pBotAI->GetAIMovement()->AcceptCommand(player->GetGUID());
					if (pVisionPlayer->GetTeamId() != player->GetTeamId())
						pVisionPlayer->SetSelection(player->GetGUID());
				}
			}
		}
	}

	G3D::Vector3 playerPos = player->GetPosition().GetVector3();
	uint32 nearPointIndex = 0;
	uint32 nearDistance = 999999;
	for (uint32 point = AIWP_SELF_TOWER1; point <= AIWP_SELF_TOWER4; point++)
	{
		uint32 eyPoint = GetEYPointIndexByTeam(point, m_TeamID);
		if (!EYPointIsOccupied(eyPoint, m_TeamID))
			continue;
		AIWaypoint* pAIWP = m_BGKeyWaypoints[point];
		if (pAIWP)
		{
			float distance = (playerPos - pAIWP->GetPosition().GetVector3()).length();
			if (distance < nearDistance)
			{
				nearDistance = distance;
				nearPointIndex = point;
			}
		}
	}
	if (UnitAI* pAI = player->GetAI())
	{
		if (BotBGAI* pBotAI = dynamic_cast<BotBGAI*>(pAI))
		{
			pBotAI->GetAIMovement()->AcceptCommand(m_BGKeyWaypoints[nearPointIndex], true);
			TryCaptureFlag(player, m_BGKeyWaypoints[nearPointIndex], nearPointIndex);
		}
	}
}
