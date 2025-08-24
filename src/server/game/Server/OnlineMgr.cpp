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

#include "OnlineMgr.h"
#include "PlayerBotMgr.h"
#include "World.h"
#include "WorldSession.h"

#include <boost/algorithm/string.hpp>

std::mutex OnlineMgr::g_uniqueMgrLock;

Json::Value ToolCharaterInfo::SerializerInfo()
{
	Json::Value result;
	if (guid != 0)
	{
		result["guid"] = guid;
		result["name"] = name;
		result["race"] = race;
		result["profession"] = profession;
		result["level"] = level;
		result["talent"] = talent;
	}
	else
	{
		result["guid"] = 0;
	}
	return result;
}

Json::Value ToolAccountInfo::SerializerInfo()
{
	WorldSession* pSession = sWorld->FindSession(id);
	Json::Value result;
	result["id"] = id;
	result["name"] = username.c_str();
	result["security"] = (pSession) ? uint32(pSession->GetSecurity()) : 0;
	result["charater"] = online.SerializerInfo();
	return result;
}

OnlineMgr::OnlineMgr()
{
}

OnlineMgr::~OnlineMgr()
{
}

OnlineMgr* OnlineMgr::instance()
{
	static OnlineMgr instance;
	return &instance;
}

bool OnlineMgr::AddNewAccount(uint32 guid, std::string& name)
{
	if (guid == 0 || name.empty())
		return false;
	std::unique_lock<std::mutex> sessionGuard(OnlineMgr::g_uniqueMgrLock);
	std::string lowerName = boost::algorithm::to_lower_copy(name);
	bool isBotAcc = sPlayerBotMgr->IsBotAccuntName(lowerName);
	if (isBotAcc)
	{
		if (m_OnlineBotAcc.find(guid) != m_OnlineBotAcc.end())
			return false;
		m_OnlineBotAcc[guid] = ToolAccountInfo(guid, name.c_str());
	}
	else
	{
		if (m_OnlinePlayerAcc.find(guid) != m_OnlinePlayerAcc.end())
			return false;
		m_OnlinePlayerAcc[guid] = ToolAccountInfo(guid, name.c_str());
	}
	return true;
}

bool OnlineMgr::CharaterOnline(uint32 accID, uint32 charID, const std::string& charName, uint16 race, uint16 pro, uint16 lv, uint8 talent)
{
	std::unique_lock<std::mutex> sessionGuard(OnlineMgr::g_uniqueMgrLock);
	if (m_OnlinePlayerAcc.find(accID) != m_OnlinePlayerAcc.end())
	{
		ToolAccountInfo& info = m_OnlinePlayerAcc.find(accID)->second;
		if (info.online.guid != 0)
			return false;
		info.online.guid = charID;
		info.online.name = charName;
		info.online.race = race;
		info.online.profession = pro;
		info.online.level = lv;
		info.online.talent = talent;
		return true;
	}
	else if (m_OnlineBotAcc.find(accID) != m_OnlineBotAcc.end())
	{
		ToolAccountInfo& info = m_OnlineBotAcc.find(accID)->second;
		if (info.online.guid != 0)
			return false;
		info.online.guid = charID;
		info.online.name = charName;
		info.online.race = race;
		info.online.profession = pro;
		info.online.level = lv;
		info.online.talent = talent;
		return true;
	}
	return false;
}

bool OnlineMgr::CharaterOffline(uint32 accID)
{
	std::unique_lock<std::mutex> sessionGuard(OnlineMgr::g_uniqueMgrLock);
	if (m_OnlinePlayerAcc.find(accID) != m_OnlinePlayerAcc.end())
	{
		ToolAccountInfo& info = m_OnlinePlayerAcc.find(accID)->second;
		if (info.online.guid == 0)
			return false;
		info.online.guid = 0;
		info.online.name.clear();
		info.online.race = 0;
		info.online.profession = 0;
		info.online.level = 0;
		info.online.talent = -1;
		return true;
	}
	else if (m_OnlineBotAcc.find(accID) != m_OnlineBotAcc.end())
	{
		ToolAccountInfo& info = m_OnlineBotAcc.find(accID)->second;
		if (info.online.guid == 0)
			return false;
		info.online.guid = 0;
		info.online.name.clear();
		info.online.race = 0;
		info.online.profession = 0;
		info.online.level = 0;
		info.online.talent = -1;
		return true;
	}
	return false;
}

bool OnlineMgr::CharaterState(uint32 accID, uint32 charID, uint16 lv, uint8 talent)
{
	std::unique_lock<std::mutex> sessionGuard(OnlineMgr::g_uniqueMgrLock);
	if (m_OnlinePlayerAcc.find(accID) != m_OnlinePlayerAcc.end())
	{
		ToolAccountInfo& info = m_OnlinePlayerAcc.find(accID)->second;
		if (info.online.guid == 0)
			return false;
		info.online.level = lv;
		info.online.talent = talent;
		return true;
	}
	else if (m_OnlineBotAcc.find(accID) != m_OnlineBotAcc.end())
	{
		ToolAccountInfo& info = m_OnlineBotAcc.find(accID)->second;
		if (info.online.guid == 0)
			return false;
		info.online.level = lv;
		info.online.talent = talent;
		return true;
	}
	return false;
}

bool OnlineMgr::SetAccountSecurity(uint32 accID, uint8 security)
{
	if (accID==0 || security > 4)
		return false;
	std::unique_lock<std::mutex> sessionGuard(OnlineMgr::g_uniqueMgrLock);
	WorldSession* pSession = sWorld->FindSession(accID);
	if (pSession)
	{
		if (pSession->GetSecurity() == AccountTypes(security))
			return true;
		pSession->SetSecurity(AccountTypes(security));
	}
	char sqlText[128] = { 0 };
	sprintf(sqlText, "SELECT id FROM account_access WHERE id=%d", accID);
	QueryResult result = LoginDatabase.Query(sqlText);
	if (result)
	{
		sprintf(sqlText, "UPDATE account_access SET gmlevel=%d WHERE id=%d", security, accID);
		LoginDatabase.Query(sqlText);
	}
	else
	{
		sprintf(sqlText, "INSERT INTO account_access (id, gmlevel, RealmID) VALUES (%d, %d, -1)", accID, security);
		LoginDatabase.Query(sqlText);
	}
	return true;
}

std::string OnlineMgr::SerializerPlayerAccount()
{
	std::unique_lock<std::mutex> sessionGuard(OnlineMgr::g_uniqueMgrLock);
	Json::Value data;
	data["entry"] = "player_acc";
	int count = 100;
	for (TOOL_ACC::iterator itInfo = m_OnlinePlayerAcc.begin();
		itInfo != m_OnlinePlayerAcc.end();
		itInfo++)
	{
		--count;
		if (count <= 0)
			break;
		ToolAccountInfo& info = itInfo->second;
		data["accounts"].append(info.SerializerInfo());
	}
	return data.toStyledString();
}

std::string OnlineMgr::SerializerBotAccount()
{
	std::unique_lock<std::mutex> sessionGuard(OnlineMgr::g_uniqueMgrLock);
	Json::Value data;
	data["entry"] = "bot_acc";
	int count = 100;
	for (TOOL_ACC::iterator itInfo = m_OnlineBotAcc.begin();
		itInfo != m_OnlineBotAcc.end();
		itInfo++)
	{
		--count;
		if (count <= 0)
			break;
		ToolAccountInfo& info = itInfo->second;
		data["accounts"].append(info.SerializerInfo());
	}
	return data.toStyledString();
}
