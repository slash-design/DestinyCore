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

#ifndef _BOT_ARENA_AI_
#define _BOT_ARENA_AI_

#include "BotAITool.h"
#include "BotAISpells.h"
#include "SpellMgr.h"
#include "PlayerBotSetting.h"
#include "GridNotifiers.h"
#include <mutex>

class Player;
class BotBGAIMovement;

class TC_GAME_API BotArenaAI : public PlayerAI
{
public:
	static BotArenaAI* CreateBotArenaAIByPlayerClass(Player* player);
	static BotArenaAI* debugArenaAI;

public:
	BotArenaAI(Player* player);
	~BotArenaAI();

	Player* GetAIPayer() { return me; }
	void UpdateAI(uint32 diff) override;
	virtual void ResetBotAI();
	void SetResetAI() { m_HasReset = false; }
	bool HasTeleport() { return !m_Teleporting.CanMovement(); }
	void SetTeleport(Position& telePos) { m_Teleporting.SetTeleport(telePos); }
	void UpdateTeleport(uint32 diff) { m_Teleporting.Update(diff, m_Movement); }
	bool IsBGSchedule();

protected:
	int32 m_UpdateTick;

	BotBGAIMovement* m_Movement;
	BotAIHorrorState* pHorrorState;
	BotAIStoped m_CheckStoped;
	BotAITeleport m_Teleporting;
	BotAIFlee m_Flee;

	bool m_HasReset;

	bool m_IsMeleeBot;
	bool m_IsRangeBot;
	bool m_IsHealerBot;

	std::mutex m_ItemLock;
};

#endif // _BOT_ARENA_AI_
