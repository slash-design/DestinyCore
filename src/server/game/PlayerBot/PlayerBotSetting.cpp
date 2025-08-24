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

#include "PlayerBotSetting.h"
#include "ObjectMgr.h"
#include "Pet.h"
#include "WorldSession.h"
#include "PlayerBotSession.h"
#include "OnlineMgr.h"
#include "MapManager.h"
#include "Item.h"
#include "ItemTemplate.h"
#include "Bag.h"
#include <Spell.h>

uint32 PlayerBotSetting::classesTrainersGUID[MAX_CLASSES][2];
std::set<BotTalentEntry> PlayerBotSetting::classesTalents[MAX_CLASSES][3] = { std::set<BotTalentEntry>() };
std::map<uint32, ItemsForLevel> PlayerBotSetting::classesEquips[MAX_CLASSES][InventoryType::INVTYPE_RELIC+1] = { std::map<uint32, ItemsForLevel>() };
std::list<uint32> PlayerBotSetting::classesCommonSpells[MAX_CLASSES] = { std::list<uint32>() };
std::vector<uint32> PlayerBotSetting::beastCreatureEntrys = std::vector<uint32>();
std::vector<uint32> PlayerBotSetting::normalMountSpells = std::vector<uint32>();
std::vector<uint32> PlayerBotSetting::fastMountSpells = std::vector<uint32>();
std::set<uint32> PlayerBotSetting::botFlyMountEntrys = std::set<uint32>();

bool ItemsForLevel::IsTenacityItem(const ItemTemplate* itemTemplate)
{
	if (!itemTemplate || itemTemplate->ExtendedData->ItemLevel < 240)
		return false;
	for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; i++)
	{
		auto type = itemTemplate->ExtendedData->StatModifierBonusStat[i];
		if (type == ItemModType::ITEM_MOD_CRIT_TAKEN_RATING ||
			type == ItemModType::ITEM_MOD_RESILIENCE_RATING)
		{
			if (itemTemplate->GetQuality() < ItemQualities::ITEM_QUALITY_EPIC)
				continue;
			if (type >= 40)
				return true;
			if (itemTemplate->GetInventoryType() == InventoryType::INVTYPE_THROWN ||
				itemTemplate->GetInventoryType() == InventoryType::INVTYPE_RANGED ||
				itemTemplate->GetInventoryType() == InventoryType::INVTYPE_RANGEDRIGHT)
			{
				if (type >= 20)
					return true;
			}
			if (itemTemplate->GetInventoryType() == InventoryType::INVTYPE_TRINKET)
				return true;
		}
	}
	return false;
}

void ItemsForLevel::AddItem(const ItemTemplate* pItem)
{
	if (!pItem || pItem->ExtendedData->AllowableClass == 0)
		return;
	if (IsTenacityItem(pItem))
	{
		bool exist = false;
		for (BotItems::iterator itItem = m_TenacityItems.begin(); itItem != m_TenacityItems.end(); itItem++)
		{
			if ((*itItem) == pItem)
			{
				exist = true;
				break;
			}
		}
		if (!exist)
		{
			m_TenacityItems.push_back(pItem);
			return;
		}
	}
	bool exist = false;
	for (BotItems::iterator itItem = m_Items.begin(); itItem != m_Items.end(); itItem++)
	{
		if ((*itItem) == pItem)
		{
			exist = true;
			break;
		}
	}
	if (!exist)
		m_Items.push_back(pItem);
}

const ItemTemplate* ItemsForLevel::RandomItem()
{
	if (m_Items.size() <= 0)
		return NULL;
	uint32 index = irand(0, m_Items.size() - 1);
	return m_Items[index];
}

const ItemTemplate* ItemsForLevel::RandomTenacityItem()
{
	if (m_TenacityItems.size() <= 0)
		return NULL;
	uint32 index = irand(0, m_TenacityItems.size() - 1);
	return m_TenacityItems[index];
}

bool BotTalentEntry::operator < (const BotTalentEntry &tal) const
{
	if (!talentEntry || !tal.talentEntry)
		return false;
	//if (talentEntry->Row == tal.talentEntry->Row)
	{
		return talentEntry->ColumnIndex < tal.talentEntry->ColumnIndex;
	}
	//else
	//	return talentEntry->Row < tal.talentEntry->Row;
	return false;
}

bool PlayerBotSetting::IsEquipByClasses(uint32 cls, const ItemTemplate* itemTemplate)
{
	if (!itemTemplate || itemTemplate->ExtendedData->AllowableClass == 0)
		return false;
	if (itemTemplate->ExtendedData->AllowableClass > 0)
	{
		if (!(itemTemplate->ExtendedData->AllowableClass & (1 << (cls - 1))))
			return false;
	}
	if (cls == 1 || cls == 6 || cls == 3 || cls == 4)
	{
		if (!IsOnlyPhysicsAttributeEquip(itemTemplate, (cls == 3) ? false : true))
			return false;
	}
	if (IsCommonEquip(itemTemplate))
		return true;
	switch (cls)
	{
	case 1:
		return IsWarriorEquip(itemTemplate);
	case 2:
		return IsPaladinEquip(itemTemplate);
	case 3:
		return IsHunterEquip(itemTemplate);
	case 4:
		return IsRogueEquip(itemTemplate);
	case 5:
		return IsPriestEquip(itemTemplate);
	case 6:
		return IsDeathKightEquip(itemTemplate);
	case 7:
		return IsShamanEquip(itemTemplate);
	case 8:
		return IsMageEquip(itemTemplate);
	case 9:
		return IsWarlockEquip(itemTemplate);
	case 11:
		return IsDruidEquip(itemTemplate);
	default:
		return false;
	}
	return false;
}

bool PlayerBotSetting::IsEquipByClsAndTal(uint32 cls, uint32 tal, const ItemTemplate* itemTemplate, int32 rndPropID)
{
	if (tal > 2 || !itemTemplate || cls < 1 || cls == 10 || cls > 11)
		return false;
	if (!itemTemplate || itemTemplate->ExtendedData->AllowableClass == 0)
		return false;
	if (itemTemplate->ExtendedData->AllowableClass > 0)
	{
		uint32 clsMask = 1 << (cls - 1);
		if (!(itemTemplate->ExtendedData->AllowableClass & clsMask))
			return false;
		if (itemTemplate->ExtendedData->AllowableClass == clsMask)
			return true;
	}

	if (IsTrinketEquip(itemTemplate))
		return true;
	std::list<uint32> enchants;
	GetRandomPropEnchantments(rndPropID, enchants);
	switch (cls)
	{
	case 1:
		if (tal == 2)
		{
			if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_WEAPON && itemTemplate->GetInventoryType() == INVTYPE_2HWEAPON)
				return false;
			//if (!IsTankAttributeEquip(itemTemplate) && !IsTankRandomAttributeByEquip(enchants))
			//	return false;
			if (!IsOnlyPhysicsRandomAttributeByEquip(enchants, true) && !IsOnlyPhysicsAttributeEquip(itemTemplate, true))
				return false;
		}
		else
		{
			if (IsTankRandomAttributeByEquip(enchants) || IsTankAttributeEquip(itemTemplate))
				return false;
			if (!IsOnlyPhysicsRandomAttributeByEquip(enchants, true) && !IsOnlyPhysicsAttributeEquip(itemTemplate, true))
				return false;
		}
		if (IsCommonEquip(itemTemplate))
			return true;
		return IsWarriorEquip(itemTemplate);
	case 2:
		if (tal == 1)
		{
			if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_WEAPON && itemTemplate->GetInventoryType() == INVTYPE_2HWEAPON)
				return false;
			//if (!IsTankRandomAttributeByEquip(enchants) && !IsTankAttributeEquip(itemTemplate))
			//	return false;
		}
		else
		{
			if (IsTankRandomAttributeByEquip(enchants) || IsTankAttributeEquip(itemTemplate))
				return false;
		}
		if (IsCommonEquip(itemTemplate))
			return true;
		return IsPaladinEquip(itemTemplate);
	case 3:
		if (IsTankRandomAttributeByEquip(enchants) || IsTankAttributeEquip(itemTemplate))
			return false;
		if (!IsOnlyPhysicsRandomAttributeByEquip(enchants, false) && !IsOnlyPhysicsAttributeEquip(itemTemplate, false))
			return false;
		if (IsCommonEquip(itemTemplate))
			return true;
		return IsHunterEquip(itemTemplate);
	case 4:
		if (IsTankRandomAttributeByEquip(enchants) || IsTankAttributeEquip(itemTemplate))
			return false;
		if (!IsOnlyPhysicsRandomAttributeByEquip(enchants, true) && !IsOnlyPhysicsAttributeEquip(itemTemplate, true))
			return false;
		if (IsCommonEquip(itemTemplate))
			return true;
		return IsRogueEquip(itemTemplate);
	case 5:
		if (IsTankRandomAttributeByEquip(enchants) || IsTankAttributeEquip(itemTemplate))
			return false;
		if (!IsOnlyMagicRandomAttributeByEquip(enchants) && !IsOnlyMagicAttributeEquip(itemTemplate))
			return false;
		if (IsCommonEquip(itemTemplate))
			return true;
		return IsPriestEquip(itemTemplate);
	case 6:
		if (tal != 1)
		{
			if (IsTankRandomAttributeByEquip(enchants) || IsTankAttributeEquip(itemTemplate))
				return false;
		}
		if (!IsOnlyPhysicsRandomAttributeByEquip(enchants, true) && !IsOnlyPhysicsAttributeEquip(itemTemplate, true))
			return false;
		if (IsCommonEquip(itemTemplate))
			return true;
		return IsDeathKightEquip(itemTemplate);
	case 7:
		if (IsTankRandomAttributeByEquip(enchants) || IsTankAttributeEquip(itemTemplate))
			return false;
		if (tal == 1)
		{
			if (!IsOnlyPhysicsRandomAttributeByEquip(enchants, false) && !IsOnlyPhysicsAttributeEquip(itemTemplate, false))
				return false;
		}
		else
		{
			if (!IsOnlyMagicRandomAttributeByEquip(enchants) && !IsOnlyMagicAttributeEquip(itemTemplate))
				return false;
		}
		if (IsCommonEquip(itemTemplate))
			return true;
		return IsShamanEquip(itemTemplate);
	case 8:
		if (IsTankRandomAttributeByEquip(enchants) || IsTankAttributeEquip(itemTemplate))
			return false;
		if (!IsOnlyMagicRandomAttributeByEquip(enchants) && !IsOnlyMagicAttributeEquip(itemTemplate))
			return false;
		if (IsCommonEquip(itemTemplate))
			return true;
		return IsMageEquip(itemTemplate);
	case 9:
		if (IsTankRandomAttributeByEquip(enchants) || IsTankAttributeEquip(itemTemplate))
			return false;
		if (!IsOnlyMagicRandomAttributeByEquip(enchants) && !IsOnlyMagicAttributeEquip(itemTemplate))
			return false;
		if (IsCommonEquip(itemTemplate))
			return true;
		return IsWarlockEquip(itemTemplate);
	case 11:
		if (IsTankRandomAttributeByEquip(enchants) || IsTankAttributeEquip(itemTemplate))
			return false;
		if (tal == 1)
		{
			if (!IsOnlyPhysicsRandomAttributeByEquip(enchants, true) && !IsOnlyPhysicsAttributeEquip(itemTemplate, true))
				return false;
		}
		else
		{
			if (!IsOnlyMagicRandomAttributeByEquip(enchants) && !IsOnlyMagicAttributeEquip(itemTemplate))
				return false;
		}
		if (IsCommonEquip(itemTemplate))
			return true;
		return IsDruidEquip(itemTemplate);
	default:
		return false;
	}
	return false;
}

bool PlayerBotSetting::MatchEquipmentSlot(uint8 pos, const ItemTemplate* itemTemplate)
{
	EquipmentSlots slot = EquipmentSlots(pos);
	InventoryType type = InventoryType(itemTemplate->GetInventoryType());
	switch (type)
	{
	case InventoryType::INVTYPE_HEAD:
		if (slot == EquipmentSlots::EQUIPMENT_SLOT_HEAD)
			return true;
		break;
	case InventoryType::INVTYPE_NECK:
		if (slot == EquipmentSlots::EQUIPMENT_SLOT_NECK)
			return true;
		break;
	case InventoryType::INVTYPE_SHOULDERS:
		if (slot == EquipmentSlots::EQUIPMENT_SLOT_SHOULDERS)
			return true;
		break;
	case InventoryType::INVTYPE_CHEST:
	case InventoryType::INVTYPE_ROBE:
		if (slot == EquipmentSlots::EQUIPMENT_SLOT_CHEST)
			return true;
		break;
	case InventoryType::INVTYPE_WAIST:
		if (slot == EquipmentSlots::EQUIPMENT_SLOT_WAIST)
			return true;
		break;
	case InventoryType::INVTYPE_LEGS:
		if (slot == EquipmentSlots::EQUIPMENT_SLOT_LEGS)
			return true;
		break;
	case InventoryType::INVTYPE_FEET:
		if (slot == EquipmentSlots::EQUIPMENT_SLOT_FEET)
			return true;
		break;
	case InventoryType::INVTYPE_WRISTS:
		if (slot == EquipmentSlots::EQUIPMENT_SLOT_WRISTS)
			return true;
		break;
	case InventoryType::INVTYPE_HANDS:
		if (slot == EquipmentSlots::EQUIPMENT_SLOT_HANDS)
			return true;
		break;
	case InventoryType::INVTYPE_FINGER:
		if (slot == EquipmentSlots::EQUIPMENT_SLOT_FINGER1 || slot == EquipmentSlots::EQUIPMENT_SLOT_FINGER2)
			return true;
		break;
	case InventoryType::INVTYPE_TRINKET:
		if (slot == EquipmentSlots::EQUIPMENT_SLOT_TRINKET1 || slot == EquipmentSlots::EQUIPMENT_SLOT_TRINKET2)
			return true;
		break;
	case InventoryType::INVTYPE_CLOAK:
		if (slot == EquipmentSlots::EQUIPMENT_SLOT_BACK)
			return true;
		break;
	case InventoryType::INVTYPE_WEAPON:
		if (slot == EquipmentSlots::EQUIPMENT_SLOT_MAINHAND || slot == EquipmentSlots::EQUIPMENT_SLOT_OFFHAND)
			return true;
		break;
	case InventoryType::INVTYPE_2HWEAPON:
	case InventoryType::INVTYPE_WEAPONMAINHAND:
		if (slot == EquipmentSlots::EQUIPMENT_SLOT_MAINHAND)
			return true;
		break;
	case InventoryType::INVTYPE_SHIELD:
	case InventoryType::INVTYPE_WEAPONOFFHAND:
		if (slot == EquipmentSlots::EQUIPMENT_SLOT_OFFHAND)
			return true;
		break;
	case InventoryType::INVTYPE_RANGED:
	case InventoryType::INVTYPE_RANGEDRIGHT:
	case InventoryType::INVTYPE_HOLDABLE:
	case InventoryType::INVTYPE_THROWN:
	case InventoryType::INVTYPE_RELIC:
		if (slot == EquipmentSlots::EQUIPMENT_SLOT_RANGED)
			return true;
		break;
	}

	return false;
}

uint32 PlayerBotSetting::GetItemLevelByAI(const ItemTemplate* item)
{
	if (!item)
		return 0;
	uint32 level = item->ExtendedData->ItemLevel;
	uint32 quality = item->GetQuality();
	if (quality <= 1)
		return level;
	if (quality == ITEM_QUALITY_HEIRLOOM)
		return 999;
	level += uint32(float(level) * (float(quality) * 0.3f));
	return level;
}

bool PlayerBotSetting::IsBetterEquip(Player* player, const ItemTemplate* itemTemplate, int32 rndPropID)
{
	if (!itemTemplate || itemTemplate->GetBaseRequiredLevel() > player->getLevel())
		return false;
	if (!IsEquipByClsAndTal(player->getClass(), player->FindTalentType(), itemTemplate, rndPropID))
		return false;
	uint16 eDest;
	InventoryResult msg = player->CanEquipNewItem(NULL_SLOT, eDest, itemTemplate->GetId(), true);
	if (msg != EQUIP_ERR_OK)
		return false;
	bool mainhandIsTwo = false;
	for (uint8 slot = EquipmentSlots::EQUIPMENT_SLOT_HEAD; slot < EquipmentSlots::EQUIPMENT_SLOT_END; slot++)
	{
		if (!MatchEquipmentSlot(slot, itemTemplate))
			continue;
		//uint16 pos = (255 << 8) | slot;
		//if (!player->IsEquipmentPos(pos))
		//	continue;

		Item* pItem = player->GetItemByPos(255, slot);
		if (!pItem)
		{
			if (slot == EquipmentSlots::EQUIPMENT_SLOT_OFFHAND && mainhandIsTwo)
				return false;
			return true;
		}
		const ItemTemplate* selfTemplate = pItem->GetTemplate();
		if (!selfTemplate)
			continue;
		if (slot == EquipmentSlots::EQUIPMENT_SLOT_MAINHAND)
		{
			if (selfTemplate->GetClass() == ItemClass::ITEM_CLASS_WEAPON && selfTemplate->GetInventoryType() == INVTYPE_2HWEAPON)
				mainhandIsTwo = true;
			else
				mainhandIsTwo = false;
		}
		if (selfTemplate->GetClass() != itemTemplate->GetClass())
			continue;
		if (selfTemplate->GetSubClass() != itemTemplate->GetSubClass())
		{
			if (selfTemplate->GetClass() != ItemClass::ITEM_CLASS_WEAPON || itemTemplate->GetClass() != ItemClass::ITEM_CLASS_WEAPON)
				continue;
			if ((selfTemplate->GetInventoryType() != INVTYPE_2HWEAPON && selfTemplate->GetInventoryType() != INVTYPE_WEAPON && selfTemplate->GetInventoryType() != INVTYPE_WEAPONMAINHAND) ||
				(itemTemplate->GetInventoryType() != INVTYPE_2HWEAPON && itemTemplate->GetInventoryType() != INVTYPE_WEAPON && itemTemplate->GetInventoryType() != INVTYPE_WEAPONMAINHAND))
				continue;
		}
		if (GetItemLevelByAI(selfTemplate) >= GetItemLevelByAI(itemTemplate))
		{
			//if (IsTrinketEquip(itemTemplate) || IsFingerEquip(itemTemplate))
			//	continue;
			return false;
		}
		return true;
	}

	return false;
}

void PlayerBotSetting::ClearUnknowMount(Player* player)
{
	for (uint32 mountID : normalMountSpells)
	{
		if (player->HasAura(mountID))
			player->RemoveOwnedAura(mountID, ObjectGuid::Empty, 0, AURA_REMOVE_BY_CANCEL);
	}
	for (uint32 mountID : fastMountSpells)
	{
		if (player->HasAura(mountID))
			player->RemoveOwnedAura(mountID, ObjectGuid::Empty, 0, AURA_REMOVE_BY_CANCEL);
	}
	if (player->IsMounted())// && AURA_EFFECT_HANDLE_REAL
	{
		player->Dismount();
		player->RemoveAurasByType(SPELL_AURA_MOUNTED);
	}
}

uint32 PlayerBotSetting::CheckMaxLevel(uint32 level)
{
	uint32 worldMaxLevel = sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL);
	if (level > worldMaxLevel)
	{
		level = worldMaxLevel;
	}
	if (level == 0)
		level = 1;
	return level;
}

bool PlayerBotSetting::IsBotFlyMountAura(uint32 aura)
{
	if (!aura)
		return false;
	return botFlyMountEntrys.find(aura) != botFlyMountEntrys.end();
}

bool PlayerBotSetting::IsOnlyPhysicsAttributeEquip(const ItemTemplate* itemTemplate, bool coverIntellect)
{
	if (!itemTemplate->HasStats())
		return false;
	for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; i++)
	{
		auto type = itemTemplate->ExtendedData->StatModifierBonusStat[i];
		switch (type)
		{
		case ItemModType::ITEM_MOD_MANA:
		case ItemModType::ITEM_MOD_SPIRIT:
		case ItemModType::ITEM_MOD_HIT_SPELL_RATING:
		case ItemModType::ITEM_MOD_CRIT_SPELL_RATING:
		case ItemModType::ITEM_MOD_HIT_TAKEN_SPELL_RATING:
		case ItemModType::ITEM_MOD_CRIT_TAKEN_SPELL_RATING:
		case ItemModType::ITEM_MOD_HASTE_SPELL_RATING:
		case ItemModType::ITEM_MOD_SPELL_HEALING_DONE:
		case ItemModType::ITEM_MOD_SPELL_DAMAGE_DONE:
		case ItemModType::ITEM_MOD_MANA_REGENERATION:
		case ItemModType::ITEM_MOD_SPELL_POWER:
		case ItemModType::ITEM_MOD_SPELL_PENETRATION:
			return false;
		case ItemModType::ITEM_MOD_INTELLECT:
			if (coverIntellect)
				return false;
		}
	}
	return true;
}

bool PlayerBotSetting::IsOnlyMagicAttributeEquip(const ItemTemplate* itemTemplate)
{
	if (!itemTemplate->HasStats())
		return false;
	for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; i++)
	{
		auto type = itemTemplate->ExtendedData->StatModifierBonusStat[i];
		switch (type)
		{
		case ItemModType::ITEM_MOD_AGILITY:
		case ItemModType::ITEM_MOD_STRENGTH:
		case ItemModType::ITEM_MOD_HIT_MELEE_RATING:
		case ItemModType::ITEM_MOD_CRIT_MELEE_RATING:
		case ItemModType::ITEM_MOD_HIT_TAKEN_MELEE_RATING:
		case ItemModType::ITEM_MOD_CRIT_TAKEN_MELEE_RATING:
		case ItemModType::ITEM_MOD_HASTE_MELEE_RATING:
		case ItemModType::ITEM_MOD_ATTACK_POWER:
		case ItemModType::ITEM_MOD_RANGED_ATTACK_POWER:
		case ItemModType::ITEM_MOD_ARMOR_PENETRATION_RATING:
			return false;
		}
	}
	return true;
}

bool PlayerBotSetting::IsTankAttributeEquip(const ItemTemplate* itemTemplate)
{
	if (!itemTemplate->HasStats())
		return false;
	for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; i++)
	{
		auto type = itemTemplate->ExtendedData->StatModifierBonusStat[i];
		switch (type)
		{
		case ItemModType::ITEM_MOD_DEFENSE_SKILL_RATING:
		case ItemModType::ITEM_MOD_DODGE_RATING:
		case ItemModType::ITEM_MOD_PARRY_RATING:
		case ItemModType::ITEM_MOD_BLOCK_RATING:
		case ItemModType::ITEM_MOD_HIT_TAKEN_MELEE_RATING:
		case ItemModType::ITEM_MOD_HIT_TAKEN_RANGED_RATING:
		case ItemModType::ITEM_MOD_HIT_TAKEN_SPELL_RATING:
		case ItemModType::ITEM_MOD_HIT_TAKEN_RATING:
		case ItemModType::ITEM_MOD_BLOCK_VALUE:
			return true;
		}
	}
	return false;
}

bool PlayerBotSetting::IsOnlyPhysicsRandomAttributeByEquip(std::list<uint32>& enchants, bool coverIntellect)
{
	if (enchants.empty())
		return false;
	bool have = false;
	for (uint32 enchant_id : enchants)
	{
		SpellItemEnchantmentEntry const* pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
		if (!pEnchant)
			continue;
		for (int s = 0; s < MAX_ITEM_ENCHANTMENT_EFFECTS; ++s)
		{
			uint32 enchant_spell_id = pEnchant->Effect[s];
			if (enchant_spell_id == 0)
				continue;
			switch (enchant_spell_id)
			{
			case ItemModType::ITEM_MOD_MANA:
			case ItemModType::ITEM_MOD_SPIRIT:
			case ItemModType::ITEM_MOD_HIT_SPELL_RATING:
			case ItemModType::ITEM_MOD_CRIT_SPELL_RATING:
			case ItemModType::ITEM_MOD_HIT_TAKEN_SPELL_RATING:
			case ItemModType::ITEM_MOD_CRIT_TAKEN_SPELL_RATING:
			case ItemModType::ITEM_MOD_HASTE_SPELL_RATING:
			case ItemModType::ITEM_MOD_SPELL_HEALING_DONE:
			case ItemModType::ITEM_MOD_SPELL_DAMAGE_DONE:
			case ItemModType::ITEM_MOD_MANA_REGENERATION:
			case ItemModType::ITEM_MOD_SPELL_POWER:
			case ItemModType::ITEM_MOD_SPELL_PENETRATION:
				return false;
			case ItemModType::ITEM_MOD_INTELLECT:
				if (coverIntellect)
					return false;
				have = true;
				break;
			default:
				have = true;
			}
		}
	}
	return have;
}

bool PlayerBotSetting::IsOnlyMagicRandomAttributeByEquip(std::list<uint32>& enchants)
{
	if (enchants.empty())
		return false;
	bool have = false;
	for (uint32 enchant_id : enchants)
	{
		SpellItemEnchantmentEntry const* pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
		if (!pEnchant)
			continue;
		for (int s = 0; s < MAX_ITEM_ENCHANTMENT_EFFECTS; ++s)
		{
			uint32 enchant_spell_id = pEnchant->Effect[s];
			if (enchant_spell_id == 0)
				continue;
			switch (enchant_spell_id)
			{
			case ItemModType::ITEM_MOD_AGILITY:
			case ItemModType::ITEM_MOD_STRENGTH:
			case ItemModType::ITEM_MOD_HIT_MELEE_RATING:
			case ItemModType::ITEM_MOD_CRIT_MELEE_RATING:
			case ItemModType::ITEM_MOD_HIT_TAKEN_MELEE_RATING:
			case ItemModType::ITEM_MOD_CRIT_TAKEN_MELEE_RATING:
			case ItemModType::ITEM_MOD_HASTE_MELEE_RATING:
			case ItemModType::ITEM_MOD_ATTACK_POWER:
			case ItemModType::ITEM_MOD_RANGED_ATTACK_POWER:
			case ItemModType::ITEM_MOD_ARMOR_PENETRATION_RATING:
				return false;
			default:
				have = true;
			}
		}
	}
	return have;
}

bool PlayerBotSetting::IsTankRandomAttributeByEquip(std::list<uint32>& enchants)
{
	if (enchants.empty())
		return false;
	for (uint32 enchant_id : enchants)
	{
		SpellItemEnchantmentEntry const* pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
		if (!pEnchant)
			continue;
		for (int s = 0; s < MAX_ITEM_ENCHANTMENT_EFFECTS; ++s)
		{
			uint32 enchant_spell_id = pEnchant->Effect[s];
			if (enchant_spell_id == 0)
				continue;
			switch (enchant_spell_id)
			{
			case ItemModType::ITEM_MOD_DEFENSE_SKILL_RATING:
			case ItemModType::ITEM_MOD_DODGE_RATING:
			case ItemModType::ITEM_MOD_PARRY_RATING:
			case ItemModType::ITEM_MOD_BLOCK_RATING:
			case ItemModType::ITEM_MOD_HIT_TAKEN_MELEE_RATING:
			case ItemModType::ITEM_MOD_HIT_TAKEN_RANGED_RATING:
			case ItemModType::ITEM_MOD_HIT_TAKEN_SPELL_RATING:
			case ItemModType::ITEM_MOD_HIT_TAKEN_RATING:
			case ItemModType::ITEM_MOD_BLOCK_VALUE:
				return true;
			}
		}
	}
	return false;
}

void PlayerBotSetting::GetRandomPropEnchantments(int32 rndPropID, std::list<uint32>& enchants)
{
	enchants.clear();
	if (rndPropID == 0)
		return;
	if (rndPropID > 0)
	{
		if (ItemRandomPropertiesEntry const* item_rand = sItemRandomPropertiesStore.LookupEntry(rndPropID))
		{
			for (uint32 i = PROP_ENCHANTMENT_SLOT_2; i < PROP_ENCHANTMENT_SLOT_2 + 3; ++i)
			{
				uint32 enchant_id = item_rand->Enchantment[i - PROP_ENCHANTMENT_SLOT_2];
				if (enchant_id > 0)
					enchants.push_back(enchant_id);
			}
		}
	}
	if (rndPropID < 0)
	{
		if (ItemRandomSuffixEntry const* item_rand = sItemRandomSuffixStore.LookupEntry(-rndPropID))
		{
			for (uint32 i = PROP_ENCHANTMENT_SLOT_0; i < PROP_ENCHANTMENT_SLOT_0 + 3; ++i)
			{
				uint32 enchant_id = item_rand->Enchantment[i - PROP_ENCHANTMENT_SLOT_0];
				if (enchant_id > 0)
					enchants.push_back(enchant_id);
			}
		}
	}
}

//bool PlayerBotSetting::MatchEquipmentSlotsByWeapon(EquipmentSlots slot, InventoryType iType)
//{
//	if (slot == EquipmentSlots::EQUIPMENT_SLOT_MAINHAND)
//		return (iType == InventoryType::INVTYPE_2HWEAPON || iType == InventoryType::INVTYPE_WEAPON || iType == InventoryType::INVTYPE_WEAPONMAINHAND);
//	if (slot == EquipmentSlots::EQUIPMENT_SLOT_OFFHAND)
//		return (iType == InventoryType::INVTYPE_WEAPON || iType == InventoryType::INVTYPE_WEAPONOFFHAND);
//
//	return false;
//}
//
//bool PlayerBotSetting::MatchEquipmentSlotsByArmor(EquipmentSlots slot, InventoryType iType)
//{
//	if (slot == EquipmentSlots::EQUIPMENT_SLOT_BODY)
//		return (iType == InventoryType::INVTYPE_BODY || iType == InventoryType::INVTYPE_ROBE);
//	if (slot >= EquipmentSlots::EQUIPMENT_SLOT_HEAD && slot <= EquipmentSlots::EQUIPMENT_SLOT_HANDS)
//		return (uint8(iType) - 1 == slot);
//	if (slot == EquipmentSlots::EQUIPMENT_SLOT_FINGER1 || slot == EquipmentSlots::EQUIPMENT_SLOT_FINGER2)
//		return (iType == InventoryType::INVTYPE_FINGER);
//	if (slot == EquipmentSlots::EQUIPMENT_SLOT_TRINKET1 || slot == EquipmentSlots::EQUIPMENT_SLOT_TRINKET2)
//		return (iType == InventoryType::INVTYPE_TRINKET);
//	if (slot == EquipmentSlots::EQUIPMENT_SLOT_BACK)
//		return (iType == InventoryType::INVTYPE_CLOAK);
//	return false;
//}
//
//bool PlayerBotSetting::MatchRangeEquipmentSlots(uint32 cls, const ItemTemplate* itemTemplate)
//{
//	return false;
//}

bool PlayerBotSetting::IsCommonEquip(const ItemTemplate* itemTemplate)
{
	switch (itemTemplate->GetInventoryType())
	{
	case INVTYPE_NECK:
	case INVTYPE_CLOAK:
	case INVTYPE_FINGER:
	case INVTYPE_TRINKET:
		return true;
	default:
		break;
	}
	if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_ARMOR /*&& itemTemplate->GetSubClass() == ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MISC */&& itemTemplate->GetInventoryType() == INVTYPE_TRINKET)
		return true;
	return false;
}

bool PlayerBotSetting::IsFingerEquip(const ItemTemplate* itemTemplate)
{
	switch (itemTemplate->GetInventoryType())
	{
	case INVTYPE_FINGER:
		return true;
	default:
		break;
	}
	if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_ARMOR /*&& itemTemplate->GetSubClass() == ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MISC*/ && itemTemplate->GetInventoryType() == INVTYPE_FINGER)
		return true;
	return false;
}

bool PlayerBotSetting::IsTrinketEquip(const ItemTemplate* itemTemplate)
{
	switch (itemTemplate->GetInventoryType())
	{
	case INVTYPE_TRINKET:
		return true;
	default:
		break;
	}
	if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_ARMOR/* && itemTemplate->GetSubClass() == ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MISC*/ && itemTemplate->GetInventoryType() == INVTYPE_TRINKET)
		return true;
	return false;
}

bool PlayerBotSetting::IsWarriorEquip(const ItemTemplate* itemTemplate)
{
	if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_WEAPON)
	{
		switch (itemTemplate->GetSubClass())
		{
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_obsolete:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_STAFF:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SPEAR:
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MISC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_DAGGER:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MACE:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_BOW:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_GUN:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_CROSSBOW:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_WAND:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_FISHING_POLE:
			return false;
		default:
			break;
		}
		return true;
	}
	else if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_ARMOR)
	{
		switch (itemTemplate->GetSubClass())
		{
		//case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MISC:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_CLOTH:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LEATHER:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LIBRAM:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_IDOL:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_TOTEM:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SIGIL:
			return false;
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL:
			if (itemTemplate->GetBaseRequiredLevel() >= 40)
				return false;
			break;
		default:
			break;
		}
		return true;
	}
	else if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_PROJECTILE)
	{
		if (itemTemplate->GetSubClass() == ItemSubclassProjectile::ITEM_SUBCLASS_ARROW || itemTemplate->GetSubClass() == ItemSubclassProjectile::ITEM_SUBCLASS_BULLET)
			return true;
	}

	return false;
}

bool PlayerBotSetting::IsPaladinEquip(const ItemTemplate* itemTemplate)
{
	if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_WEAPON)
	{
		switch (itemTemplate->GetSubClass())
		{
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_AXE:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_AXE2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_BOW:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_GUN:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MACE:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_THROWN:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_CROSSBOW:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_STAFF:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SPEAR:
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MISC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_DAGGER:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_WAND:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_FISHING_POLE:
			return false;
		default:
			break;
		}
		return true;
	}
	else if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_ARMOR)
	{
		switch (itemTemplate->GetSubClass())
		{
		//case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MISC:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_CLOTH:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LEATHER:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_IDOL:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_TOTEM:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SIGIL:
			return false;
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL:
			if (itemTemplate->GetBaseRequiredLevel() >= 40)
				return false;
			break;
		default:
			break;
		}
		return true;
	}

	return false;
}

bool PlayerBotSetting::IsDeathKightEquip(const ItemTemplate* itemTemplate)
{
	if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_WEAPON)
	{
		switch (itemTemplate->GetSubClass())
		{
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_obsolete:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_AXE:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MACE:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_BOW:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_GUN:
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_FIST:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_THROWN:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_CROSSBOW:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_STAFF:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_POLEARM:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SPEAR:
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MISC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_DAGGER:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_WAND:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_FISHING_POLE:
			return false;
		default:
			break;
		}
		return true;
	}
	else if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_ARMOR)
	{
		switch (itemTemplate->GetSubClass())
		{
		//case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MISC:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_CLOTH:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LEATHER:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LIBRAM:
		//case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_BUCKLER:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SHIELD:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_IDOL:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_TOTEM:
			return false;
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL:
			if (itemTemplate->GetBaseRequiredLevel() >= 40)
				return false;
			break;
		default:
			break;
		}
		return true;
	}

	return false;
}

bool PlayerBotSetting::IsRogueEquip(const ItemTemplate* itemTemplate)
{
	if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_WEAPON)
	{
		switch (itemTemplate->GetSubClass())
		{
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_obsolete:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MACE2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_AXE2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_BOW:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_GUN:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_CROSSBOW:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_POLEARM:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SPEAR:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_STAFF:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC2:
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MISC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_WAND:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_FISHING_POLE:
			return false;
		default:
			break;
		}
		return true;
	}
	else if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_ARMOR)
	{
		switch (itemTemplate->GetSubClass())
		{
		//case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MISC:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_CLOTH:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_PLATE:
		//case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_BUCKLER:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SHIELD:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LIBRAM:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_IDOL:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_TOTEM:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SIGIL:
			return false;
		default:
			break;
		}
		return true;
	}

	return false;
}

bool PlayerBotSetting::IsDruidEquip(const ItemTemplate* itemTemplate)
{
	if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_WEAPON)
	{
		switch (itemTemplate->GetSubClass())
		{
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_obsolete:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_AXE:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_AXE2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_BOW:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_GUN:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_CROSSBOW:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_POLEARM:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SPEAR:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC2:
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MISC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_WAND:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_FISHING_POLE:
			return false;
		default:
			break;
		}
		return true;
	}
	else if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_ARMOR)
	{
		switch (itemTemplate->GetSubClass())
		{
		//case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MISC:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_CLOTH:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_PLATE:
		//case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_BUCKLER:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SHIELD:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LIBRAM:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_TOTEM:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SIGIL:
			return false;
		default:
			break;
		}
		return true;
	}

	return false;
}

bool PlayerBotSetting::IsHunterEquip(const ItemTemplate* itemTemplate)
{
	if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_WEAPON)
	{
		switch (itemTemplate->GetSubClass())
		{
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_obsolete:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MACE:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MACE2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_STAFF:
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_FIST:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_DAGGER:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_THROWN:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_AXE:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SPEAR:
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MISC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_WAND:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_FISHING_POLE:
			return false;
		default:
			break;
		}
		return true;
	}
	else if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_ARMOR)
	{
		switch (itemTemplate->GetSubClass())
		{
		//case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MISC:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_CLOTH:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_PLATE:
		//case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_BUCKLER:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SHIELD:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LIBRAM:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_IDOL:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_TOTEM:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SIGIL:
			return false;
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LEATHER:
			if (itemTemplate->GetBaseRequiredLevel() >= 40)
				return false;
			break;
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL:
			if (itemTemplate->GetBaseRequiredLevel() < 40)
				return false;
			break;
		default:
			break;
		}
		return true;
	}
	else if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_PROJECTILE)
	{
		if (itemTemplate->GetSubClass() == ItemSubclassProjectile::ITEM_SUBCLASS_ARROW || itemTemplate->GetSubClass() == ItemSubclassProjectile::ITEM_SUBCLASS_BULLET)
			return true;
	}

	return false;
}

bool PlayerBotSetting::IsShamanEquip(const ItemTemplate* itemTemplate)
{
	if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_WEAPON)
	{
		switch (itemTemplate->GetSubClass())
		{
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_obsolete:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_STAFF:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_BOW:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_GUN:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_CROSSBOW:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD2:
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_FIST:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_DAGGER:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_THROWN:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_POLEARM:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SPEAR:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC2:
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MISC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_WAND:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_FISHING_POLE:
			return false;
		default:
			break;
		}
		return true;
	}
	else if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_ARMOR)
	{
		switch (itemTemplate->GetSubClass())
		{
		//case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MISC:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_CLOTH:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_PLATE:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LIBRAM:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_IDOL:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SIGIL:
			return false;
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LEATHER:
			if (itemTemplate->GetBaseRequiredLevel() >= 40)
				return false;
			break;
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL:
			if (itemTemplate->GetBaseRequiredLevel() < 40)
				return false;
			break;
		default:
			break;
		}
		return true;
	}

	return false;
}

bool PlayerBotSetting::IsMageEquip(const ItemTemplate* itemTemplate)
{
	if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_WEAPON)
	{
		switch (itemTemplate->GetSubClass())
		{
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_obsolete:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_AXE:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MACE:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_AXE2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MACE2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_BOW:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_GUN:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_CROSSBOW:
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_FIST:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_THROWN:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_POLEARM:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SPEAR:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC2:
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MISC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_FISHING_POLE:
			return false;
		default:
			break;
		}
		return true;
	}
	else if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_ARMOR)
	{
		switch (itemTemplate->GetSubClass())
		{
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LEATHER:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_PLATE:
		//case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_BUCKLER:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SHIELD:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LIBRAM:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_IDOL:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_TOTEM:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SIGIL:
			return false;
		default:
			break;
		}
		return true;
	}

	return false;
}

bool PlayerBotSetting::IsWarlockEquip(const ItemTemplate* itemTemplate)
{
	if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_WEAPON)
	{
		switch (itemTemplate->GetSubClass())
		{
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_obsolete:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_AXE:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MACE:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_AXE2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MACE2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_BOW:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_GUN:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_CROSSBOW:
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_FIST:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_THROWN:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_POLEARM:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SPEAR:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC2:
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MISC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_FISHING_POLE:
			return false;
		default:
			break;
		}
		return true;
	}
	else if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_ARMOR)
	{
		switch (itemTemplate->GetSubClass())
		{
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LEATHER:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_PLATE:
		//case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_BUCKLER:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SHIELD:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LIBRAM:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_IDOL:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_TOTEM:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SIGIL:
			return false;
		default:
			break;
		}
		return true;
	}

	return false;
}

bool PlayerBotSetting::IsPriestEquip(const ItemTemplate* itemTemplate)
{
	if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_WEAPON)
	{
		switch (itemTemplate->GetSubClass())
		{
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_obsolete:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_AXE:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_AXE2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MACE2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD2:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_BOW:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_GUN:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_CROSSBOW:
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_FIST:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_THROWN:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_POLEARM:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SPEAR:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_EXOTIC2:
		//case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MISC:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_FISHING_POLE:
			return false;
		default:
			break;
		}
		return true;
	}
	else if (itemTemplate->GetClass() == ItemClass::ITEM_CLASS_ARMOR)
	{
		switch (itemTemplate->GetSubClass())
		{
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LEATHER:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_PLATE:
		//case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_BUCKLER:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SHIELD:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LIBRAM:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_IDOL:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_TOTEM:
		case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SIGIL:
			return false;
		default:
			break;
		}
		return true;
	}

	return false;
}

bool PlayerBotSetting::CheckHunterPet(Player* player)
{
	if (!player || player->getClass() != Classes::CLASS_HUNTER)
		return false;
	if (player->GetPet() == NULL && beastCreatureEntrys.size() > 0)
	{
		uint32 rndEntry = urand(0, beastCreatureEntrys.size() - 1);
		Pet* pet = player->CreateTamedPetFrom(beastCreatureEntrys[rndEntry], 1515);
		if (pet)
		{
			player->GetMap()->AddToMap(pet->ToCreature());
			player->SetMinion(pet, true);
			pet->InitPetCreateSpells();
			pet->ToCreature()->SetReactState(ReactStates(1));
			pet->SettingAllSpellAutocast(true);
			pet->SavePetToDB(PET_SAVE_CURRENT_STATE);
			player->PetSpellInitialize();
			return true;
		}
	}
	else if (Pet* pet = player->GetPet())
	{
		pet->SetLevel(player->getLevel());
		pet->InitPetCreateSpells();
		//pet->resetTalents();
		pet->ToCreature()->SetReactState(ReactStates(1));
		//pet->FlushTalentsByPoints();
		pet->SettingAllSpellAutocast(true);
		player->PetSpellInitialize();
		return true;
	}
	return false;
}

uint32 PlayerBotSetting::FindPlayerTalentType(Player* player)
{
	if (!player)
		return 0;
	uint32 spec = player->GetSpecializationId();
	uint32 cls = player->getClass();
	uint32 pageTalents[3] = { 0 };
	for (uint32 page = 0; page < 3; page++)
	{
		BotTalentPage& botPage = classesTalents[cls][page];
		for (BotTalentPage::iterator itPage = botPage.begin();
			itPage != botPage.end();
			itPage++)
		{
			const TalentEntry* botTEntry = (*itPage).talentEntry;
			if (!botTEntry) continue;
			//for (int8 rank = MAX_TALENT_RANK - 1; rank >= 0; --rank)
			//{
			//	if (botTEntry->RankID[rank] == 0)
			//		continue;
			//	if (player->HasTalent(botTEntry->RankID[rank], spec))
			//		++pageTalents[page];
			//}
		}
	}

	uint32 maxPageIndex = 0;
	uint32 maxPagePoint = 0;
	for (uint32 page = 0; page < 3; page++)
	{
		if (pageTalents[page] > maxPagePoint)
		{
			maxPageIndex = page;
			maxPagePoint = pageTalents[page];
		}
	}
	return maxPageIndex;
}

uint32 PlayerBotSetting::RandomMountByLevel(uint32 level)
{
	//if (level < 40)
	//{
	//	return 6777;
	//}
	//else
	{
		uint32 count = fastMountSpells.size();
		if (count == 0)
			return 6777;
		uint32 rnd = urand(0, count - 1);
		return fastMountSpells[rnd];
	}

	return 6777;
}

void PlayerBotSetting::Initialize()
{
	for (int i = 0; i < MAX_CLASSES; i++)
	{
		classesTrainersGUID[i][0] = 0;
		classesTrainersGUID[i][1] = 0;

		for (int j = 0; j < 3; j++)
			classesTalents[i][j].clear();

		for (int j = 0; j <= InventoryType::INVTYPE_RELIC; j++)
			classesEquips[i][j].clear();

		classesCommonSpells[i].clear();
	}
	beastCreatureEntrys.clear();
	normalMountSpells.clear();
	fastMountSpells.clear();
	botFlyMountEntrys.clear();

	classesTrainersGUID[Classes::CLASS_WARRIOR][0] = 5113;
	classesTrainersGUID[Classes::CLASS_WARRIOR][1] = 4593;

	classesTrainersGUID[Classes::CLASS_PALADIN][0] = 5147;
	classesTrainersGUID[Classes::CLASS_PALADIN][1] = 23128;

	classesTrainersGUID[Classes::CLASS_DEATH_KNIGHT][0] = 28474;
	classesTrainersGUID[Classes::CLASS_DEATH_KNIGHT][1] = 28474;

	classesTrainersGUID[Classes::CLASS_ROGUE][0] = 5165;
	classesTrainersGUID[Classes::CLASS_ROGUE][1] = 4584;

	classesTrainersGUID[Classes::CLASS_DRUID][0] = 4217;
	classesTrainersGUID[Classes::CLASS_DRUID][1] = 3034;

	classesTrainersGUID[Classes::CLASS_HUNTER][0] = 5115;
	classesTrainersGUID[Classes::CLASS_HUNTER][1] = 3352;

	classesTrainersGUID[Classes::CLASS_SHAMAN][0] = 20407;
	classesTrainersGUID[Classes::CLASS_SHAMAN][1] = 3344;

	classesTrainersGUID[Classes::CLASS_MAGE][0] = 5146;
	classesTrainersGUID[Classes::CLASS_MAGE][1] = 4566;

	classesTrainersGUID[Classes::CLASS_WARLOCK][0] = 5172;
	classesTrainersGUID[Classes::CLASS_WARLOCK][1] = 4563;

	classesTrainersGUID[Classes::CLASS_PRIEST][0] = 5141;
	classesTrainersGUID[Classes::CLASS_PRIEST][1] = 4606;

	for (uint32 talentId = 0; talentId < sTalentStore.GetNumRows(); ++talentId)
	{
		TalentEntry const* talentInfo = sTalentStore.LookupEntry(talentId);

		if (!talentInfo)
			continue;

		//TalentTabEntry const* talentTabInfo = sTalentTabStore.LookupEntry(talentInfo->TalentTab);

		//if (!talentTabInfo || talentTabInfo->tabpage < 0 || talentTabInfo->tabpage > 2)
		//	continue;
		//if ((talentTabInfo->GetClass()Mask & CLASSMASK_ALL_PLAYABLE) == 0)
		//	continue;
		//for (uint32 cls = 1; cls < MAX_CLASSES; ++cls)
		//{
		//	if (talentTabInfo->GetClass()Mask & (1 << (cls - 1)))
		//	{
		//		BotTalentPage& botPage = classesTalents[cls][talentTabInfo->tabpage];
		//		botPage.insert(BotTalentEntry(cls, talentTabInfo->tabpage, talentInfo));
		//	}
		//}
	}

	ItemTemplateContainer const* its = sObjectMgr->GetItemTemplateStore();
	for (ItemTemplateContainer::const_iterator itr = its->begin(); itr != its->end(); ++itr)
	{
		const ItemTemplate& item = itr->second;
		if (item.GetRequiredSkill() > 0 /*|| item.RequiredSpell > 0*/)// || item.AllowableRace != -1)
			continue;
		//if (item.RequiredReputationFaction != 0 || item.Area != 0 || item.Map != 0)
		//	continue;
		//if (item.Class != ItemClass::ITEM_CLASS_WEAPON && item.Class != ItemClass::ITEM_CLASS_ARMOR && item.Class != ItemClass::ITEM_CLASS_PROJECTILE)
		//	continue;
		
	//hxsd
		//if (item.ItemId	>=56806)
		//	continue;
		
		//if (item.Flags2 > 0)
		//	continue;
		if (item.GetBaseRequiredLevel() <= 0 || item.ExtendedData->InventoryType <= InventoryType::INVTYPE_NON_EQUIP || item.ExtendedData->InventoryType > InventoryType::INVTYPE_RELIC)
			continue;
		if (item.GetBaseRequiredLevel() < 20 && item.GetQuality() <= 1 && item.ExtendedData->InventoryType != InventoryType::INVTYPE_AMMO)
			continue;
		for (int i = 1; i < MAX_CLASSES; i++)
		{
			if (!IsEquipByClasses(i, &item))
				continue;
			//uint32 classMask = 1 << (i - 1);
			//if (item.AllowableClass & classMask)
			{
				InventoryType iType = InventoryType((item.ExtendedData->InventoryType == InventoryType::INVTYPE_ROBE) ? (InventoryType::INVTYPE_CHEST) : item.ExtendedData->InventoryType);
				BotEquips& equips = classesEquips[i][iType];
				BotEquips::iterator itEquip = equips.find(item.GetBaseRequiredLevel());
				ItemsForLevel& forLevel = (itEquip == equips.end()) ? equips[item.GetBaseRequiredLevel()] : itEquip->second;
				forLevel.AddItem(&item);
			}
		}
	}

	auto const* ctc = sObjectMgr->GetCreatureTemplates();
	for (auto itr = ctc->begin(); itr != ctc->end(); ++itr)
	{
		const CreatureTemplate& creature = itr->second;
		if (creature.type == CreatureType::CREATURE_TYPE_BEAST && (creature.type_flags & CreatureTypeFlags::CREATURE_TYPE_FLAG_TAMEABLE_PET))
			beastCreatureEntrys.push_back(creature.Entry);
	}

	classesCommonSpells[1].push_back(33388);
	classesCommonSpells[1].push_back(33391);
	classesCommonSpells[1].push_back(196);
	classesCommonSpells[1].push_back(197);
	classesCommonSpells[1].push_back(201);
	classesCommonSpells[1].push_back(202);
	classesCommonSpells[1].push_back(198);
	classesCommonSpells[1].push_back(199);
	classesCommonSpells[1].push_back(264);
	classesCommonSpells[1].push_back(5011);
	classesCommonSpells[1].push_back(266);
	classesCommonSpells[1].push_back(200);
	classesCommonSpells[1].push_back(2567);
	classesCommonSpells[1].push_back(71);
	classesCommonSpells[1].push_back(355);
	classesCommonSpells[1].push_back(2458);
	classesCommonSpells[1].push_back(7386);

	classesCommonSpells[2].push_back(33388);
	classesCommonSpells[2].push_back(33391);
	classesCommonSpells[2].push_back(201);
	classesCommonSpells[2].push_back(202);
	classesCommonSpells[2].push_back(198);
	classesCommonSpells[2].push_back(199);
	classesCommonSpells[2].push_back(200);
	classesCommonSpells[2].push_back(7328);

	classesCommonSpells[3].push_back(33388);
	classesCommonSpells[3].push_back(33391);
	classesCommonSpells[3].push_back(1515);
	classesCommonSpells[3].push_back(6991);
	classesCommonSpells[3].push_back(883);
	classesCommonSpells[3].push_back(982);
	classesCommonSpells[3].push_back(2641);
	classesCommonSpells[3].push_back(197);
	classesCommonSpells[3].push_back(202);
	classesCommonSpells[3].push_back(200);
	classesCommonSpells[3].push_back(264);
	classesCommonSpells[3].push_back(5011);
	classesCommonSpells[3].push_back(266);

	classesCommonSpells[4].push_back(33388);
	classesCommonSpells[4].push_back(33391);
	classesCommonSpells[4].push_back(196);
	classesCommonSpells[4].push_back(201);
	classesCommonSpells[4].push_back(198);
	classesCommonSpells[4].push_back(1180);
	classesCommonSpells[4].push_back(15590);
	classesCommonSpells[4].push_back(2567);

	classesCommonSpells[5].push_back(33388);
	classesCommonSpells[5].push_back(33391);
	classesCommonSpells[5].push_back(1180);
	classesCommonSpells[5].push_back(5009);
	classesCommonSpells[5].push_back(227);
	classesCommonSpells[5].push_back(198);

	classesCommonSpells[6].push_back(33388);
	classesCommonSpells[6].push_back(33391);
	classesCommonSpells[6].push_back(197);
	classesCommonSpells[6].push_back(202);
	classesCommonSpells[6].push_back(199);
	classesCommonSpells[6].push_back(200);

	classesCommonSpells[7].push_back(33388);
	classesCommonSpells[7].push_back(33391);
	classesCommonSpells[7].push_back(196);
	classesCommonSpells[7].push_back(197);
	classesCommonSpells[7].push_back(198);
	classesCommonSpells[7].push_back(199);

	classesCommonSpells[8].push_back(33388);
	classesCommonSpells[8].push_back(33391);
	classesCommonSpells[8].push_back(1180);
	classesCommonSpells[8].push_back(5009);
	classesCommonSpells[8].push_back(227);
	classesCommonSpells[8].push_back(201);

	classesCommonSpells[9].push_back(33388);
	classesCommonSpells[9].push_back(33391);
	classesCommonSpells[9].push_back(1180);
	classesCommonSpells[9].push_back(5009);
	classesCommonSpells[9].push_back(227);
	classesCommonSpells[9].push_back(201);
	classesCommonSpells[9].push_back(712);
	classesCommonSpells[9].push_back(697);
	classesCommonSpells[9].push_back(691);

	classesCommonSpells[11].push_back(33388);
	classesCommonSpells[11].push_back(33391);
	classesCommonSpells[11].push_back(1180);
	classesCommonSpells[11].push_back(227);
	classesCommonSpells[11].push_back(198);
	classesCommonSpells[11].push_back(199);

	normalMountSpells.push_back(6777);
	fastMountSpells.push_back(23240);
	fastMountSpells.push_back(22717);
	fastMountSpells.push_back(22720);
	fastMountSpells.push_back(22719);
	fastMountSpells.push_back(22723);
	fastMountSpells.push_back(48027);
	fastMountSpells.push_back(42777);
	fastMountSpells.push_back(74918);
	fastMountSpells.push_back(73313);
	fastMountSpells.push_back(51412);
	fastMountSpells.push_back(65644);
	fastMountSpells.push_back(64659);
	fastMountSpells.push_back(65646);
	fastMountSpells.push_back(66091);
	fastMountSpells.push_back(65641);
	fastMountSpells.push_back(68188);
	fastMountSpells.push_back(66090);
	fastMountSpells.push_back(41252);
	fastMountSpells.push_back(68057);

	botFlyMountEntrys.insert(69395);
	botFlyMountEntrys.insert(67336);
	botFlyMountEntrys.insert(65439);
	botFlyMountEntrys.insert(32345);
	botFlyMountEntrys.insert(46199);
	botFlyMountEntrys.insert(75596);

	TC_LOG_INFO("server.loading", "Initialize all classes trainer talent and spell and equip.");
}

bool PlayerBotSetting::BindingPlayerHomePosition(Player* player)
{
	if (!player || !player->IsInWorld() || player->GetMap()->IsDungeon())
		return false;
	if (!MapManager::IsValidMapCoord(player->GetMapId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation()))
		return false;

	uint32 bindspell = 3286;

	// send spell for homebinding (3286)
	player->CastSpell(player, bindspell, true);
	return true;
}

PlayerBotSetting::PlayerBotSetting(Player* player) :
m_Finish(true),
m_TenacitySetting(false),
m_ResetStep(0),
m_Player(player),
m_ActiveTalentType(3)
{
}

PlayerBotSetting::~PlayerBotSetting()
{
}

uint32 PlayerBotSetting::UpdateTalentType()
{
	if (m_Player->IsPlayerBot())
	{
		uint32 lv = (m_Player->getLevel() < 9) ? 9 : m_Player->getLevel();

	}
	if (m_ActiveTalentType >= 3)
	{
		m_ActiveTalentType = PlayerBotSetting::FindPlayerTalentType(m_Player);
	}
	return m_ActiveTalentType;
}

uint32 PlayerBotSetting::GetTalentType()
{

	return m_ActiveTalentType;
}

bool PlayerBotSetting::ResetPlayerToLevel(uint32 level, uint32 talent, bool tenacity)
{
	if (!m_Player)
		return false;
	if (talent <= 2)
		m_ActiveTalentType = talent;
	else
		m_ActiveTalentType = 3;
	//TC_LOG_INFO("server.reset", ">> Reset player level to %d !", level);
	if (m_Player->getLevel() != level)
	{
		m_Player->GiveLevel(level);
		m_Player->SetUInt32Value(PLAYER_XP, 0);
	}
	m_ResetStep = 1;
	m_Finish = false;
	m_TenacitySetting = tenacity;
	return true;
}

uint32 PlayerBotSetting::SwitchPlayerTalent(uint32 talent)
{
	return m_ActiveTalentType;
}

void PlayerBotSetting::SupplementAmmo()
{
	
}

void PlayerBotSetting::UpdateReset()
{
	if (m_Finish)
		return;

	if (m_Player->IsInCombat())
		m_Player->CombatStop(true);
	switch (m_ResetStep)
	{
	case 1:
		m_Player->ResetTalents(true);
		++m_ResetStep;
		break;
	case 2:
		LearnTalents();
		++m_ResetStep;
		break;
	case 3:
		RemoveSpells();
		++m_ResetStep;
		break;
	case 4:
		LearnCommonSpells();
		++m_ResetStep;
		break;
	case 5:
		LearnSpells();
		++m_ResetStep;
		break;
	case 6:
		UnequipFromAll();
		++m_ResetStep;
		break;
	case 7:
		CheckInventroy();
		++m_ResetStep;
		break;
	case 8:
		AddEquipFromAll();
		++m_ResetStep;
		break;
	case 9:
		UpequipFromAll();
		++m_ResetStep;
		break;
	case 10:
		SupplementOtherItems();
		++m_ResetStep;
		break;
	case 11:
		if (!m_Player->IsPlayerBot())
			m_Player->SendTalentsInfoData();
		++m_ResetStep;
		break;
	case 12:
		if (m_Player->IsPlayerBot())
			CheckHunterPet(m_Player);
		++m_ResetStep;
		break;
	case 13:
		PlayerBotSetting::ClearUnknowMount(m_Player);
		m_Player->SetFullHealth();
		m_Player->UpdateSkillsForLevel();
		m_Player->UpdateAllStats();
		m_Player->SaveToDB();
		++m_ResetStep;
		break;
	}

	m_Finish = (m_ResetStep >= 14);
	if (m_Finish && m_Player->IsPlayerBot())
	{
		BotGlobleSchedule schedule(BotGlobleScheduleType::BGSType_DelayLevelup, m_Player->GetGUID());
		PlayerBotSession* pSession = dynamic_cast<PlayerBotSession*>(m_Player->GetSession());
		if (pSession)
			pSession->PushScheduleToQueue(schedule);
	}
	if (m_Finish)
		m_TenacitySetting = false;
}

void PlayerBotSetting::LearnTalents()
{
	
}

void PlayerBotSetting::LearnCommonSpells()
{
	uint8 cls = m_Player->getClass();
	if (cls <= 0 || cls >= 12 || cls == 10)
		return;
	BotCommonSpells& commonSpells = classesCommonSpells[cls];
	for (BotCommonSpells::iterator itSpell = commonSpells.begin();
		itSpell != commonSpells.end();
		itSpell++)
	{
		uint32 spellID = *itSpell;
		if (m_Player->HasSpell(spellID))
			continue;
		m_Player->LearnSpell(spellID, false);
	}

	for (BeastCreatureEntrys::iterator itMount = normalMountSpells.begin();
		itMount != normalMountSpells.end();
		itMount++)
	{
		uint32 spellID = *itMount;
		if (m_Player->HasSpell(spellID))
			continue;
		m_Player->LearnSpell(spellID, false);
	}

	if (m_Player->IsPlayerBot())
	{
		for (BeastCreatureEntrys::iterator itMount = fastMountSpells.begin();
			itMount != fastMountSpells.end();
			itMount++)
		{
			uint32 spellID = *itMount;
			if (m_Player->HasSpell(spellID))
				continue;
			m_Player->LearnSpell(spellID, false);
		}
	}
}

void PlayerBotSetting::RemoveSpells()
{
	const TrainerSpellData* spellData = sObjectMgr->GetNpcTrainerSpells(classesTrainersGUID[m_Player->getClass()][(m_Player->GetTeamId() == TeamId::TEAM_ALLIANCE) ? 0 : 1]);
	if (!spellData)
		return;
	for (TrainerSpellMap::const_iterator itBTS = spellData->spellList.begin();
		itBTS != spellData->spellList.end();
		itBTS++)
	{
		const TrainerSpell& tSpell = itBTS->second;
		uint32 spellID = tSpell.SpellID;
		if (m_Player->HasSpell(spellID))
		{
			m_Player->RemoveSpell(spellID, false, false);
		}
	}
}

void PlayerBotSetting::LearnSpells()
{
	uint8 level = m_Player->getLevel();
	const TrainerSpellData* spellData = sObjectMgr->GetNpcTrainerSpells(classesTrainersGUID[m_Player->getClass()][(m_Player->GetTeamId() == TeamId::TEAM_ALLIANCE) ? 0 : 1]);
	if (!spellData)
		return;
	for (TrainerSpellMap::const_iterator itBTS = spellData->spellList.begin();
		itBTS != spellData->spellList.end();
		itBTS++)
	{
		const TrainerSpell& tSpell = itBTS->second;
		uint32 spellID = tSpell.SpellID;
		if (tSpell.ReqLevel > level)
		{
			continue;
		}
		if (m_Player->HasSpell(spellID))
			continue;
		if (tSpell.IsCastable())
		{
			m_Player->CastSpell(m_Player, spellID, true);
			//TC_LOG_WARN("PlayerBotSetting", "Player %s Learn spell %d, spell is castable, do cast.", m_Player->GetName().c_str(), spellID);
		}
		else
			m_Player->LearnSpell(spellID, false);
	}
}

void PlayerBotSetting::CheckInventroy()
{
	if (!m_Player->GetItemByPos(255, InventorySlots::INVENTORY_SLOT_BAG_START) ||
		!m_Player->GetItemByPos(255, InventorySlots::INVENTORY_SLOT_BAG_START + 1) ||
		!m_Player->GetItemByPos(255, InventorySlots::INVENTORY_SLOT_BAG_START + 2))
	{
		const ItemTemplate* item = sObjectMgr->GetItemTemplate(21876);
		uint32 count = 1;
		uint32 noSpaceForCount = 0;
		ItemPosCountVec dest;
		InventoryResult msg = m_Player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, item->GetId(), count, &noSpaceForCount);
		if (msg != EQUIP_ERR_OK)
			count -= noSpaceForCount;
		if (count <= 0 || dest.empty())
			return;
		Item* itemInst = m_Player->StoreNewItem(dest, item->GetId(), true, GenerateItemRandomPropertyId(item->GetId()));
		if (itemInst)
			EquipItem(itemInst);
	}
}

void PlayerBotSetting::UnequipFromAll()
{
	bool isBot = m_Player->IsPlayerBot();
	for (uint8 slot = EquipmentSlots::EQUIPMENT_SLOT_HEAD; slot < EquipmentSlots::EQUIPMENT_SLOT_END; slot++)
	{
		uint16 pos = (255 << 8) | slot;
		if (m_Player->IsEquipmentPos(pos) || m_Player->IsBagPos(pos))
		{
			InventoryResult msg = m_Player->CanUnequipItem(pos, false);
			if (msg != EQUIP_ERR_OK)
				continue;
		}
		Item* pItem = m_Player->GetItemByPos(255, slot);
		if (!pItem)
			continue;
		m_Player->DestroyItem(255, slot, true);
	}

	for (uint8 slot = InventoryPackSlots::INVENTORY_SLOT_ITEM_START+1; slot < InventoryPackSlots::INVENTORY_SLOT_ITEM_END; slot++)
	{
		Item* pItem = m_Player->GetItemByPos(255, slot);
		if (!pItem)
			continue;
		if (!isBot && pItem->GetEntry() == 6948)
			continue;
		m_Player->DestroyItem(255, slot, true);
	}

	for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
	{
		if (Bag* pBag = m_Player->GetBagByPos(i))
		{
			for (uint32 j = 0; j < pBag->GetBagSize(); j++)
			{
				Item* pItem = pBag->GetItemByPos(uint8(j));
				if (!pItem)
					continue;
				if (!isBot && pItem->GetEntry() == 6948)
					continue;
				m_Player->DestroyItem(i, uint8(j), true);
			}
		}
	}

}

void PlayerBotSetting::AddEquipFromAll()
{
	m_NeedEquips.clear();
	uint32 level = m_Player->getLevel();
	uint8 prof = m_Player->getClass();
	if (prof <= 0 || prof >= MAX_CLASSES)
		return;
	const ItemTemplate* firstFinger = NULL;
	const ItemTemplate* firstTrinket = NULL;
	for (int i = 0; i < InventoryType::INVTYPE_RELIC; i++)
	{
		//BotEquips& equips = classesEquips[prof][i];
		//BotEquips::iterator itEquip = equips.find(level);
		//if (itEquip == equips.end())
		//	continue;
		if (i != InventoryType::INVTYPE_CLOAK && i > InventoryType::INVTYPE_TRINKET)
			continue;
		if (i == InventoryType::INVTYPE_FINGER && !firstFinger)
		{
			const ItemTemplate* item = GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_FINGER, level, firstFinger);
			firstFinger = item;
			AddOnceEquip(item);
		}
		else if (i == InventoryType::INVTYPE_TRINKET && !firstTrinket)
		{
			const ItemTemplate* item = GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_TRINKET, level, firstFinger);
			firstTrinket = item;
			AddOnceEquip(item);
		}
		else
		{
			const ItemTemplate* item = GetRandomItemFromLoopLV(prof, (InventoryType)i, level);
			AddOnceEquip(item);
		}
	}
	if (firstFinger)
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_FINGER, level, firstFinger));
	if (firstTrinket)
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_TRINKET, level, firstTrinket));

	switch (prof)
	{
	case 1:
		RandomWeaponByWarrior();
		break;
	case 2:
		RandomWeaponByPaladin();
		break;
	case 3:
		RandomWeaponByHunter();
		break;
	case 4:
		RandomWeaponByRogue();
		break;
	case 5:
		RandomWeaponByPriest();
		break;
	case 6:
		RandomWeaponByDeathKight();
		break;
	case 7:
		RandomWeaponByShaman();
		break;
	case 8:
		RandomWeaponByMage();
		break;
	case 9:
		RandomWeaponByWarlock();
		break;
	case 11:
		RandomWeaponByDruid();
		break;
	default:
		break;
	}
}

void PlayerBotSetting::UpequipFromAll()
{
	for (BotNeedEquips::iterator itNeed = m_NeedEquips.begin();
		itNeed != m_NeedEquips.end();
		itNeed++)
	{
		Item* itemInst = (*itNeed);
		if (itemInst->GetTemplate()->GetInventoryType() == InventoryType::INVTYPE_AMMO)
		{
			//m_Player->SetAmmo(itemInst->GetEntry());
		}
		else
			EquipItem(itemInst);
	}
	m_NeedEquips.clear();
}

bool PlayerBotSetting::EquipItem(Item* pItem)
{
	uint16 dest;
	InventoryResult msg = m_Player->CanEquipItem(NULL_SLOT, dest, pItem, !pItem->IsBag());
	if (msg != EQUIP_ERR_OK)
	{
		return false;
	}

	uint16 src = pItem->GetPos();
	if (dest == src)                                           // prevent equip in same slot, only at cheat
		return false;

	Item* pDstItem = m_Player->GetItemByPos(dest);
	if (!pDstItem)                                         // empty slot, simple case
	{
		m_Player->RemoveItem(pItem->GetBagSlot(), pItem->GetSlot(), true);
		m_Player->EquipItem(dest, pItem, true);
		m_Player->AutoUnequipOffhandIfNeed();
	}
	else                                                    // have currently equipped item, not simple case
	{
		uint8 dstbag = pDstItem->GetBagSlot();
		uint8 dstslot = pDstItem->GetSlot();

		msg = m_Player->CanUnequipItem(dest, !pItem->IsBag());
		if (msg != EQUIP_ERR_OK)
		{
			return false;
		}

		// check dest->src move possibility
		ItemPosCountVec sSrc;
		uint16 eSrc = 0;
		if (m_Player->IsInventoryPos(src))
		{
			msg = m_Player->CanStoreItem(pItem->GetBagSlot(), pItem->GetSlot(), sSrc, pDstItem, true);
			if (msg != EQUIP_ERR_OK)
				msg = m_Player->CanStoreItem(pItem->GetBagSlot(), NULL_SLOT, sSrc, pDstItem, true);
			if (msg != EQUIP_ERR_OK)
				msg = m_Player->CanStoreItem(NULL_BAG, NULL_SLOT, sSrc, pDstItem, true);
		}
		else if (m_Player->IsBankPos(src))
		{
			msg = m_Player->CanBankItem(pItem->GetBagSlot(), pItem->GetSlot(), sSrc, pDstItem, true);
			if (msg != EQUIP_ERR_OK)
				msg = m_Player->CanBankItem(pItem->GetBagSlot(), NULL_SLOT, sSrc, pDstItem, true);
			if (msg != EQUIP_ERR_OK)
				msg = m_Player->CanBankItem(NULL_BAG, NULL_SLOT, sSrc, pDstItem, true);
		}
		else if (m_Player->IsEquipmentPos(src))
		{
			msg = m_Player->CanEquipItem(pItem->GetSlot(), eSrc, pDstItem, true);
			if (msg == EQUIP_ERR_OK)
				msg = m_Player->CanUnequipItem(eSrc, true);
		}

		if (msg != EQUIP_ERR_OK)
		{
			return false;
		}

		// now do moves, remove...
		m_Player->RemoveItem(dstbag, dstslot, false);
		m_Player->RemoveItem(pItem->GetBagSlot(), pItem->GetSlot(), false);

		// add to dest
		m_Player->EquipItem(dest, pItem, true);

		// add to src
		if (m_Player->IsInventoryPos(src))
			m_Player->StoreItem(sSrc, pDstItem, true);
		else if (m_Player->IsBankPos(src))
			m_Player->BankItem(sSrc, pDstItem, true);
		else if (m_Player->IsEquipmentPos(src))
			m_Player->EquipItem(eSrc, pDstItem, true);

		m_Player->AutoUnequipOffhandIfNeed();
	}
	return true;
}

void PlayerBotSetting::SupplementOtherItems()
{
	uint8 prof = m_Player->getClass();
	switch (prof)
	{
	case 1:
		SupplementItemByWarrior();
		break;
	case 2:
		SupplementItemByPaladin();
		break;
	}
}

const ItemTemplate* PlayerBotSetting::GetRandomAmmoByType(ItemSubclassProjectile iType, uint32 startLV)
{
	BotEquips& equips = classesEquips[3][InventoryType::INVTYPE_AMMO];
	for (int i = startLV; i > 0; i--)
	{
		BotEquips::iterator itEquip = equips.find(i);
		if (itEquip == equips.end() || itEquip->second.m_Items.size() <= 0)
			continue;
		for (int j = 0; j < 3; j++)
		{
			const ItemTemplate* item = itEquip->second.RandomItem();
			if (item && item->GetSubClass() == iType)
				return item;
		}
	}
	return NULL;
}

const ItemTemplate* PlayerBotSetting::GetRandomItemFromLoopLV(uint32 prof, InventoryType iType, uint32 startLV, const ItemTemplate* filter)
{
	BotEquips& equips = classesEquips[prof][iType];
	for (int i = startLV; i > 0; i--)
	{
		BotEquips::iterator itEquip = equips.find(i);
		if (itEquip == equips.end())
			continue;
		if (m_TenacitySetting)
		{
			for (int j = 0; j < 2; j++)
			{
				const ItemTemplate* item = itEquip->second.RandomTenacityItem();
				if (item && item != filter)
					return item;
			}
		}
		for (int j = 0; j < 2; j++)
		{
			const ItemTemplate* item = itEquip->second.RandomItem();
			if (item && item != filter)
				return item;
		}
	}
	return NULL;
}

bool PlayerBotSetting::IsTenacityEquipSlot(uint8 slot)
{
	switch (EquipmentSlots(slot))
	{
	case EquipmentSlots::EQUIPMENT_SLOT_HEAD:
	case EquipmentSlots::EQUIPMENT_SLOT_NECK:
	case EquipmentSlots::EQUIPMENT_SLOT_SHOULDERS:
	case EquipmentSlots::EQUIPMENT_SLOT_BODY:
	case EquipmentSlots::EQUIPMENT_SLOT_CHEST:
	case EquipmentSlots::EQUIPMENT_SLOT_WAIST:
	case EquipmentSlots::EQUIPMENT_SLOT_LEGS:
	case EquipmentSlots::EQUIPMENT_SLOT_FEET:
	case EquipmentSlots::EQUIPMENT_SLOT_WRISTS:
	case EquipmentSlots::EQUIPMENT_SLOT_HANDS:
	case EquipmentSlots::EQUIPMENT_SLOT_FINGER1:
	case EquipmentSlots::EQUIPMENT_SLOT_FINGER2:
	case EquipmentSlots::EQUIPMENT_SLOT_BACK:
	case EquipmentSlots::EQUIPMENT_SLOT_MAINHAND:
	case EquipmentSlots::EQUIPMENT_SLOT_OFFHAND:
	case EquipmentSlots::EQUIPMENT_SLOT_RANGED:
		return true;
	}
	return false;
}

bool PlayerBotSetting::EquipIsTidiness()
{
	uint8 level = m_Player->getLevel();
	uint32 noMatchEquipCount = 0;
	for (uint8 slot = EquipmentSlots::EQUIPMENT_SLOT_HEAD; slot < EquipmentSlots::EQUIPMENT_SLOT_END; slot++)
	{
		Item* pItem = m_Player->GetItemByPos(255, slot);
		if (!pItem)
		{
			++noMatchEquipCount;
			if (level <= 20)
			{
				if (noMatchEquipCount > 10)
					return false;
			}
			else if (noMatchEquipCount >= 5)
				return false;
			continue;
		}
	}
	return true;
}

bool PlayerBotSetting::CheckNeedTenacityFlush()
{
	uint32 noMatchEquipCount = 0;
	for (uint8 slot = EquipmentSlots::EQUIPMENT_SLOT_HEAD; slot < EquipmentSlots::EQUIPMENT_SLOT_END; slot++)
	{
		if (!IsTenacityEquipSlot(slot))
			continue;
		//uint16 pos = (255 << 8) | slot;
		//if (m_Player->IsEquipmentPos(pos) || m_Player->IsBagPos(pos))
		//{
		//	InventoryResult msg = m_Player->CanUnequipItem(pos, false);
		//	if (msg != EQUIP_ERR_OK)
		//		continue;
		//}
		Item* pItem = m_Player->GetItemByPos(255, slot);
		if (!pItem)
		{
			++noMatchEquipCount;
			if (noMatchEquipCount > 2)
				return true;
			continue;
		}
		const ItemTemplate* pTemplate = pItem->GetTemplate();
		if (!ItemsForLevel::IsTenacityItem(pTemplate))
		{
			++noMatchEquipCount;
			if (noMatchEquipCount > 2)
				return true;
		}
	}
	return false;
}

bool PlayerBotSetting::IsTenacityInventoryType(InventoryType iType)
{
	if (iType == InventoryType::INVTYPE_NON_EQUIP || iType == InventoryType::INVTYPE_TRINKET || iType == InventoryType::INVTYPE_BAG ||
		iType == InventoryType::INVTYPE_TABARD || iType == InventoryType::INVTYPE_HOLDABLE || iType == InventoryType::INVTYPE_AMMO ||
		/*iType == InventoryType::INVTYPE_QUIVER||*/ iType == InventoryType::INVTYPE_RELIC)
		return false;
	return true;
}

void PlayerBotSetting::AddOnceEquip(const ItemTemplate* item)
{
	if (!item)
		return;
	//m_Player->AddItem(item->GetId(), 1);
	uint32 count = 1;
	if (item->GetClass() == ItemClass::ITEM_CLASS_PROJECTILE && item->GetInventoryType() == InventoryType::INVTYPE_AMMO)
		count = 1000;
	uint32 noSpaceForCount = 0;
	ItemPosCountVec dest;
	InventoryResult msg = m_Player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, item->GetId(), count, &noSpaceForCount);
	if (msg != EQUIP_ERR_OK)
		count -= noSpaceForCount;
	if (count <= 0 || dest.empty())
		return;
	Item* itemInst = m_Player->StoreNewItem(dest, item->GetId(), true, GenerateItemRandomPropertyId(item->GetId()));
	if (itemInst)
		m_NeedEquips.push_back(itemInst);
}

void PlayerBotSetting::RandomWeaponByWarrior()
{
	uint32 level = m_Player->getLevel();
	uint32 prof = 1;
	switch (m_ActiveTalentType)
	{
	case 0:
	{
		AddOnceEquip(GetRandomItemFromLoopLV(1, InventoryType::INVTYPE_2HWEAPON, level));
	}
	break;
	case 1:
	{
		ItemTemplate* item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPON, level);
		if (!item1)
			item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPONMAINHAND, level);
		AddOnceEquip(item1);
		ItemTemplate* item2 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPON, level, item1);
		if (!item2)
			item2 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPONOFFHAND, level);
		AddOnceEquip(item2);
	}
	break;
	case 2:
	{
		ItemTemplate* item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPON, level);
		if (!item1)
			item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPONMAINHAND, level);
		AddOnceEquip(item1);
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_SHIELD, level));
	}
	break;
	default:
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_2HWEAPON, level));
		break;
	}
	AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_THROWN, level));
}

void PlayerBotSetting::RandomWeaponByPaladin()
{
	uint32 level = m_Player->getLevel();
	uint32 prof = 2;
	switch (m_ActiveTalentType)
	{
	case 0:
	{
		ItemTemplate* item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPON, level);
		if (!item1)
			item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPONMAINHAND, level);
		AddOnceEquip(item1);
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_SHIELD, level));
	}
	break;
	case 1:
	{
		ItemTemplate* item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPON, level);
		if (!item1)
			item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPONMAINHAND, level);
		AddOnceEquip(item1);
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_SHIELD, level));
	}
	break;
	case 2:
	{
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_2HWEAPON, level));
	}
	break;
	default:
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_2HWEAPON, level));
		break;
	}
	AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_RELIC, level));
}

void PlayerBotSetting::RandomWeaponByDeathKight()
{
	uint32 level = m_Player->getLevel();
	uint32 prof = 6;
	switch (m_ActiveTalentType)
	{
	case 0:
	{
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_2HWEAPON, level));
	}
	break;
	case 1:
	{
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_2HWEAPON, level));
	}
	break;
	case 2:
	{
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_2HWEAPON, level));
	}
	break;
	default:
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_2HWEAPON, level));
		break;
	}
}

void PlayerBotSetting::RandomWeaponByRogue()
{
	uint32 level = m_Player->getLevel();
	uint32 prof = 4;
	ItemTemplate* item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPON, level);
	if (!item1)
		item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPONMAINHAND, level);
	AddOnceEquip(item1);
	ItemTemplate* item2 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPON, level, item1);
	if (!item2)
		item2 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPONOFFHAND, level);
	AddOnceEquip(item2);
	AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_THROWN, level));
}

void PlayerBotSetting::RandomWeaponByDruid()
{
	uint32 level = m_Player->getLevel();
	uint32 prof = 11;
	AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_2HWEAPON, level));
	AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_RELIC, level));
}

void PlayerBotSetting::RandomWeaponByHunter()
{
	uint32 level = m_Player->getLevel();
	uint32 prof = 3;
	AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_2HWEAPON, level));
	InventoryType iType = (irand(0, 99) < 50) ? InventoryType::INVTYPE_RANGED : InventoryType::INVTYPE_RANGEDRIGHT;
	const ItemTemplate* item = GetRandomItemFromLoopLV(prof, iType, level);
	AddOnceEquip(item);
	if (item && item->GetClass() == ItemClass::ITEM_CLASS_WEAPON)
	{
		switch (item->GetSubClass())
		{
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_BOW:
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_CROSSBOW:
			AddOnceEquip(GetRandomAmmoByType(ItemSubclassProjectile::ITEM_SUBCLASS_ARROW, level));
			break;
		case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_GUN:
			AddOnceEquip(GetRandomAmmoByType(ItemSubclassProjectile::ITEM_SUBCLASS_BULLET, level));
			break;
		default:
			break;
		}
	}
}

void PlayerBotSetting::RandomWeaponByShaman()
{
	uint32 level = m_Player->getLevel();
	uint32 prof = 7;
	switch (m_ActiveTalentType)
	{
	case 0:
	{
		ItemTemplate* item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPON, level);
		if (!item1)
			item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPONMAINHAND, level);
		AddOnceEquip(item1);
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_SHIELD, level));
	}
	break;
	case 1:
	{
		if (m_Player->HasSpell(30798)) // talent two weapon
		{
			ItemTemplate* item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPON, level);
			if (!item1)
				item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPONMAINHAND, level);
			AddOnceEquip(item1);
			ItemTemplate* item2 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPON, level, item1);
			if (!item2)
				item2 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPONOFFHAND, level);
			AddOnceEquip(item2);
		}
		else
			AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_2HWEAPON, level));
	}
	break;
	case 2:
	{
		ItemTemplate* item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPON, level);
		if (!item1)
			item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPONMAINHAND, level);
		AddOnceEquip(item1);
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_SHIELD, level));
	}
	break;
	default:
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_2HWEAPON, level));
		break;
	}
	AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_RELIC, level));
}

void PlayerBotSetting::RandomWeaponByMage()
{
	uint32 level = m_Player->getLevel();
	uint32 prof = 8;
	InventoryType iType = (irand(0, 99) < 50) ? InventoryType::INVTYPE_2HWEAPON : InventoryType::INVTYPE_WEAPON;
	ItemTemplate* item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, iType, level);
	if (!item1)
		item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPONMAINHAND, level);
	AddOnceEquip(item1);
	if (iType == InventoryType::INVTYPE_WEAPON)
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_HOLDABLE, level, item1));
	AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_RANGEDRIGHT, level));
}

void PlayerBotSetting::RandomWeaponByWarlock()
{
	uint32 level = m_Player->getLevel();
	uint32 prof = 9;
	InventoryType iType = (irand(0, 99) < 50) ? InventoryType::INVTYPE_2HWEAPON : InventoryType::INVTYPE_WEAPON;
	ItemTemplate* item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, iType, level);
	if (!item1)
		item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPONMAINHAND, level);
	AddOnceEquip(item1);
	if (iType == InventoryType::INVTYPE_WEAPON)
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_HOLDABLE, level, item1));
	AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_RANGEDRIGHT, level));
}

void PlayerBotSetting::RandomWeaponByPriest()
{
	uint32 level = m_Player->getLevel();
	uint32 prof = 5;
	InventoryType iType = (irand(0, 99) < 50) ? InventoryType::INVTYPE_2HWEAPON : InventoryType::INVTYPE_WEAPON;
	ItemTemplate* item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, iType, level);
	if (!item1)
		item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPONMAINHAND, level);
	AddOnceEquip(item1);
	if (iType == InventoryType::INVTYPE_WEAPON)
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_HOLDABLE, level, item1));
	AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_RANGEDRIGHT, level));
}

void PlayerBotSetting::SupplementItemByWarrior()
{
	uint32 level = m_Player->getLevel();
	uint32 prof = 1;
	switch (m_ActiveTalentType)
	{
	case 0:
	{
		ItemTemplate* item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPON, level);
		if (!item1)
			item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPONMAINHAND, level);
		AddOnceEquip(item1);
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_SHIELD, level));
	}
	break;
	case 1:
	{
		AddOnceEquip(GetRandomItemFromLoopLV(1, InventoryType::INVTYPE_2HWEAPON, level));
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_SHIELD, level));
	}
	break;
	case 2:
	{
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPON, level));
		AddOnceEquip(GetRandomItemFromLoopLV(1, InventoryType::INVTYPE_2HWEAPON, level));
	}
	break;
	}
	m_NeedEquips.clear();
}

void PlayerBotSetting::SupplementItemByPaladin()
{
	uint32 level = m_Player->getLevel();
	uint32 prof = 2;
	switch (m_ActiveTalentType)
	{
	case 0:
	case 1:
	{
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_2HWEAPON, level));
	}
	break;
	case 2:
	{
		ItemTemplate* item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPON, level);
		if (!item1)
			item1 = (ItemTemplate*)GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_WEAPONMAINHAND, level);
		AddOnceEquip(item1);
		AddOnceEquip(GetRandomItemFromLoopLV(prof, InventoryType::INVTYPE_SHIELD, level));
	}
	break;
	}
	m_NeedEquips.clear();
}
