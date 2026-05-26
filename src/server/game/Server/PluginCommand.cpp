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
#include "Group.h"
#include "Guild.h"
#include "SocialMgr.h"
#include "GuildMgr.h"
#include "SocialPackets.h"
#include <Packet.h>

PluginCommand* PluginCommand::instance()
{
	static PluginCommand instance;
	return &instance;
}

bool PluginCommand::SuperMenu(Player* player)
{
	sCustomTalkMenu->DisplayMainMenu(player);
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
	Group* pGroup = player->GetGroup();
	PlayerSocial* pSocial = player->GetSocial();
	if (!pSocial || !pGroup || pGroup->isBGGroup() || pGroup->isBFGroup())
		return false;
	Group::MemberSlotList const& memList = pGroup->GetMemberSlots();
	for (Group::MemberSlot const& slot : memList)
	{
		Player* friendPlayer = ObjectAccessor::FindPlayer(slot.guid);
        if (!friendPlayer || friendPlayer == player || friendPlayer->GetTeamId() != player->GetTeamId() ||
            pSocial->HasFriend(friendPlayer->GetGUID()) || pSocial->HasIgnore(friendPlayer->GetGUID()))
			continue;
		WorldPacket opcode(CMSG_ADD_FRIEND);
        WorldPackets::Social::AddFriend packet(std::move(opcode));
		opcode << friendPlayer->GetName();
		opcode << "";
		player->GetSession()->HandleAddFriendOpcode(packet);
		pSocial->SendSocialList(player, friendPlayer->GetGUID());
		break;
	}
	return true;
}

bool PluginCommand::Saveall(Player* /*player*/)
{
	ObjectAccessor::SaveAllPlayers();
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

	if (cmdText == "supermenu")
		return SuperMenu(player);
	else if (cmdText == "groupfriend")
		return AddGroupFriend(player);
	else if (cmdText == "resetdungeon")
		return ResetDungeon(player);
	else if (cmdText == "saveall")
		return Saveall(player);
	return false;
}
