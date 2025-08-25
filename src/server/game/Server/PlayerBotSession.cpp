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

#include "PlayerBotSession.h"
#include "Player.h"
#include "BattlegroundMgr.h"
#include "BotAI.h"
#include "BotGroupAI.h"
#include "BotFieldAI.h"
#include "LFGMgr.h"
#include "CharacterPackets.h"
#include "DB2Structure.h"
#include "LFGPacketsCommon.h"

PlayerBotSession::PlayerBotSession(uint32 id, std::string& name, uint32 battlenetAccountId, AccountTypes sec, uint8 expansion, time_t mute_time, LocaleConstant locale, uint32 recruiter, bool isARecruiter, std::string&& battlenetAccountName) :
m_LastCastTime(CAST_SCHEDULE_TICK),
m_NoWorldTick(0),
m_AccountBot(false),

WorldSession(id, std::string(name), 0, nullptr, SEC_PLAYER, 6, 0, "Wn64", LOCALE_deDE, 0, false, std::string(battlenetAccountName))
{
    SetAddress("DestinyCore");
}

bool PlayerBotSession::IsBotSession()
{
	return true;
}

bool PlayerBotSession::HasBGSchedule()
{
	for (BotSchedules::iterator itSc = m_Schedules.begin(); itSc != m_Schedules.end(); itSc++)
	{
		BotGlobleScheduleType bgsType = (*itSc).bbgType;
		if (bgsType == BGSType_EnterBG || bgsType == BGSType_InBGQueue ||
			bgsType == BGSType_LeaveBG || bgsType == BGSType_OutBGQueue ||
			bgsType == BGSType_EnterAA || bgsType == BGSType_InAAQueue ||
			bgsType == BGSType_LeaveAA || bgsType == BGSType_OutAAQueue)
			return true;
	}
	return false;
}

bool PlayerBotSession::Update(uint32 diff, PacketFilter& updater)
{
	bool updateResult = WorldSession::Update(diff, updater);
	ProcessNoWorld(diff);
	m_LastCastTime -= diff;
	if (m_LastCastTime <= 0)
	{
		//std::lock_guard<std::mutex> lock(m_optQueueLock);
		CastSchedule(CAST_SCHEDULE_TICK);
		m_LastCastTime = CAST_SCHEDULE_TICK;
	}
	return updateResult;
}

void PlayerBotSession::PushScheduleToQueue(BotGlobleSchedule& schedule)
{
	//std::lock_guard<std::mutex> lock(m_optQueueLock);
	for (BotSchedules::iterator itSc = m_Schedules.begin(); itSc != m_Schedules.end(); itSc++)
	{
		if ((*itSc).bbgType == schedule.bbgType)
			return;
	}
	if (schedule.bbgType == BGSType_DelayLevelup)
	{
		if (PlayerLoading())
			return;
		if (Player* player = GetPlayer())
		{
			if (!player->IsSettingFinish())
				return;
		}
		else
			return;
	}

	if (schedule.bbgType == BotGlobleScheduleType::BGSType_OutBGQueue)
	{
		RemoveScheduleByType(BotGlobleScheduleType::BGSType_InBGQueue);
		RemoveScheduleByType(BotGlobleScheduleType::BGSType_EnterBG);
	}
	if (schedule.bbgType == BotGlobleScheduleType::BGSType_OutAAQueue)
	{
		RemoveScheduleByType(BotGlobleScheduleType::BGSType_InAAQueue);
		RemoveScheduleByType(BotGlobleScheduleType::BGSType_EnterAA);
	}
	m_Schedules.push_back(schedule);
}

void PlayerBotSession::RemoveScheduleByType(BotGlobleScheduleType eType)
{
	for (BotSchedules::iterator itSc = m_Schedules.begin(); itSc != m_Schedules.end(); itSc++)
	{
		if ((*itSc).bbgType == eType)
		{
			m_Schedules.erase(itSc);
			return;
		}
	}
}

bool PlayerBotSession::HasScheduleByType(BotGlobleScheduleType eType)
{
	for (BotSchedules::iterator itSc = m_Schedules.begin(); itSc != m_Schedules.end(); itSc++)
	{
		if ((*itSc).bbgType == eType)
		{
			return true;
		}
	}
	return false;
}

bool PlayerBotSession::HasSchedules()
{
	return m_Schedules.size() > 0;
}

bool PlayerBotSession::IsAccountBotSession()
{
	return m_AccountBot;
}

bool PlayerBotSession::PlayerIsReady()
{
	Player* player = GetPlayer();
	if (!player)
		return true;
	if (!player->IsSettingFinish())
		return false;
	BotFieldAI* pFieldAI = dynamic_cast<BotFieldAI*>(player->GetAI());
	if (!pFieldAI)
		return true;
	if (pFieldAI->HasTeleport())
		return false;

	return true;
}

void PlayerBotSession::ProcessNoWorld(uint32 diff)
{
	if (PlayerLoading())
		return;
	Player* player = GetPlayer();
	if (!player)
		return;

	if (player->IsInWorld())
	{
		m_NoWorldTick = 0;
		return;
	}

	if (player->IsBeingTeleported())
	{
        //HandleMoveWorldportAckOpcode;
        m_NoWorldTick = 1000;
		return;
	}

	if (m_NoWorldTick == 0)
	{
		m_NoWorldTick = 2000;
		return;
	}

	m_NoWorldTick -= int32(diff);
	if (m_NoWorldTick > 0)
		return;
	if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI()))
	{
		if (pGroupAI->HasTeleport())
			pGroupAI->UpdateTeleport(diff);
		else
			pGroupAI->SetTeleportToMaster();
		m_NoWorldTick = 500;
	}
	else if (BotBGAI* pGroupAI = dynamic_cast<BotBGAI*>(player->GetAI()))
	{
		if (player->InBattleground())
		{
			PlayerBotMgr::SwitchPlayerBotAI(player, PlayerBotAIType::PBAIT_FIELD, true);
			WorldPacket opcode(CMSG_BATTLEFIELD_LEAVE);
			WorldPackets::Battleground::BattlefieldLeave battlefieldLeave(std::move(opcode));
            HandleBattlefieldLeaveOpcode(battlefieldLeave);
		}
		HandleMoveWorldportAck();
		m_NoWorldTick = 500;
	}
	else// if (BotFieldAI* pGroupAI = dynamic_cast<BotFieldAI*>(player->GetAI()))
	{
        HandleMoveWorldportAck();
		m_NoWorldTick = 500;
	}
}

void PlayerBotSession::CastSchedule(uint32 diff)
{
	if (m_Schedules.empty())
		return;
	bool result = false;
	BotGlobleSchedule& schedule = *m_Schedules.begin();
	schedule.processTick += diff;
	if (schedule.processTick > 60000 * 3)
	{
		m_Schedules.erase(m_Schedules.begin());
		return;
	}
	if (!PlayerIsReady())
		return;
	switch (schedule.bbgType)
	{
	case BGSType_Online:
		result = ProcessOnline(schedule);
		break;
	case BGSType_Online_GUID:
		result = ProcessOnlineByGUID(schedule);
		break;
	case BGSType_Offline:
		result = ProcessOffline(schedule);
		break;
	case BGSType_Settting:
		result = ProcessSetting(schedule);
		break;
	case BGSType_InBGQueue:
		result = ProcessInBGQueue(schedule);
		break;
	case BGSType_OutBGQueue:
		result = ProcessOutBGQueue(schedule);
		break;
	case BGSType_EnterBG:
		result = ProcessEnterBG(schedule);
		break;
	case BGSType_LeaveBG:
		result = ProcessLeaveBG(schedule);
		break;
	case BGSType_InAAQueue:
		result = ProcessInAAQueue(schedule);
		break;
	case BGSType_OutAAQueue:
		result = ProcessOutAAQueue(schedule);
		break;
	case BGSType_EnterAA:
		result = ProcessEnterAA(schedule);
		break;
	case BGSType_LeaveAA:
		result = ProcessLeaveAA(schedule);
		break;
	case BGSType_DelayLevelup:
		result = ProcessDelayLevelup(schedule);
		break;
	case BGSType_InLFGQueue:
		result = ProcessInLFGQueue(schedule);
		break;
	case BGSType_OutLFGQueue:
		result = ProcessOutLFGQueue(schedule);
		break;
	case BGSType_AcceptLFGProposal:
		result = ProcessAcceptLFGProposal(schedule);
		break;
	case BGSType_OfferPetitionSign:
		result = ProcessOfferPetitionSign(schedule);
		break;
	default:
		result = true;
		break;
	}
	if (result)
	{
		m_Schedules.erase(m_Schedules.begin());
	}
}

bool PlayerBotSession::ProcessOnline(BotGlobleSchedule& schedule)
{
	if (sPlayerBotMgr->m_MaxOnlineBot <= sPlayerBotMgr->m_BotOnlineCount)
	{
		LogoutPlayer(false);
		return true;
	}

	if (schedule.parameter1 <= 0)
		return true;
	if (PlayerLoading())
		return false;
	if (GetPlayer())
		return true;
	PlayerBotBaseInfo* pInfo = sPlayerBotMgr->GetPlayerBotAccountInfo(GetAccountId());
	if (!pInfo)
	{
		pInfo = sPlayerBotMgr->GetAccountBotAccountInfo(GetAccountId());
		if (!pInfo)
		{
			ClearAllSchedule();
			return false;
		}
	}

	bool fuction = true;
	if (schedule.parameter1 > 1)
		fuction = false;
	PlayerBotCharBaseInfo& charInfo = (schedule.parameter2 == 0) ? pInfo->GetRandomCharacterByFuction(fuction) : pInfo->GetCharacter(fuction, schedule.parameter2);
	if (charInfo.guid == 0)
		return true;

    WorldPacket _worldPacket(CMSG_PLAYER_LOGIN);
    WorldPackets::Character::PlayerLogin cmd(std::move(_worldPacket));
    cmd.Guid = ObjectGuid::Create<HighGuid::Player>(charInfo.guid);
    cmd.FarClip = 0.0f;
    HandlePlayerLoginOpcode(cmd);
	HandleContinuePlayerLogin();
	return false;
}

bool PlayerBotSession::ProcessOnlineByGUID(BotGlobleSchedule& schedule)
{
    if (sPlayerBotMgr->m_MaxOnlineBot <= sPlayerBotMgr->m_BotOnlineCount)
    {
        LogoutPlayer(false);
        return true;
    }

	if (schedule.playerGUID.IsEmpty())
		return true;
	if (PlayerLoading())
		return false;
	if (GetPlayer())
		return true;
    WorldPacket _worldPacket(CMSG_PLAYER_LOGIN);
    WorldPackets::Character::PlayerLogin cmd(std::move(_worldPacket));
    cmd.Guid = schedule.playerGUID;
    cmd.FarClip = 0.0f;
    HandlePlayerLoginOpcode(cmd);
	HandleContinuePlayerLogin();
	return false;
}

bool PlayerBotSession::ProcessOffline(BotGlobleSchedule& schedule)
{
	if (PlayerLoading())
		return false;
	Player* player = GetPlayer();
	if (!player)
		return true;
	if (schedule.scheduleState > 0)
		return false;
    WorldPackets::Character::LogoutRequest logoutReq(time(NULL) - 18);
	//LogoutPlayer(false);
	schedule.scheduleState = 1;
	return false;
}

bool PlayerBotSession::ProcessSetting(BotGlobleSchedule& schedule)
{
	if (schedule.parameter3 == 0 || schedule.parameter3 > 4)
		schedule.parameter3 = 4;
    if (schedule.parameter2 > 110 || schedule.parameter2 == 0)
        schedule.parameter2 = 110;
	if (schedule.parameter1 > schedule.parameter2)
		schedule.parameter2 = schedule.parameter1;
	if (PlayerLoading())
		return false;
	Player* player = GetPlayer();
	if (!player)
	{
		ClearAllSchedule();
		return false;
	}
	if (!player->IsInWorld() || !player->IsSettingFinish())
		return false;
	if (schedule.scheduleState != 0)
		return true;
	bool needTenacity = (schedule.parameter4 != 0) ? true : false;
	if (needTenacity)
		needTenacity = player->CheckNeedTenacityFlush();
	if (!needTenacity && player->CalculateTalentsTiers() < 10)
	{
		if (player->IsSettingFinish() && player->getLevel() >= schedule.parameter1 && player->getLevel() <= schedule.parameter2)
		{
			if (schedule.parameter3 >= 4 || (player->FindTalentType() + 1 == schedule.parameter3))
				return true;
		}
	}

	uint32 flushTalent = (schedule.parameter3 > 0 && schedule.parameter3 < 4) ? schedule.parameter3 - 1 : 3;
	player->ResetPlayerToLevel(schedule.parameter2, flushTalent, needTenacity);
	schedule.scheduleState = 1;
	return false;
}

bool PlayerBotSession::ProcessInBGQueue(BotGlobleSchedule& schedule)
{
	if (PlayerLoading())
		return false;
	Player* player = GetPlayer();
	if (!player)
	{
		ClearAllSchedule();
		return false;
	}
	if (!player->IsInWorld())
		return false;
	if (player->InBattlegroundQueue())
		return true;
	if (player->InBattleground() || player->InArena() || player->GetMap()->IsDungeon())
	{
		ClearAllSchedule();
		return false;
	}

	Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(BattlegroundTypeId(schedule.parameter1));
	if (!bg)
	{
		ClearAllSchedule();
		return false;
	}
	PVPDifficultyEntry const* bracketEntry = DB2Manager::GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
	if (!bracketEntry)
	{
		ClearAllSchedule();
		return false;
	}

	WorldPacket cmd(CMSG_BATTLEMASTER_JOIN);
	WorldPackets::Battleground::BattlemasterJoin packet(std::move(cmd));
	//packet.QueueID = MS::Battlegrounds::QueueOffsets::Battleground + MS::Battlegrounds::BattlegroundTypeId::BattlegroundRandom;
	//packet.RolesMask = 14;
    HandleBattlemasterJoinOpcode(packet);
	return false;
}

bool PlayerBotSession::ProcessOutBGQueue(BotGlobleSchedule& schedule)
{
	if (PlayerLoading())
		return false;
	Player* player = GetPlayer();
	if (!player)
	{
		ClearAllSchedule();
		return false;
	}
	if (!player->IsInWorld() || player->InBattleground() || player->InArena() || !player->InBattlegroundQueue() || player->GetMap()->IsDungeon())
		return true;

	LogoutPlayer(false);
	return false;
}

bool PlayerBotSession::ProcessEnterBG(BotGlobleSchedule& schedule)
{
	if (PlayerLoading())
		return false;
	Player* player = GetPlayer();
	if (!player)
	{
		ClearAllSchedule();
		return false;
	}
	if (!player->IsInWorld())
		return false;

	if (player->IsInCombat())
		player->CombatStop(true);

	if (player->GetMap()->IsDungeon())
	{
		BotGlobleSchedule schedule1(BotGlobleScheduleType::BGSType_OutBGQueue, 0);
		schedule1.parameter1 = schedule.parameter1;
		ClearAllSchedule();
		PushScheduleToQueue(schedule1);
		return false;
	}
	if (player->InBattleground() || player->InArena())
		return true;

	if (player->InBattlegroundQueue())
	{
		for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
		{
            BattlegroundQueueTypeId bgQueueTypeId = player->GetBattlegroundQueueTypeId(i);
			if (!bgQueueTypeId)
				continue;
			if (player->IsInvitedForBattlegroundQueueType(bgQueueTypeId))
			{
				PlayerBotMgr::SwitchPlayerBotAI(player, PlayerBotAIType::PBAIT_BG, true);

                WorldPacket cmd(CMSG_BATTLEFIELD_PORT);
                WorldPackets::Battleground::BattlefieldPort packet(std::move(cmd));
				packet.Ticket.Id = schedule.parameter1;
				packet.Ticket.RequesterGuid = schedule.playerGUID;
				packet.Ticket.Type = WorldPackets::LFG::RideType::Battlegrounds;
				packet.Ticket.Time = time(0);
				packet.AcceptedInvite = true;
                HandleBattleFieldPortOpcode(packet);
				//HandleWorldPortAck();
				break;
			}
		}
	}
	return false;
}

bool PlayerBotSession::ProcessLeaveBG(BotGlobleSchedule& schedule)
{
	if (PlayerLoading())
		return false;
	Player* player = GetPlayer();
	if (!player)
	{
		ClearAllSchedule();
		return false;
	}
	if (!player->InBattleground())
		return true;

	PlayerBotMgr::SwitchPlayerBotAI(player, PlayerBotAIType::PBAIT_FIELD, true);

	WorldPackets::Battleground::BattlefieldLeave leave(std::move(WorldPacket()));
    HandleBattlefieldLeaveOpcode(leave);
	//HandleWorldPortAck();
	return false;
}

bool PlayerBotSession::ProcessInAAQueue(BotGlobleSchedule& schedule)
{
	if (PlayerLoading())
		return false;
	Player* player = GetPlayer();
	if (!player)
	{
		ClearAllSchedule();
		return false;
	}
	if (!player->IsInWorld())
		return false;
	if (player->InBattlegroundQueue())
		return true;
	if (player->HasAura(26013) || player->InBattleground() || player->InArena() || player->GetMap()->IsDungeon())
	{
		ClearAllSchedule();
		return false;
	}

	WorldPacket packet(CMSG_BATTLEMASTER_JOIN_SKIRMISH);
	WorldPackets::Battleground::BattlemasterJoinArena cmd(std::move(packet));
	//cmd.RolesMask = schedule.parameter1;
	//cmd.Bracket = schedule.parameter2;
    HandleBattlemasterJoinArena(cmd);
	return false;
}

bool PlayerBotSession::ProcessOutAAQueue(BotGlobleSchedule& schedule)
{
	if (PlayerLoading())
		return false;
	Player* player = GetPlayer();
	if (!player)
	{
		ClearAllSchedule();
		return false;
	}
	if (!player->IsInWorld() || player->InBattleground() || player->InArena() || !player->InBattlegroundQueue() || player->GetMap()->IsDungeon())
		return true;

	LogoutPlayer(false);
	return true;
}

bool PlayerBotSession::ProcessEnterAA(BotGlobleSchedule& schedule)
{
	if (PlayerLoading())
		return false;
	Player* player = GetPlayer();
	if (!player)
	{
		ClearAllSchedule();
		return false;
	}
	if (!player->IsInWorld())
		return false;
	if (player->GetMap()->IsDungeon())
	{
		BotGlobleSchedule schedule1(BotGlobleScheduleType::BGSType_OutAAQueue, 0);
		schedule1.parameter1 = schedule.parameter1;
		schedule1.parameter2 = schedule.parameter2;
		schedule1.parameter3 = schedule.parameter3;
		ClearAllSchedule();
		PushScheduleToQueue(schedule1);
		return false;
	}
	if (player->InBattleground() || player->InArena())
		return true;

	if (player->InBattlegroundQueue())
	{
		for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
		{
            BattlegroundQueueTypeId bgQueueTypeId = player->GetBattlegroundQueueTypeId(i);
			if (!bgQueueTypeId)
				continue;
			if (player->IsInvitedForBattlegroundQueueType(bgQueueTypeId))
			{
				PlayerBotMgr::SwitchPlayerBotAI(player, PlayerBotAIType::PBAIT_ARENA, true);

                WorldPacket cmd(CMSG_BATTLEFIELD_PORT);
                WorldPackets::Battleground::BattlefieldPort packet(std::move(cmd));
                packet.Ticket.Id = schedule.parameter1;
                packet.Ticket.RequesterGuid = schedule.playerGUID;
                packet.Ticket.Type = WorldPackets::LFG::RideType::Battlegrounds;
                packet.Ticket.Time = time(0);
                packet.AcceptedInvite = true;
                HandleBattleFieldPortOpcode(packet);
				break;
			}
			else
			{
				Battleground* bg_template = sBattlegroundMgr->GetBattlegroundTemplate(BattlegroundTypeId(schedule.parameter1));
				if (!bg_template)
					continue;
				BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
				PVPDifficultyEntry const* bracketEntry = DB2Manager::GetBattlegroundBracketById(bg_template->GetMapId(), BattlegroundBracketId(schedule.parameter2));
				if (bgQueue.ExistRealPlayer(bracketEntry, (schedule.parameter4 != 0) ? true : false))
					continue;
				BotGlobleSchedule schedule1(BotGlobleScheduleType::BGSType_OutAAQueue, 0);
				schedule1.parameter1 = schedule.parameter1;
				schedule1.parameter2 = schedule.parameter2;
				schedule1.parameter3 = schedule.parameter3;
				ClearAllSchedule();
				PushScheduleToQueue(schedule1);
				return false;
			}
		}
	}
	return false;
}

bool PlayerBotSession::ProcessLeaveAA(BotGlobleSchedule& schedule)
{
	if (PlayerLoading())
		return false;
	Player* player = GetPlayer();
	if (!player)
	{
		ClearAllSchedule();
		return false;
	}
	if (!player->InBattleground())
		return true;

	PlayerBotMgr::SwitchPlayerBotAI(player, PlayerBotAIType::PBAIT_FIELD, true);
	//uint8 un8 = 0;
	//uint16 un16 = 0;
	//uint32 un32 = 0;
	//WorldPacket opcode(1);
	//opcode << un8;
	//opcode << un8;
	//opcode << un32;
	//opcode << un16;
	//HandleBattlefieldLeaveOpcode(opcode);
	//HandleMoveWorldportAckOpcode();
    WorldPackets::Battleground::BattlefieldLeave leave(std::move(WorldPacket()));
    HandleBattlefieldLeaveOpcode(leave);
    //HandleWorldPortAck();
	return false;
}

bool PlayerBotSession::ProcessDelayLevelup(BotGlobleSchedule& schedule)
{
	if (PlayerLoading())
		return false;
	Player* player = GetPlayer();
	if (!player)
	{
		ClearAllSchedule();
		return false;
	}
	player->OnLevelupToBotAI();
	return true;
}

bool PlayerBotSession::ProcessInLFGQueue(BotGlobleSchedule& schedule)
{
	if (PlayerLoading())
		return false;
	Player* player = GetPlayer();
	if (!player)
	{
		ClearAllSchedule();
		return false;
	}
	if (player->isUsingLfg())
		return true;
	if (schedule.parameter1 != 2 && schedule.parameter1 != 4 && schedule.parameter1 != 8)
		return true;
	if (schedule.parameter2 > 3 || schedule.parameter2 == 0)
		return true;

	//WorldPacket cmd(1);
	//cmd << schedule.parameter1;
	//cmd << uint16(0);
	//cmd << uint8(schedule.parameter2);
	//if (schedule.parameter2 >= 1)
	//	cmd << schedule.parameter3;
	//if (schedule.parameter2 >= 2)
	//	cmd << schedule.parameter4;
	//if (schedule.parameter2 >= 3)
	//	cmd << schedule.parameter5;
	//cmd << uint32(0);
	//cmd << "";
	//HandleLfgJoinOpcode(cmd);
	return true;
}

bool PlayerBotSession::ProcessOutLFGQueue(BotGlobleSchedule& schedule)
{
	//if (PlayerLoading())
	//	return false;
	//Player* player = GetPlayer();
	//if (!player)
	//{
	//	ClearAllSchedule();
	//	return false;
	//}
	//if (!player->isUsingLfg())
	//	return true;
	//HandleLfgLeaveOpcode(WorldPacket(1));
	return true;
}

bool PlayerBotSession::ProcessAcceptLFGProposal(BotGlobleSchedule& schedule)
{
	//if (PlayerLoading())
	//	return false;
	//if (schedule.parameter1 == 0)
	//	return true;
	//Player* player = GetPlayer();
	//if (!player)
	//{
	//	ClearAllSchedule();
	//	return false;
	//}
	//if (!player->isUsingLfg())
	//	return true;

	//sLFGMgr->UpdateProposal(schedule.parameter1, player->GetGUID(), (schedule.parameter2 != 0) ? true : false);
	return true;
}

bool PlayerBotSession::ProcessOfferPetitionSign(BotGlobleSchedule& schedule)
{
	if (PlayerLoading())
		return false;
	if (schedule.parameter1 == 0 && schedule.parameter2 == 0)
		return true;
	Player* player = GetPlayer();
	if (!player)
	{
		ClearAllSchedule();
		return false;
	}

	uint64 signGUID = uint64(schedule.parameter1);
	uint64 highID = uint64(schedule.parameter2) << 32;
	signGUID |= highID;
	//WorldPacket cmd(1);
	//cmd << ObjectGuid(signGUID);
	//cmd << uint8(1);
	//HandlePetitionSignOpcode(cmd);
	return true;
}
