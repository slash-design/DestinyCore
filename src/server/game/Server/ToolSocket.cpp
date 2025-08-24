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
#include "PlayerBotMgr.h"
#include "FieldBotMgr.h"
#include "OnlineMgr.h"
#include "ToolSocketMgr.h"
#include "CommandBG.h"
#include "Config.h"
#include "AccountMgr.h"


#include <memory>
#include <fstream>
#include <boost/algorithm/string.hpp>

using boost::asio::ip::tcp;

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
		std::unique_lock<std::mutex> sessionGuard(PlayerBotMgr::g_uniqueLock);
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
	std::unique_lock<std::mutex> sessionGuard(PlayerBotMgr::g_uniqueLock);
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
		else if (entry == "bg_scorerate")
			CmdBGScoreRate(jsonCmd);
		else if (entry == "create_acc")
			CmdCreateAccount(jsonCmd);
		else if (entry == "player_acc")
			CmdPlayerAccount(jsonCmd);
		else if (entry == "bot_acc")
			CmdBotAccount(jsonCmd);
		else if (entry == "set_security")
			CmdAccountSecurity(jsonCmd);
		else if (entry == "bot_offline")
			CmdOffline(jsonCmd);
		else if (entry == "bot_offline_all")
			CmdOfflineAll(jsonCmd);
		else if (entry == "bot_leave_all")
			CmdLeaveBGAll(jsonCmd);
		else if (entry == "bot_online")
			CmdBotOnline(jsonCmd);
		else if (entry == "bot_change")
			CmdBotChange(jsonCmd);
		else if (entry == "player_change")
			CmdPlayerChange(jsonCmd);
		else if (entry == "bg_model")
			CmdBGModel(jsonCmd);
		else if (entry == "pve_autosetting")
			CmdPVEAutoSetting(jsonCmd);
		else if (entry == "pve_maxlevel")
			CmdPVEMaxLevel(jsonCmd);
		else if (entry == "pve_maxdungeon")
			CmdPVEMaxDungeon(jsonCmd);
		else if (entry == "pve_driving")
			CmdPVEDriving(jsonCmd);
		else if (entry == "pve_pull")
			CmdPVEPull(jsonCmd);
		else if (entry == "pve_addion")
			CmdPVEAddion(jsonCmd);
		else if (entry == "pve_revive")
			CmdPVEAutoRevive(jsonCmd);
		else if (entry == "pve_field_creature")
			CmdPVEFieldCreature(jsonCmd);
		else if (entry == "pve_field_driving")
			CmdPVEFieldDriving(jsonCmd);
		else if (entry == "pve_field_warfare")
			CmdPVEFieldWarfare(jsonCmd);
		else if (entry == "pvp_diminishing")
			CmdPVPDiminishing(jsonCmd);
		else if (entry == "pvp_canbreak_controll")
			CmdPVPCanBreakControll(jsonCmd);
		else if (entry == "auto_build_arena")
			CmdAutoBuildArenaTeam(jsonCmd);
		else if (entry == "arena_reset")
			CmdArenaTeamReset(jsonCmd);
		else if (entry == "build_arena")
			CmdBuildBotsArenaTeam(jsonCmd);
		else if (entry == "down_botarena")
			CmdDownBotArenaTeam(jsonCmd);
		else if (entry == "arena_ishell")
			CmdBotArenaHell(jsonCmd);
		else if (entry == "arenateam_tactics")
			CmdBotArenaTeamTactics(jsonCmd);
		else if (entry == "disable_dkquest")
			CmdDisableDKQuest(jsonCmd);
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
#ifdef INCOMPLETE_BOT
	return;
#endif
/*	std::fstream _file;
	_file.open("pve.cfg", std::ios::in);
	if (!_file)
		return;
	FILE* pFile = fopen("pve.cfg", "r");
	if (!pFile)
		return;
	fseek(pFile, 0, SEEK_END);
	int size = ftell(pFile);
	if (size <= 0)
	{
		fclose(pFile);
		return;
	}
	char* infos = new char[size + 1];
	memset(infos, 0, size + 1);
	fseek(pFile, 0, SEEK_SET);
	fread(infos, size, 1, pFile);
	fclose(pFile);

	std::string cfgString(infos);
*/
	Json::Reader jsonReader;
	Json::Value jsonValue;
	/*if (!jsonReader.parse(cfgString, jsonValue))
	{
		TC_LOG_ERROR("ToolSocket", "Parse configure string error. text is %s", cfgString.c_str());
		return;
	}*/
	TC_LOG_ERROR("ToolSocket", "load Gtools\n");
	Json::Value jsonScoreRate = sConfigMgr->GetFloatDefault("bgscorerate", 1.0f);

		float bgScoreReate =  sConfigMgr->GetFloatDefault("bgscorerate", 1.0f);
		if (bgScoreReate < 0.2f)
			bgScoreReate = 0.2f;
		if (bgScoreReate > 8.0f)
			bgScoreReate = 8.0f;
		BotUtility::BattlegroundScoreRate = bgScoreReate;

       Json::Value jsonAutoSetting = sConfigMgr->GetIntDefault("auto_setting", 1);
		int autoSetting =  sConfigMgr->GetIntDefault("auto_setting", 1);
		BotUtility::BotCanSettingToMaster = (autoSetting != 0) ? true : false;

Json::Value jsonMaxLevel = sConfigMgr->GetIntDefault("max_level", 6);
		int maxLevel = sConfigMgr->GetIntDefault("max_level", 6);
		if (maxLevel >= 0 && maxLevel < 7)
		{
			uint32 realLevel = 110;
			switch (maxLevel)
			{
			case 0:
				realLevel = 45;
				break;
			case 1:
				realLevel = 60;
				break;
			case 2:
				realLevel = 70;
				break;
			case 3:
				realLevel = 80;
				break;
			case 4:
				realLevel = 90;
				break;
			case 5:
				realLevel = 100;
				break;
			case 6:
				realLevel = 110;
				break;
			}
			sWorld->setIntConfig(CONFIG_MAX_PLAYER_LEVEL, realLevel);
		}

Json::Value jsonMaxDungeon = sConfigMgr->GetIntDefault("maxdungeon", 0);
		int maxDungeon = sConfigMgr->GetIntDefault("maxdungeon", 0);
		BotGroupAI::PVE_MAX_DUNGEON = (maxDungeon != 0) ? true : false;
Json::Value jsonDriving = sConfigMgr->GetIntDefault("driving", 1);
		int driving = sConfigMgr->GetIntDefault("driving", 1);
		BotGroupAI::PVE_DRIVING = (driving != 0) ? true : false;

Json::Value jsonPull = sConfigMgr->GetIntDefault("pull", 1);
		int pull = sConfigMgr->GetIntDefault("pull", 1);
		BotGroupAI::PVE_PULL = (pull != 0) ? true : false;

Json::Value jsonAddion = sConfigMgr->GetFloatDefault("addion", 1.0f);
		float modifyAddion = sConfigMgr->GetFloatDefault("addion", 1.0f);
		if (modifyAddion < 0.5f)
			modifyAddion = 0.5f;
		if (modifyAddion > 15.0f)
			modifyAddion = 15.0f;
		BotUtility::DungeonBotDamageModify = modifyAddion;

Json::Value jsonEndure = sConfigMgr->GetFloatDefault("endure", 1.0f);
		modifyAddion = sConfigMgr->GetFloatDefault("endure", 1.0f);
		if (modifyAddion < 0.5f)
			modifyAddion = 0.5f;
		if (modifyAddion > 15.0f)
			modifyAddion = 15.0f;
		BotUtility::DungeonBotEndureModify = modifyAddion;

Json::Value jsonRevive = sConfigMgr->GetIntDefault("auto_revive", 0);
		int revive  = sConfigMgr->GetIntDefault("auto_revive", 0);
		BotUtility::BotCanForceRevive = (revive != 0) ? true : false;

Json::Value jsonFieldCreature = sConfigMgr->GetIntDefault("field_creature", 1);	
		int fCreature = sConfigMgr->GetIntDefault("field_creature", 1);
		FieldBotMgr::FIELDBOT_CREATURE = (fCreature != 0) ? true : false;

Json::Value jsonFieldDriving = sConfigMgr->GetIntDefault("field_driving", 0);
		int fDriving  = sConfigMgr->GetIntDefault("field_driving", 0);
		FieldBotMgr::FIELDBOT_DRIVING = (fDriving != 0) ? true : false;

Json::Value jsonWarfareSize = sConfigMgr->GetIntDefault("warfare_size", 0);
		int warfareSize  = sConfigMgr->GetIntDefault("warfare_size", 0);
		if (warfareSize >= 0 && warfareSize <= 3)
			FieldBotMgr::FIELDWARFARE_SIZE = warfareSize;

Json::Value jsonDiminishing = sConfigMgr->GetIntDefault("diminishing", 1);
		BotUtility::ControllSpellDiminishing = (sConfigMgr->GetIntDefault("diminishing", 1) != 0) ? true : false;


Json::Value jsonCanBreakControll = sConfigMgr->GetIntDefault("canbreak_controll", 1);
		BotUtility::ControllSpellFromDmgBreak = (sConfigMgr->GetIntDefault("canbreak_controll", 1) != 0) ? true : false;

//Json::Value jsonAutoBuildArena = sConfigMgr->GetIntDefault("auto_buildarena", 1);
//		ArenaTeamMgr::g_AutoBuildArenaTeam = (sConfigMgr->GetIntDefault("auto_buildarena", 1) != 0) ? true : false;

Json::Value jsonDownBotArena =sConfigMgr->GetIntDefault("downbotarena", 1);
		BotUtility::DownBotArenaTeam = (sConfigMgr->GetIntDefault("downbotarena", 1) != 0) ? true : false;

Json::Value jsonArenaIsHell = sConfigMgr->GetIntDefault("arenahell", 0);
		BotUtility::ArenaIsHell = (sConfigMgr->GetIntDefault("arenahell", 0) != 0) ? true : false;

Json::Value jsonArenaTeamTactics = sConfigMgr->GetIntDefault("bottactics", 1);
        uint32 tactics = sConfigMgr->GetIntDefault("bottactics", 1);
		if (tactics < 3)
			BotUtility::BotArenaTeamTactics = tactics;

Json::Value dkquest = sConfigMgr->GetIntDefault("dkquest", 0);
		BotUtility::DisableDKQuest = (sConfigMgr->GetIntDefault("dkquest", 0) != 0) ? true : false;

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
	std::unique_lock<std::mutex> sessionGuard(PlayerBotMgr::g_uniqueLock);
	_processCmd.push(jsonValue);
	//std::unique_lock<std::mutex> sessionGuard(_consoleLock, std::defer_lock);
	//sessionGuard.lock();
	//sWorld->QueueCliCommand(new CliCommandHolder(this, cmdString.c_str(), &CommandPrint, &CommandFinished));
}

//void ToolSocket::CommandPrint(void* callbackArg, const char* text)
//{
//	TC_LOG_ERROR("ToolSocket", "Process command result %s", text);
//}
//
//void ToolSocket::CommandFinished(void* callbackArg, bool success)
//{
//	std::string text = success ? "success" : "error";
//	uint16 size = text.size() + 1;
//	MessageBuffer* retMsg = new MessageBuffer(2 + size);
//	retMsg->Write(&size, 2);
//	retMsg->Write(text.c_str(), size);
//	retMsg->WriteCompleted(2 + size);
//	((ToolSocket*)callbackArg)->SendPacket(retMsg);
//}

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

void ToolSocket::CmdHeartbeat(Json::Value& info)
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

void ToolSocket::CmdBGScoreRate(Json::Value& info)
{
	float rate = info["scorerate"].asDouble();
	if (rate < 0.2f)
		rate = 0.2f;
	if (rate > 8.0f)
		rate = 8.0f;
	BotUtility::BattlegroundScoreRate = rate;
	SendNormalResult("bg_scorerate", true);
}

void ToolSocket::CmdCreateAccount(Json::Value& info)
{
	std::string name = info["cmdName"].asString();
	std::string pass = info["cmdPass"].asString();
	std::string lowerName = boost::algorithm::to_lower_copy(name);
	bool isBotAcc = sPlayerBotMgr->IsBotAccuntName(lowerName);
	if (isBotAcc || name.empty() || pass.empty())
	{
		Json::Value test;
		test["entry"] = "create_acc";
		test["result"] = "error";
		SendResult(test.toStyledString());
		return;
	}

	bool createResult = false;
	createResult = sAccountMgr->CreateAccount(name, pass, "") == AccountOpResult::AOR_OK;
	if (createResult)
	{
		sPlayerBotMgr->UpdateLastAccountIndex(name);
		sPlayerBotMgr->AddNewAccountBotBaseInfo(name);
	}
	SendNormalResult("create_acc", createResult);
}

void ToolSocket::CmdPlayerAccount(Json::Value& info)
{
	SendResult(sOnlineMgr->SerializerPlayerAccount());
}

void ToolSocket::CmdBotAccount(Json::Value& info)
{
	SendResult(sOnlineMgr->SerializerBotAccount());
}

void ToolSocket::CmdAccountSecurity(Json::Value& info)
{
	int accID = info["accid"].asInt();
	int secu = info["security"].asInt();
	bool succ = sOnlineMgr->SetAccountSecurity(accID, secu);
	SendNormalResult("set_security", succ);
}

void ToolSocket::CmdOffline(Json::Value& info)
{
	int accID = info["guid"].asInt();
	bool succ = sPlayerBotMgr->PlayerBotLogout(accID);
	SendNormalResult("bot_offline", succ);
}

void ToolSocket::CmdOfflineAll(Json::Value& info)
{
	sPlayerBotMgr->AllPlayerBotLogout();
	SendNormalResult("bot_offline_all", true);
}

void ToolSocket::CmdLeaveBGAll(Json::Value& info)
{
	int accID = info["guid"].asInt();
	bool isLeave = sPlayerBotMgr->AllPlayerLeaveBG(accID);
	SendNormalResult("bot_leave_all", isLeave);
}

void ToolSocket::CmdBotOnline(Json::Value& info)
{
	uint32 faction = info["faction"].asInt();
	uint32 pro = info["pro"].asInt();
	uint32 count = info["count"].asInt();
	if (pro < 1 || pro == 6 || pro == 10 || pro > 11 || count < 1 || count > 90)
	{
		SendNormalResult("bot_online", false);
		return;
	}
	sPlayerBotMgr->AddNewPlayerBot((faction== 0 ? true : false), Classes(pro), count);
	SendNormalResult("bot_online", true);
}

void ToolSocket::CmdBotChange(Json::Value& info)
{
	uint32 guid = info["guid"].asInt();
	uint32 minlv = info["minlv"].asInt();
	uint32 maxlv = info["maxlv"].asInt();
	uint32 talent = info["talent"].asInt();
	if (minlv < 20 || minlv > 80 || maxlv < minlv || maxlv < 20 || maxlv > 80 || talent > 2)
	{
		SendNormalResult("bot_change", false);
		return;
	}
	minlv = PlayerBotSetting::CheckMaxLevel(minlv);
	maxlv = PlayerBotSetting::CheckMaxLevel(maxlv);
	if (maxlv < minlv)
		maxlv = minlv;
	bool result = sPlayerBotMgr->ChangePlayerBotSetting(guid, minlv, maxlv, talent);
	SendNormalResult("bot_change", result);
}

void ToolSocket::CmdPlayerChange(Json::Value& info)
{
	uint32 guid = info["guid"].asInt();
	uint32 minlv = info["minlv"].asInt();
	uint32 maxlv = info["maxlv"].asInt();
	uint32 talent = info["talent"].asInt();
	if (minlv < 20 || minlv > 80 || maxlv < minlv || maxlv < 20 || maxlv > 80 || talent > 2)
	{
		SendNormalResult("player_change", false);
		return;
	}
	minlv = PlayerBotSetting::CheckMaxLevel(minlv);
	WorldSession* pWorldSession = sWorld->FindSession(guid);
	if (pWorldSession && !pWorldSession->PlayerLoading())
	{
		Player* player = pWorldSession->GetPlayer();
		if (!player || player->InBattlegroundQueue() || player->InBattleground() ||
			player->GetBattleground() || player->IsInCombat())
		{
			SendNormalResult("player_change", false);
			return;
		}
		PlayerBotSession* pBotSession = dynamic_cast<PlayerBotSession*>(pWorldSession);
		if (pBotSession && pBotSession->HasSchedules())
			return;
		maxlv = PlayerBotSetting::CheckMaxLevel(maxlv);
		if (maxlv < minlv)
			maxlv = minlv;
		uint32 level = urand(minlv, maxlv);
		bool result = player->ResetPlayerToLevel(level, talent);
		SendNormalResult("player_change", result);
		return;
	}
	SendNormalResult("player_change", false);
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

void ToolSocket::CmdPVEAutoSetting(Json::Value& info)
{
	uint32 setting = info["auto_setting"].asInt();
	BotUtility::BotCanSettingToMaster = (setting != 0) ? true : false;
	SendNormalResult("pve_autosetting", true);
}

void ToolSocket::CmdPVEMaxLevel(Json::Value& info)
{
	uint32 max_level = info["max_level"].asInt();
	if (max_level > 3)
	{
		SendNormalResult("pve_maxlevel", false);
		return;
	}
	uint32 realLevel = 80;
	switch (max_level)
	{
	case 0:
		realLevel = 45;
		break;
	case 1:
		realLevel = 60;
		break;
	case 2:
		realLevel = 70;
		break;
	case 3:
		realLevel = 80;
		break;
	}
	sWorld->setIntConfig(CONFIG_MAX_PLAYER_LEVEL, realLevel);
	SendNormalResult("pve_maxlevel", true);
}

void ToolSocket::CmdPVEMaxDungeon(Json::Value& info)
{
	uint32 max = info["maxdungeon"].asInt();
	BotGroupAI::PVE_MAX_DUNGEON = (max != 0) ? true : false;
	SendNormalResult("pve_maxdungeon", true);
}

void ToolSocket::CmdPVEDriving(Json::Value& info)
{
	uint32 driving = info["driving"].asInt();
	BotGroupAI::PVE_DRIVING = (driving != 0) ? true : false;
	SendNormalResult("pve_driving", true);
}

void ToolSocket::CmdPVEPull(Json::Value& info)
{
	uint32 pull = info["pull"].asInt();
	BotGroupAI::PVE_PULL = (pull != 0) ? true : false;
	SendNormalResult("pve_pull", true);
}

void ToolSocket::CmdPVEAddion(Json::Value& info)
{
	float modifyAddion = (float)info["addion"].asDouble();
	float modifyEndure = (float)info["endure"].asDouble();
	if (modifyAddion < 0.5f)
		modifyAddion = 0.5f;
	if (modifyAddion > 15.0f)
		modifyAddion = 15.0f;
	if (modifyEndure < 0.5f)
		modifyEndure = 0.5f;
	if (modifyEndure > 15.0f)
		modifyEndure = 15.0f;
	BotUtility::DungeonBotDamageModify = modifyAddion;
	BotUtility::DungeonBotEndureModify = modifyEndure;
	SendNormalResult("pve_addion", true);
}

void ToolSocket::CmdPVEAutoRevive(Json::Value& info)
{
	uint32 revive = info["auto_revive"].asInt();
	BotUtility::BotCanForceRevive = (revive != 0) ? true : false;
	SendNormalResult("pve_revive", true);
}

void ToolSocket::CmdPVEFieldCreature(Json::Value& info)
{
	uint32 fc = info["field_creature"].asInt();
	FieldBotMgr::FIELDBOT_CREATURE = (fc != 0) ? true : false;
	SendNormalResult("pve_field_creature", true);
}

void ToolSocket::CmdPVEFieldDriving(Json::Value& info)
{
	uint32 fd = info["field_driving"].asInt();
	FieldBotMgr::FIELDBOT_DRIVING = (fd != 0) ? true : false;
	SendNormalResult("pve_field_driving", true);
}

void ToolSocket::CmdPVEFieldWarfare(Json::Value& info)
{
	uint32 warfareSize = info["field_size"].asInt();
	if (warfareSize > 3)
	{
		SendNormalResult("pve_field_warfare", false);
		return;
	}
	FieldBotMgr::FIELDWARFARE_SIZE = warfareSize;
	SendNormalResult("pve_field_warfare", true);
}

void ToolSocket::CmdPVPDiminishing(Json::Value& info)
{
	uint32 jsonDiminishing = info["diminishing"].asInt();
	BotUtility::ControllSpellDiminishing = (jsonDiminishing != 0) ? true : false;
	SendNormalResult("pvp_diminishing", true);
}

void ToolSocket::CmdPVPCanBreakControll(Json::Value& info)
{
	uint32 jsonCanBreakControll = info["canbreak_controll"].asInt();
	BotUtility::ControllSpellFromDmgBreak = (jsonCanBreakControll != 0) ? true : false;
	SendNormalResult("pvp_canbreak_controll", true);
}

void ToolSocket::CmdAutoBuildArenaTeam(Json::Value& info)
{
	//uint32 jsonAutoBuildArena = info["auto_build"].asInt();
	//ArenaTeamMgr::g_AutoBuildArenaTeam = (jsonAutoBuildArena != 0) ? true : false;
	//if (ArenaTeamMgr::g_AutoBuildArenaTeam)
	//	sArenaTeamMgr->ResetPlayerBotTeamFinish();
	//SendNormalResult("auto_build_arena", true);
}

void ToolSocket::CmdArenaTeamReset(Json::Value& info)
{
	//sArenaTeamMgr->DestoryAllArenaTeam();
	//sArenaTeamMgr->ResetPlayerBotTeamFinish();
	//SendNormalResult("arena_reset", true);
}

void ToolSocket::CmdBuildBotsArenaTeam(Json::Value& info)
{
	//std::string name = info["name"].asString();
	//TeamId team = (info["team"].asInt()) ? TeamId::TEAM_ALLIANCE : TeamId::TEAM_HORDE;
	//uint32 aaType = uint32(info["type"].asUInt());
	//uint32 cls1 = uint32(info["cls1"].asUInt());
	//uint32 cls2 = uint32(info["cls2"].asUInt());
	//uint32 cls3 = uint32(info["cls3"].asUInt());
	//uint32 cls4 = uint32(info["cls4"].asUInt());
	//uint32 cls5 = uint32(info["cls5"].asUInt());
	//uint32 tal1 = uint32(info["tal1"].asUInt());
	//uint32 tal2 = uint32(info["tal2"].asUInt());
	//uint32 tal3 = uint32(info["tal3"].asUInt());
	//uint32 tal4 = uint32(info["tal4"].asUInt());
	//uint32 tal5 = uint32(info["tal5"].asUInt());
	//bool result = sArenaTeamMgr->BuildBotsArenaTeam(name, team, aaType,
	//	cls1, cls2, cls3, cls4, cls5,
	//	tal1, tal2, tal3, tal4, tal5);
	//SendNormalResult("build_arena", result);
}

void ToolSocket::CmdDownBotArenaTeam(Json::Value& info)
{
	uint32 downBotArena = info["downbotarena"].asInt();
	BotUtility::DownBotArenaTeam = (downBotArena != 0) ? true : false;
	SendNormalResult("down_botarena", true);
}

void ToolSocket::CmdBotArenaHell(Json::Value& info)
{
	uint32 arenaHell = info["arenahell"].asInt();
	BotUtility::ArenaIsHell = (arenaHell != 0) ? true : false;
	SendNormalResult("arena_ishell", true);
}

void ToolSocket::CmdBotArenaTeamTactics(Json::Value& info)
{
	uint32 tactics = info["bottactics"].asInt();
	if (tactics >= 3)
		SendNormalResult("arenateam_tactics", false);
	BotUtility::BotArenaTeamTactics = tactics;
	SendNormalResult("arenateam_tactics", true);
}

void ToolSocket::CmdDisableDKQuest(Json::Value& info)
{
	uint32 dkquest = info["dkquest"].asInt();
	BotUtility::DisableDKQuest = (dkquest != 0) ? true : false;
	SendNormalResult("disable_dkquest", true);
}
