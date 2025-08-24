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

#include "PluginCommand.h"
#include "CustomTalkMenu.h"
#include "PlayerBotMgr.h"
#include "Group.h"
#include "Guild.h"
#include "SocialMgr.h"
#include "FieldBotMgr.h"
#include "GuildMgr.h"

PluginCommand* PluginCommand::instance()
{
	static PluginCommand instance;
	return &instance;
}

bool PluginCommand::BindingHomePosition(Player* player)
{
	return PlayerBotSetting::BindingPlayerHomePosition(player);
}

bool PluginCommand::SuperMenu(Player* player)
{
	sCustomTalkMenu->DisplayMainMenu(player);
	return true;
}

bool PluginCommand::OnlineCmd(Player* player, uint32 cls)
{
	if (cls < 1 || cls > 11 || cls == 6 || cls == 10)
		return false;
	sPlayerBotMgr->AddNewPlayerBot(((player->GetTeamId() == TEAM_ALLIANCE) ? true : false), Classes(cls), 1);
	return true;
}

bool PluginCommand::AccountCmd(Player* player, uint32 cls)
{
	if (cls < 1 || cls > 11 || cls == 10)
		return false;
	sPlayerBotMgr->AddNewAccountBot(((player->GetTeamId() == TEAM_ALLIANCE) ? true : false), Classes(cls));
	return true;
}

bool PluginCommand::OfflineCmd(Player* player)
{
	sPlayerBotMgr->AllPlayerBotLogout();
	return true;
}

bool PluginCommand::ResetDungeon(Player* player)
{
	if (!player || !player->IsInWorld())
		return false;
	if (player->GetMap()->IsDungeon())
		return true;
	/*Group* pGroup = player->GetGroup();
	if (pGroup)
	{
		pGroup->ResetRaidDungeon();
	}
	else
	{
		player->ResetInstances(INSTANCE_RESET_ALL, false);
		player->ResetInstances(INSTANCE_RESET_ALL, true);
	}*/
	return true;
}

bool PluginCommand::AddGroupFriend(Player* player)
{
	/*Group* pGroup = player->GetGroup();
	PlayerSocial* pSocial = player->GetSocial();
	if (!pSocial || !pGroup || pGroup->isBGGroup() || pGroup->isBFGroup())
		return false;
	Group::MemberSlotList const& memList = pGroup->GetMemberSlots();
	for (Group::MemberSlot const& slot : memList)
	{
		Player* friendPlayer = ObjectAccessor::FindPlayer(slot.guid);
		if (!friendPlayer || friendPlayer == player || friendPlayer->GetTeamId() != player->GetTeamId() ||
			pSocial->HasFriend(friendPlayer->GetGUID().GetCounter()) || pSocial->HasIgnore(friendPlayer->GetGUID().GetCounter()))
			continue;
		WorldPacket opcode(1);
		opcode << friendPlayer->GetName();
		opcode << "";
		player->GetSession()->HandleAddFriendOpcode(opcode);
		pSocial->SendSocialList(player);
		break;
	}*/
	return true;
}

bool PluginCommand::OnlineFriends(Player* player)
{
	if (player)
		sPlayerBotMgr->LoginFriendBotByPlayer(player);
	return true;
}

bool PluginCommand::Saveall(Player* player)
{
	ObjectAccessor::SaveAllPlayers();
	Group* pGroup = player->GetGroup();
	if (pGroup)
	{
		sPlayerBotMgr->LogoutAllGroupPlayerBot(pGroup, true);
	}
	return true;
}

bool PluginCommand::ToggleWarfareAid(Player* player)
{
	if (sFieldBotMgr->ExistWarfare())
	{
		FieldWarfare::AID_TARGET_TEAM = !FieldWarfare::AID_TARGET_TEAM;
		return true;
	}
	return false;
}

bool PluginCommand::OnlineArenaTeamMember(Player* player, uint32 arenaType)
{
	/*ArenaTeam* arenaTeam = sArenaTeamMgr->GetArenaTypeTeamByGUID(player->GetGUID(), arenaType);
	if (arenaTeam)
	{
		for (ArenaTeam::MemberList::iterator itaMem = arenaTeam->m_membersBegin(); itaMem != arenaTeam->m_membersEnd(); itaMem++)
		{
			ArenaTeamMember& mem = *itaMem;
			ObjectGuid guid = mem.Guid;
			if (guid == player->GetGUID())
				continue;
			sPlayerBotMgr->DelayLoginPlayerBotByGUID(guid);
		}
		return true;
	}*/
	return true;
}

bool PluginCommand::OnlineGuildMember(Player* player)
{
	Guild* pGuild = player->GetGuild();
	if (!pGuild)
		return true;
	std::list<ObjectGuid> guids = pGuild->GetAllMemberGUID();
	for (ObjectGuid& guid : guids)
	{
		if (guid == player->GetGUID())
			continue;
		sPlayerBotMgr->DelayLoginPlayerBotByGUID(guid);
	}
	return true;
}

bool PluginCommand::ProcessCommand(Player* player, std::string cmd)
{
	if (!player)
		return false;
	uint32 index = cmd.find(' ');
	if (index < 1)
		return false;
	std::string cmdText = cmd.substr(0, index);
	std::string paramText = cmd.substr(index + 1);
	int param = atoi(paramText.c_str());

	if (cmdText == "bindinghome")
		return BindingHomePosition(player);
	else if (cmdText == "supermenu")
		return SuperMenu(player);
	else if (cmdText == "offlineallbot")
		return OfflineCmd(player);
	else if (cmdText == "onlineselfbot")
		return OnlineCmd(player, param);
	else if (cmdText == "onlineaccbot")
		return AccountCmd(player, param);
	else if (cmdText == "groupfriend")
		return AddGroupFriend(player);
	else if (cmdText == "resetdungeon")
		return ResetDungeon(player);
	else if (cmdText == "onlinefriends")
		return OnlineFriends(player);
	else if (cmdText == "saveall")
		return Saveall(player);
	else if (cmdText == "warfareaid")
		return ToggleWarfareAid(player);
	else if (cmdText == "onlinearenateam2")
		return OnlineArenaTeamMember(player, 2);
	else if (cmdText == "onlinearenateam3")
		return OnlineArenaTeamMember(player, 3);
	else if (cmdText == "onlinearenateam5")
		return OnlineArenaTeamMember(player, 5);
	else if (cmdText == "onlineguildmember")
		return OnlineGuildMember(player);
	return false;
}
