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

#ifndef __ONLINE_MGR_H__
#define __ONLINE_MGR_H__

#include "Log.h"
#include "Common.h"
#include "SharedDefines.h"
#include "DatabaseEnv.h"
#include "json.h"

struct ToolCharaterInfo
{
	uint32 guid;
	std::string name;
	uint16 race;
	uint16 profession;
	uint16 level;
	uint8 talent;

	ToolCharaterInfo()
	{
		guid = 0;
		race = profession = level = 0;
		talent = -1;
	}
	ToolCharaterInfo(uint32 id, const std::string &na, uint16 ra, uint16 pro, uint16 lv) :
		guid(id), name(na), race(ra), profession(pro), level(lv)
	{
		talent = -1;
	}

	Json::Value SerializerInfo();
};

struct ToolAccountInfo
{
	uint32 id;
	std::string username;
	ToolCharaterInfo online;

	ToolAccountInfo() : id(0) {}
	ToolAccountInfo(uint32 uid, const char *name) : id(uid), username(name)
	{
	}

	Json::Value SerializerInfo();
};

class OnlineMgr
{
public:
	typedef std::map<uint32, ToolAccountInfo> TOOL_ACC;

private:
	OnlineMgr();
	~OnlineMgr();

public:
	OnlineMgr(OnlineMgr const&) = delete;
	OnlineMgr(OnlineMgr&&) = delete;

	OnlineMgr& operator= (OnlineMgr const&) = delete;
	OnlineMgr& operator= (OnlineMgr&&) = delete;

	static OnlineMgr* instance();

	bool AddNewAccount(uint32 guid, std::string& name);
	bool CharaterOnline(uint32 accID, uint32 charID, const std::string& charName, uint16 race, uint16 pro, uint16 lv, uint8 talent);
	bool CharaterOffline(uint32 accID);
	bool CharaterState(uint32 accID, uint32 charID, uint16 lv, uint8 talent);
	bool SetAccountSecurity(uint32 accID, uint8 security);
	std::string SerializerPlayerAccount();
	std::string SerializerBotAccount();

private:
	TOOL_ACC m_OnlinePlayerAcc;
	TOOL_ACC m_OnlineBotAcc;

public:
	static std::mutex g_uniqueMgrLock;
};

#define sOnlineMgr OnlineMgr::instance()

#endif // !__ONLINE_MGR_H__
