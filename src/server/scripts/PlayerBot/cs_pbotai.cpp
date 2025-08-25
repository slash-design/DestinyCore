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

#include "Chat.h"
#include "ScriptMgr.h"
#include "BotMovementAI.h"
#include "BotAI.h"
#include "BotGroupAI.h"
#include "BotFieldAI.h"
#include "AIWaypointsMgr.h"
#include "Battleground.h"
#include "BattlegroundAB.h"
#include "BattlegroundAV.h"
#include "MoveSplineInit.h"
#include "Totem.h"
#include "Pet.h"
#include "Group.h"
#include "WorldSession.h"
#include "Item.h"
#include "RBAC.h"
#include <Language.h>

static bool HandlePBotAIFaceTo(ChatHandler* handler, const char* args)
{
	Player* pSelf = handler->GetSession()->GetPlayer();
	for (int i = 0; i < 4; i++)
	{
		Spell* pCurrentSpell = pSelf->GetCurrentSpell(i);
		if (pCurrentSpell)
		{
			if (pCurrentSpell->m_spellInfo)
			{
				char text[128] = { 0 };
				sprintf(text, "Casting spell %d, Target %d.", pCurrentSpell->m_spellInfo->Id, pCurrentSpell->m_targets.GetUnitTargetGUID().GetCounter());
				handler->SendSysMessage(text);
			}
		}
	}
	Player* pPlayer = handler->getSelectedPlayer();
	if (!pPlayer)
		return false;
	for (uint32 i = 1; i <= DiminishingGroup::DIMINISHING_DRAGONS_BREATH; i++)
	{
		DiminishingLevels diminiLevel = pPlayer->GetDiminishing(DiminishingGroup(i));
		if (diminiLevel <= 0)
			continue;
		char text[128] = { 0 };
		sprintf(text, "Has diminishing index %d - LV %d.", i, diminiLevel);
		handler->SendSysMessage(text);
	}
	handler->SendSysMessage("Search diminishing finish.");

	//Movement::MoveSplineInit init(pPlayer);
	//init.MoveTo(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZMinusOffset());
	//init.SetFacing(pSelf);
	//init.SetOrientationFixed(true);
	//init.Launch();
	return true;
}

static bool HandlePBotAIMoveTo(ChatHandler* handler, const char* args)
{
	Player* pSelf = handler->GetSession()->GetPlayer();
	char* desc = (char*)args;
	if (!pSelf || !desc)
		return true;
	BotMovementAI* botMoveAI = dynamic_cast<BotMovementAI*> (pSelf->GetAI());
	if (!botMoveAI)
		return true;
	int entry = atoi(desc);
	AIWaypoint* aiwp = sAIWPMgr->FindAIWaypoint(entry);
	if (aiwp)
		botMoveAI->MovementTo(aiwp->posX, aiwp->posY, aiwp->posZ);
	else
		return false;
	return true;
}

static bool HandlePBotAIMovePoints(ChatHandler* handler, const char* args)
{
	if (!*args)
		return false;
	Player* pPlayer = handler->getSelectedPlayer();
	Player* pSelf = handler->GetSession()->GetPlayer();
	if (pSelf == pPlayer || !pPlayer)
		return false;
	BotMovementAI* botMoveAI = dynamic_cast<BotMovementAI*> (pPlayer->GetAI());
	if (!botMoveAI)
		return false;
	char* pidStr = strtok((char*)args, " ");
	char* idStr = strtok(NULL, " ");
	uint32 pid = (uint32)atoi(pidStr);
	uint32 id = (uint32)atoi(idStr);
	if (pid <= 0 || id <= 0)
		return false;
	botMoveAI->MovementToPath(pSelf, pid, id);
	return true;
}

static bool HandlePBotAIAddAIWaypoint(ChatHandler* handler, const char* args)
{
	Player* pSelf = handler->GetSession()->GetPlayer();
	if (!pSelf || pSelf->GetTransport())
	{
		return false;
	}
	return sAIWPMgr->CreateAIWPCreature(pSelf);
}

static bool HandlePBotAIDelAIWaypoint(ChatHandler* handler, const char* args)
{
	Player* pSelf = handler->GetSession()->GetPlayer();
	if (!pSelf || pSelf->GetTransport())
	{
		return false;
	}
	Creature* creature = handler->getSelectedCreature();
	if (!creature)
		return false;
	return sAIWPMgr->DeleteAIWPCreature(creature);
}

static bool HandlePBotAIMovAIWaypoint(ChatHandler* handler, const char* args)
{
	Player* pSelf = handler->GetSession()->GetPlayer();
	if (!pSelf || pSelf->GetTransport())
	{
		return false;
	}
	Creature* creature = handler->getSelectedCreature();
	if (!creature)
		return false;
	return sAIWPMgr->MoveAIWPCreature(pSelf, creature);
}

static bool HandlePBotAIShowAllAIWaypoint(ChatHandler* handler, const char* args)
{
	Player* pSelf = handler->GetSession()->GetPlayer();
	if (!pSelf || pSelf->GetTransport())
	{
		return false;
	}
	return sAIWPMgr->ShowAllAIWaypointsCreature(pSelf);
}

static bool HandlePBotAIDescAIWaypoint(ChatHandler* handler, const char* args)
{
	Player* pSelf = handler->GetSession()->GetPlayer();
	if (!pSelf || pSelf->GetTransport())
	{
		return false;
	}
	Creature* creature = handler->getSelectedCreature();
	if (!creature || !args)
		return false;
	char* desc = (char*)args;

	return sAIWPMgr->AIWPCreatureDesc(creature, desc);
}

static bool HandlePBotAIEndBG(ChatHandler* handler, const char* args)
{
	Player* pSelf = handler->GetSession()->GetPlayer();
	if (!pSelf || pSelf->GetTransport())
	{
		return false;
	}
	const Battleground* bg = pSelf->GetBattleground();
	if (!bg || bg->GetStatus() == BattlegroundStatus::STATUS_WAIT_LEAVE || bg->GetStatus() == BattlegroundStatus::STATUS_WAIT_JOIN)
		return false;
	((Battleground*)bg)->EndBattleground(ALLIANCE);
	return true;
}

static bool HandlePBotAIDebugTarget(ChatHandler* handler, const char* args)
{
	Player* pPlayer = handler->getSelectedPlayer();
	Player* pSelf = handler->GetSession()->GetPlayer();
	if (pSelf == pPlayer || !pPlayer)
		return false;
	BotFieldAI* fieAI = dynamic_cast<BotFieldAI*> (pPlayer->GetAI());
	if (fieAI)
	{
		if (BotFieldAI::debugFieldAI == fieAI)
			BotFieldAI::debugFieldAI = NULL;
		else
			BotFieldAI::debugFieldAI = fieAI;
	}
	else
		BotFieldAI::debugFieldAI = NULL;

	BotGroupAI* botAI = dynamic_cast<BotGroupAI*> (pPlayer->GetAI());
	if (botAI)
	{
		if (BotGroupAI::debugGroupAI == botAI)
			BotGroupAI::debugGroupAI = NULL;
		else
			BotGroupAI::debugGroupAI = botAI;
	}
	else
		BotGroupAI::debugGroupAI = NULL;

	BotBGAI* bgAI = dynamic_cast<BotBGAI*> (pPlayer->GetAI());
	if (bgAI)
	{
		if (BotBGAI::debugBGAI == bgAI)
			BotBGAI::debugBGAI = NULL;
		else
			BotBGAI::debugBGAI = bgAI;
	}
	else
		BotBGAI::debugBGAI = NULL;
	return true;
}

static bool HandlePBotAISearch(ChatHandler* handler, const char* args)
{
	Player* pSelf = handler->GetSession()->GetPlayer();
	if (!pSelf)
		return false;
	uint32 objType = args ? (uint32)atoi(args) : 0;
	if (objType == 0)
		return false;
	std::list<Creature*> creatureResults;
	Trinity::AllWorldObjectsInRange checker(pSelf, 10);
	Trinity::CreatureListSearcher<Trinity::AllWorldObjectsInRange> searcher(pSelf, creatureResults, checker);
	//pSelf->VisitNearbyGridObject(8, searcher);

	if (creatureResults.empty())
	{
		handler->SendSysMessage("No find creature.");
	}
	else
	{
		for (Creature* pObject : creatureResults)
		{
			handler->SendSysMessage("Find self creature.");
			return true;
		}
	}

	std::list<GameObject*> goResults;
	Position pos = pSelf->GetPosition();
	Trinity::GameObjectInRangeCheck checkerGO(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), 10);
	Trinity::GameObjectListSearcher<Trinity::GameObjectInRangeCheck> searcherGO(pSelf, goResults, checkerGO);
	//pSelf->VisitNearbyGridObject(10, searcherGO);
	if (goResults.empty())
	{
		handler->SendSysMessage("No find GameObject.");
	}
	else
	{
		for (GameObject* pObject : goResults)
		{
			handler->SendSysMessage("Find self GameObject.");
			return true;
		}
	}
	return true;
}

static bool HandlePBotAITeleport(ChatHandler* handler, const char* args)
{
	Player* pPlayer = handler->getSelectedPlayer();
	Player* pSelf = handler->GetSession()->GetPlayer();
	if (pSelf == pPlayer || !pPlayer)
		return false;
	pSelf->TeleportTo(pPlayer->GetMapId(), pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation());
	return true;
}

static bool HandlePBotAIABFlag(ChatHandler* handler, const char* args)
{
	Player* pSelf = handler->GetSession()->GetPlayer();
	if (!pSelf)
		return false;
	//uint32 menuIndex = args ? (uint32)atoi(args) : 0;
	//if (menuIndex > 0)
	//{
	//	GossipMenu& menu = pSelf->PlayerTalkClass->GetGossipMenu();
	//	menu.ClearMenu();
	//	menu.AddMenuItem(-1, GossipOptionIcon::GOSSIP_ICON_TABARD, "Test menu index", 0, 0, "", 0);
	//	pSelf->PlayerTalkClass->SendGossipMenu(menuIndex, pSelf->GetGUID());
	//	return true;
	//}

	BattlegroundMap* bgMap = dynamic_cast<BattlegroundMap*>(pSelf->GetMap());
	if (!bgMap)
		return false;
	BattlegroundAB* pBGAB = dynamic_cast<BattlegroundAB*>(bgMap->GetBG());
	if (!pBGAB)
		return false;
	GameObject const* pFlag = pBGAB->GetNearGameObjectFlag(pSelf);
	if (pFlag)
	{
		SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(21651);
		if (!spellInfo)
			return false;
		SpellCastTargets targets;
		targets.SetTargetMask(TARGET_FLAG_GAMEOBJECT);
		targets.SetGOTarget((GameObject*)pFlag);
		Spell* spell = new Spell(pSelf, spellInfo, TRIGGERED_NONE, ObjectGuid::Empty, false);
		spell->prepare(&targets);
		pSelf->UpdateCriteria(CRITERIA_TYPE_USE_GAMEOBJECT, pFlag->GetEntry());
	}
	else
		return false;
	return true;
}

static bool HandlePBotAIFindGO(ChatHandler* handler, const char* args)
{
	Player* pSelf = handler->GetSession()->GetPlayer();
	if (!pSelf || !pSelf->InBattleground())
		return false;
	Battleground* pBattleground = pSelf->GetBattleground();
	if (!pBattleground)
		return false;
	BattlegroundAV* pAV = dynamic_cast<BattlegroundAV*>(pBattleground);
	if (!pAV)
		return false;
	std::list<GameObject*> results;
	Trinity::GameObjectInRangeCheck checker(pSelf->GetPositionX(), pSelf->GetPositionY(), pSelf->GetPositionZ(), 5);
	Trinity::GameObjectListSearcher<Trinity::GameObjectInRangeCheck> searcher(pSelf, results, checker);
	//pSelf->VisitNearbyGridObject(5, searcher);
	for (GameObject* pGameObject : results)
	{
		//uint32 oType = pAV->GetBGCreatureIndexByGUID(pGameObject->GetGUID());
		uint32 oType = pAV->GetObjectType(pGameObject->GetGUID());
		char text[128] = { 0 };
		sprintf(text, "AV GameObject index %d", oType);
		handler->SendSysMessage(text);
	}
	handler->SendSysMessage("AV find near GameObject end.");
	return true;
}

static bool HandlePBotAIECPInfo(ChatHandler* handler, const char* args)
{
	Player* pSelf = handler->GetSession()->GetPlayer();
	Player* pSelect = handler->getSelectedPlayer();
	if (!pSelf || !pSelect)
		return false;
	int32 selfECP = pSelf->GetEquipCombatPower();
	int32 selECP = pSelect->GetEquipCombatPower();
	int32 ecpGap = selfECP - selECP;
	float critMeleeTaken = pSelect->GetRatingBonusValue(CR_CRIT_MELEE);
	float critRangeTaken = pSelect->GetRatingBonusValue(CR_CRIT_RANGED);
	float critSpellTaken = pSelect->GetRatingBonusValue(CR_CRIT_SPELL);
	char text[128] = { 0 };
	if (ecpGap == 0)
	{
		sprintf(text, "|cff000000ECP gap %d, CTaken(%.2f, %.2f, %.2f) |r", ecpGap, critMeleeTaken, critRangeTaken, critSpellTaken);
	}
	else if (ecpGap > 0)
	{
		float pct = (float(ecpGap * 1.2f) / float(selfECP + selECP)) * 100.0f;
		sprintf(text, "|cff0000ccECP gap %d, PCT %.2f, CTaken(%.2f, %.2f, %.2f) |r", ecpGap, pct, critMeleeTaken, critRangeTaken, critSpellTaken);
	}
	else
	{
		ecpGap *= -1;
		float pct = (float(ecpGap * 1.2f) / float(selfECP + selECP)) * 100.0f;
		sprintf(text, "|cffcc0000ECP gap %d, PCT %.2f, CTaken(%.2f, %.2f, %.2f) |r", ecpGap, pct, critMeleeTaken, critRangeTaken, critSpellTaken);
	}
	handler->SendSysMessage(text);
	return true;
}

static bool HandlePBotPVPEquip(ChatHandler* handler, const char* args)
{
	Player* pSelf = handler->GetSession()->GetPlayer();
	Player* pSelect = handler->getSelectedPlayer();
	if (!pSelf || !pSelect || !pSelect->IsSettingFinish())
		return false;
	pSelect->ResetPlayerToLevel(pSelect->getLevel(), pSelect->FindTalentType(), true);
	return true;
}

static bool HandlePBotPetTalents(ChatHandler* handler, const char* args)
{
	Player* pSelect = handler->getSelectedPlayerOrSelf();
	if (!pSelect || !pSelect->IsSettingFinish())
		return false;
	Pet* pet = pSelect->GetPet();
	if (!pet)
		return false;
	//pet->ResetTalents();
	//pet->FlushTalentsByPoints();
	pet->SettingAllSpellAutocast(true);
	return true;
}

static bool HandlePBotSearchGO(ChatHandler* handler, const char* args)
{
	Player* pSelect = handler->getSelectedPlayerOrSelf();
	if (!pSelect)
		return false;
	//pSelect->GetSession()->SendAuctionHello(pSelect->GetGUID(), NULL);
	float range = args ? atof(args) : 5;
	std::list<GameObject*> results;
	Trinity::GameObjectInRangeCheck checker(pSelect->GetPositionX(), pSelect->GetPositionY(), pSelect->GetPositionZ(), range);
	Trinity::GameObjectListSearcher<Trinity::GameObjectInRangeCheck> searcher(pSelect, results, checker);
	//pSelect->VisitNearbyGridObject(range, searcher);
	for (GameObject* pGameObject : results)
	{
		char text[128] = { 0 };
		sprintf(text, "Search GO entry %d.", pGameObject->GetEntry());
		handler->SendSysMessage(text);
	}
	handler->SendSysMessage("Search GO End.");
	return true;
}

static bool HandlePBotTestRoll(ChatHandler* handler, const char* args)
{
	Player* player = handler->GetSession()->GetPlayer();
	Player* target = handler->getSelectedPlayerOrSelf();
	if (!player || !args)
		return false;
	uint32 itemId = 0;
    if (args[0] == '[')                                        // [name] manual form
    {
        char const* itemNameStr = strtok((char*)args, "]");

        if (itemNameStr && itemNameStr[0])
        {
            std::string itemName = itemNameStr + 1;
            auto itr = std::find_if(sItemSparseStore.begin(), sItemSparseStore.end(), [&itemName](ItemSparseEntry const* sparse)
                {
                    for (uint32 i = 0; i < MAX_LOCALES; ++i)
                        if (itemName == sparse->Display->Str[i])
                            return true;
                    return false;
                });

            if (itr == sItemSparseStore.end())
            {
                handler->PSendSysMessage(LANG_COMMAND_COULDNOTFIND, itemNameStr + 1);
                handler->SetSentErrorMessage(true);
                return false;
            }

            itemId = itr->ID;
        }
        else
            return false;
    }
    else                                                    // item_id or [name] Shift-click form |color|Hitem:item_id:0:0:0|h[name]|h|r
    {
        char const* id = handler->extractKeyFromLink((char*)args, "Hitem");
        if (!id)
            return false;
        itemId = atoul(id);
    }

	Item* item = player->GetItemByEntry(itemId);
	if (!item)
		return false;
	if (PlayerBotSetting::IsBetterEquip(target, item->GetTemplate(), item->GetItemRandomPropertyId()))
		handler->SendSysMessage("Select player can roll this item.");
	else
		handler->SendSysMessage("Select player not roll this item.");
	return true;
}

class pbotai_commandscript : public CommandScript
{
public:
	pbotai_commandscript() : CommandScript("pbotai_commandscript") { }

	std::vector<ChatCommand> GetCommands() const override
	{
		static std::vector<ChatCommand> pbotaccCommandTable =
		{
			{ "moveto", rbac::RBAC_PREM_COMMAND_PBOTAI_MOVETO, true, &HandlePBotAIMoveTo, "" },
			{ "movepoints", rbac::RBAC_PREM_COMMAND_PBOTAI_MOVEPOINTS, true, &HandlePBotAIMovePoints, "" },
			{ "addwp", rbac::RBAC_PREM_COMMAND_PBOTAI_ADD_AIWP, true, &HandlePBotAIAddAIWaypoint, "" },
			{ "delwp", rbac::RBAC_PREM_COMMAND_PBOTAI_DEL_AIWP, true, &HandlePBotAIDelAIWaypoint, "" },
			{ "movwp", rbac::RBAC_PREM_COMMAND_PBOTAI_MOV_AIWP, true, &HandlePBotAIMovAIWaypoint, "" },
			{ "descwp", rbac::RBAC_PREM_COMMAND_PBOTAI_POINTSDESC, true, &HandlePBotAIDescAIWaypoint, "" },
			{ "showwp", rbac::RBAC_PREM_COMMAND_PBOTAI_SHOWALL_AIWP, true, &HandlePBotAIShowAllAIWaypoint, "" },
			{ "endbg", rbac::RBAC_PREM_COMMAND_PBOTAI_END_BG, true, &HandlePBotAIEndBG, "" },
			{ "faceto", rbac::RBAC_PREM_COMMAND_PBOTAI_FACETO, true, &HandlePBotAIFaceTo, "" },
			{ "dtarget", rbac::RBAC_PREM_COMMAND_PBOTAI_DEBUGTARGET, true, &HandlePBotAIDebugTarget, "" },
			{ "search", rbac::RBAC_PREM_COMMAND_PBOTAI_SEARCH, true, &HandlePBotAISearch, "" },
			{ "teleport", rbac::RBAC_PREM_COMMAND_PBOTAI_TELEPORT, true, &HandlePBotAITeleport, "" },
			{ "abflag", rbac::RBAC_PREM_COMMAND_PBOTAI_ABFLAG, true, &HandlePBotAIABFlag, "" },
			{ "oindex", rbac::RBAC_PREM_COMMAND_PBOTAI_FINDGO, true, &HandlePBotAIFindGO, "" },
			{ "ecp", rbac::RBAC_PREM_COMMAND_PBOTAI_ECP, true, &HandlePBotAIECPInfo, "" },
			{ "pvpequip", rbac::RBAC_PREM_COMMAND_PBOTAI_RESETPVPEQUIP, true, &HandlePBotPVPEquip, "" },
			{ "pettalent", rbac::RBAC_PREM_COMMAND_PBOTAI_PETTALENTS, true, &HandlePBotPetTalents, "" },
			{ "searchgo", rbac::RBAC_PREM_COMMAND_PBOTAI_SEARCHGO, true, &HandlePBotSearchGO, "" },
			{ "testroll", rbac::RBAC_PREM_COMMAND_PBOTAI_TESTROLL, true, &HandlePBotTestRoll, "" },
		};

		static std::vector<ChatCommand> commandTable =
		{
			{ "pbotai", rbac::RBAC_PREM_COMMAND_PBOTAI, false, NULL, "", pbotaccCommandTable },
		};

		return commandTable;
	}
};

void AddSC_pbotai_commandscript()
{
	new pbotai_commandscript();
}
