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

#ifndef __TOOLSOCKET_H__
#define __TOOLSOCKET_H__

#include "Common.h"
#include "Socket.h"
#include "Util.h"
#include "WorldPacket.h"
#include "DatabaseEnv.h"
#include "json.h"
#include <chrono>
#include <boost/asio/ip/tcp.hpp>

using boost::asio::ip::tcp;

class TC_GAME_API ToolSocket : public Socket<ToolSocket>
{
	typedef Socket<ToolSocket> BaseSocket;

public:
	ToolSocket(tcp::socket&& socket);
	~ToolSocket();

	ToolSocket(ToolSocket const& right) = delete;
	ToolSocket& operator=(ToolSocket const& right) = delete;

	void Start();
	bool Update();

	void ProcessToolCmd();

protected:
	void OnClose();
	void ReadHandler();
	void LoadConfigure();
	void ProcessCmd(std::string cmdString);

	//static void CommandPrint(void* callbackArg, const char* text);
	//static void CommandFinished(void* callbackArg, bool);

private:
	void SendNormalResult(std::string entry, bool result);
	void SendResult(std::string result);
	void SendPacket(MessageBuffer* packet);

	void CmdHeartbeat(Json::Value& info);
	void CmdAuthorization(Json::Value& info);
	void CmdBGXPReward(Json::Value& info);
	void CmdBGScoreRate(Json::Value& info);
	void CmdCreateAccount(Json::Value& info);
	void CmdPlayerAccount(Json::Value& info);
	void CmdBotAccount(Json::Value& info);
	void CmdAccountSecurity(Json::Value& info);
	void CmdOffline(Json::Value& info);
	void CmdOfflineAll(Json::Value& info);
	void CmdLeaveBGAll(Json::Value& info);
	void CmdBotOnline(Json::Value& info);
	void CmdBotChange(Json::Value& info);
	void CmdPlayerChange(Json::Value& info);
	void CmdBGModel(Json::Value& info);
	void CmdPVEAutoSetting(Json::Value& info);
	void CmdPVEMaxLevel(Json::Value& info);
	void CmdPVEMaxDungeon(Json::Value& info);
	void CmdPVEDriving(Json::Value& info);
	void CmdPVEPull(Json::Value& info);
	void CmdPVEAddion(Json::Value& info);
	void CmdPVEAutoRevive(Json::Value& info);
	void CmdPVEFieldCreature(Json::Value& info);
	void CmdPVEFieldDriving(Json::Value& info);
	void CmdPVEFieldWarfare(Json::Value& info);
	void CmdPVPDiminishing(Json::Value& info);
	void CmdPVPCanBreakControll(Json::Value& info);
	void CmdAutoBuildArenaTeam(Json::Value& info);
	void CmdArenaTeamReset(Json::Value& info);
	void CmdBuildBotsArenaTeam(Json::Value& info);
	void CmdDownBotArenaTeam(Json::Value& info);
	void CmdBotArenaHell(Json::Value& info);
	void CmdBotArenaTeamTactics(Json::Value& info);
	void CmdDisableDKQuest(Json::Value& info);

private:
	std::mutex _consoleLock;
	bool _authed;
	std::queue<MessageBuffer*> _bufferQueue;
	std::queue<Json::Value> _processCmd;

public:
	static ToolSocket* g_Tool;
};

#endif // __TOOLSOCKET_H__
