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

#include "CustomTalkMenu.h"
#include "WorldSession.h"
#include "BotAITool.h"
#include "GossipDef.h"
#include "SpellAuras.h"
#include "Group.h"
#include "PlayerBotMgr.h"
#include "FieldBotMgr.h"

CustomTalkMenu::~CustomTalkMenu()
{
	ClearMenu();
}

CustomTalkMenu* CustomTalkMenu::instance()
{
	static CustomTalkMenu instance;
	return &instance;
}

void CustomTalkMenu::ClearMenu()
{
	for (PlayerMenuStates::iterator itMenu = m_PlayerDisplayItems.begin();
		itMenu != m_PlayerDisplayItems.end();
		itMenu++)
	{
		delete (itMenu->second);
	}
	m_PlayerDisplayItems.clear();

	for (CustomTalkMenuItems::iterator itMenu = m_MenuItems.begin();
		itMenu != m_MenuItems.end();
		itMenu++)
	{
		delete (itMenu->second);
	}
	m_MenuItems.clear();
}

void CustomTalkMenu::DisplayMenuToPlayer(Player* player, PlayerMenuState* pState)
{
	if (!pState)
		return;
	GossipMenu& menu = player->PlayerTalkClass->GetGossipMenu();
	menu.ClearMenu();
	if (pState->items.size() <= 32)
	{
		for (CustomItem* pItem : pState->items)
		{
			if (!pItem)
				continue;
			uint8 icon = GossipOptionIcon::GOSSIP_ICON_TRAINER;
			if (pItem->entry == 0)
				icon = GossipOptionIcon::GOSSIP_ICON_TABARD;
			else if (pItem->func == 0)
				icon = GossipOptionIcon::GOSSIP_ICON_CHAT;
			//if (pItem->price > 0)
			//{
			//	std::string topText;
			//	consoleToUtf8(std::string(" 需要荣誉 "), topText);
			//	std::string outputText = pItem->name + "|cffff0000 Need Honor |r";
			//	menu.AddMenuItem(-1, icon, outputText, 0, 0, "", 0);
			//}
			//else
				menu.AddMenuItem(-1, icon, pItem->name, 0, 0, "", 0);
		}
	}
	player->PlayerTalkClass->SendGossipMenu(1, player->GetGUID());
}

PlayerMenuState* CustomTalkMenu::GetPlayerMenu(Player* player, bool reset)
{
	if (!player)
		return NULL;
	ObjectGuid guid = player->GetGUID();
	PlayerMenuStates::iterator itStates = m_PlayerDisplayItems.find(guid);
	if (itStates != m_PlayerDisplayItems.end())
	{
		if (reset)
			itStates->second->items.clear();
		return itStates->second;
	}
	PlayerMenuState* pStates = new PlayerMenuState(guid);
	m_PlayerDisplayItems[guid] = pStates;
	return pStates;
}

uint32 CustomTalkMenu::SearchMenuItemByType(PlayerMenuState* pState, Player* player, uint32 type)
{
	if (!pState || !player)
		return 0;
	uint32 count = 0;
	for (CustomTalkMenuItems::iterator itMenu = m_MenuItems.begin();
		itMenu != m_MenuItems.end();
		itMenu++)
	{
		CustomItem* pItem = itMenu->second;
		if (pItem->menu != type)
			continue;
		uint32 mask = player->getRaceMask();
		if ((pItem->mask & mask) == 0)
			continue;
		if (pItem->level > player->getLevel())
			continue;
		pState->items.push_back(pItem);
		++count;
		if (count >= 31)
			break;
	}

	if (count < 31)
	{
		for (CustomTalkMenuItems::iterator itMenu = m_ArenaItems.begin();
			itMenu != m_ArenaItems.end();
			itMenu++)
		{
			CustomItem* pItem = itMenu->second;
			if (pItem->menu != type)
				continue;
			uint32 mask = player->getRaceMask();
			if ((pItem->mask & mask) == 0)
				continue;
			if (pItem->level > player->getLevel())
				continue;
			pState->items.push_back(pItem);
			++count;
			if (count >= 31)
				break;
		}
	}

	return count;
}

CustomItem* CustomTalkMenu::FindMenuItem(uint32 entry)
{
	CustomTalkMenuItems::iterator itMenu = m_MenuItems.find(entry);
	if (itMenu == m_MenuItems.end())
	{
		itMenu = m_ArenaItems.find(entry);
		if (itMenu != m_MenuItems.end())
			return itMenu->second;
		return NULL;
	}
	return itMenu->second;
}

void CustomTalkMenu::Initialize()
{
	uint32 oldMSTime = getMSTime();

	QueryResult result = WorldDatabase.Query("SELECT entry,menu,func,mask,level,name,param1,param2,param3,param4,param5,price FROM menu");
	if (!result)
	{
		TC_LOG_INFO("server.loading", ">> CustomTalkMenu Find 0 item!");
		return;
	}

	ClearMenu();
	//std::string topText;
	//consoleToUtf8(std::string("--<超级功能>--"), topText);
	//m_MenuItems[0] = new CustomItem(0, -1, -1, 0, 0, topText, 0, 0, 0, 0, 0, 0);
	do
	{
		Field* fields = result->Fetch();

		uint32 entry = fields[0].GetUInt32();
		uint32 menu = fields[1].GetUInt32();
		uint32 func = fields[2].GetUInt32();
		uint32 mask = fields[3].GetUInt32();
		uint32 level = fields[4].GetUInt32();
		std::string name = fields[5].GetString();
		float param1 = fields[6].GetFloat();
		float param2 = fields[7].GetFloat();
		float param3 = fields[8].GetFloat();
		float param4 = fields[9].GetFloat();
		float param5 = fields[10].GetFloat();
		uint32 price = fields[11].GetUInt32();
		//if (entry >= 224 && entry <= 227)
		//	continue;
		CustomItem* item = new CustomItem(entry, menu, func, mask, level, name, param1, param2, param3, param4, param5, price);
		m_MenuItems[entry] = item;
	} while (result->NextRow());

	TC_LOG_INFO("server.loading", ">> Loaded %u Talk menu info in %u ms", m_MenuItems.size(), GetMSTimeDiffToNow(oldMSTime));
}

void CustomTalkMenu::DisplayMainMenu(Player* player)
{
	if (!player)
		return;
	player->PlayerTalkClass->SendCloseGossip();
	PlayerMenuState* pMenu = GetPlayerMenu(player, true);
	pMenu->items.push_back(m_MenuItems[0]);
	uint32 count = SearchMenuItemByType(pMenu, player, 0);
	//player->SetStandState(UNIT_STAND_STATE_STAND);
	DisplayMenuToPlayer(player, pMenu);
}

void CustomTalkMenu::OnSelectMenuItem(Player* player, uint32 index)
{
	if (!player)
		return;

	PlayerMenuState* pMenu = GetPlayerMenu(player);
	if (index >= pMenu->items.size())
	{
		player->PlayerTalkClass->SendCloseGossip();
		pMenu->items.clear();
		player->Whisper(std::string("Select menu error!"), Language::LANG_COMMON, player);
		return;
	}

	CustomItem* pSelectItem = pMenu->items[index];
	if (!pSelectItem)
	{
		player->PlayerTalkClass->SendCloseGossip();
		pMenu->items.clear();
		return;
	}
	if (pSelectItem->entry == 0)
	{
		DisplayMenuToPlayer(player, pMenu);
		return;
	}

	if (pSelectItem->func == 0 && index == 0)// && pSelectItem->menu != 0
	{
		pMenu->items.clear();
		if (pSelectItem->menu != 0)
		{
			CustomItem* pPopItem = FindMenuItem(pSelectItem->menu);
			if (pPopItem)
				pMenu->items.push_back(pPopItem);
			else
			{
				player->PlayerTalkClass->SendCloseGossip();
				pMenu->items.clear();
				player->Whisper(std::string("Select menu error!"), Language::LANG_COMMON, player);
				return;
			}
		}
		else
			pMenu->items.push_back(m_MenuItems[0]);
		uint32 count = SearchMenuItemByType(pMenu, player, pSelectItem->menu);
		DisplayMenuToPlayer(player, pMenu);
	}
	else if (MenuFunction(pSelectItem->func) == MenuFunction::MENUF_POP)
	{
		pMenu->items.clear();
		pMenu->items.push_back(pSelectItem);
		uint32 count = SearchMenuItemByType(pMenu, player, pSelectItem->entry);
		DisplayMenuToPlayer(player, pMenu);
	}
	else
	{
		uint32 point = player->GetHonorLevel();
		if (pSelectItem->price > 0)
		{
			if (point < pSelectItem->price)
			{
				DisplayMenuToPlayer(player, pMenu);
				std::string topText;
				consoleToUtf8(std::string("荣誉点数不足！"), topText);
				player->Whisper(topText, Language::LANG_COMMON, player);
				return;
			}
		}
		bool success = false;
		switch (MenuFunction(pSelectItem->func))
		{
		case MenuFunction::MENUF_TELEPORT:
			success = ProcessTeleport(pSelectItem, player);
			player->PlayerTalkClass->SendCloseGossip();
			pMenu->items.clear();
			break;
		case MenuFunction::MENUF_NPC:
			success = ProcessSummon(pSelectItem, player);
			player->PlayerTalkClass->SendCloseGossip();
			pMenu->items.clear();
			break;
		case MenuFunction::MENUF_SPELL:
			success = ProcessSpell(pSelectItem, player);
			player->PlayerTalkClass->SendCloseGossip();
			pMenu->items.clear();
			break;
		case MenuFunction::MENUF_AURA:
			success = ProcessAura(pSelectItem, player);
			player->PlayerTalkClass->SendCloseGossip();
			pMenu->items.clear();
			break;
		case MenuFunction::MENUF_ITEM:
			success = ProcessItem(pSelectItem, player);
			DisplayMenuToPlayer(player, pMenu);
			break;
		case MenuFunction::MENUF_INTERFACE:
			success = ProcessInterface(pSelectItem, player);
			player->PlayerTalkClass->SendCloseGossip();
			pMenu->items.clear();
			break;
		case MenuFunction::MENUF_POLYMORPH:
			success = ProcessPolymorph(pSelectItem, player);
			player->PlayerTalkClass->SendCloseGossip();
			pMenu->items.clear();
			break;
		case MenuFunction::MENUF_ARENALIST:
			success = ProcessArenaList(pSelectItem, player);
			player->PlayerTalkClass->SendCloseGossip();
			pMenu->items.clear();
			break;
		default:
			success = false;
			player->PlayerTalkClass->SendCloseGossip();
			pMenu->items.clear();
			player->Whisper(std::string("Unknow menu command!"), Language::LANG_COMMON, player);
			return;
		}
		if (!success)
		{
			player->Whisper(std::string("Execute menu error!"), Language::LANG_COMMON, player);
		}
		else
		{
			if (pSelectItem->price > 0)
				player->AddHonorXP(int32(pSelectItem->price) * -1);
		}
	}
}

void CustomTalkMenu::UpdateArenaItems(std::list<CustomItem*>& arenaItems)
{
	for (PlayerMenuStates::iterator itStates = m_PlayerDisplayItems.begin(); itStates != m_PlayerDisplayItems.end(); itStates++)
	{
		if (itStates->second->items.empty())
			continue;
		itStates->second->items.clear();
        if (Player* player = ObjectAccessor::FindPlayer(itStates->first))
			player->PlayerTalkClass->SendCloseGossip();
	}
	for (CustomTalkMenuItems::iterator itMenu = m_ArenaItems.begin();
		itMenu != m_ArenaItems.end();
		itMenu++)
	{
		CustomItem* pItem = itMenu->second;
		delete pItem;
	}
	m_ArenaItems.clear();
	for (CustomItem* item : arenaItems)
	{
		if (item)
			m_ArenaItems[item->entry] = item;
	}
	arenaItems.clear();
}

bool CustomTalkMenu::ProcessTeleport(CustomItem* pMenuItem, Player* player)
{
	if (!pMenuItem || pMenuItem->func == 0 || !player || player->IsPlayerBot())
		return false;
	if (pMenuItem->param2 == 0 && pMenuItem->param3 == 0 && pMenuItem->param4 == 0)
		return false;
	if (player->IsFlying())
		return false;
	if (player->IsInCombat())
	{
		player->Whisper(std::string("Combating not used!"), Language::LANG_COMMON, player);
		return true;
	}

	player->SaveRecallPosition();
	player->TeleportTo(uint32(pMenuItem->param1), pMenuItem->param2, pMenuItem->param3, pMenuItem->param4, player->GetOrientation());
	return true;
}

bool CustomTalkMenu::ProcessSummon(CustomItem* pMenuItem, Player* player)
{
	if (!pMenuItem || pMenuItem->func == 0 || !player || player->IsPlayerBot())
		return false;
	if (pMenuItem->param1 <= 0)
		return false;
	if (player->IsFlying())
		return false;

	uint32 entry = uint32(pMenuItem->param1);
	if(player->SummonCreature(entry, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 300000) != NULL)
		return true;
	return false;
}

bool CustomTalkMenu::ProcessSpell(CustomItem* pMenuItem, Player* player)
{
	if (!pMenuItem || pMenuItem->func == 0 || !player || player->IsPlayerBot())
		return false;
	if (pMenuItem->param1 <= 0)
		return false;
	if (player->IsFlying() || player->IsMounted())
		return false;

	uint32 entry = uint32(pMenuItem->param1);
	if (!player->HasSpell(entry))
		player->LearnSpell(entry, false);
	return true;
}

bool CustomTalkMenu::ProcessAura(CustomItem* pMenuItem, Player* player)
{
	if (!pMenuItem || pMenuItem->func == 0 || !player || player->IsPlayerBot())
		return false;
	if (pMenuItem->param1 <= 0)
		return false;
	if (player->IsFlying() || player->IsMounted())
		return false;

	uint32 spellId = uint32(pMenuItem->param1);
	bool allGroup = (pMenuItem->param2 == 0) ? false : true;
	if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId))
	{
		Group* pGroup = player->GetGroup();
		if (allGroup && pGroup)
		{
			Group::MemberSlotList const& memList = pGroup->GetMemberSlots();
			for (Group::MemberSlot const& slot : memList)
			{
				Player* pGroupPlayer = ObjectAccessor::FindPlayer(slot.guid);
				if (!pGroupPlayer || !pGroupPlayer->IsAlive() || player->GetMap() != pGroupPlayer->GetMap())
					continue;
				Aura::TryRefreshStackOrCreate(spellInfo, player->GetGUID(), MAX_EFFECT_MASK, pGroupPlayer, pGroupPlayer);
			}
		}
		else
			Aura::TryRefreshStackOrCreate(spellInfo, player->GetGUID(), MAX_EFFECT_MASK, player, player);
	}
	//player->CastSpell(player, spellId, true);
	return true;
}

bool CustomTalkMenu::ProcessItem(CustomItem* pMenuItem, Player* player)
{
	if (!pMenuItem || pMenuItem->func == 0 || !player || player->IsPlayerBot())
		return false;
	if (pMenuItem->param1 <= 0)
		return false;
	if (player->IsFlying())
		return false;

	uint32 entry = uint32(pMenuItem->param1);
	uint32 count = (pMenuItem->param2 <= 0) ? 1 : uint32(pMenuItem->param2);
	if (BotUtility::StoreNewItemByEntry(player, entry, count))
		return true;
	return false;
}

bool CustomTalkMenu::ProcessInterface(CustomItem* pMenuItem, Player* player)
{
	if (!pMenuItem || pMenuItem->func == 0 || !player || player->IsPlayerBot())
		return false;
	if (pMenuItem->param1 <= 0)
		return false;
	if (player->IsFlying())
		return false;
	uint32 type = uint32(pMenuItem->param1);
	switch (type)
	{
	case 1:
	{
		//WorldPacket opcode(1);
        //opcode << uint64(player->GetGUID().GetRawValue());
        //player->GetSession()->HandleGetMailList(opcode);
		player->GetSession()->SendShowMailBox(player->GetGUID());
	}
		break;
	case 2:
		player->GetSession()->SendShowBank(player->GetGUID());
		break;
	case 3:
		player->GetSession()->SendAuctionHello(player->GetGUID(), NULL);
		break;
	case 4:
		player->DurabilityRepairAll(true, 1.0f, false);
		break;
	default:
		return false;
	}
	return true;
}

bool CustomTalkMenu::ProcessPolymorph(CustomItem* pMenuItem, Player* player)
{
	if (!pMenuItem || pMenuItem->func == 0 || !player || player->IsPlayerBot())
		return false;
	if (player->IsFlying())
		return false;
	uint32 type = uint32(pMenuItem->param1);
	float size = pMenuItem->param2;
	if (size <= 0)
		size = 1.0f;
	if (type == 0)
	{
		player->DeMorph();
		player->SetObjectScale(1.0f);
	}
	else
	{
		player->DeMorph();
		player->SetObjectScale(1.0f);
		player->SetDisplayId(type);
		player->SetObjectScale(size);
	}
	return true;
}

bool CustomTalkMenu::ProcessArenaList(CustomItem* pMenuItem, Player* player)
{
	if (!pMenuItem || pMenuItem->func == 0 || !player || player->IsPlayerBot() || !player->GetSession())
		return false;
	if (player->IsFlying())
		return false;
	uint32 type = uint32(pMenuItem->param1);
	if (type == 0)
		return true;
	TeamId team = (pMenuItem->param2 != 0) ? TeamId::TEAM_ALLIANCE : TeamId::TEAM_HORDE;
	if (player->GetTeamId() == team)
		return false;
	/*if (sFieldBotMgr->ExistWarfare() || !sArenaTeamMgr->CanJoinRatedArenaQueue(player, pMenuItem->param3))
	{
		std::string allonlineText;
		consoleToUtf8(std::string("|cffff8800当前状态无法开始竞技场。|r"), allonlineText);
		sWorld->SendGlobalText(allonlineText.c_str(), NULL);
	}
	else if (sPlayerBotMgr->CanReadyArenaByArenaTeamID(type))
	{
		uint8 arenaSlot = 3;
		if (pMenuItem->param3 == 2)
			arenaSlot = 0;
		else if (pMenuItem->param3 == 3)
			arenaSlot = 1;
		else if (pMenuItem->param3 == 5)
			arenaSlot = 2;
		if (arenaSlot < 3)
		{
			WorldPacket opcode(1);
			opcode << player->GetGUID();
			opcode << arenaSlot;
			opcode << uint8(1);
			opcode << uint8(1);
			player->GetSession()->HandleBattlemasterJoinArena(opcode);
			sArenaTeamMgr->SetNextPriorArenaTeam(team, pMenuItem->param3, type);
		}
	}
	else
	{
		std::string allonlineText;
		consoleToUtf8(std::string("|cffff8800选定的竞技场战队有成员当前正忙，无法挑战。|r"), allonlineText);
		sWorld->SendGlobalText(allonlineText.c_str(), NULL);
	}*/
	return true;
}
