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

#ifndef _COMMAND_WS_H_
#define _COMMAND_WS_H_

#include "CommandBG.h"

#define AIWP_SELF_FLAG 0
#define AIWP_SELF_PORT1 1
#define AIWP_SELF_PORT2 2
#define AIWP_SELF_PORT3 3
#define AIWP_ENEMY_FLAG 4
#define AIWP_ENEMY_PORT1 5
#define AIWP_ENEMY_PORT2 6
#define AIWP_ENEMY_PORT3 7

#define MAX_WSAIWP_COUNT 8

typedef std::vector<BGCommandInfo> BGCommands;

class CommandWS : public CommandBG
{
public:
	CommandWS(Battleground* pBG, TeamId team);
	~CommandWS();

	void Initialize() override;
	const Creature* GetMatchGraveyardNPC(const Player* player) override;
	void Update(uint32 diff) override;
	bool CanUpMount(Player* player) override;

private:
	void InsureAttackAndDefance(uint32 count);
	void RndStartCommand();
	void ProcessRegulation();

	void ProcessAllPicked();
	void ProcessAllGuared(ObjectGuid guaredGuid);
	void ProcessAllAttack(ObjectGuid& attackGuid);
	void ProcessAttackAndGuard(ObjectGuid attackGuid, ObjectGuid guaredGuid);

	void TryPickEnemyFlag(uint64 guid);
	void TryPickSelfFlag(uint64 guid);
	void TryCaptureFlag(uint64 guid);
	ObjectGuid SelfBGFlagPicker();
	ObjectGuid EnemyBGFlagPicker();
	ObjectGuid GetBGFlagFromSelf();
	ObjectGuid GetBGFlagFromEnemy();

	GameObject* SearchDropedFlag(uint64 guid, TeamId team);

private:
	int32 lastUpdateTick;
};

#endif // !_COMMAND_WS_H_
