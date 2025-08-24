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

#include "BotBGAIMovement.h"
#include "PathfindingMgr.h"
#include "CommandBG.h"
#include "WorldSession.h"
#include "MoveSplineInit.h"
#include "VMapFactory.h"
#include "MotionMaster.h"

BotAIVehicleMovement3D::BotAIVehicleMovement3D(Player* player) :
m_Player(player),
m_NextMoveGap(3.0f)
{
}

void BotAIVehicleMovement3D::ClearMovement()
{
	m_MovementPos = Position();
	m_NextMovementPos = Position();
}

bool BotAIVehicleMovement3D::HaveNextmovement()
{
	if (m_NextMovementPos.GetPositionX() != 0 && m_NextMovementPos.GetPositionY() != 0 && m_NextMovementPos.GetPositionZ() != 0)
		return true;
	return false;
}

bool BotAIVehicleMovement3D::HaveCurrentmovement()
{
	if (m_MovementPos.GetPositionX() != 0 && m_MovementPos.GetPositionY() != 0 && m_MovementPos.GetPositionZ() != 0)
		return true;
	return false;
}

void BotAIVehicleMovement3D::AddMovement(Unit* pTarget, float maxOffset)
{
	if (!pTarget || HaveNextmovement())
		return;
	Position pos = pTarget->GetPosition();
	float x = pos.GetPositionX() + frand(-maxOffset, maxOffset);
	float y = pos.GetPositionY() + frand(-maxOffset, maxOffset);
	float z = pos.GetPositionZ() + frand(-maxOffset, maxOffset);
	m_NextMovementPos = Position(x, y, z);
}

bool BotAIVehicleMovement3D::UpdateVehicleMovement3D()
{
	Unit* vehicle = m_Player->GetVehicleBase();
	if (!vehicle)
	{
		ClearMovement();
		return false;
	}
	float dist = HaveCurrentmovement() ? vehicle->GetDistance(m_MovementPos) : 0;
	if (dist > m_NextMoveGap)
	{
		vehicle->GetMotionMaster()->Clear();
		vehicle->GetMotionMaster()->MovePoint(1, m_MovementPos);
	}
	else if (HaveNextmovement())
	{
		m_MovementPos = m_NextMovementPos;
		m_NextMovementPos = Position();
		vehicle->GetMotionMaster()->Clear();
		vehicle->GetMotionMaster()->MovePoint(1, m_MovementPos);
	}
	else
	{
		ClearMovement();
		vehicle->GetMotionMaster()->Clear();
	}
	return true;
}

BotBGAIMovement::BotBGAIMovement(Player* player, BotBGAI* ai) :
m_Player(player),
m_BGAI(ai),
m_FieldAI(NULL),
m_GroupAI(NULL),
m_ArenaAI(NULL),
m_DuelAI(NULL),
m_IsFlagTarget(false),
lastPathfindSure(0),
pTargetAIWP(NULL),
m_MovementTick(0),
m_LastSyncTick(0)
{
}

BotBGAIMovement::BotBGAIMovement(Player* player, BotFieldAI* ai) :
m_Player(player),
m_BGAI(NULL),
m_FieldAI(ai),
m_GroupAI(NULL),
m_ArenaAI(NULL),
m_DuelAI(NULL),
m_IsFlagTarget(false),
lastPathfindSure(0),
pTargetAIWP(NULL),
m_MovementTick(0),
m_LastSyncTick(0)
{
}

BotBGAIMovement::BotBGAIMovement(Player* player, BotGroupAI* ai) :
m_Player(player),
m_BGAI(NULL),
m_FieldAI(NULL),
m_GroupAI(ai),
m_ArenaAI(NULL),
m_DuelAI(NULL),
m_IsFlagTarget(false),
lastPathfindSure(0),
pTargetAIWP(NULL),
m_MovementTick(0),
m_LastSyncTick(0)
{
}

BotBGAIMovement::BotBGAIMovement(Player* player, BotArenaAI* ai) :
m_Player(player),
m_BGAI(NULL),
m_FieldAI(NULL),
m_GroupAI(NULL),
m_ArenaAI(ai),
m_DuelAI(NULL),
m_IsFlagTarget(false),
lastPathfindSure(0),
pTargetAIWP(NULL),
m_MovementTick(0),
m_LastSyncTick(0)
{
}

BotBGAIMovement::BotBGAIMovement(Player* player, BotDuelAI* ai) :
m_Player(player),
m_BGAI(NULL),
m_FieldAI(NULL),
m_GroupAI(NULL),
m_ArenaAI(NULL),
m_DuelAI(ai),
m_IsFlagTarget(false),
lastPathfindSure(0),
pTargetAIWP(NULL),
m_MovementTick(0),
m_LastSyncTick(0)
{
}

BotBGAIMovement::~BotBGAIMovement()
{
}

void BotBGAIMovement::ClearMovement()
{
	m_IsFlagTarget = false;
	m_MovementTick = 0;
	lastPathfindSure = 0;
	targetGuid.Clear();
	pTargetAIWP = NULL;
	m_Player->StopMoving();
	if (Unit* pVehicle = m_Player->GetVehicleBase())
	{
		pVehicle->StopMoving();
	}
}

void BotBGAIMovement::AcceptCommand(AIWaypoint* targetAIWP, bool isFlag)
{
	if (!targetAIWP)
		return;
	if (pTargetAIWP)
	{
		if (targetAIWP->entry == pTargetAIWP->entry && m_IsFlagTarget == isFlag)
			return;
	}
	m_IsFlagTarget = isFlag;
	pTargetAIWP = targetAIWP;
	m_MovementTick = 0;
	if (pTargetAIWP)
		targetGuid.Clear();
}

void BotBGAIMovement::AcceptCommand(ObjectGuid guid, bool isFlag)
{
	m_IsFlagTarget = isFlag;
	targetGuid = guid;
	if (!targetGuid.IsEmpty())
		pTargetAIWP = NULL;
}

void BotBGAIMovement::ExecuteMovementCommand()
{
	if (m_Player->HasUnitState(UNIT_STATE_CASTING))
		return;
	if (!targetGuid.IsEmpty() && (targetGuid.IsPlayer() || targetGuid.IsCreature()))
		MovementTo(targetGuid, 2.0f);
	else// if (m_Player->IsStopped() || lastPathfindSure == 0)
	{
		if (!targetGuid.IsEmpty())
			MovementTo(targetGuid, 2.0f);
		else if (pTargetAIWP)
			MovementToAIWP(5.0f);
	}
}

bool BotBGAIMovement::ExecuteCruxMovementCommand()
{
	if (!m_BGAI)
		return false;
	if (m_Player->HasUnitState(UNIT_STATE_CASTING))
		return true;
	if (m_IsFlagTarget)
	{
		if (pTargetAIWP)
		{
			m_Player->SetSelection(ObjectGuid::Empty);
			MovementToAIWP(1.0f);
			return true;
		}
	}
	if (targetGuid.IsEmpty())
		return false;
	if (targetGuid.IsGameObject())
	{
		MovementTo(targetGuid, 0.5f);
		return true;
	}
	else if (targetGuid.IsPlayer())
	{
		Player* player = ObjectAccessor::FindPlayer(targetGuid);
		if (!player || !player->IsAlive())
			return false;
		float distance = m_Player->GetDistance(player->GetPosition());
		if (player->GetTeamId() != m_Player->GetTeamId())
		{
			if (distance < BOTAI_SEARCH_RANGE)
			{
				m_Player->SetSelection(targetGuid);
				MovementTo(targetGuid, 5);// m_IsMeleeBot ? 5.0f : BOTAI_RANGESPELL_DISTANCE);
			}
			return false;
		}
		if (distance < BOTAI_RANGESPELL_DISTANCE)
			return false;
		if (m_BGAI->NearHasEnemy())
			return false;
		MovementTo(targetGuid, 5.0f);
		return true;
	}
	return false;
}

void BotBGAIMovement::MovementToAIWP(float offset)
{
	if (!pTargetAIWP)
		return;
	if (m_MovementTick > 0 && m_Player->GetDistance(pTargetAIWP->GetPosition()) > BOTAI_SEARCH_RANGE)
	{
		--m_MovementTick;
		if (m_MovementTick < 0)
			m_MovementTick = 0;
		return;
	}
	MovementTo(pTargetAIWP->posX, pTargetAIWP->posY, pTargetAIWP->posZ, offset);
}

void BotBGAIMovement::MovementTo(float x, float y, float z, float offset /* = 0 */)
{
	if (m_BGAI && m_BGAI->IsNotSelect(m_Player))
		return;
	targetGuid.Clear();
	if (m_MovementTick > 0 && !m_Player->GetSelectedUnit())
	{
		--m_MovementTick;
		return;
	}
	m_MovementTick = 3;
	if (IsNearToPosition(x, y, z, (offset == 0) ? 0.8f : offset * 0.9f))
		return;

	uint32 sessionID = 0;
	if (m_Player->GetTypeId() == TypeID::TYPEID_PLAYER)
		sessionID = m_Player->GetSession()->GetAccountId();
	Unit* pVehicle = m_Player->GetVehicleBase();
	PathParameter* pathParam = new PathParameter(sessionID, pVehicle ? pVehicle : m_Player);
	pathParam->targetPosX = x;
	pathParam->targetPosY = y;
	pathParam->targetPosZ = z;
	pathParam->offset = offset;

	Pathfinding path(pathParam, NULL, NULL);
	if (pathParam->offset < 0)
		pathParam->offset *= -1;
	float posx = pathParam->targetPosX + ((pathParam->offset != 0) ? frand(pathParam->offset * (-1), pathParam->offset) : 0);
	float posy = pathParam->targetPosY + ((pathParam->offset != 0) ? frand(pathParam->offset * (-1), pathParam->offset) : 0);
	float posz = pathParam->targetPosZ;
	path.UpdateAllowedPositionZ(posx, posy, posz);
	bool result = path.CalculatePath(posx, posy, posz);
	if (!result || (path.GetPathType() & PATHFIND_NOPATH))
	{
		if (m_BGAI)
		{
//if (irand(0,3)==1 && !m_Player->IsInCombat()) 
//	{
		//m_Player->NearTeleportTo(x, y, z, 0);
	 //TeleportToValidPosition();
//	 TC_LOG_ERROR("PFThread::ThreadRun", "Path not find then tele!");
//	 }
	 //m_Player->NearTeleportTo(x, y, z, 0);
			TC_LOG_ERROR("PFThread::ThreadRun", "Path not find1");
			pathParam->findOK = false;
		}
		else
			pathParam->findOK = true;
	}
	else
		pathParam->findOK = true;

	if (pathParam->findOK)
	{
		pathParam->finishPaths.clear();
		const std::vector<G3D::Vector3>& points = path.GetPath();
		for (std::vector<G3D::Vector3>::const_iterator itPoints = points.begin();
			itPoints != points.end();
			itPoints++)
		{
			pathParam->finishPaths.push_back(*itPoints);
		}
		pathParam->destPosition = path.GetActualEndPosition();
	}

	if (!pathParam->findOK && pathParam->finishPaths.size() > 2)
		pathParam->finishPaths.erase(pathParam->finishPaths.begin());
	ApplyFinishPath(pathParam);
	delete pathParam;
}

void BotBGAIMovement::MovementTo(ObjectGuid guid, float offset /* = 0 */)
{
	if (m_BGAI)
	{
		if (m_BGAI->IsNotSelect(m_Player) || guid.IsEmpty())
			return;
	}
	pTargetAIWP = NULL;
	Position pos;
	pos.m_positionX = pos.m_positionY = pos.m_positionZ = 0;
	uint32 sessionID = 0;
	if (guid.IsPlayer())
	{
		Player* player = ObjectAccessor::FindPlayer(guid);
		if (player)
		{
			sessionID = player->GetSession()->GetAccountId();
			pos = player->GetPosition();
		}
		else
			return;
	}
	else if (guid.IsCreature())
	{
		Creature* creature = m_Player->GetMap()->GetCreature(guid);
		if (!creature)
			return;
		pos = creature->GetPosition();
	}
	else
	{
		GameObject* pObject = m_Player->GetMap()->GetGameObject(guid);
		if (!pObject)
			return;
		pos = pObject->GetPosition();
	}
	if (IsNearToPosition(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), (offset == 0) ? 1.0f : offset * 0.9f))
		return;
	Unit* pVehicle = m_Player->GetVehicleBase();
	PathParameter* pathParam = new PathParameter(sessionID, pVehicle ? pVehicle : m_Player);
	pathParam->targetPosX = pos.GetPositionX();
	pathParam->targetPosY = pos.GetPositionY();
	pathParam->targetPosZ = pos.GetPositionZ();
	pathParam->offset = offset;

	Pathfinding path(pathParam, NULL, NULL);
	if (pathParam->offset < 0)
		pathParam->offset *= -1;
	float posx = pathParam->targetPosX + ((pathParam->offset != 0) ? frand(pathParam->offset * (-1), pathParam->offset) : 0);
	float posy = pathParam->targetPosY + ((pathParam->offset != 0) ? frand(pathParam->offset * (-1), pathParam->offset) : 0);
	float posz = pathParam->targetPosZ;
	path.UpdateAllowedPositionZ(posx, posy, posz);
	bool result = path.CalculatePath(posx, posy, posz);
	if (!result || (path.GetPathType() & PATHFIND_NOPATH))
	{
		if (m_BGAI)
		{
			TC_LOG_ERROR("PFThread::ThreadRun", "Path not find2.");
			pathParam->findOK = false;
		}
		else
			pathParam->findOK = true;
	}
	else
		pathParam->findOK = true;

	if (pathParam->findOK)
	{
		pathParam->finishPaths.clear();
		const std::vector<G3D::Vector3>& points = path.GetPath();
		for (std::vector<G3D::Vector3>::const_iterator itPoints = points.begin();
			itPoints != points.end();
			itPoints++)
		{
			pathParam->finishPaths.push_back(*itPoints);
		}
		pathParam->destPosition = path.GetActualEndPosition();
	}

	if (!pathParam->findOK && pathParam->finishPaths.size() > 2)
		pathParam->finishPaths.erase(pathParam->finishPaths.begin());
	ApplyFinishPath(pathParam);
	delete pathParam;
}

void BotBGAIMovement::MovementToTarget()
{
	if (m_BGAI && m_BGAI->IsNotSelect(m_Player))
		return;
	Unit* pSelect = m_Player->GetSelectedUnit();
	if (pSelect)// && pSelect->ToPlayer())
	{
		//if (IsNearToPosition(pSelect->GetPositionX(), pSelect->GetPositionY(), pSelect->GetPositionZ(), 2.0f * 0.9f))
		//	return;
		Unit* pVehicle = m_Player->GetVehicleBase();
		PathParameter* pathParam = new PathParameter(m_Player->GetSession()->GetAccountId(), pVehicle ? pVehicle : m_Player);
		pathParam->targetPosX = pSelect->GetPositionX();
		pathParam->targetPosY = pSelect->GetPositionY();
		pathParam->targetPosZ = pSelect->GetPositionZ();
		pathParam->offset = 2.0f;

		Pathfinding path(pathParam, NULL, NULL);
		if (pathParam->offset < 0)
			pathParam->offset *= -1;
		float posx = pathParam->targetPosX + ((pathParam->offset != 0) ? frand(pathParam->offset * (-1), pathParam->offset) : 0);
		float posy = pathParam->targetPosY + ((pathParam->offset != 0) ? frand(pathParam->offset * (-1), pathParam->offset) : 0);
		float posz = pathParam->targetPosZ;
		path.UpdateAllowedPositionZ(posx, posy, posz);
		bool result = path.CalculatePath(posx, posy, posz);
		if (!result || (path.GetPathType() & PATHFIND_NOPATH))
		{
			if (m_BGAI)
			{
				TC_LOG_ERROR("PFThread::ThreadRun", "Path not find3.");
				pathParam->findOK = false;
			}
			else
				pathParam->findOK = true;
		}
		else
			pathParam->findOK = true;

		if (pathParam->findOK)
		{
			pathParam->finishPaths.clear();
			const std::vector<G3D::Vector3>& points = path.GetPath();
			for (std::vector<G3D::Vector3>::const_iterator itPoints = points.begin();
				itPoints != points.end();
				itPoints++)
			{
				pathParam->finishPaths.push_back(*itPoints);
			}
			pathParam->destPosition = path.GetActualEndPosition();
		}

		if (!pathParam->findOK && pathParam->finishPaths.size() > 2)
			pathParam->finishPaths.erase(pathParam->finishPaths.begin());
		ApplyFinishPath(pathParam);
		delete pathParam;
	}
}

void BotBGAIMovement::ApplyFinishPath(PathParameter* pathParam)
{
	if (m_BGAI && m_BGAI->IsNotSelect(m_Player))
		return;
	if (!pathParam)
		return;
	lastPathfindSure = pathParam->findOK ? 0 : lastPathfindSure + 1;
	if (m_Player->IsVehicle())
	{
		Unit* pVehicle = m_Player->GetVehicleBase();
		if (pVehicle)
			pVehicle->GetMotionMaster()->Clear();
	}
	else
		m_Player->GetMotionMaster()->Clear();
	if (pathParam->findOK)
	{
        Position pos = m_Player->GetPosition();
		pathParam->TrimOldPathpoint(pos);
	}
	else if (lastPathfindSure < 1)
	{
		//if (pathParam->destPosition.x == 0 && pathParam->destPosition.y == 0 && pathParam->destPosition.z == 0)
		//{
		//	m_Player->StopMoving();
		//	TeleportToValidPosition();
		//	lastPathfindSure = 0;
		//}
		//m_Player->SetSelection(ObjectGuid::Empty);
		m_MovementTick = 0;
		return;
	}
	else
	{
		lastPathfindSure = 0;
		m_MovementTick = 0;
		if (pathParam->destPosition.x == 0 && pathParam->destPosition.y == 0 && pathParam->destPosition.z == 0)
		{
			m_Player->StopMoving();
			TeleportToValidPosition();
			return;
		}
		for (std::vector<G3D::Vector3>::iterator itVec3 = pathParam->finishPaths.begin();
			itVec3 != pathParam->finishPaths.end();
			itVec3++)
		{
			float z = (*itVec3).z;
			m_Player->GetMap()->GetHeight(m_Player->GetPhaseShift(), (*itVec3).x, (*itVec3).y, z);
			(*itVec3).z = z;
		}
		m_Player->GetMap()->GetHeight(m_Player->GetPhaseShift(), pathParam->destPosition.x, pathParam->destPosition.y, pathParam->destPosition.z);
	}
	if (Unit* pVehicle = m_Player->GetVehicleBase())
	{
		pVehicle->GetMotionMaster()->MovePathfinding(pathParam);
	}
	else
		m_Player->GetMotionMaster()->MovePathfinding(pathParam);
}

void BotBGAIMovement::TeleportToValidPosition()
{
	if (!m_BGAI || !m_Player->GetMap())
		return;
	Position selfPos = m_Player->GetPosition();
	Position nearPosition;
	CommandBG* bgCommander = m_Player->GetMap()->GetCommander(m_Player->GetTeamId());
	if (bgCommander)
	{
		nearPosition = bgCommander->GetNearTeleportPoint(selfPos);
		if (selfPos.IsInDist(&nearPosition, 25))
		{
			nearPosition.SetOrientation(m_Player->GetOrientation());
			m_Player->SetSelection(ObjectGuid::Empty);
			m_BGAI->SetTeleport(nearPosition);
			//NearUnitVec& enemys = RangeEnemyListByTargetIsMe(BOTAI_RANGESPELL_DISTANCE);
			//for (Unit* enemy : enemys)
			//	enemy->SetTarget(ObjectGuid::Empty);
			return;
		}
	}
	float loopLayerRange = 8.0f;
	for (int i = 1; i <= 10; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			G3D::Vector3 findPos;
			findPos.x = m_Player->GetPositionX() + irand(loopLayerRange * i, loopLayerRange * i + loopLayerRange);
			findPos.y = m_Player->GetPositionY() + irand(loopLayerRange * i, loopLayerRange * i + loopLayerRange);
			findPos.z = m_Player->GetPositionZ(); float findZ = findPos.z + 2;
			m_Player->GetMap()->GetHeight(m_Player->GetPhaseShift(), findPos.x, findPos.y, findZ);
			findPos.z = findZ;
			G3D::Vector3 targetPos = findPos + (findPos - m_Player->GetVector3()).direction() * loopLayerRange;
			findZ = targetPos.z;
			m_Player->GetMap()->GetHeight(m_Player->GetPhaseShift(), targetPos.x, targetPos.y, findZ);
			targetPos.z = findZ;
			uint32 sessionID = m_Player->GetSession()->GetAccountId();
			PathParameter pathParam(sessionID, m_Player);
			pathParam.posX = targetPos.x;
			pathParam.posY = targetPos.y;
			pathParam.posZ = targetPos.z;
			pathParam.offset = 0;
			pathParam.targetPosX = findPos.x;
			pathParam.targetPosY = findPos.y;
			pathParam.targetPosZ = findPos.z;
			Pathfinding path(&pathParam, NULL, NULL);
			bool result = path.CalculatePath(targetPos.x, targetPos.y, targetPos.z);
			if (!result || (path.GetPathType() & PATHFIND_NOPATH))
				continue;
			m_Player->SetSelection(ObjectGuid::Empty);
            Position pos = Position(findPos.x, findPos.y, findPos.z, m_Player->GetOrientation());
			if (m_BGAI)
				m_BGAI->SetTeleport(pos);
			else if (m_GroupAI)
				m_GroupAI->SetTeleport(pos);
			else if (m_ArenaAI)
				m_ArenaAI->SetTeleport(pos);
			else if (m_DuelAI)
				m_DuelAI->SetTeleport(pos);
			m_Player->StopMoving();
			return;
		}
	}
	//TC_LOG_ERROR("BotBGAI.Pathfinding", "Pathfinding error by %s. Force tele to near wp.", me->GetName().c_str());
	if (bgCommander)
	{
		nearPosition.SetOrientation(m_Player->GetOrientation());
		m_Player->SetSelection(ObjectGuid::Empty);
		if (m_BGAI)
			m_BGAI->SetTeleport(nearPosition);
		else if (m_GroupAI)
			m_GroupAI->SetTeleport(nearPosition);
		//NearUnitVec& enemys = RangeEnemyListByTargetIsMe(BOTAI_RANGESPELL_DISTANCE);
		//for (Unit* enemy : enemys)
		//	enemy->SetTarget(ObjectGuid::Empty);
	}
}

void BotBGAIMovement::SyncPosition(Position& pos, bool immed)
{
	++m_LastSyncTick;
	if (m_LastSyncTick <= 8 && !immed)
	{
		return;
	}
	m_LastSyncTick = 0;
	m_MovementTick = 0;
	if (m_Player->IsFlying())
		return;

	Position targetPos(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), m_Player->GetOrientation());
	targetPos.m_positionZ = 0.01f + m_Player->GetMap()->GetHeight(m_Player->GetPhaseShift(), targetPos.GetPositionX(), targetPos.GetPositionY(), targetPos.m_positionZ);
	WorldSession* pSession = m_Player->GetSession();
	MovementInfo movementInfo;
	movementInfo.pos = targetPos;
	movementInfo.time = getMSTime();
	movementInfo.guid = m_Player->GetGUID();
    movementInfo.flags = 0;
    movementInfo.flags2 = 0;

	WorldPacket data(CMSG_MOVE_FALL_LAND);// MSG_MOVE_STOP);
    data << movementInfo;
	m_Player->SendMessageToSet(&data, m_Player);
}

bool BotBGAIMovement::IsNearToPosition(float x, float y, float z, float range)
{
	if (!m_Player->InBattleground())
		return false;
	if (Unit* pVehicle = m_Player->GetVehicleBase())
	{
		float dist = pVehicle->GetDistance(x, y, z);
		if (range < 1.0f)
			range = 1.0f;
		if (dist <= range)
			return true;
	}
	else
	{
		float dist = m_Player->GetDistance(x, y, z);
		if (range < 1.0f)
			range = 1.0f;
		if (dist <= range)
			return true;
	}
	return false;
}

bool BotBGAIMovement::CanMovementTo(float x, float y, float z)
{
	if (!m_BGAI || m_BGAI->IsNotSelect(m_Player))
		return false;

	uint32 sessionID = 0;
	if (m_Player->GetTypeId() == TypeID::TYPEID_PLAYER)
		sessionID = m_Player->GetSession()->GetAccountId();
	Unit* pVehicle = m_Player->GetVehicleBase();
	PathParameter* pathParam = new PathParameter(sessionID, pVehicle ? pVehicle : m_Player);
	pathParam->targetPosX = x;
	pathParam->targetPosY = y;
	pathParam->targetPosZ = z;
	pathParam->offset = 0;

	Pathfinding path(pathParam, NULL, NULL);
	if (pathParam->offset < 0)
		pathParam->offset *= -1;
	float posx = pathParam->targetPosX;
	float posy = pathParam->targetPosY;
	float posz = pathParam->targetPosZ;
	path.UpdateAllowedPositionZ(posx, posy, posz);
	bool result = path.CalculatePath(posx, posy, posz);
	if (!result || (path.GetPathType() & PATHFIND_NOPATH))
	{
		if (m_BGAI)
		{
			delete pathParam;
			return false;
		}
	}
	delete pathParam;
	return true;
}

bool BotBGAIMovement::SimulationMovementTo(float x, float y, float z, Position& outPos)
{
	if (!m_BGAI || m_BGAI->IsNotSelect(m_Player))
		return false;

	uint32 sessionID = 0;
	if (m_Player->GetTypeId() == TypeID::TYPEID_PLAYER)
		sessionID = m_Player->GetSession()->GetAccountId();
	Unit* pVehicle = m_Player->GetVehicleBase();
	PathParameter* pathParam = new PathParameter(sessionID, pVehicle ? pVehicle : m_Player);
	pathParam->targetPosX = x;
	pathParam->targetPosY = y;
	pathParam->targetPosZ = z;
	pathParam->offset = 0;

	Pathfinding path(pathParam, NULL, NULL);
	if (pathParam->offset < 0)
		pathParam->offset *= -1;
	float posx = pathParam->targetPosX;
	float posy = pathParam->targetPosY;
	float posz = pathParam->targetPosZ;
	path.UpdateAllowedPositionZ(posx, posy, posz);
	bool result = path.CalculatePath(posx, posy, posz);
	if (!result || (path.GetPathType() & PATHFIND_NOPATH))
	{
		if (m_BGAI)
		{
			delete pathParam;
			return false;
		}
	}

	Map* pMap = m_Player->GetMap();
	uint32 size = path.GetPath().size();
	if (size <= 2)
	{
		delete pathParam;
		return false;
	}
	bool hasPos = false;
	for (uint32 i = 0; i < size; i++)
	{
		if (i >= size - 1)
			break;
		const G3D::Vector3& p = path.GetPath()[i];
		const G3D::Vector3& np = path.GetPath()[i + 1];
		if (pMap->isInLineOfSight(m_Player->GetPhaseShift(), p.x, p.y, p.z + 2.0f, np.x, np.y, np.z + 2.0f, VMAP::ModelIgnoreFlags::Nothing))
		{
			outPos.m_positionX = p.x;
			outPos.m_positionY = p.y;
			outPos.m_positionZ = p.z;
			hasPos = true;
		}
	}
	if (!hasPos)
	{
		delete pathParam;
		return false;
	}
	//outPos.m_positionZ = pMap->GetHeight(m_Player->GetPhaseMask(), outPos.m_positionX, outPos.m_positionY, outPos.m_positionZ);
	delete pathParam;
	return true;
}

uint32 BotBGAIMovement::GetTargetFindpathPointCount(Player* self, Unit* pTarget)
{
	if (!self || !pTarget)
		return 0;
	uint32 sessionID = 0;
	if (self->GetTypeId() == TypeID::TYPEID_PLAYER)
		sessionID = self->GetSession()->GetAccountId();
	Unit* pVehicle = self->GetVehicleBase();
	PathParameter* pathParam = new PathParameter(sessionID, pVehicle ? pVehicle : self);
	pathParam->targetPosX = pTarget->GetPositionX();
	pathParam->targetPosY = pTarget->GetPositionY();
	pathParam->targetPosZ = pTarget->GetPositionZ();
	pathParam->offset = 0;

	Pathfinding path(pathParam, NULL, NULL);
	if (pathParam->offset < 0)
		pathParam->offset *= -1;
	float posx = pathParam->targetPosX;
	float posy = pathParam->targetPosY;
	float posz = pathParam->targetPosZ;
	path.UpdateAllowedPositionZ(posx, posy, posz);
	bool result = path.CalculatePath(posx, posy, posz);
	if (!result || (path.GetPathType() & PATHFIND_NOPATH))
	{
		delete pathParam;
		return 0;
	}

	uint32 size = path.GetPath().size();
	delete pathParam;
	if (size <= 2)
		return 0;
	return size;
}
