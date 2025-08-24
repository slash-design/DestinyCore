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

#ifndef SC_BOTFOLLOWERAI_H
#define SC_BOTFOLLOWERAI_H

#include "ScriptSystem.h"
#include "PlayerAI.h"
#include "Pathfinding.h"
#include "BotAITool.h"

class TC_GAME_API BotMovementAI : public PlayerAI
{
public:
	explicit BotMovementAI(Player* player) : PlayerAI(player), m_UpdateTick(BOTAI_UPDATE_TICK), pHorrorState(NULL)
	{
	}
	~BotMovementAI()
	{
	}

	void DamageDealt(Unit* victim, uint32& damage, DamageEffectType damageType) override;
	void DamageEndure(Unit* attacker, uint32& damage, DamageEffectType damageType);
	void UpdateAI(uint32 diff) override;
	void MovementTo(Player* player);
	void MovementTo(float x, float y, float z);
	void MovementToPath(Player* player, uint32 pid, uint32 index);
	void ApplyFinishPath(PathParameter* pathParam);

private:
	bool HasAuraMechanic(Unit* pTarget, Mechanics mask);
	void ProcessHorror(uint32 diff);

private:
	int32 m_UpdateTick;
	BotAIHorrorState* pHorrorState;
};

#endif // SC_BOTFOLLOWERAI_H
