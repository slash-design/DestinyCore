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

#ifndef _BOT_BGAI_MOVEMENT_H_
#define _BOT_BGAI_MOVEMENT_H_

#include "ScriptSystem.h"
#include "PlayerAI.h"
#include "BotAI.h"
#include "BotFieldAI.h"
#include "BotGroupAI.h"
#include "BotArenaAI.h"
#include "BotDuelAI.h"
#include "AIWaypointsMgr.h"

//class BotBGAI;

class TC_GAME_API BotAIVehicleMovement3D
{
public:
	BotAIVehicleMovement3D(Player* player);
	void SetStopMoveGap(float gap) { m_NextMoveGap = gap; }
	void ClearMovement();
	bool HaveNextmovement();
	bool HaveCurrentmovement();
	void AddMovement(Unit* pTarget, float maxOffset);
	bool UpdateVehicleMovement3D();

private:
	Player* m_Player;
	Position m_MovementPos;
	Position m_NextMovementPos;
	float m_NextMoveGap;
};

class BotBGAIMovement
{
public:
	BotBGAIMovement(Player* player, BotBGAI* ai);
	BotBGAIMovement(Player* player, BotFieldAI* ai);
	BotBGAIMovement(Player* player, BotGroupAI* ai);
	BotBGAIMovement(Player* player, BotArenaAI* ai);
	BotBGAIMovement(Player* player, BotDuelAI* ai);
	~BotBGAIMovement();

	ObjectGuid GetTargetObjectID() { return targetGuid; }
	void ClearMovement();
	void AcceptCommand(AIWaypoint* targetAIWP, bool isFlag = false);
	void AcceptCommand(ObjectGuid guid, bool isFlag = false);
	void ExecuteMovementCommand();
	bool ExecuteCruxMovementCommand();
	void MovementToAIWP(float offset = 0);
	void MovementTo(float x, float y, float z, float offset = 0);
	void MovementTo(ObjectGuid guid, float offset = 0);
	void MovementToTarget();
	void ApplyFinishPath(PathParameter* pathParam);
	void SyncPosition(Position& pos, bool immed = false);
	bool CanMovementTo(float x, float y, float z);
	bool SimulationMovementTo(float x, float y, float z, Position& outPos);

	static uint32 GetTargetFindpathPointCount(Player* self, Unit* pTarget);

private:
	bool IsNearToPosition(float x, float y, float z, float range);
	void TeleportToValidPosition();

private:
	Player* m_Player;
	BotBGAI* m_BGAI;
	BotFieldAI* m_FieldAI;
	BotGroupAI* m_GroupAI;
	BotArenaAI* m_ArenaAI;
	BotDuelAI* m_DuelAI;

	bool m_IsFlagTarget;
	ObjectGuid targetGuid;
	AIWaypoint* pTargetAIWP;
	int32 m_MovementTick;
	uint32 lastPathfindSure;
	uint32 m_LastSyncTick;
};

#endif // !_BOT_BGAI_MOVEMENT_H_
