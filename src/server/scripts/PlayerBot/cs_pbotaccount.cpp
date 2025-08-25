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

#include "PlayerBotMgr.h"
#include "Chat.h"
#include "ScriptMgr.h"
#include "CustomTalkMenu.h"
#include <World.h>
#include "RBAC.h"

static bool HandlePBotLogoutAllCommand(ChatHandler* handler, const char* args)
{
	sPlayerBotMgr->AllPlayerBotLogout();

	return true;
}

static bool HandlePBotLoginClassesCommand(ChatHandler* handler, const char* args)
{
	if (!*args)
		return false;
	char* clsStr = strtok((char*)args, " ");
	char* numStr = strtok(NULL, " ");
	if (!clsStr)
		return false;
	uint32 prof = (uint32)atoi(clsStr);
	uint32 count = numStr ? (uint32)atoi(numStr) : 1;

	sPlayerBotMgr->AddNewPlayerBotByClass(count, (Classes)prof);

	return true;
}

static bool HandlePBotLoginAllCommand(ChatHandler* handler, const char* args)
{
	sPlayerBotMgr->AllPlayerBotRandomLogin();

	return true;
}

static bool HandlePBotCreateCommand(ChatHandler* handler, const char* args)
{
	sPlayerBotMgr->SupplementPlayerBot();

	return true;
}

static bool HandlePBotResetLVCommand(ChatHandler* handler, const char* args)
{
	if (!*args)
		return false;
	char* lvStr = strtok((char*)args, " ");
	char* lmNumStr = strtok(NULL, " ");
	char* blNumStr = strtok(NULL, " ");
	if (!lvStr)
		return false;

	uint32 level = (uint32)atoi(lvStr);
	uint32 lmNum = lmNumStr ? (uint32)atoi(lmNumStr) : 300;
	uint32 blNum = blNumStr ? (uint32)atoi(blNumStr) : 300;
	if (level <= 0)
		return false;
	if (level > 80) level = 80;
	const SessionMap sessions = sWorld->GetAllSessions();
	for (SessionMap::const_iterator itSession = sessions.begin();
		itSession != sessions.end();
		itSession++)
	{
		const WorldSession* sess = itSession->second;
		//if (!sess->IsBotSession())
		//	continue;
		Player* player = (Player*)sess->GetPlayer();
		if (player && (uint32)player->getLevel() != level)
		{
			bool isOK = false;
			if (player->GetTeamId() == TeamId::TEAM_ALLIANCE && lmNum > 0)
			{
				isOK = player->ResetPlayerToLevel(level);
				if (isOK)
					--lmNum;
			}
			else if (player->GetTeamId() == TeamId::TEAM_HORDE && blNum > 0)
			{
				isOK = player->ResetPlayerToLevel(level);
				if (isOK)
					--blNum;
			}
		}
		if (lmNum <= 0 && blNum <= 0)
			break;
	}
	return true;
}

static bool HandlePBotLoginAccountCommand(ChatHandler* handler, const char* args)
{
	if (!*args)
		return false;
	char* minAccStr = strtok((char*)args, " ");
	char* maxAccStr = strtok(NULL, " ");
	char* accIndexStr = strtok(NULL, " ");
	if (!minAccStr || !maxAccStr || !accIndexStr)
		return false;
	uint32 minAcc = (uint32)atoi(minAccStr);
	uint32 maxAcc = (uint32)atoi(maxAccStr);
	if (maxAcc < minAcc)
		maxAcc = minAcc;
	uint32 accIndex = (uint32)atoi(accIndexStr);
	sPlayerBotMgr->AllPlayerBotLogout();
	for (uint32 acc = minAcc; acc <= maxAcc; acc++)
	{
		if (!sPlayerBotMgr->LoginBotByAccountIndex(acc, accIndex))
		{
			char text[128] = { 0 };
			snprintf(text, 127, "Login bot error. acc %d, index %d.", acc, accIndex);
			sWorld->SendGlobalText(text, NULL);
		}
	}
	return true;
}

class pbotaccount_commandscript : public CommandScript
{
public:
	pbotaccount_commandscript() : CommandScript("pbotaccount_commandscript") { }

	std::vector<ChatCommand> GetCommands() const override
	{
		static std::vector<ChatCommand> pbotaccCommandTable =
		{
			{ "resetlevel", rbac::RBAC_PREM_COMMAND_PBOTACCOUNT_RESETLEVEL, true, &HandlePBotResetLVCommand, "" },
			{ "logoutall", rbac::RBAC_PREM_COMMAND_PBOTACCOUNT_LOGOUTALL, true, &HandlePBotLogoutAllCommand, "" },
			{ "loginall", rbac::RBAC_PREM_COMMAND_PBOTACCOUNT_LOGINALL, true, &HandlePBotLoginAllCommand, "" },
			{ "botcreate", rbac::RBAC_PREM_COMMAND_PBOTACCOUNT_BOTCREATE, true, &HandlePBotCreateCommand, "" },
			{ "loginclasses", rbac::RBAC_PREM_COMMAND_PBOTACCOUNT_LOGINCLASSES, true, &HandlePBotLoginClassesCommand, "" },
			{ "loginacc", rbac::RBAC_PREM_COMMAND_PBOTACCOUNT_LOGINCLASSES, true, &HandlePBotLoginAccountCommand, "" },
		};

		static std::vector<ChatCommand> commandTable =
		{
			{ "pbotacc", rbac::RBAC_PREM_COMMAND_PBOTACCOUNT, false, NULL, "", pbotaccCommandTable },
			{ "resetgossip", rbac::RBAC_PREM_COMMAND_RESETCUSTOMGOSSIP, true, &HandlePBotResetGossipCommand, "" },
			{ "displaymodel", rbac::RBAC_PREM_COMMAND_RESETCUSTOMGOSSIP, true, &HandlePBotDisplayModelCommand, "" },
		};

		return commandTable;
	}

	static bool HandlePBotResetGossipCommand(ChatHandler* handler, const char* args)
	{
		sCustomTalkMenu->Initialize();
		Player* player = handler->GetSession()->GetPlayer();
		player->Whisper(std::string("Success reset gossip menu!"), Language::LANG_COMMON, player);
		return true;
	}

	static bool HandlePBotDisplayModelCommand(ChatHandler* handler, const char* args)
	{
		char* idStr = strtok((char*)args, " ");
		char* sizeStr = strtok(NULL, " ");
		if (!idStr)
			return false;
		uint32 id = (uint32)atoi(idStr);
		float size = sizeStr ? (float)atof(sizeStr) : 1.0f;
		Player* player = handler->GetSession()->GetPlayer();
		if (id == 0)
		{
			player->DeMorph();
			player->SetObjectScale(1.0f);
		}
		else
		{
			player->SetDisplayId(id);
			player->SetObjectScale(size);
		}
		return true;
	}
};

void AddSC_pbotaccount_commandscript()
{
	new pbotaccount_commandscript();
}
