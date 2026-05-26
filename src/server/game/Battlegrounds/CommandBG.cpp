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

#include "CommandBG.h"

CommandModel CommandBG::g_CommandModelType = CM_Regulation;

CommandBG::CommandBG(Battleground* pBG, TeamId team) :
m_pBattleground(pBG),
m_TeamID(team)
{
}

CommandBG::~CommandBG()
{
}

void CommandBG::Initialize()
{
}

void CommandBG::ReadyGame()
{
}

void CommandBG::StartGame()
{
}

void CommandBG::Update(uint32 /*diff*/)
{
}
