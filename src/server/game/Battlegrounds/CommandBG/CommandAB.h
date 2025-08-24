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

#ifndef _COMMAND_AB_H_
#define _COMMAND_AB_H_

#include "CommandBG.h"

#define AIWP_SELF_STABLES 0
#define AIWP_SELF_LUMBER_MILL 1
#define AIWP_SELF_BLACKSMITH 2
#define AIWP_SELF_GOLD_MINE 3
#define AIWP_SELF_FARM 4
#define AIWP_ENEMY_FARM 5
#define AIWP_ENEMY_GOLD_MINE 6
#define AIWP_ENEMY_BLACKSMITH 7
#define AIWP_ENEMY_LUMBER_MILL 8
#define AIWP_ENEMY_STABLES 9
#define AIWP_LM_BRIDGE 10
#define AIWP_BL_BRIDGE 11
#define AIWP_LM_START 12
#define AIWP_BL_START 13

#define MAX_ABAIWP_COUNT 14

class CommandAB : public CommandBG
{
public:
	CommandAB(Battleground* pBG, TeamId team);
	~CommandAB();

	void Initialize() override;
	const Creature* GetMatchGraveyardNPC(const Player* player) override;
	void Update(uint32 diff) override;
	AIWaypoint* GetReadyPosition() override;

private:
	void RndStartCommand();
	void ProcessRegulation();

	void ProcessABNodeRequirement(uint32 abNode, AIWaypoint* waypoint, PlayerGUIDs& players);
	bool ABFlagIsOccupied(uint32 abNode, TeamId team);
	PlayerGUIDs GetABFlagRangePlayerByTeam(uint32 abNode, TeamId team);
	bool AcceptCommandByPlayerGUID(uint64 guid, AIWaypoint* targetAIWP, bool isFlag = false);
	bool AcceptCommandByPlayerGUID(uint64 guid, ObjectGuid flagGuid, bool isFlag = false);
	void TryOccupiedABNode(uint64 guid);
	uint32 GetABNodeIndexByTeam(uint32 index, TeamId team);

private:
	int32 lastUpdateTick;

};

#endif // !_COMMAND_AB_H_
