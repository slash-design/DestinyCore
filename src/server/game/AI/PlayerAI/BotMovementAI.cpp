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

#include "BotMovementAI.h"
#include "WaypointManager.h"
#include "PathfindingMgr.h"
#include "WorldSession.h"
#include "BotAITool.h"
#include "MapManager.h"
#include "MotionMaster.h"
#include "WaypointMovementGenerator.h"

void BotMovementAI::DamageDealt(Unit* victim, uint32& damage, DamageEffectType damageType)
{
	if (!victim || !me->IsInWorld() || damage == 0)
		return;

	if (me->InBattleground())
	{
		//Player* vicPlayer = victim->ToPlayer();
		//if (!vicPlayer)
		//	return;
		//int32 meCP = me->GetEquipCombatPower();
		//int32 vicCP = vicPlayer->GetEquipCombatPower();
		//int32 cpGap = meCP - vicCP;
		//if (cpGap == 0)
		//	return;
		//float addion = 0;
		//if (cpGap > 0 && meCP > 0)
		//{
		//	addion = float(cpGap) / float(meCP);
		//	addion = 1.0f - addion;
		//}
		//else if (cpGap < 0 && vicCP > 0)
		//{
		//	addion = float(cpGap * (-1.0f)) / float(vicCP);
		//	addion += 1.0f;
		//}
		//if (addion <= 0)
		//	return;
		//float result = float(damage);
		//switch (damageType)
		//{
		//case DamageEffectType::DIRECT_DAMAGE:
		//case DamageEffectType::SPELL_DIRECT_DAMAGE:
		//case DamageEffectType::DOT:
		//	result *= addion;
		//	break;
		//default:
		//	return;
		//}
		//damage = uint32(result);
	}
	else if (me->GetMap()->IsDungeon())
	{
		damage = uint32(float(damage) * BotUtility::DungeonBotDamageModify);
	}
}

void BotMovementAI::DamageEndure(Unit* attacker, uint32& damage, DamageEffectType damageType)
{
	if (!attacker || !me->IsInWorld() || damage == 0)
		return;

	if (me->InBattleground())
	{
		//Player* attPlayer = attacker->ToPlayer();
		//if (!attPlayer)
		//	return;
		//int32 meCP = me->GetEquipCombatPower();
		//int32 attCP = attPlayer->GetEquipCombatPower();
		//int32 cpGap = attCP - meCP;
		//if (cpGap == 0)
		//	return;
		//float addion = 0;
		//if (cpGap > 0)
		//{
		//	addion = float(cpGap) / float(attPlayer->GetEquipCombatPower());
		//	addion = 1.0f - addion;
		//}
		//else
		//{
		//	addion = float(cpGap * (-1.0f)) / float(me->GetEquipCombatPower());
		//	addion += 1.0f;
		//}
		//if (addion <= 0)
		//	return;
		//float result = float(damage);
		//switch (damageType)
		//{
		//case DamageEffectType::DIRECT_DAMAGE:
		//case DamageEffectType::SPELL_DIRECT_DAMAGE:
		//case DamageEffectType::DOT:
		//	result *= addion;
		//	break;
		//default:
		//	return;
		//}
		//damage = uint32(result);
	}
	else if (me->GetMap()->IsDungeon())
	{
		damage = uint32(float(damage) * (1.0f / BotUtility::DungeonBotEndureModify));
	}
}

void BotMovementAI::UpdateAI(uint32 diff)
{
	m_UpdateTick -= diff;
	if (m_UpdateTick > 0)
		return;
	m_UpdateTick = BOTAI_UPDATE_TICK;

	//ProcessHorror(diff);
}

void BotMovementAI::MovementTo(Player* player)
{
	uint32 sessionID = 0;
	if (me->GetTypeId() == TypeID::TYPEID_PLAYER)
		sessionID = ((Player*)me)->GetSession()->GetAccountId();
	PathParameter* pathParam = new PathParameter(sessionID, me);
	pathParam->targetPosX = player->GetPositionX();
	pathParam->targetPosY = player->GetPositionY();
	pathParam->targetPosZ = player->GetPositionZ();
	//sFPMgr->AddPFParameter(pathParam);

	Pathfinding path(pathParam, NULL, NULL);
	bool result = path.CalculatePath(pathParam->targetPosX, pathParam->targetPosY, pathParam->targetPosZ);
	if (!result || (path.GetPathType() & PATHFIND_NOPATH))
	{
		TC_LOG_ERROR("PFThread::ThreadRun", "Path not find4.");
		pathParam->findOK = false;
	}
	else
		pathParam->findOK = true;
	pathParam->finishPaths.clear();
	const std::vector<G3D::Vector3>& points = path.GetPath();
	for (std::vector<G3D::Vector3>::const_iterator itPoints = points.begin();
		itPoints != points.end();
		itPoints++)
	{
		pathParam->finishPaths.push_back(*itPoints);
	}
	pathParam->destPosition = path.GetActualEndPosition();
	ApplyFinishPath(pathParam);
}

void BotMovementAI::MovementTo(float x, float y, float z)
{
	uint32 sessionID = 0;
	if (me->GetTypeId() == TypeID::TYPEID_PLAYER)
		sessionID = ((Player*)me)->GetSession()->GetAccountId();
	PathParameter* pathParam = new PathParameter(sessionID, me);
	pathParam->targetPosX = x;
	pathParam->targetPosY = y;
	pathParam->targetPosZ = z;
	//sFPMgr->AddPFParameter(pathParam);

	Pathfinding path(pathParam, NULL, NULL);
	bool result = path.CalculatePath(pathParam->targetPosX, pathParam->targetPosY, pathParam->targetPosZ);
	if (!result || (path.GetPathType() & PATHFIND_NOPATH))
	{
		TC_LOG_ERROR("PFThread::ThreadRun", "Path not find5.");
	}
	pathParam->findOK = true;
	pathParam->finishPaths.clear();
	const std::vector<G3D::Vector3>& points = path.GetPath();
	for (std::vector<G3D::Vector3>::const_iterator itPoints = points.begin();
		itPoints != points.end();
		itPoints++)
	{
		pathParam->finishPaths.push_back(*itPoints);
	}
	pathParam->destPosition = path.GetActualEndPosition();
	//ApplyFinishPath(pathParam);

	for (uint32 i = 0; i < pathParam->finishPaths.size(); ++i)
		((Player*)me->ToPlayer())->SummonCreature(VISUAL_WAYPOINT, pathParam->finishPaths[i].x, pathParam->finishPaths[i].y, pathParam->finishPaths[i].z, 0, TEMPSUMMON_TIMED_DESPAWN, 240000);
}

void BotMovementAI::ApplyFinishPath(PathParameter* pathParam)
{
	if (!pathParam)
		return;
	if (pathParam->findOK)
	{
		me->GetMotionMaster()->Clear();
		me->GetMotionMaster()->MovePathfinding(pathParam);
	}
}

void BotMovementAI::MovementToPath(Player* player, uint32 pid, uint32 index)
{
	const WaypointPath* paths = sWaypointMgr->GetPath(pid);
	if (!paths || paths->nodes.size() < index)
		return;
    const WaypointNode* pPoint = (&paths->nodes.at(index));
	//me->TeleportTo(me->GetMapId(), pPoint->x, pPoint->y, pPoint->z, pPoint->orientation);
	//player->TeleportTo(player->GetMapId(), pPoint->x, pPoint->y, pPoint->z, pPoint->orientation);
	me->GetMotionMaster()->MovePoint(pPoint->id, pPoint->x, pPoint->y, pPoint->z);
}

bool BotMovementAI::HasAuraMechanic(Unit* pTarget, Mechanics mask)
{
	if (!pTarget)
		return false;
	return (pTarget->HasAuraWithMechanic(1 << mask));
}

void BotMovementAI::ProcessHorror(uint32 diff)
{
	if (me->IsBeingTeleported())
		return;
	if (HasAuraMechanic(me, Mechanics::MECHANIC_HORROR) ||
		HasAuraMechanic(me, Mechanics::MECHANIC_DISORIENTED) ||
		HasAuraMechanic(me, Mechanics::MECHANIC_FEAR))
	{
		if (!me->IsAlive() || me->HasAura(27827)) // (27827 ¾ÈÊêÖ®»ê ÉñÄÁËÀÍöºó)
		{
			me->StopMoving();
			return;
		}

		if (HasAuraMechanic(me, Mechanics::MECHANIC_ROOT) || HasAuraMechanic(me, Mechanics::MECHANIC_STUN) || HasAuraMechanic(me, Mechanics::MECHANIC_CHARM))
		{
			me->StopMoving();
			return;
		}

		if (!pHorrorState)
		{
			pHorrorState = new BotAIHorrorState(me);
			me->GetMotionMaster()->Clear();
			me->StopMoving();
		}
		else if (me->IsFalling())
		{
			me->StopMoving();
			float validZ = me->GetMap()->GetHeight(me->GetPhaseShift(), me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
			me->TeleportTo(me->GetMapId(), me->GetPositionX(), me->GetPositionY(), validZ, me->GetOrientation());
			return;
		}
		pHorrorState->UpdateHorror(diff, 0);
	}
	else if (pHorrorState)
	{
		delete pHorrorState;
		pHorrorState = NULL;
		me->StopMoving();
	}
}
