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

#include "CommandBG.h"
#include "PlayerBotSession.h"
#include "BotAI.h"

CommandModel CommandBG::g_CommandModelType = CM_Regulation;

CommandBG::CommandBG(Battleground* pBG, TeamId team) :
m_BGGC(BGGS_None),
m_ComModelStatus(0),
m_pBattleground(pBG),
m_TeamID(team),
m_BGProcessTimerTick(0)
{
}

CommandBG::~CommandBG()
{
}

void CommandBG::Initialize()
{
	m_BGGC = BGGameCommand::BGGS_Init;
	//m_BGKeyWaypoints.clear();
	//for (AIWPEntrys::iterator itEntry = m_AIWPEntrys.begin(); itEntry != m_AIWPEntrys.end(); itEntry++)
	//{
	//	AIWaypoint* pAIWP = sAIWPMgr->FindAIWaypoint(*itEntry);
	//	if (pAIWP)
	//		m_BGKeyWaypoints.push_back(pAIWP);
	//	else
	//	{
	//		m_BGKeyWaypoints.push_back(NULL);
	//		TC_LOG_ERROR("CommandBG", "Initialize battleground aiwp %d error.", *itEntry);
	//	}
	//}
}

void CommandBG::ReadyGame()
{
	m_BGGC = BGGameCommand::BGGS_Ready;
	//for (uint64 guid : m_PlayerGUIDs)
	//{
	//	if (UnitAI* pAI = GetPlayerAI(guid))
	//	{
	//		if (BotBGAI* pBotAI = dynamic_cast<BotBGAI*>(pAI))
	//			pBotAI->ReadyBattleground();
	//	}
	//}
}

void CommandBG::StartGame()
{
	m_BGGC = BGGameCommand::BGGS_Start;
	//for (uint64 guid : m_PlayerGUIDs)
	//{
	//	if (UnitAI* pAI = GetPlayerAI(guid))
	//	{
	//		if (BotBGAI* pBotAI = dynamic_cast<BotBGAI*>(pAI))
	//			pBotAI->StartBattleground();
	//	}
	//}
}

void CommandBG::OnPlayerDead(uint64 guid)
{
	PlayerStatus::iterator itGUID = m_PlayerGUIDs.find(guid);
	if (itGUID == m_PlayerGUIDs.end())
		return;
	itGUID->second = false;
}

bool CommandBG::AddPlayerBot(Player* player, Battleground* pBG)
{
	if (!player || !pBG)
		return false;
	if (m_PlayerGUIDs.find(player->GetGUID()) != m_PlayerGUIDs.end())
		return false;

	BotBGAI* pBotAI = NULL;
	if (UnitAI* pAI = player->GetAI())
	{
		pBotAI = dynamic_cast<BotBGAI*>(pAI);
		if (pBotAI)
			pBotAI->ResetBotAI();
	}
	if (pBG->GetStatus() == BattlegroundStatus::STATUS_WAIT_LEAVE)
	{
		PlayerBotSession* pSession = dynamic_cast<PlayerBotSession*>((WorldSession*)player->GetSession());
		if (pSession)
		{
			BotGlobleSchedule schedule(BotGlobleScheduleType::BGSType_LeaveBG, 0);
			pSession->PushScheduleToQueue(schedule);
		}
		return false;
	}
	else if (pBG->GetStatus() == BattlegroundStatus::STATUS_WAIT_JOIN)
	{
		if (pBotAI)
			pBotAI->ReadyBattleground();
	}
	else if (pBG->GetStatus() == BattlegroundStatus::STATUS_IN_PROGRESS)
	{
		if (pBotAI)
			pBotAI->StartBattleground();
	}

	m_PlayerGUIDs[player->GetGUID()] = false;
	return true;
}

void CommandBG::RemovePlayerBot(Player* player)
{
	if (!player)
		return;

	PlayerStatus::iterator itGUID = m_PlayerGUIDs.find(player->GetGUID());
	if (itGUID != m_PlayerGUIDs.end())
	{
		if (BotBGAI* pBotAI = GetBotBGAI(itGUID->first))
		{
			pBotAI->LeaveBattleground();
		}
		m_PlayerGUIDs.erase(itGUID);
	}
}

void CommandBG::Update(uint32 diff)
{
	if (m_BGGC != BGGS_None)
	{
		if (m_BGGC == BGGS_Init)
		{
			m_BGProcessTimerTick = 0;
			m_BGKeyWaypoints.clear();
			for (AIWPEntrys::iterator itEntry = m_AIWPEntrys.begin(); itEntry != m_AIWPEntrys.end(); itEntry++)
			{
				AIWaypoint* pAIWP = sAIWPMgr->FindAIWaypoint(*itEntry);
				if (pAIWP)
					m_BGKeyWaypoints.push_back(pAIWP);
				else
				{
					m_BGKeyWaypoints.push_back(NULL);
					//TC_LOG_ERROR("CommandBG", "Initialize battleground aiwp %d error.", *itEntry);
				}
			}
		}
		else if (m_BGGC == BGGS_Ready)
		{
			m_BGProcessTimerTick = 1;
			for (PlayerStatus::iterator itGUID = m_PlayerGUIDs.begin();
				itGUID != m_PlayerGUIDs.end();
				itGUID++)
			{
				if (BotBGAI* pBotAI = GetBotBGAI(itGUID->first))
					pBotAI->ReadyBattleground();
			}
		}
		else if (m_BGGC == BGGS_Start)
		{
			for (PlayerStatus::iterator itGUID = m_PlayerGUIDs.begin();
				itGUID != m_PlayerGUIDs.end();
				itGUID++)
			{
				if (BotBGAI* pBotAI = GetBotBGAI(itGUID->first))
					pBotAI->StartBattleground();
			}
		}
		m_BGGC = BGGS_None;
	}
}

UnitAI* CommandBG::GetPlayerAI(uint64 guid)
{
	PlayerStatus::iterator itGUID = m_PlayerGUIDs.find(guid);
	if (itGUID == m_PlayerGUIDs.end())
		return NULL;
	Player* player = ObjectAccessor::FindPlayer(ObjectGuid::Create<HighGuid::Player>(guid));
	if (!player)
		return NULL;
	return player->GetAI();
}

BotBGAI* CommandBG::GetBotBGAI(uint64 guid)
{
	UnitAI* pAI = GetPlayerAI(guid);
	if (!pAI)
		return NULL;
	return (dynamic_cast<BotBGAI*>(pAI));
}

Player* CommandBG::GetBGPlayer(uint64 guid)
{
	PlayerStatus::iterator itGUID = m_PlayerGUIDs.find(guid);
	if (itGUID == m_PlayerGUIDs.end())
		return NULL;
	return ObjectAccessor::FindPlayer(ObjectGuid::Create<HighGuid::Player>(guid));
}

Position CommandBG::GetNearTeleportPoint(Position& currentPos)
{
	AIWaypoint* pNearPoint = m_BGKeyWaypoints[0];
	float nearDistance = (currentPos.GetVector3() - pNearPoint->GetPosition().GetVector3()).length();
	for (AIWaypoint* pPoint : m_BGKeyWaypoints)
	{
		float distance = (currentPos.GetVector3() - pPoint->GetPosition().GetVector3()).length();
		if (distance < nearDistance)
		{
			pNearPoint = pPoint;
			nearDistance = distance;
		}
	}
	return pNearPoint->GetPosition();
}

bool CommandBG::GroupAllPlayerReadyToWayPoint(AIWaypoint* pKeyPoint)
{
	if (!pKeyPoint)
		return false;
	bool allIsReady = true;
	for (PlayerStatus::iterator itGUID = m_PlayerGUIDs.begin();
		itGUID != m_PlayerGUIDs.end();
		itGUID++)
	{
		if (itGUID->second)
			continue;
		Player* player = GetBGPlayer(itGUID->first);
		if (!player)
			continue;
		float distance = player->GetDistance(pKeyPoint->posX, pKeyPoint->posY, pKeyPoint->posZ);
		if (distance > 10 || player->IsInCombat())
		{
			allIsReady = false;
			break;
		}
	}
	return allIsReady;
}

void CommandBG::ProcessGroupFocus(AIWaypoint* selfFocus, AIWaypoint* enemyFocus)
{
	if (!selfFocus || !enemyFocus)
		return;
	for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
	{
		BotBGAI* pBotAI = GetBotBGAI(itGuid->first);
		if (!pBotAI)
			continue;
		if (itGuid->second)
		{
			//Player* player = GetBGPlayer(itGuid->first);
			//if (player && !player->IsInCombat())
			//{
			//	if (player->GetDistance(enemyFocus->GetPosition()) < 5.0f)
			//	{
			//		itGuid->second = false;
			//		pBotAI->AcceptCommand(selfFocus);
			//	}
			//}
			//else
			{
				pBotAI->GetAIMovement()->AcceptCommand(enemyFocus);
			}
		}
		else
			pBotAI->GetAIMovement()->AcceptCommand(selfFocus);
	}
	bool allIsReady = GroupAllPlayerReadyToWayPoint(selfFocus);
	if (allIsReady)
	{
		for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
		{
			itGuid->second = true;
		}
	}
}

Position CommandBG::GetPositionByGuid(uint64 guid)
{
	ObjectGuid oGuid = ObjectGuid::Create<HighGuid::Player>(guid);
	if (oGuid.IsPlayer())
	{
		Player* player = ObjectAccessor::FindPlayer(oGuid);
		if (player)
		{
			return player->GetPosition();
		}
	}
	else if (m_pBattleground && m_pBattleground->GetBgMap())
	{
		GameObject* pObject = m_pBattleground->GetBgMap()->GetGameObject(oGuid);
		if (pObject)
			return pObject->GetPosition();
	}
	Position pos;
	pos.m_positionX = pos.m_positionY = pos.m_positionZ = 0;
	return pos;
}
