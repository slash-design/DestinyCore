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
	void CmdCreateAccount(Json::Value& info);
	void CmdPlayerAccount(Json::Value& info);
	void CmdAccountSecurity(Json::Value& info);
	void CmdBGModel(Json::Value& info);

private:
	std::mutex _consoleLock;
	bool _authed;
	std::queue<MessageBuffer*> _bufferQueue;
	std::queue<Json::Value> _processCmd;

public:
	static ToolSocket* g_Tool;
};

#endif // __TOOLSOCKET_H__
