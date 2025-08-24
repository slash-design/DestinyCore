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

#ifndef __AIWAYPOINTSMGR_H__
#define __AIWAYPOINTSMGR_H__

#include "Log.h"
#include "Common.h"
#include "SharedDefines.h"
#include "DatabaseEnv.h"

#define RESOLVER_BUF_SIZE 128
#define MAX_LINK_AIWP 10

class AIWaypointsMgr;

struct AIWaypoint
{
	uint32 entry;
	uint32 mapID;
	float posX;
	float posY;
	float posZ;
	AIWaypoint* linkAWP[MAX_LINK_AIWP];
	std::string processLink;
	std::string pointDesc;

	AIWaypoint(uint32 id, uint32 map, float x, float y, float z, std::string linkInfo, std::string desc) :
		entry(id), mapID(map), posX(x), posY(y), posZ(z), processLink(linkInfo)
	{
		for (int i = 0; i < MAX_LINK_AIWP; i++)
			linkAWP[i] = NULL;
	}

	void LinkToAIWP(AIWaypoint* aiwp);
	void BreakToAIWP(AIWaypoint* aiwp);
	//void ProcessLinkInfo(AIWaypointsMgr* aiwpMgr);
	void NewToDatabase();
	void SaveToDatabase();
	void DeleteToDatabase();
	Position GetPosition()
	{
		Position pos;
		pos.m_positionX = posX;
		pos.m_positionY = posY;
		pos.m_positionZ = posZ;
		return pos;
	}

private:
	static char resolverBuffer[RESOLVER_BUF_SIZE];
};

class TempSummon;
class Player;
class Creature;

class TC_GAME_API AIWaypointsMgr
{
public:
	typedef std::map<uint32, AIWaypoint*> AIWPMap;
	typedef std::map<uint32, TempSummon*> AIWPCreatures;

public:
	AIWaypointsMgr();
	~AIWaypointsMgr();

	static AIWaypointsMgr* instance();

	bool LoadAIWaypoints();
	AIWaypoint* FindAIWaypoint(uint32 entry);

	bool ShowAllAIWaypointsCreature(Player* player);
	bool CreateAIWPCreature(Player* player);
	bool DeleteAIWPCreature(Creature* pCreature);
	bool MoveAIWPCreature(Player* player, Creature* pCreature);
	bool AIWPCreatureDesc(Creature* pCreature, std::string descStr);

private:
	AIWaypoint* AddNewWaypoint(uint32 map, float x, float y, float z);
	bool MovementWaypoint(uint32 entry, float x, float y, float z);
	bool WaypointDesc(uint32 point, std::string descStr);
	bool LinkWaypoint(uint32 srcPoint, uint32 dstPoint);
	bool BreakWaypoint(uint32 srcPoint, uint32 dstPoint);
	bool DeleteWaypoint(uint32 entry);

private:
	uint32 m_lastAIWPEntry;
	AIWPMap m_AIWaypointMap;
	AIWPCreatures m_AIWPCreatures;
};

#define sAIWPMgr AIWaypointsMgr::instance()

#endif // __AIWAYPOINTSMGR_H__
