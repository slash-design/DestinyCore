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

#ifndef _BOT_WARRIOR_AI_H_
#define _BOT_WARRIOR_AI_H_

#include "ScriptSystem.h"
#include "BotAI.h"
#include "AIWaypointsMgr.h"

enum WarriorTalentType
{
	WarriorTalent_Weapon,
	WarriorTalent_Defance,
	WarriorTalent_Rage
};

class TC_GAME_API BotWarriorAI : public BotBGAI
{
public:
	BotWarriorAI(Player* player) :
		BotBGAI(player),
		m_NeedPuniness(-1),
		m_BotTalentType(0),
		m_ChargeWaitTick(0),
		m_IsIDLEBuff(false)
	{}
	~BotWarriorAI() {}

	void ResetBotAI() override;

protected:
	uint32 GetRagePowerPer();
	void EachTick() override;
	void ClearMechanicAura() override;
	bool NeedWaitSpecialSpell(uint32 diff) override;
	void InitializeSpells();
	void UpdateTalentType();
	void ProcessReady() override;
	bool ProcessNormalSpell() override;
	void ProcessMeleeSpell(Unit* pTarget) override;
	void ProcessRangeSpell(Unit* pTarget) override;
	void ProcessFlee() override;

	void UpdateWarriorPose();

	void ProcessWeaponRangeSpell(Unit* pTarget);
	void ProcessRageRangeSpell(Unit* pTarget);
	void ProcessDefanceRangeSpell(Unit* pTarget);
	void ProcessWeaponMeleeSpell(Unit* pTarget);
	void ProcessRageMeleeSpell(Unit* pTarget);
	void ProcessDefanceMeleeSpell(Unit* pTarget);
	bool ProcessSurviveSpell();
	bool ProcessFullAttack(Unit* pTarget);
	void OnCastCharge(Unit* pTarget);

private:
	int32 m_NeedPuniness;
	uint32 m_BotTalentType;
	int32 m_ChargeWaitTick;
	bool m_IsIDLEBuff;

	uint32 WarriorIDLE_AOEAddLife;// = 47440;			// ÃüÁîÅ­ºð
	uint32 WarriorIDLE_AOEAddPower;// = 47436;			// ¹¥Ç¿Å­ºð

	uint32 WarriorWeapon_Status;// = 2457;
	uint32 WarriorDefance_Status;// = 71;
	uint32 WarriorRage_Status;// = 2458;

	uint32 WarriorCommon_PowerAtt;// = 47450;			// Ó¢ÓÂ´ò»÷
	uint32 WarriorCommon_PowerThrow;// = 57755;			// Ó¢ÓÂÍ¶ÖÀ
	uint32 WarriorCommon_PowerRelife;// = 55694;		// ¿ñÅ­»Ö¸´£¨ÓÐ¿ñ±©Ê±Ê¹ÓÃ»ØÑª£©
	uint32 WarriorCommon_ClearCtrl;// = 18499;			// ¿ñ±©Ö®Å­£¨ÒÆ³ý¿ØÖÆ£©
	uint32 WarriorCommon_AOEFear;// = 5246;			// ½üÕ½·¶Î§Èº¿Ö¾å
	uint32 WarriorCommon_SweepAtt;// = 47520;			// Ë³ÅüÕ¶
	uint32 WarriorCommon_AddPower;// = 2687;			// ¼ÓÅ­Æø
	uint32 WarriorCommon_AOEDecPower;// = 47437;		// ÈºÌå¼õ¹¥Ç¿ºð

	uint32 WarriorDefance_HPojia;// = 47498;			// »ÙÃð´ò»÷
	uint32 WarriorDefance_Fuchou;// = 57823;			// ¸´³ð
	uint32 WarriorDefance_ShieldBlock;// = 2565;		// ¶ÜÅÆ¸ñµ²
	uint32 WarriorDefance_ShieldAtt;// = 47488;			// ¶ÜÅÆÃÍ»÷
	uint32 WarriorDefance_Pojia;// = 7386;				// ÆÆ¼×
	uint32 WarriorDefance_MaxLife;// = 12975;			// ÆÆ¸ª³ÁÖÛ
	uint32 WarriorDefance_ShiledWall;// = 871;			// ¶ÜÇ½
	uint32 WarriorDefance_Disarm;// = 676;				// ½ÉÐµ
	uint32 WarriorDefance_Support;// = 3411;			// Ô®Öú
	uint32 WarriorDefance_Conk;// = 12809;				// µ¥Ìå»÷ÔÎ
	uint32 WarriorDefance_AOEConk;// = 46968;			// Ç°·½·¶Î§»÷ÔÎ

	uint32 WarriorWeaponDefance_AOEAtt;// = 47502;		// À×öªÒ»»÷
	uint32 WarriorWeaponDefance_Bleed;// = 47465;		// ËºÁÑ
	uint32 WarriorWeaponDefance_SpellReflect;// = 23920;	// ·¨Êõ·´Éä
	uint32 WarriorWeaponDefance_ShieldHit;// = 72;		// ¶Ü»÷£¨Ê©·¨´ò¶Ï£©

	uint32 WarriorWeapon_SwordStorm;// = 46924;			// ½£ÈÐ·ç±©
	uint32 WarriorWeapon_HighThrow;// = 64382;			// ÆÆÎÞµÐÍ¶ÖÀ
	uint32 WarriorWeapon_Charge;// = 11578;			// ³å·æ
	uint32 WarriorWeapon_Suppress;// = 7384;			// Ñ¹ÖÆ
	uint32 WarriorWeapon_Backstorm;// = 20230;			// ·´»÷·ç±©
	uint32 WarriorWeapon_DeadAtt;// = 47486;			// ÖÂËÀ´ò»÷

	uint32 WarriorWeaponRage_FullKill;// = 47471;		// Õ¶É±
	uint32 WarriorWeaponRage_WinAttack;// = 34428;		// ³ËÊ¤×·»÷
	uint32 WarriorWeaponRage_Backfillet;// = 1715;		// ¶Ï½î

	uint32 WarriorRage_Harsh;// = 12323;				// ´Ì¶úÅ­ºð
	uint32 WarriorRage_HeadAtt;// = 6552;				// È­»÷£¨Ê©·¨´ò¶Ï£©
	uint32 WarriorRage_Intercept;// = 20252;			// À¹½Ø
	uint32 WarriorRage_Whirlwind;// = 1680;			// Ðý·çÕ¶
	uint32 WarriorRage_Impertinency;// = 1719;			// Â³Ã§
	uint32 WarriorRage_Needdead;// = 12292;			// ËÀÍöÖ®Ô¸
	uint32 WarriorRage_Bloodthirsty;// = 23881;			// ÊÈÑª
	uint32 WarriorRage_ReIntercept;// = 60970;			// ½â³ýÒÆ¶¯ÏÞÖÆºÍ½â³ýÀ¹½ØCD
};

#endif // !_BOT_WARRIOR_AI_H_
