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

#ifndef _COMMANDBG_H_
#define _COMMANDBG_H_

#define COMMANDBG_UPDATE_TICK 5000

#include "Battleground.h"
#include "Player.h"

class Player;
class Battleground;

enum CommandModel
{
	CM_Dispersibility,
	CM_GroupFocus,
	CM_Regulation,
	CM_Over
};

class CommandBG
{
public:
	CommandBG(Battleground* pBG, TeamId team);
	virtual ~CommandBG();

	static void SettingCommandModel(CommandModel modelType) { g_CommandModelType = modelType; }

	virtual void Initialize();
	virtual void ReadyGame();
	virtual void StartGame();
	virtual void Update(uint32 diff);

	void UpdateBelongBattleground(Battleground* pBG) { m_pBattleground = pBG; }

protected:
	Battleground* m_pBattleground;
	TeamId m_TeamID;

protected:
	static CommandModel g_CommandModelType;
};

#endif // _COMMANDBG_H_
