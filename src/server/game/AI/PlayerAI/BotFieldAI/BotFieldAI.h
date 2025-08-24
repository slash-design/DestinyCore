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

#ifndef _BOT_FIELD_AI_
#define _BOT_FIELD_AI_

#include "BotAITool.h"
#include "BotAISpells.h"
#include "SpellMgr.h"
#include "PlayerBotSetting.h"
#include "GridNotifiers.h"
#include <mutex>

class Player;
class BotBGAIMovement;

typedef std::list<Player*> NearPlayerList;
typedef std::vector<Player*> NearPlayerVec;
typedef std::list<Creature*> NearCreatureList;
typedef std::vector<Creature*> NearCreatureVec;
typedef std::list<Unit*> NearUnitList;
typedef std::vector<Unit*> NearUnitVec;

class TC_GAME_API BotFieldAI : public PlayerAI
{
public:
	static BotFieldAI* CreateBotFieldAIByPlayerClass(Player* player);
	static BotFieldAI* debugFieldAI;

public:
	BotFieldAI(Player* player);
	~BotFieldAI();

	Player* GetAIPayer() { return me; }
	void UpdateAI(uint32 diff) override;
	virtual void ResetBotAI();
	void SetResetAI() { m_HasReset = false; }
	void SetDrivingPVP(bool driving);
	void SetWarfareTarget(Unit* pTarget);
	bool HasTeleport() { return !m_Teleporting.CanMovement(); }
	void SetTeleport(uint32 mapID, Position& telePos) { m_Teleporting.SetTeleport(mapID, telePos); }
	void UpdateTeleport(uint32 diff) { m_Teleporting.Update(diff, m_Movement); }
	bool HasCruxMovement() { return m_CruxMovement.HasCruxMovement(); }
	void SetCruxMovement(Position& pos);
	bool IsNotSelect(Unit* pTarget);
	bool IsIDLEBot();
	virtual void OnLevelUp(uint32 talentType) {}
	void SearchCreatureListFromRange(Unit* center, NearCreatureVec& nearCreatures, float range, bool selfFaction);

protected:
	bool ProcessGroupInvite();
	bool TryUpMount();
	void Dismount();
	void ProcessHorror(uint32 diff);
	bool HasAuraMechanic(Unit* pTarget, Mechanics mask);
	bool IsNotMovement();
	bool IsInvincible(Unit* pTarget);
	bool IsBGSchedule();

	virtual void UpEnergy() {}
	Unit* GetCombatTarget(float range = BOTAI_SEARCH_RANGE);
	bool CanSelectPlayerEnemy(Player* player);
	bool NonCombatProcess();
	bool DoFaceToTarget(Unit* pTarget);
	SpellCastResult TryCastSpell(uint32 spellID, Unit* pTarget = NULL, bool force = false, bool dismount = true);
	SpellCastResult TryCastPullSpell(uint32 spellID, Unit* pTarget = NULL);
	SpellCastResult PetTryCastSpell(uint32 spellID, Unit* pTarget = NULL, bool force = false);
	void SettingPetAutoCastSpell(uint32 spellID, bool autoCast);

	virtual void ClearMechanicAura() {}
	virtual bool NeedWaitSpecialSpell(uint32 diff);
	virtual bool NeedFlee();
	void FleeMovement();
	virtual void ProcessFlee();
	virtual bool ProcessNormalSpell() { return false; }
	virtual void ProcessRangeSpell(Unit* pTarget) {}
	virtual void ProcessMeleeSpell(Unit* pTarget) {}
	virtual void ProcessHealthSpell(Unit* pTarget) {}
	void ProcessIDLE();
	void ProcessHealth();
	void ProcessCombat(Unit* pTarget);
	bool ProcessWarfare();

	void ChaseTarget(Unit* pTarget, bool isMelee, float range = 3.0f);
	NearUnitVec SearchFriend(float range = BOTAI_SEARCH_RANGE);
	NearPlayerVec SearchFarFriend(float minRange, float maxRange, bool isIDLE);
	NearPlayerVec ExistFriendAttacker(float range = BOTAI_RANGESPELL_DISTANCE);
	NearUnitVec SearchNeedHealth(float range = BOTAI_SEARCH_RANGE);
	NearUnitVec SearchLifePctByFriendRange(Unit* pTarget, float lifePct, float range = NEEDFLEE_CHECKRANGE);
	Unit* RandomRangeEnemyByCasting(float range);
	NearUnitVec RangeEnemyListByHasAura(uint32 aura, float range);
	NearUnitVec RangeEnemyListByNonAura(uint32 aura, float range);
	NearUnitVec RangeEnemyListByTargetIsMe(float range);
	NearUnitVec RangeListByTargetIsTarget(Unit* pTarget, float range);
	NearUnitVec RangeEnemyListByTargetRange(Unit* pTarget, float range);
	NearUnitVec SearchFarEnemy(float minRange, float maxRange);

	Unit* GetBotAIValidSelectedUnit();
	bool TargetIsRange(Player* pTarget);
	bool TargetIsMagic(Player* pTarget);
	bool TargetIsCastMagic(Player* pTarget);
	bool TargetIsStealth(Player* pTarget);

public:
	virtual uint32 GetReviveSpell() { return 0; }
	virtual bool IsMeleeBotAI();
	virtual bool IsRangeBotAI();
	virtual bool IsHealerBotAI();
	virtual bool IsAttacker() { return true; }

protected:
	int32 m_UpdateTick;
	bool m_DrivingPVP;
	uint32 m_UseMountID;
	ObjectGuid m_WarfareTargetID;

	BotBGAIMovement* m_Movement;
	BotAIGuild m_Guild;
	BotAIHorrorState* pHorrorState;
	BotAIStoped m_CheckStoped;
	BotAITeleport m_Teleporting;
	BotAIUseFood m_UseFood;
	BotAIUsePotion m_UsePotion;
	BotAIFindNearLoot m_FindLoot;
	BotAITrade m_AITrade;
	BotAIFieldRevive m_Revive;
	BotAIFlee m_Flee;
	BotAIIDLEMovement m_IDLE;
	BotAICruxMovement m_CruxMovement;
	BotAIWishStore m_WishStore;
	BotAICheckSetting m_CheckSetting;
	BotAIRecordCastSpell m_CastRecords;
	BotAICheckDuel m_CheckDuel;

	bool m_HasReset;

	bool m_IsMeleeBot;
	bool m_IsRangeBot;
	bool m_IsHealerBot;

	std::mutex m_ItemLock;
};

#endif // !_BOT_FIELD_AI_
