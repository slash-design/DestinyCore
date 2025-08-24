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

#ifndef _COMMAND_EY_H_
#define _COMMAND_EY_H_

#include "CommandBG.h"

#define AIWP_SELF_START 0
#define AIWP_SELF_TOWER1 1
#define AIWP_SELF_TOWER2 2
#define AIWP_SELF_TOWER3 3
#define AIWP_SELF_TOWER4 4
#define AIWP_ENEMY_START 5
#define AIWP_EY_FLAG 6

#define MAX_EYAIWP_COUNT 7

class CommandEY : public CommandBG
{
public:
	CommandEY(Battleground* pBG, TeamId team);
	~CommandEY();

	void Initialize() override;
	void StartGame() override;
	bool AddPlayerBot(Player* player, Battleground* pBG) override;
	const Creature* GetMatchGraveyardNPC(const Player* player) override;
	void Update(uint32 diff) override;
	void OnPlayerRevive(Player* player) override;
	bool CanUpMount(Player* player) override;
	//AIWaypoint* GetReadyPosition() override;

private:
	void RndStartCommand();
	void ProcessRegulation();

	void ProcessEYPointRequirement(uint32 point, AIWaypoint* waypoint, PlayerGUIDs& players);
	bool EYPointIsOccupied(uint32 point, TeamId team);
	PlayerGUIDs GetEYPointRangePlayerByTeam(uint32 point, TeamId team);
	bool AcceptCommandByPlayerGUID(uint64 guid, AIWaypoint* targetAIWP, bool isFlag = false);
	bool AcceptCommandByPlayerGUID(uint64 guid, ObjectGuid flagGuid, bool isFlag = false);
	uint32 GetEYPointIndexByTeam(uint32 index, TeamId team);
	void TryPickStormFlag(uint64 guid);
	void TryCaptureFlag(Player* player, AIWaypoint* pAIWP, uint32 point);
	void ProcessFlagPicker(Player* player);
	uint32 GetCaptureFlagPoint(uint32 index, TeamId team);

private:
	int32 lastUpdateTick;
};

#endif // !_COMMAND_EY_H_
