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

#include "BotAISpells.h"

void BotWarriorSpells::InitializeSpells(Player* player)
{
	WarriorIDLE_AOEAddLife = BotUtility::FindMaxRankSpellByExist(player, 45517);
	WarriorIDLE_AOEAddPower = BotUtility::FindMaxRankSpellByExist(player, 27578);
	WarriorWeapon_Status = BotUtility::FindMaxRankSpellByExist(player, 122990);
	WarriorDefance_Status = BotUtility::FindMaxRankSpellByExist(player, 71);
	WarriorRage_Status = BotUtility::FindMaxRankSpellByExist(player, 122989);
	WarriorCommon_PowerAtt = BotUtility::FindMaxRankSpellByExist(player, 25712);
	WarriorCommon_PowerThrow = BotUtility::FindMaxRankSpellByExist(player, 57755);
	WarriorCommon_PowerRelife = BotUtility::FindMaxRankSpellByExist(player, 184364);
	WarriorCommon_ClearCtrl = BotUtility::FindMaxRankSpellByExist(player, 18499);
	WarriorCommon_AOEFear = BotUtility::FindMaxRankSpellByExist(player, 65930);
	WarriorCommon_SweepAtt = BotUtility::FindMaxRankSpellByExist(player, 231833);
	WarriorCommon_AddPower = BotUtility::FindMaxRankSpellByExist(player, 173401);
	WarriorCommon_AOEDecPower = BotUtility::FindMaxRankSpellByExist(player, 27579);
	WarriorDefance_HPojia = BotUtility::FindMaxRankSpellByExist(player, 186688);
	WarriorDefance_Fuchou = BotUtility::FindMaxRankSpellByExist(player, 40392);
	WarriorDefance_ShieldBlock = BotUtility::FindMaxRankSpellByExist(player, 38031);
	WarriorDefance_ShieldAtt = BotUtility::FindMaxRankSpellByExist(player, 23922);
	WarriorDefance_Pojia = BotUtility::FindMaxRankSpellByExist(player, 25051);
	WarriorDefance_MaxLife = BotUtility::FindMaxRankSpellByExist(player, 12975);
	WarriorDefance_ShiledWall = BotUtility::FindMaxRankSpellByExist(player, 871);
	WarriorDefance_Disarm = BotUtility::FindMaxRankSpellByExist(player, 11879);
	WarriorDefance_Support = BotUtility::FindMaxRankSpellByExist(player, 41198);
	WarriorDefance_Conk = BotUtility::FindMaxRankSpellByExist(player, 22427);
	WarriorDefance_AOEConk = BotUtility::FindMaxRankSpellByExist(player, 46968);
	WarriorDefance_Pull = BotUtility::FindMaxRankSpellByExist(player, 355);
	WarriorWeapon_SwordStorm = BotUtility::FindMaxRankSpellByExist(player, 9632);
	WarriorWeaponDefance_AOEAtt = BotUtility::FindMaxRankSpellByExist(player, 165749);
	WarriorWeaponDefance_Bleed = BotUtility::FindMaxRankSpellByExist(player, 43931);
	WarriorWeaponDefance_SpellReflect = BotUtility::FindMaxRankSpellByExist(player, 57643);
	WarriorWeaponDefance_ShieldHit = BotUtility::FindMaxRankSpellByExist(player, 149383);
	WarriorWeapon_HighThrow = BotUtility::FindMaxRankSpellByExist(player, 65940);
	WarriorWeapon_Charge = BotUtility::FindMaxRankSpellByExist(player, 100);
	WarriorWeapon_Suppress = BotUtility::FindMaxRankSpellByExist(player, 7384);
	WarriorWeapon_Backstorm = BotUtility::FindMaxRankSpellByExist(player, 22857);
	WarriorWeapon_DeadAtt = BotUtility::FindMaxRankSpellByExist(player, 27580);
	WarriorWeaponRage_FullKill = BotUtility::FindMaxRankSpellByExist(player, 5308);
	WarriorWeaponRage_WinAttack = BotUtility::FindMaxRankSpellByExist(player, 34428);
	WarriorWeaponRage_Backfillet = BotUtility::FindMaxRankSpellByExist(player, 1715);
	WarriorRage_Harsh = BotUtility::FindMaxRankSpellByExist(player, 12323);
	WarriorRage_HeadAtt = BotUtility::FindMaxRankSpellByExist(player, 6552);
	WarriorRage_Intercept = BotUtility::FindMaxRankSpellByExist(player, 20615);
	WarriorRage_Whirlwind = BotUtility::FindMaxRankSpellByExist(player, 12950);
	WarriorRage_Impertinency = BotUtility::FindMaxRankSpellByExist(player, 13847);
	WarriorRage_Needdead = BotUtility::FindMaxRankSpellByExist(player, 199261);
	WarriorRage_Bloodthirsty = BotUtility::FindMaxRankSpellByExist(player, 39071);
	WarriorRage_ReIntercept = BotUtility::FindMaxRankSpellByExist(player, 184364);
}

void BotPaladinSpells::InitializeSpells(Player* player)
{
	PaladinIDLE_MountAura = BotUtility::FindMaxRankSpellByExist(player, 225454);
	PaladinIDLE_CastAura = BotUtility::FindMaxRankSpellByExist(player, 81455);
	PaladinIDLE_JudgeAura = BotUtility::FindMaxRankSpellByExist(player, 8990);
	PaladinIDLE_ArmorAura = BotUtility::FindMaxRankSpellByExist(player, 41105);
	PaladinIDLE_AOEGuardWish = BotUtility::FindMaxRankSpellByExist(player, 210256);
	PaladinIDLE_GuardWish = BotUtility::FindMaxRankSpellByExist(player, 210256);
	PaladinIDLE_AOEKingWish = BotUtility::FindMaxRankSpellByExist(player, 43223);
	PaladinIDLE_KingWish = BotUtility::FindMaxRankSpellByExist(player, 56525);
	PaladinIDLE_AOEWitWish = BotUtility::FindMaxRankSpellByExist(player, 203539);
	PaladinIDLE_WitWish = BotUtility::FindMaxRankSpellByExist(player, 175365);
	PaladinIDLE_AOEStrWish = BotUtility::FindMaxRankSpellByExist(player, 29381);
	PaladinIDLE_StrWish = BotUtility::FindMaxRankSpellByExist(player, 56520);
	PaladinIDLE_JusticeRage = BotUtility::FindMaxRankSpellByExist(player, 25780);
	PaladinIDLE_Revive = BotUtility::FindMaxRankSpellByExist(player, 7328);

	PaladinGuard_UnShield = BotUtility::FindMaxRankSpellByExist(player, 29386);
	PaladinGuard_FreeAura = BotUtility::FindMaxRankSpellByExist(player, 1044);
	PaladinGuard_Invincible = BotUtility::FindMaxRankSpellByExist(player, 642);
	PaladinGuard_Sacrifice = BotUtility::FindMaxRankSpellByExist(player, 187190);
	PaladinGuard_AOESacrifice = BotUtility::FindMaxRankSpellByExist(player, 13903);
	PaladinGuard_BlockShield = BotUtility::FindMaxRankSpellByExist(player, 31904);
	PaladinGuard_PhyImmune = BotUtility::FindMaxRankSpellByExist(player, 66009);
	PaladinGuard_Pull = BotUtility::FindMaxRankSpellByExist(player, 210487);

	PaladinAssist_UpPower = BotUtility::FindMaxRankSpellByExist(player, 31842);
	PaladinAssist_RevengeStamp = BotUtility::FindMaxRankSpellByExist(player, 45095);
	PaladinAssist_LifeStamp = BotUtility::FindMaxRankSpellByExist(player, 165745);
	PaladinAssist_ManaStamp = BotUtility::FindMaxRankSpellByExist(player, 130433);
	PaladinAssist_JusticeStamp = BotUtility::FindMaxRankSpellByExist(player, 38008);
	PaladinAssist_StunStamp = BotUtility::FindMaxRankSpellByExist(player, 50907);
	PaladinAssist_ComStamp = BotUtility::FindMaxRankSpellByExist(player, 13903);
	PaladinAssist_Confession = BotUtility::FindMaxRankSpellByExist(player, 173315);
	PaladinAssist_StunMace = BotUtility::FindMaxRankSpellByExist(player, 66863);
	PaladinAssist_ReviveMana = BotUtility::FindMaxRankSpellByExist(player, 173521);
	PaladinAssist_HealCrit = BotUtility::FindMaxRankSpellByExist(player, 210294);
	PaladinAssist_LowMana = BotUtility::FindMaxRankSpellByExist(player, 20271);
	PaladinAssist_AuraUP = BotUtility::FindMaxRankSpellByExist(player, 31821);
	PaladinAssist_Dispel = BotUtility::FindMaxRankSpellByExist(player, 4987);

	PaladinHeal_FastHoly = BotUtility::FindMaxRankSpellByExist(player, 19750);
	PaladinHeal_BigHoly = BotUtility::FindMaxRankSpellByExist(player, 13952);
	PaladinHeal_FullHoly = BotUtility::FindMaxRankSpellByExist(player, 9257);

	PaladinMelee_AOEOffertory = BotUtility::FindMaxRankSpellByExist(player, 251152);
	PaladinMelee_KillMace = BotUtility::FindMaxRankSpellByExist(player, 37259);
	PaladinMelee_FlyShield = BotUtility::FindMaxRankSpellByExist(player, 31935);
	PaladinMelee_ShieldAtt = BotUtility::FindMaxRankSpellByExist(player, 53600);
	PaladinMelee_MaceAtt = BotUtility::FindMaxRankSpellByExist(player, 53595);
	PaladinMelee_HolyAtt = BotUtility::FindMaxRankSpellByExist(player, 25914);
	PaladinMelee_LifeJudge = BotUtility::FindMaxRankSpellByExist(player, 31804);
	PaladinMelee_ManaJudge = BotUtility::FindMaxRankSpellByExist(player, 41368);
	PaladinMelee_FleeJudge = BotUtility::FindMaxRankSpellByExist(player, 201371);
	PaladinMelee_WeaponAtt = BotUtility::FindMaxRankSpellByExist(player, 213844);
	PaladinMelee_HolyStrom = BotUtility::FindMaxRankSpellByExist(player, 163888);

	PaladinFlag_MomentHoly = 251152;
	PaladinFlag_Discipline = 25771;
}

void BotDeathknightSpells::InitializeSpells(Player* player)
{
	DKStatus_Frost = BotUtility::FindMaxRankSpellByExist(player, 50689);
	DKStatus_Evil = BotUtility::FindMaxRankSpellByExist(player, 50689);
	DKStatus_Blood = BotUtility::FindMaxRankSpellByExist(player, 50689);

	DKIDLE_Buffer = BotUtility::FindMaxRankSpellByExist(player, 165762);
	DKIDLE_SummonPet = BotUtility::FindMaxRankSpellByExist(player, 52451);
	DKIDLE_SummonAllPets = BotUtility::FindMaxRankSpellByExist(player, 52478);

	DKBlock_Silence = BotUtility::FindMaxRankSpellByExist(player, 66018);
	DKBlock_Cast = BotUtility::FindMaxRankSpellByExist(player, 173047);

	DKPulls_Pull = BotUtility::FindMaxRankSpellByExist(player, 222409);
	DKPulls_DKPull = BotUtility::FindMaxRankSpellByExist(player, 53276);

	DKDefense_MgcShield = BotUtility::FindMaxRankSpellByExist(player, 19645);
	DKDefense_NoMgcArea = BotUtility::FindMaxRankSpellByExist(player, 52893);
	DKDefense_Contract = BotUtility::FindMaxRankSpellByExist(player, 48743);
	DKDefense_IceBody = BotUtility::FindMaxRankSpellByExist(player, 66023);
	DKDefense_IceArmor = BotUtility::FindMaxRankSpellByExist(player, 132103);
	DKDefense_BoneShield = BotUtility::FindMaxRankSpellByExist(player, 232049);

	DKAssist_RuneLife = BotUtility::FindMaxRankSpellByExist(player, 59754);
	DKAssist_BloodBrand = BotUtility::FindMaxRankSpellByExist(player, 206940);
	DKAssist_Frenzied = BotUtility::FindMaxRankSpellByExist(player, 188541);
	DKAssist_BloodBuf = BotUtility::FindMaxRankSpellByExist(player, 55233);
	DKAssist_RuneWeapon = BotUtility::FindMaxRankSpellByExist(player, 47568);
	DKAssist_Infect = BotUtility::FindMaxRankSpellByExist(player, 91939);
	DKAssist_RuneShunt = BotUtility::FindMaxRankSpellByExist(player, 7122);
	DKAssist_IceLock = BotUtility::FindMaxRankSpellByExist(player, 53534);
	DKAssist_DeadRevive = BotUtility::FindMaxRankSpellByExist(player, 121147);
	DKAssist_NonFear = BotUtility::FindMaxRankSpellByExist(player, 49039);
	DKAssist_NextCrit = BotUtility::FindMaxRankSpellByExist(player, 79092);
	DKAssist_EatIce = BotUtility::FindMaxRankSpellByExist(player, 79092);
	DKAssist_PetPower = BotUtility::FindMaxRankSpellByExist(player, 49206);
	DKAssist_SummonFlyAtt = BotUtility::FindMaxRankSpellByExist(player, 49206);
	DKAssist_SummonRuneWeapon = BotUtility::FindMaxRankSpellByExist(player, 49028);

	DKAttack_IceSickness = BotUtility::FindMaxRankSpellByExist(player, 52372);
	DKAttack_NearAOE = BotUtility::FindMaxRankSpellByExist(player, 92025);
	DKAttack_AreaAOE = BotUtility::FindMaxRankSpellByExist(player, 43265);
	DKAttack_BloodAtt = BotUtility::FindMaxRankSpellByExist(player, 43265);
	DKAttack_ShadowAtt = BotUtility::FindMaxRankSpellByExist(player, 49921);
	DKAttack_FrostAtt = BotUtility::FindMaxRankSpellByExist(player, 60951);
	DKAttack_DoDestroy = BotUtility::FindMaxRankSpellByExist(player, 246593);
	DKAttack_RuneAttack = BotUtility::FindMaxRankSpellByExist(player, 62322);
	DKAttack_LifeAttack = BotUtility::FindMaxRankSpellByExist(player, 53639);
	DKAttack_IceWindAtt = BotUtility::FindMaxRankSpellByExist(player, 61061);
	DKAttack_CorpseExplosion = BotUtility::FindMaxRankSpellByExist(player, 17616);
	DKAttack_NaturalAtt = BotUtility::FindMaxRankSpellByExist(player, 164330);
	DKAttack_CoreAtt = BotUtility::FindMaxRankSpellByExist(player, 206930);
}

void BotRogueSpells::InitializeSpells(Player* player)
{
	RogueGuard_Sneak = BotUtility::FindMaxRankSpellByExist(player, 1784);
	RogueGuard_ShadowCloak = BotUtility::FindMaxRankSpellByExist(player, 31224);
	RogueGuard_Disappear = BotUtility::FindMaxRankSpellByExist(player, 1856);
	RogueGuard_Dodge = BotUtility::FindMaxRankSpellByExist(player, 248777);
	RogueGuard_Sprint = BotUtility::FindMaxRankSpellByExist(player, 65864);

	RogueSneak_Stick = BotUtility::FindMaxRankSpellByExist(player, 30980);
	RogueSneak_Premeditate = BotUtility::FindMaxRankSpellByExist(player, 235777);
	RogueSneak_Ambush = BotUtility::FindMaxRankSpellByExist(player, 8676);
	RogueSneak_Surprise = BotUtility::FindMaxRankSpellByExist(player, 1833);

	RogueAssist_ShadowDance = BotUtility::FindMaxRankSpellByExist(player, 185313);
	RogueAssist_ShadowFlash = BotUtility::FindMaxRankSpellByExist(player, 145426);
	RogueAssist_ReadyCD = BotUtility::FindMaxRankSpellByExist(player, 145426);
	RogueAssist_Blind = BotUtility::FindMaxRankSpellByExist(player, 2094);
	RogueAssist_Disarm = BotUtility::FindMaxRankSpellByExist(player, 236077);
	RogueAssist_NextCrit = BotUtility::FindMaxRankSpellByExist(player, 213981);
	RogueAssist_blood = BotUtility::FindMaxRankSpellByExist(player, 60177);
	RogueAssist_FastEnergy = BotUtility::FindMaxRankSpellByExist(player, 13750);
	RogueAssist_BlockCast = BotUtility::FindMaxRankSpellByExist(player, 1766);
	RogueAssist_Paralyze = BotUtility::FindMaxRankSpellByExist(player, 1776);
	RogueAssist_FastSpeed = BotUtility::FindMaxRankSpellByExist(player, 33735);

	RogueAOE_Knife = BotUtility::FindMaxRankSpellByExist(player, 51723);
	RogueAOE_AllDance = BotUtility::FindMaxRankSpellByExist(player, 51723);

	RogueAttack_Blood = BotUtility::FindMaxRankSpellByExist(player, 65954);
	RogueAttack_Ghost = BotUtility::FindMaxRankSpellByExist(player, 123437);
	RogueAttack_Injure = BotUtility::FindMaxRankSpellByExist(player, 31022);
	RogueAttack_PoisonAtt = BotUtility::FindMaxRankSpellByExist(player, 76511);
	RogueAttack_BackAtt = BotUtility::FindMaxRankSpellByExist(player, 53);
	RogueAttack_EvilAtt = BotUtility::FindMaxRankSpellByExist(player, 1752);

	RogueAttack_Damage = BotUtility::FindMaxRankSpellByExist(player, 196819);
	RogueAttack_Separate = BotUtility::FindMaxRankSpellByExist(player, 1079);
	RogueAttack_Stun = BotUtility::FindMaxRankSpellByExist(player, 408);
	RogueAttack_PoisonDmg = BotUtility::FindMaxRankSpellByExist(player, 145416);
	RogueAttack_Incision = BotUtility::FindMaxRankSpellByExist(player, 5171);
	RogueRange_Throw = BotUtility::FindMaxRankSpellByExist(player, 158692);

	RogueFlag_Dance = 185313;
}

void BotDruidSpells::InitializeSpells(Player* player)
{
	DruidIDLE_FerityWish = BotUtility::FindMaxRankSpellByExist(player, 24752);
	DruidIDLE_AOEFerityWish = BotUtility::FindMaxRankSpellByExist(player, 165754);
	DruidIDLE_Revive = BotUtility::FindMaxRankSpellByExist(player, 50769);
	DruidIDLE_CombatReive = BotUtility::FindMaxRankSpellByExist(player, 20484);

	DruidStatus_Travel = BotUtility::FindMaxRankSpellByExist(player, 783);
	DruidStatus_Bear = BotUtility::FindMaxRankSpellByExist(player, 5487);
	DruidStatus_Cat = BotUtility::FindMaxRankSpellByExist(player, 768);
	DruidStatus_Bird = BotUtility::FindMaxRankSpellByExist(player, 24858);
	DruidStatus_Tree = BotUtility::FindMaxRankSpellByExist(player, 33891);

	DruidGuard_Sneak = BotUtility::FindMaxRankSpellByExist(player, 5215);
	DruidGuard_Harden = BotUtility::FindMaxRankSpellByExist(player, 182872);
	DruidGuard_Thorns = BotUtility::FindMaxRankSpellByExist(player, 209334);
	DruidGuard_AutoTwine = BotUtility::FindMaxRankSpellByExist(player, 66071);
	DruidGuard_Twine = BotUtility::FindMaxRankSpellByExist(player, 339);
	DruidGuard_Control = BotUtility::FindMaxRankSpellByExist(player, 33786);
	DruidGuard_Pofu = BotUtility::FindMaxRankSpellByExist(player, 61336);
	DruidGuard_TreeMan = BotUtility::FindMaxRankSpellByExist(player, 6913);

	DruidAssist_PersonSpirit = BotUtility::FindMaxRankSpellByExist(player, 13752);
	DruidAssist_BeastSpirit = BotUtility::FindMaxRankSpellByExist(player, 13752);
	DruidAssist_Active = BotUtility::FindMaxRankSpellByExist(player, 6950);
	DruidAssist_DecCruse = BotUtility::FindMaxRankSpellByExist(player, 30281);
	DruidAssist_DecCruel = BotUtility::FindMaxRankSpellByExist(player, 14253);

	DruidCast_Moonfire = BotUtility::FindMaxRankSpellByExist(player, 65856);
	DruidCast_Insect = BotUtility::FindMaxRankSpellByExist(player, 65855);
	DruidCast_Anger = BotUtility::FindMaxRankSpellByExist(player, 65862);
	DruidCast_Spark = BotUtility::FindMaxRankSpellByExist(player, 98993);

	DruidAOE_Hurricane = BotUtility::FindMaxRankSpellByExist(player, 55881);
	DruidAOE_Typhoon = BotUtility::FindMaxRankSpellByExist(player, 51817);
	DruidAOE_FallStar = BotUtility::FindMaxRankSpellByExist(player, 100806);

	DruidHeal_Nourishing = BotUtility::FindMaxRankSpellByExist(player, 63556);
	DruidHeal_Relife = BotUtility::FindMaxRankSpellByExist(player, 774);
	DruidHeal_Coalescence = BotUtility::FindMaxRankSpellByExist(player, 66067);
	DruidHeal_Touch = BotUtility::FindMaxRankSpellByExist(player, 5185);
	DruidHeal_LifeBurst = BotUtility::FindMaxRankSpellByExist(player, 57763);
	DruidHeal_MergerLife = BotUtility::FindMaxRankSpellByExist(player, 18562);
	DruidHeal_MomentHeal = BotUtility::FindMaxRankSpellByExist(player, 127316);

	DruidHeal_AOETranquility = BotUtility::FindMaxRankSpellByExist(player, 740);
	DruidHeal_AOEFerity = BotUtility::FindMaxRankSpellByExist(player, 173170);

	DruidCat_Stun = BotUtility::FindMaxRankSpellByExist(player, 203123);
	DruidCat_Bite = BotUtility::FindMaxRankSpellByExist(player, 22568);
	DruidCat_Roar = BotUtility::FindMaxRankSpellByExist(player, 52610);
	DruidCat_Separate = BotUtility::FindMaxRankSpellByExist(player, 1943);

	DruidCat_Tiger = BotUtility::FindMaxRankSpellByExist(player, 5217);
	DruidCat_FastMove = BotUtility::FindMaxRankSpellByExist(player, 1850);
	DruidCat_Charge = BotUtility::FindMaxRankSpellByExist(player, 16979);
	DruidCat_Surprise = BotUtility::FindMaxRankSpellByExist(player, 75008);
	DruidCat_Sack = BotUtility::FindMaxRankSpellByExist(player, 201427);
	DruidCat_Claw = BotUtility::FindMaxRankSpellByExist(player, 91776);
	DruidCat_BackStab = BotUtility::FindMaxRankSpellByExist(player, 5221);
	DruidCat_Attack = BotUtility::FindMaxRankSpellByExist(player, 26103);
	DruidCat_Sweep = BotUtility::FindMaxRankSpellByExist(player, 1822);
	DruidCat_Laceration = BotUtility::FindMaxRankSpellByExist(player, 19820);

	DruidBear_DecAtt = BotUtility::FindMaxRankSpellByExist(player, 10968);
	DruidBear_AddPower = BotUtility::FindMaxRankSpellByExist(player, 8599);
	DruidBear_PowerLife = BotUtility::FindMaxRankSpellByExist(player, 22842);
	DruidBear_Laceration = BotUtility::FindMaxRankSpellByExist(player, 22689);
	DruidBear_Sweep = BotUtility::FindMaxRankSpellByExist(player, 61896);
	DruidBear_Attack = BotUtility::FindMaxRankSpellByExist(player, 61598);
	DruidBear_NextAtt = BotUtility::FindMaxRankSpellByExist(player, 6807);
	DruidBear_Stun = BotUtility::FindMaxRankSpellByExist(player, 1464);
	DruidBear_Charge = BotUtility::FindMaxRankSpellByExist(player, 39435);
}

void BotHunterSpells::InitializeSpells(Player* player)
{
	HunterIDLE_SummonPet = BotUtility::FindMaxRankSpellByExist(player, 23498);
	HunterIDLE_RevivePet = BotUtility::FindMaxRankSpellByExist(player, 982);
	HunterIDLE_ManaAura = BotUtility::FindMaxRankSpellByExist(player, 210754);
	HunterIDLE_DodgeAura = BotUtility::FindMaxRankSpellByExist(player, 210753);
	HunterIDLE_EagleAura = BotUtility::FindMaxRankSpellByExist(player, 231555);
	HunterIDLE_DragonAura = BotUtility::FindMaxRankSpellByExist(player, 210752);
	HunterIDLE_ShotAura = BotUtility::FindMaxRankSpellByExist(player, 31519);

	HunterTrap_FarFrozen = BotUtility::FindMaxRankSpellByExist(player, 209789);
	HunterTrap_Frozen = BotUtility::FindMaxRankSpellByExist(player, 43447);
	HunterTrap_Ice = BotUtility::FindMaxRankSpellByExist(player, 165769);
	HunterTrap_Viper = BotUtility::FindMaxRankSpellByExist(player, 43449);
	HunterTrap_Explode = BotUtility::FindMaxRankSpellByExist(player, 43444);
	HunterTrap_Fire = BotUtility::FindMaxRankSpellByExist(player, 155623);
	HunterTrap_Shot = BotUtility::FindMaxRankSpellByExist(player, 80003);

	HunterAssist_ClearRoot = BotUtility::FindMaxRankSpellByExist(player, 53271);
	HunterAssist_PetCommand = BotUtility::FindMaxRankSpellByExist(player, 205440);
	HunterAssist_HealPet = BotUtility::FindMaxRankSpellByExist(player, 37381);
	HunterAssist_PetStun = BotUtility::FindMaxRankSpellByExist(player, 7093);
	HunterAssist_PetRage = BotUtility::FindMaxRankSpellByExist(player, 19574);
	HunterAssist_Stamp = BotUtility::FindMaxRankSpellByExist(player, 1130);
	HunterAssist_FalseDead = BotUtility::FindMaxRankSpellByExist(player, 5384);
	HunterAssist_BackJump = BotUtility::FindMaxRankSpellByExist(player, 781);
	HunterAssist_FastSpeed = BotUtility::FindMaxRankSpellByExist(player, 3045);
	HunterAssist_ReadyCD = BotUtility::FindMaxRankSpellByExist(player, 203551);
	HunterAssist_Mislead = BotUtility::FindMaxRankSpellByExist(player, 34477);

	HunterMelee_BackRoot = BotUtility::FindMaxRankSpellByExist(player, 116599);
	HunterMelee_NoDamage = BotUtility::FindMaxRankSpellByExist(player, 31567);
	HunterMelee_DecSpeed = BotUtility::FindMaxRankSpellByExist(player, 195645);
	HunterMelee_NextAtt = BotUtility::FindMaxRankSpellByExist(player, 31566);
	HunterMelee_MeleeAtt = BotUtility::FindMaxRankSpellByExist(player, 190928);

	HunterDebug_Damage = BotUtility::FindMaxRankSpellByExist(player, 160503);
	HunterDebug_Mana = BotUtility::FindMaxRankSpellByExist(player, 31407);
	HunterDebug_Sleep = BotUtility::FindMaxRankSpellByExist(player, 19386);

	HunterShot_AOEShot = BotUtility::FindMaxRankSpellByExist(player, 22908);
	HunterShot_CharmShot = BotUtility::FindMaxRankSpellByExist(player, 23601);
	HunterShot_Explode = BotUtility::FindMaxRankSpellByExist(player, 15495);
	HunterShot_Aim = BotUtility::FindMaxRankSpellByExist(player, 48871);
	HunterShot_Silence = BotUtility::FindMaxRankSpellByExist(player, 248919);
	HunterShot_Shock = BotUtility::FindMaxRankSpellByExist(player, 5116);
	HunterShot_Cast = BotUtility::FindMaxRankSpellByExist(player, 65867);
	HunterShot_MgcShot = BotUtility::FindMaxRankSpellByExist(player, 69989);
	HunterShot_KillShot = BotUtility::FindMaxRankSpellByExist(player, 69989);
	HunterShot_MulShot = BotUtility::FindMaxRankSpellByExist(player, 2643);
	HunterShot_QMLShot = BotUtility::FindMaxRankSpellByExist(player, 53209);
}

void BotShamanSpells::InitializeSpells(Player* player)
{
	ShamanIDLE_LifeWeapon = BotUtility::FindMaxRankSpellByExist(player, 53209);
	ShamanIDLE_IceWeapon = BotUtility::FindMaxRankSpellByExist(player, 78273);
	ShamanIDLE_FireWeapon = BotUtility::FindMaxRankSpellByExist(player, 160098);
	ShamanIDLE_PhyWeapon = BotUtility::FindMaxRankSpellByExist(player, 159974);
	ShamanIDLE_FastWeapon = BotUtility::FindMaxRankSpellByExist(player, 32911);
	ShamanIDLE_Revive = BotUtility::FindMaxRankSpellByExist(player, 2008);

	ShamanShield_Earth = BotUtility::FindMaxRankSpellByExist(player, 226078);
	ShamanShield_Water = BotUtility::FindMaxRankSpellByExist(player, 79949);
	ShamanShield_Lightning = BotUtility::FindMaxRankSpellByExist(player, 20545);

	ShamanAssist_Frog = BotUtility::FindMaxRankSpellByExist(player, 11641);
	ShamanAssist_HealCrit = BotUtility::FindMaxRankSpellByExist(player, 137531);
	ShamanAssist_MomentHeal = BotUtility::FindMaxRankSpellByExist(player, 127316);
	ShamanAssist_MomentCast = BotUtility::FindMaxRankSpellByExist(player, 16166);
	ShamanAssist_BlockCast = BotUtility::FindMaxRankSpellByExist(player, 52870);
	ShamanAssist_Cleansing = BotUtility::FindMaxRankSpellByExist(player, 370);
	ShamanAssist_FireNova = BotUtility::FindMaxRankSpellByExist(player, 11969);
	ShamanAssist_Heroic = BotUtility::FindMaxRankSpellByExist(player, 32182);
	ShamanAssist_DecCruel = BotUtility::FindMaxRankSpellByExist(player, 14253);

	ShamanAtt_StormStrike = BotUtility::FindMaxRankSpellByExist(player, 17364);
	ShamanAtt_FireStrike = BotUtility::FindMaxRankSpellByExist(player, 60103);

	ShamanCast_LightningArrow = BotUtility::FindMaxRankSpellByExist(player, 218013);
	ShamanCast_LightningChain = BotUtility::FindMaxRankSpellByExist(player, 190332);
	ShamanCast_LightningStorm = BotUtility::FindMaxRankSpellByExist(player, 71935);
	ShamanCast_FireThud = BotUtility::FindMaxRankSpellByExist(player, 23038);
	ShamanCast_IceThud = BotUtility::FindMaxRankSpellByExist(player, 22582);
	ShamanCast_EarthThud = BotUtility::FindMaxRankSpellByExist(player, 43305);
	ShamanCast_FireStrike = BotUtility::FindMaxRankSpellByExist(player, 58972);

	ShamanHealth_Fast = BotUtility::FindMaxRankSpellByExist(player, 71985);
	ShamanHealth_Bast = BotUtility::FindMaxRankSpellByExist(player, 253330);
	ShamanHealth_Chain = BotUtility::FindMaxRankSpellByExist(player, 237925);
	ShamanHealth_Torrent = BotUtility::FindMaxRankSpellByExist(player, 237920);
	ShamanDispel_Refine = BotUtility::FindMaxRankSpellByExist(player, 234893);

	ShamanTotem_Recycle = BotUtility::FindMaxRankSpellByExist(player, 5394);

	ShamanTotem_Life = BotUtility::FindMaxRankSpellByExist(player, 35199);
	ShamanTotem_Mana = BotUtility::FindMaxRankSpellByExist(player, 24854);
	ShamanTotem_BMana = BotUtility::FindMaxRankSpellByExist(player, 24854);

	ShamanTotem_SummonFire = BotUtility::FindMaxRankSpellByExist(player, 27623);
	ShamanTotem_MgcPower = BotUtility::FindMaxRankSpellByExist(player, 31985);
	ShamanTotem_Attack = BotUtility::FindMaxRankSpellByExist(player, 38116);
	ShamanTotem_AOEAttack = BotUtility::FindMaxRankSpellByExist(player, 39591);
	ShamanTotem_MgcHeal = BotUtility::FindMaxRankSpellByExist(player, 31633);

	ShamanTotem_DecMove = BotUtility::FindMaxRankSpellByExist(player, 51485);
	ShamanTotem_SummonSoil = BotUtility::FindMaxRankSpellByExist(player, 73903);
	ShamanTotem_PhyPower = BotUtility::FindMaxRankSpellByExist(player, 65992);
	ShamanTotem_Armor = BotUtility::FindMaxRankSpellByExist(player, 73393);

	ShamanTotem_AbsorbBuff = BotUtility::FindMaxRankSpellByExist(player, 148819);
	ShamanTotem_AttSpeed = BotUtility::FindMaxRankSpellByExist(player, 27621);
	ShamanTotem_MgcSpeed = BotUtility::FindMaxRankSpellByExist(player, 27621);

	ShamanFlag_NoHeroic = 27621;
}

void BotMageSpells::InitializeSpells(Player* player)
{
	MageIDLE_ManaGem = BotUtility::FindMaxRankSpellByExist(player, 36883);
	MageIDLE_ArcaneMagic = BotUtility::FindMaxRankSpellByExist(player, 13326);
	MageIDLE_AOEArcaneMagic = BotUtility::FindMaxRankSpellByExist(player, 129171);
	MageIDLE_MgcArmor = BotUtility::FindMaxRankSpellByExist(player, 164309);
	MageIDLE_FrostArmor = BotUtility::FindMaxRankSpellByExist(player, 79563);
	MageIDLE_IceArmor = BotUtility::FindMaxRankSpellByExist(player, 165743);
	MageIDLE_FireArmor = BotUtility::FindMaxRankSpellByExist(player, 35915);
	MageIDLE_MagicAdd = BotUtility::FindMaxRankSpellByExist(player, 70408);
	MageIDLE_MagicDec = BotUtility::FindMaxRankSpellByExist(player, 44475);
	MageIDLE_SummonRite = BotUtility::FindMaxRankSpellByExist(player, 43987);

	MageGuard_MagicShield = BotUtility::FindMaxRankSpellByExist(player, 56778);
	MageGuard_FrostShield = BotUtility::FindMaxRankSpellByExist(player, 201565);
	MageGuard_FrostScherm = BotUtility::FindMaxRankSpellByExist(player, 41590);
	MageGuard_FrostNova = BotUtility::FindMaxRankSpellByExist(player, 64919);
	MageGuard_FireBreath = BotUtility::FindMaxRankSpellByExist(player, 31661);
	MageGuard_FireNova = BotUtility::FindMaxRankSpellByExist(player, 11969);

	MageAssist_Mirror = BotUtility::FindMaxRankSpellByExist(player, 166894);
	MageAssist_Rouse = BotUtility::FindMaxRankSpellByExist(player, 12051);
	MageAssist_Stealth = BotUtility::FindMaxRankSpellByExist(player, 66);
	MageAssist_Teleport = BotUtility::FindMaxRankSpellByExist(player, 14514);
	MageAssist_DecCurse = BotUtility::FindMaxRankSpellByExist(player, 15729);
	MageAssist_Grace = BotUtility::FindMaxRankSpellByExist(player, 29976);
	MageAssist_ArcanePower = BotUtility::FindMaxRankSpellByExist(player, 12042);
	MageAssist_CastSpeed = BotUtility::FindMaxRankSpellByExist(player, 12472);
	MageAssist_FastColddown = BotUtility::FindMaxRankSpellByExist(player, 235219);
	MageAssist_FrostPet = BotUtility::FindMaxRankSpellByExist(player, 31687);
	MageAssist_FireCritAura = BotUtility::FindMaxRankSpellByExist(player, 19428);

	MageConfine_BreakCast = BotUtility::FindMaxRankSpellByExist(player, 29443);
	MageConfine_AuraSteal = BotUtility::FindMaxRankSpellByExist(player, 30449);
	MageConfine_ArcaneSlow = BotUtility::FindMaxRankSpellByExist(player, 246);
	MageConfine_ToSheep = BotUtility::FindMaxRankSpellByExist(player, 118);
	MageConfine_Freeze = BotUtility::FindMaxRankSpellByExist(player, 79130);

	MageAOE_ArcaneExplode = BotUtility::FindMaxRankSpellByExist(player, 9433);
	MageAOE_Snowstorm = BotUtility::FindMaxRankSpellByExist(player, 15783);
	MageAOE_IcePiton = BotUtility::FindMaxRankSpellByExist(player, 12557);
	MageAOE_FireStorm = BotUtility::FindMaxRankSpellByExist(player, 13899);

	MageArcane_Barrage = BotUtility::FindMaxRankSpellByExist(player, 44425);
	MageArcane_Bullet = BotUtility::FindMaxRankSpellByExist(player, 5143);
	MageArcane_ArcaneShock = BotUtility::FindMaxRankSpellByExist(player, 16067);
	MageFrost_IceArrow = BotUtility::FindMaxRankSpellByExist(player, 9672);
	MageFrost_IceLance = BotUtility::FindMaxRankSpellByExist(player, 43571);
	MageFrost_FFArrow = BotUtility::FindMaxRankSpellByExist(player, 70616);
	MageFire_FireArrow = BotUtility::FindMaxRankSpellByExist(player, 133);
	MageFire_FireShock = BotUtility::FindMaxRankSpellByExist(player, 15574);
	MageFire_Firing = BotUtility::FindMaxRankSpellByExist(player, 2948);
	MageFire_BigFireBall = BotUtility::FindMaxRankSpellByExist(player, 33051);
	MageFire_FireBomb = BotUtility::FindMaxRankSpellByExist(player, 178551);

	MagePet_FrostNova = 40875;
	MageFlag_FireStun = 201565;
	MageFlag_FastFStorm = 201565;
	MageFlag_FastBFBall = 201565;
	MageFlag_FastFFArrow = 201565;
	MageFlag_CanFrozen = 201565;
	MageFlag_Scherm = 201565;
}

void BotWarlockSpells::InitializeSpells(Player* player)
{
	WarlockIDLE_LowArmor = BotUtility::FindMaxRankSpellByExist(player, 20798);
	WarlockIDLE_Armor = BotUtility::FindMaxRankSpellByExist(player, 13787);
	WarlockIDLE_HighArmor = BotUtility::FindMaxRankSpellByExist(player, 44520);
	WarlockIDLE_SoulLink = BotUtility::FindMaxRankSpellByExist(player, 79957);
	WarlockIDLE_ShadowShield = BotUtility::FindMaxRankSpellByExist(player, 53915);
	WarlockIDLE_SummonFireDemon = BotUtility::FindMaxRankSpellByExist(player, 688);
	WarlockIDLE_SummonHollowDemon = BotUtility::FindMaxRankSpellByExist(player, 697);
	WarlockIDLE_SummonSuccubus = BotUtility::FindMaxRankSpellByExist(player, 712);
	WarlockIDLE_SummonDogDemon = BotUtility::FindMaxRankSpellByExist(player, 691);
	WarlockIDLE_SummonGuardDemon = BotUtility::FindMaxRankSpellByExist(player, 30146);
	WarlockIDLE_FastSummon = BotUtility::FindMaxRankSpellByExist(player, 53915);
	WarlockIDLE_OpenGate = BotUtility::FindMaxRankSpellByExist(player, 48018);
	WarlockIDLE_TeleGate = BotUtility::FindMaxRankSpellByExist(player, 48020);
	WarlockIDLE_SummonRite = BotUtility::FindMaxRankSpellByExist(player, 60429);

	WarlockDemon_ToDemon = BotUtility::FindMaxRankSpellByExist(player, 54840);
	WarlockDemon_Charge = BotUtility::FindMaxRankSpellByExist(player, 104205);
	WarlockDemon_MeleeAOE = BotUtility::FindMaxRankSpellByExist(player, 215559);
	WarlockDemon_Sacrifice = BotUtility::FindMaxRankSpellByExist(player, 192502);

	WarlockAssist_DemonPower = BotUtility::FindMaxRankSpellByExist(player, 193396);
	WarlockAssist_ExtractMana = BotUtility::FindMaxRankSpellByExist(player, 108416);
	WarlockAssist_ConvertMana = BotUtility::FindMaxRankSpellByExist(player, 1454);
	WarlockAssist_StealLife = BotUtility::FindMaxRankSpellByExist(player, 12693);
	WarlockAssist_StealMana = BotUtility::FindMaxRankSpellByExist(player, 17008);
	WarlockAssist_BaseFear = BotUtility::FindMaxRankSpellByExist(player, 12096);
	WarlockAssist_FastFear = BotUtility::FindMaxRankSpellByExist(player, 6789);
	WarlockAssist_AOEFear = BotUtility::FindMaxRankSpellByExist(player, 5484);

	WarlockAOE_MeleeFire = BotUtility::FindMaxRankSpellByExist(player, 22539);
	WarlockAOE_RainFire = BotUtility::FindMaxRankSpellByExist(player, 16005);
	WarlockAOE_ShadowRage = BotUtility::FindMaxRankSpellByExist(player, 39082);

	WarlockCurse_UpDmg = BotUtility::FindMaxRankSpellByExist(player, 79956);
	WarlockCurse_MoveLow = BotUtility::FindMaxRankSpellByExist(player, 29539);
	WarlockCurse_MgcDmg = BotUtility::FindMaxRankSpellByExist(player, 14868);
	WarlockCurse_MeleeLow = BotUtility::FindMaxRankSpellByExist(player, 8552);
	WarlockCurse_CastLow = BotUtility::FindMaxRankSpellByExist(player, 12889);

	WarlockDot_LeechSoul = BotUtility::FindMaxRankSpellByExist(player, 48181);
	WarlockDot_HighDmg = BotUtility::FindMaxRankSpellByExist(player, 30108);
	WarlockDot_LowDmg = BotUtility::FindMaxRankSpellByExist(player, 172);
	WarlockDot_AOEDmg = BotUtility::FindMaxRankSpellByExist(player, 32863);
	WarlockDot_Sacrifice = BotUtility::FindMaxRankSpellByExist(player, 15505);

	WarlockCast_ShadowArrow = BotUtility::FindMaxRankSpellByExist(player, 9613);
	WarlockCast_ShadowShock = BotUtility::FindMaxRankSpellByExist(player, 131792);
	WarlockCast_ChaosArrow = BotUtility::FindMaxRankSpellByExist(player, 79939);
	WarlockCast_FullBurn = BotUtility::FindMaxRankSpellByExist(player, 41960);
	WarlockCast_FireBurn = BotUtility::FindMaxRankSpellByExist(player, 19428);
	WarlockCast_BigFireBall = BotUtility::FindMaxRankSpellByExist(player, 47825);

	WarlockFlag_SoulItem = 6265;
	WarlockFlag_SoulLink = 79957;
	WarlockFlag_OpenGate = 48018;
	WarlockFlag_Sacrifice = 223061;
}

void BotPriestSpells::InitializeSpells(Player* player)
{
	PriestIDLE_AllHardRes = BotUtility::FindMaxRankSpellByExist(player, 43939);
	PriestIDLE_HardRes = BotUtility::FindMaxRankSpellByExist(player, 23948);
	PriestIDLE_SoulFire = BotUtility::FindMaxRankSpellByExist(player, 48168);
	PriestIDLE_AllSpiritRes = BotUtility::FindMaxRankSpellByExist(player, 43939);
	PriestIDLE_SpiritRes = BotUtility::FindMaxRankSpellByExist(player, 23948);
	PriestIDLE_Bloodsucker = BotUtility::FindMaxRankSpellByExist(player, 15286);
	PriestIDLE_AllShadowRes = BotUtility::FindMaxRankSpellByExist(player, 53915);
	PriestIDLE_ShadowRes = BotUtility::FindMaxRankSpellByExist(player, 53915);
	PriestIDLE_ShadowStatus = BotUtility::FindMaxRankSpellByExist(player, 16592);
	PriestIDLE_Revive = BotUtility::FindMaxRankSpellByExist(player, 2006);

	PriestGuard_ShadowFear = BotUtility::FindMaxRankSpellByExist(player, 34984);
	PriestGuard_AOEFear = BotUtility::FindMaxRankSpellByExist(player, 8122);
	PriestGuard_DefFear = BotUtility::FindMaxRankSpellByExist(player, 65544);
	PriestGuard_RecoverMana = BotUtility::FindMaxRankSpellByExist(player, 65544);
	PriestGuard_DmgAnnul = BotUtility::FindMaxRankSpellByExist(player, 33206);
	PriestGuard_DefShield = BotUtility::FindMaxRankSpellByExist(player, 17);
	PriestGuard_SelfHealth = BotUtility::FindMaxRankSpellByExist(player, 19236);
	PriestGuard_GuardSoul = BotUtility::FindMaxRankSpellByExist(player, 47788);

	PriestAssist_SoulAbs = BotUtility::FindMaxRankSpellByExist(player, 196762);
	PriestAssist_AddHolyPower = BotUtility::FindMaxRankSpellByExist(player, 10060);
	PriestAssist_AllDispel = BotUtility::FindMaxRankSpellByExist(player, 4526);
	PriestAssist_Dispel = BotUtility::FindMaxRankSpellByExist(player, 528);
	PriestAssist_ShadowDemon = BotUtility::FindMaxRankSpellByExist(player, 10060);
	PriestAssist_Silence = BotUtility::FindMaxRankSpellByExist(player, 8988);
	PriestAssist_AllResMana = BotUtility::FindMaxRankSpellByExist(player, 64843);
	PriestAssist_AllResLife = BotUtility::FindMaxRankSpellByExist(player, 64843);
	PriestAssist_DecIllness = BotUtility::FindMaxRankSpellByExist(player, 3592);

	PriestDebuf_Ache = BotUtility::FindMaxRankSpellByExist(player, 11639);
	PriestDebuf_Drown = BotUtility::FindMaxRankSpellByExist(player, 41375);
	PriestDebuf_Plague = BotUtility::FindMaxRankSpellByExist(player, 138490);
	PriestAOE_ShadowExplode = BotUtility::FindMaxRankSpellByExist(player, 32000);
	PriestAOE_HolyNova = BotUtility::FindMaxRankSpellByExist(player, 20694);

	PriestShadow_ShadowTouch = BotUtility::FindMaxRankSpellByExist(player, 18152);
	PriestShadow_Knocking = BotUtility::FindMaxRankSpellByExist(player, 17194);
	PriestShadow_Lech = BotUtility::FindMaxRankSpellByExist(player, 15407);
	PriestHoly_Smite = BotUtility::FindMaxRankSpellByExist(player, 585);
	PriestHoly_BigFire = BotUtility::FindMaxRankSpellByExist(player, 17141);
	PriestPrecept_ManaBurn = BotUtility::FindMaxRankSpellByExist(player, 2691);

	PriestHeal_ZeroHeal = BotUtility::FindMaxRankSpellByExist(player, 29170);
	PriestHeal_LowHeal = BotUtility::FindMaxRankSpellByExist(player, 8812);
	PriestHeal_Resume = BotUtility::FindMaxRankSpellByExist(player, 27606);
	PriestHeal_FastHeal = BotUtility::FindMaxRankSpellByExist(player, 27608);
	PriestHeal_BigHeal = BotUtility::FindMaxRankSpellByExist(player, 34119);
	PriestHeal_LinkHeal = BotUtility::FindMaxRankSpellByExist(player, 32546);
	PriestHeal_UnionHeal = BotUtility::FindMaxRankSpellByExist(player, 225275);
	PriestHeal_RingHeal = BotUtility::FindMaxRankSpellByExist(player, 49306);
	PriestHeal_AOEHeal = BotUtility::FindMaxRankSpellByExist(player, 596);
	PriestHeal_Awareness = BotUtility::FindMaxRankSpellByExist(player, 47540);

	PriestFlag_DeadSoul = 20711;
	PriestFlag_NonShield = 6788;
}
