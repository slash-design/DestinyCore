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

#include "ToolSocket.h"
#include "BigNumber.h"
#include "Opcodes.h"
#include "SharedDefines.h"
#include "World.h"
#include "AccountMgr.h"
#include "OnlineMgr.h"
#include "ToolSocketMgr.h"
#include "CommandBG.h"
#include "Config.h"
#include "AccountMgr.h"


#include <memory>
#include <fstream>
#include <mutex>
#include <boost/algorithm/string.hpp>

using boost::asio::ip::tcp;

static std::mutex g_toolSocketLock;

ToolSocket* ToolSocket::g_Tool = NULL;

ToolSocket::ToolSocket(tcp::socket&& socket)
	: Socket(std::move(socket)), _authed(false)
{
}

ToolSocket::~ToolSocket()
{
	if (this == ToolSocket::g_Tool)
	{
		ToolSocket::g_Tool = NULL;
		TC_LOG_ERROR("ToolSocket", "Release tool socket, set g_Tool to null.");
	}
}

void ToolSocket::Start()
{
	std::string ip_address = GetRemoteIpAddress().to_string();
    LoginDatabasePreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_TOOL_IPBIND);
	stmt->setString(0, ip_address);

	PreparedQueryResult result = LoginDatabase.Query(stmt);
	if (result && (ToolSocket::g_Tool == NULL))
	{
		ToolSocket::g_Tool = this;
		_authed = true;
		LoadConfigure();
		AsyncRead();
	}
	else
	{

		DelayedCloseSocket();
		return;
	}
}

bool ToolSocket::Update()
{
	{
		std::unique_lock<std::mutex> sessionGuard(g_toolSocketLock);
		while (_bufferQueue.size())
		{
			MessageBuffer* buffer = _bufferQueue.front();
			QueuePacket(std::move(*buffer));
			_bufferQueue.pop();
			delete buffer;
		}
	}

	if (!BaseSocket::Update())
		return false;

	return true;
}

void ToolSocket::ProcessToolCmd()
{
	std::unique_lock<std::mutex> sessionGuard(g_toolSocketLock);
	while (_processCmd.size())
	{
		Json::Value& jsonCmd = _processCmd.front();
		std::string entry = jsonCmd["entry"].asString();
		if (entry == "heartbeat")
			CmdHeartbeat(jsonCmd);
		else if (entry == "authorization")
			CmdAuthorization(jsonCmd);
		else if (entry == "xp_reward")
			CmdBGXPReward(jsonCmd);
		else if (entry == "create_acc")
			CmdCreateAccount(jsonCmd);
		else if (entry == "player_acc")
			CmdPlayerAccount(jsonCmd);
		else if (entry == "set_security")
			CmdAccountSecurity(jsonCmd);
		else if (entry == "bg_model")
			CmdBGModel(jsonCmd);
		else
		{
			TC_LOG_ERROR("ToolSocket", "Can`t find tool opcode case by entry : %s.", entry.c_str());
			SendNormalResult(entry, false);
		}
		_processCmd.pop();
	}
}

void ToolSocket::OnClose()
{
}

void ToolSocket::ReadHandler()
{
	if (!IsOpen() || !_authed)
		return;

	MessageBuffer& packet = GetReadBuffer();
	while (packet.GetActiveSize() > 0)
	{
		uint16 size = 0;
		std::size_t readHeaderSize = 2;
		memcpy((void*)&size, packet.GetReadPointer(), readHeaderSize);
		packet.ReadCompleted(readHeaderSize);

		if (size > 0 && size <= 1024 && packet.GetRemainingSpace() >= size)
		{
			char* data = new char[size];
			memcpy(data, packet.GetReadPointer(), size);
			packet.ReadCompleted(size);
			ProcessCmd(data);
		}
		else if (size != 0)
		{
			_authed = false;
			DelayedCloseSocket();
			return;
		}
		else
		{
			packet.ReadCompleted(packet.GetActiveSize());
			break;
		}
	}

	AsyncRead();
}

void ToolSocket::LoadConfigure()
{
}

void ToolSocket::ProcessCmd(std::string cmdString)
{
	Json::Reader jsonReader;
	Json::Value jsonValue;
	if (!jsonReader.parse(cmdString, jsonValue))
	{
		TC_LOG_ERROR("ToolSocket", "Parse tool string error. text is %s", cmdString.c_str());
		return;
	}
	std::unique_lock<std::mutex> sessionGuard(g_toolSocketLock);
	_processCmd.push(jsonValue);
}

void ToolSocket::SendNormalResult(std::string entry, bool result)
{
	Json::Value test;
	test["entry"] = entry;
	test["result"] = result ? "success" : "error";
	SendResult(test.toStyledString());
}

void ToolSocket::SendResult(std::string result)
{
	if (result.empty() || !IsOpen())
		return;
	uint16 size = result.size() + 1;
	MessageBuffer* retMsg = new MessageBuffer(2 + size);
	retMsg->Write(&size, 2);
	retMsg->Write(result.c_str(), size);
	retMsg->WriteCompleted(2 + size);
	SendPacket(retMsg);
}

void ToolSocket::SendPacket(MessageBuffer* packet)
{
	if (!IsOpen())
		return;

	_bufferQueue.push(packet);
}

void ToolSocket::CmdHeartbeat(Json::Value& /*info*/)
{
	SendNormalResult("heartbeat", true);
}

void ToolSocket::CmdAuthorization(Json::Value& info)
{
	std::string authorization = info["authorization"].asString();
	SendNormalResult("authorization", authorization.empty());
}

void ToolSocket::CmdBGXPReward(Json::Value& info)
{
	bool can = info["reward"].asBool();
	sWorld->setBoolConfig(CONFIG_BG_XP_FOR_KILL, can);
	SendNormalResult("xp_reward", true);
}

void ToolSocket::CmdCreateAccount(Json::Value& info)
{
	std::string name = info["cmdName"].asString();
	std::string pass = info["cmdPass"].asString();
	if (name.empty() || pass.empty())
	{
		Json::Value test;
		test["entry"] = "create_acc";
		test["result"] = "error";
		SendResult(test.toStyledString());
		return;
	}

	bool createResult = sAccountMgr->CreateAccount(name, pass, "") == AccountOpResult::AOR_OK;
	SendNormalResult("create_acc", createResult);
}

void ToolSocket::CmdPlayerAccount(Json::Value& /*info*/)
{
	SendResult(sOnlineMgr->SerializerPlayerAccount());
}

void ToolSocket::CmdAccountSecurity(Json::Value& info)
{
	int accID = info["accid"].asInt();
	int secu = info["security"].asInt();
	bool succ = sOnlineMgr->SetAccountSecurity(accID, secu);
	SendNormalResult("set_security", succ);
}

void ToolSocket::CmdBGModel(Json::Value& info)
{
	uint32 model = info["model"].asInt();
	if (model < CommandModel::CM_Over)
	{
		CommandBG::SettingCommandModel(CommandModel(model));
		SendNormalResult("bg_model", true);
	}
	else
	{
		SendNormalResult("bg_model", false);
	}
}
