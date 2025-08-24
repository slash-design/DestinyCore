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

#ifndef _PLUGIN_COMMAND_H_
#define _PLUGIN_COMMAND_H_

#include "Log.h"
#include "Common.h"
#include "SharedDefines.h"
#include "DatabaseEnv.h"
#include "Player.h"

class PluginCommand
{
private:
	PluginCommand() {};
	~PluginCommand() {};

public:
	PluginCommand(PluginCommand const&) = delete;
	PluginCommand(PluginCommand&&) = delete;

	PluginCommand& operator= (PluginCommand const&) = delete;
	PluginCommand& operator= (PluginCommand&&) = delete;

	static PluginCommand* instance();

	bool ProcessCommand(Player* player, std::string cmd);

private:
	bool BindingHomePosition(Player* player);
	bool SuperMenu(Player* player);
	bool OnlineCmd(Player* player, uint32 cls);
	bool AccountCmd(Player* player, uint32 cls);
	bool OfflineCmd(Player* player);
	bool AddGroupFriend(Player* player);
	bool ResetDungeon(Player* player);
	bool OnlineFriends(Player* player);
	bool Saveall(Player* player);
	bool ToggleWarfareAid(Player* player);
	bool OnlineArenaTeamMember(Player* player, uint32 arenaType);
	bool OnlineGuildMember(Player* player);
};

#define sPluginCommand PluginCommand::instance()

#endif // !_PLUGIN_COMMAND_H_
