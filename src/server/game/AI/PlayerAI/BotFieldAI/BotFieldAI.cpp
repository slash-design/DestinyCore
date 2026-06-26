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

#include "BotFieldAI.h"
#include "MoveSplineInit.h"
#include "BotBGAIMovement.h"
#include "PlayerBotMgr.h"
#include "FieldBotMgr.h"
#include "PlayerBotSession.h"
#include "Spell.h"
#include "Pet.h"
#include "BotFieldClassAI.h"
#include "Group.h"
#include "WorldSession.h"
#include "PartyPackets.h"
#include "MiscPackets.h"
#include "MotionMaster.h"
#include "CreatureAI.h"
#include "SpellHistory.h"

BotFieldAI* BotFieldAI::debugFieldAI = NULL;

BotFieldAI* BotFieldAI::CreateBotFieldAIByPlayerClass(Player* player)
{
	PlayerBotSetting::ClearUnknowMount(player);
	BotFieldAI* pAI = NULL;
	switch (player->getClass())
	{
	case CLASS_WARRIOR:
		pAI = new FieldWarriorAI(player);
		break;
	case CLASS_MAGE:
		pAI = new FieldMageAI(player);
		break;
	case CLASS_PRIEST:
		pAI = new FieldPriestAI(player);
		break;
	case CLASS_HUNTER:
		pAI = new FieldHunterAI(player);
		break;
	case CLASS_WARLOCK:
		pAI = new FieldWarlockAI(player);
		break;
	case CLASS_PALADIN:
		pAI = new FieldPaladinAI(player);
		break;
	case CLASS_ROGUE:
		pAI = new FieldRogueAI(player);
		break;
	case CLASS_SHAMAN:
		pAI = new FieldShamanAI(player);
		break;
	case CLASS_DRUID:
		pAI = new FieldDruidAI(player);
		break;
	case CLASS_DEATH_KNIGHT:
		pAI = new FieldDeathknightAI(player);
		break;
	}
	if (!pAI)
		pAI = new BotFieldAI(player);
	pAI->ResetBotAI();
	return pAI;
}

BotFieldAI::BotFieldAI(Player* player) :
PlayerAI(player),
m_UpdateTick(BOTAI_UPDATE_TICK),
m_DrivingPVP(false),
m_Movement(new BotBGAIMovement(player, this)),
m_UseMountID(PlayerBotSetting::RandomMountByLevel(player->getLevel())),
m_WarfareTargetID(ObjectGuid::Empty),
m_Guild(player),
pHorrorState(NULL),
m_CheckStoped(player),
m_Teleporting(player),
m_UseFood(player),
m_UsePotion(player),
m_FindLoot(player),
m_AITrade(player),
m_Revive(player),
m_Flee(player),
m_IDLE(player),
m_CruxMovement(player),
m_WishStore(player),
m_CheckSetting(player),
m_CastRecords(player),
m_CheckDuel(player),
m_HasReset(false)
{
	if (!me->IsPvP())
	{
		BotUtility::PlayerBotTogglePVP(player, true);
	}
        // ================== 新增代码开始 ==================
	// 初始化自动升级定时器为 15 到 30 分钟随机触发一次（单位：毫秒）
	// 注意：如果你现在是为了测试，可以暂时改成 1 分钟，比如：
	// m_AutoLevelTimer = urand(1 * MINUTE * IN_MILLISECONDS, 2 * MINUTE * IN_MILLISECONDS);
	m_AutoLevelTimer = urand(15 * MINUTE * IN_MILLISECONDS, 30 * MINUTE * IN_MILLISECONDS);
	// ================== 新增代码结束 ==================
}

BotFieldAI::~BotFieldAI()
{

}

void BotFieldAI::UpdateAI(uint32 diff)
{
    // ==========================================================
    // 【AI 导演系统：野生群演控制中枢 (BotFieldAI 专属)】
    // ==========================================================
    if (m_isDirectorDrafted)
    {
        // 【0. 距离计算与防走丢判定】
        float distToAnchor = 0.0f;
        bool outOfBounds = false;
        
        if (m_directorAnchorMapId != 0)
        {
            if (me->GetMapId() != m_directorAnchorMapId) 
                outOfBounds = true;
            else 
            {
                float dx = me->GetPositionX() - m_directorAnchorX;
                float dy = me->GetPositionY() - m_directorAnchorY;
                distToAnchor = std::sqrt(dx*dx + dy*dy);
                
                // 只有超过 150 码（确诊掉出世界或被传送走）才暴力引渡
                if (distToAnchor > 150.0f) outOfBounds = true;
            }

            if (outOfBounds)
            {
                me->InterruptNonMeleeSpells(false);
                me->SetStandState(UNIT_STAND_STATE_STAND);
                me->StopMoving();
                me->GetMotionMaster()->Clear();

                bool teleSuccess = me->TeleportTo(m_directorAnchorMapId, m_directorAnchorX, m_directorAnchorY, m_directorAnchorZ, frand(0.0f, 6.28f));
                if (!teleSuccess && me->GetMapId() == m_directorAnchorMapId)
                    me->Relocate(m_directorAnchorX, m_directorAnchorY, m_directorAnchorZ, frand(0.0f, 6.28f));

                return; // 等待传送落地
            }
        }

        // 【1. 星探赎身机制】
        if (me->GetGroup() || me->GetGroupInvite())
        {
            SetDirectorDrafted(false);     
            m_isDirectorSleeping = false;  
            me->SetStandState(UNIT_STAND_STATE_STAND); 
            TC_LOG_ERROR("server", ">>> [星探发掘] 群演 [%s] 恢复自由身！", me->GetName().c_str());
        }
        else
        {
            // 【2. 常规群演苦力逻辑】
            m_directorCheckTimer += diff;
            
            // 【修复报错1】：回退为原本绝对正确的宏定义获取方式
            bool isInCity = me->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_RESTING); 
            // 城里 300 码内不休眠（基本覆盖主城），野外保持 65 码
            float sleepRadius = isInCity ? 300.0f : 65.0f; 
            float wakeRadius  = isInCity ? 150.0f : 50.0f;

            if (m_directorCheckTimer >= 3000) 
            {
                m_directorCheckTimer = 0; 
                float nearestDist = 9999.0f;
                Player* nearestP = nullptr;

                Map::PlayerList const& players = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                {
                    Player* p = itr->GetSource();
                    if (p && !p->IsPlayerBot() && p->IsAlive())
                    {
                        float d = p->GetDistance(me);
                        if (d < nearestDist) { nearestDist = d; nearestP = p; }
                    }
                }

                if (m_isDirectorSleeping)
                {
                    // 【优化1】：使用动态唤醒距离
                    if (nearestP && nearestDist < wakeRadius)
                    {
                        m_isDirectorSleeping = false;                    
                        me->SetStandState(UNIT_STAND_STATE_STAND);       
                        me->Dismount();    

                        // 【核心掩护】：唤醒时立刻向四周散开
                        m_directorWanderTimer = 6000;
                    }
                }
                else 
                {
                    // 【优化2】：使用动态休眠距离
                    if (!nearestP || nearestDist >= sleepRadius)
                    {
                        m_isDirectorSleeping = true;                    
                        me->StopMoving();                               
                        me->GetMotionMaster()->Clear();                 
                        me->SetStandState(UNIT_STAND_STATE_SIT);        
                    }
                }
            } 

            // --- 提线木偶微型大脑 (终极防卡墙版) ---
            if (!m_isDirectorSleeping && !me->IsInCombat())
            {
                // ================= 【修复1：绝对独立的倒计时锁】 =================
                if (m_directorInteractTimer > 0)
                {
                    if (m_directorInteractTimer > diff)
                    {
                        m_directorInteractTimer -= diff;
                        // 发呆期间，有极其微小的概率做个动作假装活人
                        if (urand(1, 1000) <= 2)
                        {
                            uint32 emotes[] = { EMOTE_ONESHOT_TALK, EMOTE_ONESHOT_BOW, EMOTE_ONESHOT_QUESTION, EMOTE_ONESHOT_EXCLAMATION };
                            me->HandleEmoteCommand(emotes[urand(0, 3)]); 
                        }
                        return; // 倒计时没走完，物理切断本帧，绝对不准思考！
                    }
                    else
                    {
                        m_directorInteractTimer = 0; // 倒计时完毕，解锁大脑！
                    }
                }

                // ================= 思考与寻路逻辑 =================
                m_directorWanderTimer += diff;
                
                if (m_directorWanderTimer >= 4000) 
                {
                    m_directorWanderTimer = urand(0, 1500); 
                    bool isAttacking = false;
                    
                    bool isInCity = me->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_RESTING); 

                    // --- 野外打野逻辑 ---
                    if (!isInCity && urand(1, 100) <= 30)
                    {
                        Creature* target = nullptr;
                        std::list<Creature*> creatureList;
                        me->GetCreatureListWithEntryInGrid(creatureList, 0, 25.0f);
                        for (auto c : creatureList)
                        {
                            if (c && c->IsAlive() && me->IsValidAttackTarget(c))
                            {
                                if (c->getFaction() == 160) continue;
                                if (c->HasUnitFlag((UnitFlags)4)) continue;
                                if (c->GetMaxHealth() > 5000000 && !c->IsInCombatWith(me)) continue;
                                target = c;
                                break; 
                            }
                        }

                        if (target)
                        {
                            // 【破解死结1】：打怪前强行站立
                            me->SetStandState(UNIT_STAND_STATE_STAND);
                            me->SetWalk(false); 
                            me->Attack(target, true);
                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MoveChase(target);
                            isAttacking = true;
                        }
                    }

                    // --- 行为树分流 ---
                    if (!isAttacking)
                    {
                        // 【破解死结2】：每次移动前，绝对强行站立并清空移动队列！无论它刚才是不是坐着！
                        me->SetStandState(UNIT_STAND_STATE_STAND);
                        me->GetMotionMaster()->Clear();
                        me->SetWalk(true);

                        if (!isInCity && distToAnchor > 40.0f)
                        {
                            me->GetMotionMaster()->MovePoint(1, m_directorAnchorX, m_directorAnchorY, m_directorAnchorZ);
                            m_directorInteractTimer = urand(5000, 15000); 
                        }
                        else if (!isInCity) 
                        {
                            // 【破解死结3】：野外街溜子使用原生安全寻路
                            me->GetMotionMaster()->MoveRandom(15.0f);
                            m_directorInteractTimer = urand(5000, 10000);
                        }
                        else if (isInCity) // 主城
                        {
                            int destinyRoll = urand(1, 100);

                            // 【第一层：30% 几率】去绝对坐标：银行/拍卖行
                            if (destinyRoll <= 30) 
                            {
                                static const struct { uint32 mapId; float x, y, z; } CityPOIs[] = {
                                    {1, 1643.72f, -4443.32f, 18.62f},   {1, 1513.90f, -4354.57f, 20.55f},
                                    {0, 1588.68f, 241.05f, -52.14f},    {0, 1579.11f, 187.98f, -56.79f},
                                    {530, 9683.39f, -7520.52f, 15.74f}, {530, 9805.19f, -7487.27f, 13.55f},
                                    {530, -2000.13f, 5350.65f, -9.35f}, {530, -2023.69f, 5390.86f, -7.48f},
                                    {571, 5927.02f, 729.74f, 642.13f},  {571, 5627.04f, 693.37f, 651.99f},
                                    {0, -8888.40f, 566.25f, 93.35f},    {0, -8823.97f, 683.89f, 97.23f},
                                    {0, -4889.87f, -993.15f, 503.94f},  {0, -4902.07f, -973.80f, 501.52f},
                                    {530, -4022.18f, -11734.09f, -151.85f}, {530, -3919.18f, -11547.06f, -150.15f}
                                };

                                std::vector<int> validIndices;
                                for (int i = 0; i < sizeof(CityPOIs) / sizeof(CityPOIs[0]); ++i)
                                {
                                    if (CityPOIs[i].mapId == me->GetMapId() && me->GetDistance2d(CityPOIs[i].x, CityPOIs[i].y) < 400.0f)
                                        validIndices.push_back(i);
                                }

                                if (!validIndices.empty())
                                {
                                    int targetIndex = validIndices[urand(0, validIndices.size() - 1)];
                                    float offsetAngle = frand(0.0f, 6.28f);
                                    // 【破解死结4】：室内坐标散布范围从 12码 极度缩小到 3码以内，绝对防止卡墙宕机！
                                    float offsetDist  = frand(1.0f, 3.0f); 
                                    
                                    me->GetMotionMaster()->MovePoint(1, 
                                        CityPOIs[targetIndex].x + offsetDist * std::cos(offsetAngle), 
                                        CityPOIs[targetIndex].y + offsetDist * std::sin(offsetAngle), 
                                        CityPOIs[targetIndex].z);
                                    m_directorInteractTimer = urand(15000, 45000); 
                                    return; // 正确切断
                                }
                            }

                            // 【第二层：40% 几率】在附近找NPC、邮箱或椅子
                            if (destinyRoll <= 70)
                            {
                                Creature* poiNpc = nullptr;
                                GameObject* poiGo = nullptr;
                                
                                std::vector<Creature*> validNpcs;
                                std::list<Creature*> creatureList;
                                me->GetCreatureListWithEntryInGrid(creatureList, 0, 100.0f);
                                for (auto c : creatureList)
                                {
                                    if (c && c->IsAlive() && c->IsFriendlyTo(me) && !c->IsPet())
                                    {
                                        if (c->IsVendor() || c->IsGossip() || c->IsQuestGiver())
                                            validNpcs.push_back(c);
                                    }
                                }

                                if (!validNpcs.empty())
                                    poiNpc = validNpcs[urand(0, validNpcs.size() - 1)];

                                if (!poiNpc) 
                                {
                                    std::vector<GameObject*> validGos;
                                    std::list<GameObject*> goList;
                                    me->GetGameObjectListWithEntryInGrid(goList, 0, 80.0f);
                                    for (auto go : goList)
                                    {
                                        if (go && (go->GetGoType() == GAMEOBJECT_TYPE_MAILBOX || go->GetGoType() == GAMEOBJECT_TYPE_CHAIR))
                                            validGos.push_back(go);
                                    }
                                    if (!validGos.empty())
                                        poiGo = validGos[urand(0, validGos.size() - 1)];
                                }

                                if (poiNpc)
                                {
                                    float angle = frand(0.0f, 6.28f);
                                    float dist = frand(1.5f, 3.0f); 
                                    me->GetMotionMaster()->MovePoint(1, poiNpc->GetPositionX() + dist * std::cos(angle), poiNpc->GetPositionY() + dist * std::sin(angle), poiNpc->GetPositionZ());
                                    me->SetFacingToObject(poiNpc);
                                    
                                    m_directorInteractTimer = urand(10000, 30000);
                                    if (urand(1, 100) <= 20) me->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                                }
                                else if (poiGo)
                                {
                                    float angle = frand(0.0f, 6.28f);
                                    float dist = (poiGo->GetGoType() == GAMEOBJECT_TYPE_MAILBOX) ? frand(1.0f, 2.0f) : 0.0f;
                                    me->GetMotionMaster()->MovePoint(1, poiGo->GetPositionX() + dist * std::cos(angle), poiGo->GetPositionY() + dist * std::sin(angle), poiGo->GetPositionZ());
                                    me->SetFacingToObject(poiGo);
                                    
                                    if (poiGo->GetGoType() == GAMEOBJECT_TYPE_MAILBOX)
                                    {
                                        m_directorInteractTimer = urand(10000, 25000);
                                        me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING); 
                                    }
                                    else 
                                    {
                                        m_directorInteractTimer = urand(15000, 45000);
                                        me->HandleEmoteCommand(EMOTE_STATE_SIT); 
                                    }
                                }
                                else
                                {
                                    // 没找到任何东西，直接随机安全漫步
                                    me->GetMotionMaster()->MoveRandom(20.0f);
                                    m_directorInteractTimer = urand(5000, 15000);
                                }
                            }
                            else 
                            {
                                // 【第三层：剩余 30% 几率】使用核心原生安全寻路 MoveRandom
                                // 彻底摒弃手工三角函数带来的卡墙 bug！
                                me->GetMotionMaster()->MoveRandom(35.0f);
                                m_directorInteractTimer = urand(5000, 15000); 
                            }
                        }
                    }
                }
            }

            // ================= 核心洛巴托米手术 =================
            if (m_isDirectorSleeping) return; 
            if (!me->IsInCombat()) return; 
        } 
    }

    // 强行驻留锁
    if (m_isCommandStopped)
    {
        me->StopMoving();
        return; 
    }
    
    // ================== 新增自动升级逻辑 ==================
    if (me->IsAlive() && !me->IsInCombat())
    {
        if (m_AutoLevelTimer <= diff)
        {
            uint8 maxLevel = sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL);
            if (me->getLevel() < maxLevel)
            {
                me->GiveLevel(me->getLevel() + 1);
            }
            m_AutoLevelTimer = urand(15 * MINUTE * IN_MILLISECONDS, 30 * MINUTE * IN_MILLISECONDS);
        }
        else
        {
            m_AutoLevelTimer -= diff;
        }
    }
    // ======================================================

    m_UpdateTick -= diff;
    if (m_UpdateTick > 0)
        return;
    m_UpdateTick = BOTAI_UPDATE_TICK;

    if (!me->IsSettingFinish())
        return;
    UpdateTeleport(BOTAI_UPDATE_TICK);
    if (!m_Teleporting.CanMovement())
        return;
    me->UpdateObjectVisibility(false);
    m_Guild.UpdateGuildProcess();
    if (ProcessGroupInvite())
        return;
    if (IsBGSchedule())
    {
        BotUtility::TryTeleportHome(this);
        return;
    }

    if (!m_HasReset)
        ResetBotAI();
        
    if (me->IsAlive())
    {
         // ================== 核心 AI 升级：全系能量无尽模式 ==================
        for (uint8 i = 0; i < MAX_POWERS; ++i)
        {
            if (me->GetMaxPower((Powers)i) > 0 && me->GetPower((Powers)i) < me->GetMaxPower((Powers)i))
            {
                me->SetPower((Powers)i, me->GetMaxPower((Powers)i));
            }
        }
        // =======================================================================

        Position pos = me->GetPosition();
        m_CheckStoped.UpdatePosition(diff);
        BotUtility::TryTeleportPlayerPet(me);
        ClearMechanicAura();
        if (!IsNotMovement())
            ProcessHorror(diff);
        if (NeedWaitSpecialSpell(BOTAI_UPDATE_TICK))
            return;

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;
        if (!m_CruxMovement.HasCruxMovement() && NonCombatProcess())
            return;

        if (!me->IsInCombat() && ProcessNormalSpell())
            return;
        m_Movement->SyncPosition(pos);
        if (TryUpMount())
            return;
        if (!me->HasAura(m_UseMountID) && !me->HasUnitState(UNIT_STATE_CASTING))
            m_UsePotion.TryUsePotion();
        if (me->IsInCombat())
            UpEnergy();
        Unit* pTarget = GetBotAIValidSelectedUnit();

        // ================== 核心 AI 升级：野外防卫本能 ==================
        if (!pTarget && me->IsInCombat())
        {
            Unit* pAttacker = me->getAttackerForHelper();
            if (pAttacker && pAttacker->IsAlive() && me->IsValidAttackTarget(pAttacker))
            {
                pTarget = pAttacker;
                me->SetSelection(pTarget->GetGUID());
                me->Attack(pTarget, true);
            }
        }
        // ================================================================

        if (m_CruxMovement.HasCruxMovement())
        {
            m_CruxMovement.UpdateCruxMovement(m_Movement);
        }
        else if (pTarget && pTarget->IsAlive() && !IsInvincible(pTarget))
        {
            float distance = me->GetDistance(pTarget->GetPosition());
            if (distance < BOTAI_SEARCH_RANGE)
            {
                if (IsHealerBotAI() && me->getLevel() >= 10)
                    ProcessHealth();
                else
                    ProcessCombat(pTarget);
            }
            else if (distance > BOTAI_SEARCH_RANGE * 2.5f || me->GetMap() != pTarget->GetMap())
            {
                me->SetSelection(ObjectGuid::Empty);
            }
            else
            {
                m_Movement->MovementToTarget();
            }
        }
        else if (pTarget = GetCombatTarget())
        {
            me->AttackStop();
            me->SetSelection(pTarget->GetGUID());
        }
        else
        {
            me->SetSelection(ObjectGuid::Empty);
            ProcessIDLE();
        }
    }
    else
    {
        m_CastRecords.ClearRecordSpell();
        m_WishStore.ClearStores();
        m_CruxMovement.ClearMovement();
        me->SetSelection(ObjectGuid::Empty);
        m_Revive.UpdateRevive(BOTAI_UPDATE_TICK, m_Teleporting);
    }
}

void BotFieldAI::ResetBotAI()
{
	PlayerBotSetting::ClearUnknowMount(me);
	m_Movement->ClearMovement();
	if (m_UseMountID != 0)
	{
		if (!me->HasSpell(m_UseMountID))
			me->LearnSpell(m_UseMountID, false);
	}
	m_IsMeleeBot = IsMeleeBotAI();
	m_IsRangeBot = IsRangeBotAI();
	m_IsHealerBot = IsHealerBotAI();
	m_HasReset = true;

	m_CastRecords.ClearRecordSpell();
}

void BotFieldAI::SetDrivingPVP(bool driving)
{
	if (m_DrivingPVP != driving)
	{
		m_DrivingPVP = driving;
		me->SetSelection(ObjectGuid::Empty);
		me->CombatStop(true);
	}
}

void BotFieldAI::SetWarfareTarget(Unit* pTarget)
{
	if (pTarget)
		m_WarfareTargetID = pTarget->GetGUID();
	else
		m_WarfareTargetID = ObjectGuid::Empty;
}

void BotFieldAI::SetCruxMovement(Position& pos)
{
	me->SetSelection(ObjectGuid::Empty);
	m_CruxMovement.SetMovement(pos);
}

bool BotFieldAI::ProcessGroupInvite()
{
	if (Group* pGroup = me->GetGroup())
	{
		//if (pGroup->isLFGGroup())
		{
			m_Movement->ClearMovement();
			PlayerBotMgr::SwitchPlayerBotAI(me, PlayerBotAIType::PBAIT_GROUP, true);
			return true;
		}
		//return false;
	}
	WorldSession* pWorldSession = me->GetSession();
	if (!pWorldSession)
		return false;
	PlayerBotSession* pSession = dynamic_cast<PlayerBotSession*>(pWorldSession);
	if (!pSession || pSession->HasSchedules())
		return false;
	if (me->IsInWorld() && me->GetMap()->IsDungeon())
	{
		BotUtility::TryTeleportHome(this);
		return true;
	}
	Group* pGroup = me->GetGroupInvite();
	if (!pGroup)
		return false;
	WorldPacket opcode(CMSG_PARTY_INVITE_RESPONSE);
	if (pSession->HasSchedules() || me->InBattleground())
	{
		WorldPackets::Party::PartyInviteResponse packet(std::move(opcode));
		packet.Accept = 0;
		pWorldSession->HandlePartyInviteResponseOpcode(packet);
	}
	else
	{
		WorldPackets::Party::PartyInviteResponse packet(std::move(opcode));
		packet.Accept = 1;
		pWorldSession->HandlePartyInviteResponseOpcode(packet);
		m_Movement->ClearMovement();
		PlayerBotMgr::SwitchPlayerBotAI(me, PlayerBotAIType::PBAIT_GROUP, true);
	}
	return true;
}

bool BotFieldAI::IsNotSelect(Unit* pTarget)
{
	if (!pTarget || !pTarget->IsAlive())
		return true;
	if (pTarget->HasAura(27827)) // (27827     ֮             )
		return true;
	return false;
}

bool BotFieldAI::IsIDLEBot()
{
	if (me->HasUnitState(UNIT_STATE_CASTING))
		return false;
	if (me->IsInCombat() || !me->IsAlive())
		return false;
	if (!m_Teleporting.CanMovement())
		return false;
	if (m_UseFood.HasFoodState())
		return false;
	if (m_FindLoot.HasLoot())
		return false;

	return true;
}

bool BotFieldAI::TryUpMount()
{
	if (me->IsInCombat() || me->GetSelectedUnit() || me->HasAura(m_UseMountID) || me->getLevel() < 20)
		return false;
	if (!me->GetMap()->IsOutdoors(me->GetPhaseShift(), me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
		return false;
	if (me->IsMounted())
		return false;
	if (me->HasUnitState(UNIT_STATE_CASTING))
		return false;
	m_Movement->ClearMovement();
	return (TryCastSpell(m_UseMountID, me) == SPELL_CAST_OK);
}

void BotFieldAI::Dismount()
{
	if (!me->HasAura(m_UseMountID))
		return;
	me->RemoveOwnedAura(m_UseMountID, ObjectGuid::Empty, 0, AURA_REMOVE_BY_CANCEL);
}

void BotFieldAI::ProcessHorror(uint32 diff)
{
	if (HasAuraMechanic(me, Mechanics::MECHANIC_HORROR) ||
		HasAuraMechanic(me, Mechanics::MECHANIC_DISORIENTED) ||
		HasAuraMechanic(me, Mechanics::MECHANIC_FEAR))
	{
		if (!pHorrorState)
		{
            Position pos = me->GetPosition();
			pHorrorState = new BotAIHorrorState(me);
			me->GetMotionMaster()->Clear();
			m_Movement->ClearMovement();
			me->UpdatePosition(pos, true);
			m_Movement->SyncPosition(pos, true);
			me->SetSelection(ObjectGuid::Empty);
		}
		pHorrorState->UpdateHorror(diff, m_Movement);
	}
	else if (pHorrorState)
	{
		delete pHorrorState;
		pHorrorState = NULL;
		m_Movement->ClearMovement();
	}
}

bool BotFieldAI::HasAuraMechanic(Unit* pTarget, Mechanics mask)
{
	if (!pTarget)
		return false;
	return (pTarget->HasAuraWithMechanic(1 << mask));
}

bool BotFieldAI::IsNotMovement()
{
	if (HasAuraMechanic(me, Mechanics::MECHANIC_ROOT))
	{
		me->StopMoving();
		return true;
	}
	if (IsNotSelect(me))
	{
		me->StopMoving();
		return true;
	}
	//if (me->IsStopped())
	return false;
	//return true;
}

bool BotFieldAI::IsInvincible(Unit* pTarget)
{
	if (HasAuraMechanic(pTarget, Mechanics::MECHANIC_BANISH))
	{
		return true;
	}
	//if (HasAuraMechanic(pTarget, Mechanics::MECHANIC_IMMUNE_SHIELD))
	//{
	//	return true;
	//}
	return false;
}

bool BotFieldAI::IsBGSchedule()
{
	if (me->GetSession()->HasBGSchedule() || me->InBattleground() || me->isUsingLfg())
		return true;
	return false;
}

bool BotFieldAI::CanSelectPlayerEnemy(Player* player)
{
	if (!player || player->GetTeamId() == me->GetTeamId() || IsNotSelect(player))
		return false;
	if (!m_DrivingPVP && !FieldBotMgr::FIELDBOT_DRIVING && !player->IsPvP())
		return false;
	if (TargetIsStealth(player))
		return false;
	if (m_DrivingPVP || FieldBotMgr::FIELDBOT_DRIVING)
	{
		return true;
	}
	else
	{
		Player* pTarget = player->GetSelectedPlayer();
		if (!pTarget)
			return false;
		if (pTarget->GetTeamId() == me->GetTeamId())
		{
			return true;
		}
	}
	return false;
}

Unit* BotFieldAI::GetCombatTarget(float range)
{
	NearUnitVec validTarget;
	NearPlayerList playersNearby;
	me->GetPlayerListInGrid(playersNearby, range);
	for (Player* pVisionPlayer : playersNearby)
	{
		if (CanSelectPlayerEnemy(pVisionPlayer))
			validTarget.push_back(pVisionPlayer);
	}
	bool hasPlayerEnemy = validTarget.empty() ? false : true;
	if (!hasPlayerEnemy)
	{
		NearCreatureVec creatures;
		SearchCreatureListFromRange(me, creatures, range, false);
		for (Creature* pCreature : creatures)
		{
			if (hasPlayerEnemy)
			{
				ObjectGuid guid = pCreature->GetTarget();
				if (guid == ObjectGuid::Empty)
					continue;
				if (guid != me->GetGUID())
					continue;
			}
			validTarget.push_back(pCreature);
		}
	}
	if (validTarget.empty())
		return NULL;
	return validTarget[urand(0, validTarget.size() - 1)];
}

bool BotFieldAI::NonCombatProcess()
{
	{
		std::lock_guard<std::mutex> lock(m_ItemLock);
		if (m_CheckDuel.CheckDuel())
			sPlayerBotMgr->SwitchPlayerBotAI(me, PlayerBotAIType::PBAIT_DUEL, true);
		m_CheckSetting.UpdateCheckSetting();
		//m_Guild.UpdateGuildProcess();
		if (m_AITrade.ProcessTrade())
			return true;
		if (m_UseFood.UpdateBotFood(BOTAI_UPDATE_TICK, m_UseMountID))
			return true;
		if (m_FindLoot.DoFindLoot(BOTAI_UPDATE_TICK, m_Movement, m_UseMountID))
			return true;
		//if (ProcessGroupInvite())
		//	return true;
		m_WishStore.UpdateWishStore();
	}
	return false;
}

bool BotFieldAI::DoFaceToTarget(Unit* pTarget)
{
	float relative = me->GetRelativeAngle(pTarget->GetPositionX(), pTarget->GetPositionY());
	if (relative >= M_PI_2 && !IsNotMovement())// (fabsf(selfAngle) > M_PI_4)
	{
		//me->SetInFront(pTarget);
		//me->SetFacingToObject(pTarget);
		Movement::MoveSplineInit init(me);
		init.MoveTo(me->GetPositionX(), me->GetPositionY(), me->GetPositionZMinusOffset());
		init.SetFacing(pTarget);
		init.SetOrientationFixed(true);
		init.Launch();
		return true;
	}
	return false;
}

SpellCastResult BotFieldAI::TryCastSpell(uint32 spellID, Unit* pTarget, bool force, bool dismount)
{
	if (!spellID || !me->HasSpell(spellID))
	{
		if (spellID)
			SetResetAI();
		return SpellCastResult::SPELL_FAILED_SPELL_LEARNED;
	}
	SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellID);
	if (!spellInfo || spellInfo->IsPassive())
		return SpellCastResult::SPELL_FAILED_UNKNOWN;
	if (!m_WishStore.CanWishStore(spellID, pTarget))
		return SpellCastResult::SPELL_FAILED_UNKNOWN;
    Spell* spell = new Spell(me, spellInfo,
        force ? TriggerCastFlags(TriggerCastFlags::TRIGGERED_IGNORE_POWER_AND_REAGENT_COST | TriggerCastFlags::TRIGGERED_IGNORE_CAST_ITEM) : TriggerCastFlags::TRIGGERED_NONE, ObjectGuid::Empty);
	spell->m_CastItem = NULL;
	SpellCastTargets targets;
	targets.SetUnitTarget(pTarget);
	//spell->InitExplicitTargets(targets);
	//SpellCastResult castResult = spell->CheckCast(true);
	//if (castResult != SpellCastResult::SPELL_CAST_OK)
	//{
	//	spell->finish(false);
	//	delete spell;
	//	return castResult;
	//}
	if (dismount)
		Dismount();
	SpellCastResult castResult = spell->prepare(&targets);
	if (castResult != SpellCastResult::SPELL_CAST_OK)
	{
		if (castResult == SpellCastResult::SPELL_FAILED_NOT_MOUNTED)
			PlayerBotSetting::ClearUnknowMount(me);
		else if (castResult == SpellCastResult::SPELL_FAILED_BAD_TARGETS)
		{
			if (pTarget && me->GetTarget() == pTarget->GetGUID())
				me->SetTarget(ObjectGuid::Empty);
			if (pTarget && pTarget->ToPlayer() && !pTarget->IsPlayerBot() && !pTarget->IsPvP())
			{
				WorldPacket opcode(CMSG_TOGGLE_PVP);
				WorldPackets::Misc::TogglePvP packet(std::move(opcode));
				pTarget->ToPlayer()->GetSession()->HandleTogglePvP(packet);
			}
		}
		//spell->finish(false);
		//delete spell;
		return castResult;
	}
	m_WishStore.TryWishStore(spellID, pTarget);
	return SpellCastResult::SPELL_CAST_OK;
}

SpellCastResult BotFieldAI::TryCastPullSpell(uint32 spellID, Unit* pTarget)
{
	if (!spellID || !me->HasSpell(spellID))
		return SpellCastResult::SPELL_FAILED_SPELL_LEARNED;
	SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellID);
	if (!spellInfo || spellInfo->IsPassive())
		return SpellCastResult::SPELL_FAILED_UNKNOWN;
    Spell* spell = new Spell(me, spellInfo,
        TriggerCastFlags(TriggerCastFlags::TRIGGERED_IGNORE_POWER_AND_REAGENT_COST | TriggerCastFlags::TRIGGERED_IGNORE_SPELL_AND_CATEGORY_CD), ObjectGuid::Empty);
	spell->m_CastItem = NULL;
	SpellCastTargets targets;
	targets.SetUnitTarget(pTarget);
	//spell->InitExplicitTargets(targets);
	//SpellCastResult castResult = spell->CheckCast(true);
	//if (castResult != SpellCastResult::SPELL_CAST_OK)
	//{
	//	spell->finish(false);
	//	delete spell;
	//	return castResult;
	//}
	Dismount();
	SpellCastResult castResult = spell->prepare(&targets);
	if (castResult != SpellCastResult::SPELL_CAST_OK)
	{
		//spell->finish(false);
		//delete spell;
		return castResult;
	}
	return SpellCastResult::SPELL_CAST_OK;
}

SpellCastResult BotFieldAI::PetTryCastSpell(uint32 spellID, Unit* pTarget, bool force)
{
	Pet* pPet = me->GetPet();
	if (!pPet || !pPet->IsAlive())
		return SpellCastResult::SPELL_FAILED_UNKNOWN;
	if (!spellID || !pPet->HasSpell(spellID))
		return SpellCastResult::SPELL_FAILED_SPELL_LEARNED;
	SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellID);
	if (!spellInfo || spellInfo->IsPassive())
		return SpellCastResult::SPELL_FAILED_UNKNOWN;
    Spell* spell = new Spell(pPet, spellInfo, TRIGGERED_NONE);
	pTarget = pTarget ? pTarget : pPet;
	SpellCastResult castResult = spell->CheckPetCast(pTarget);
	if (castResult == SPELL_FAILED_UNIT_NOT_INFRONT && !pPet->isPossessed() && !pPet->IsVehicle())
	{
		if (pTarget && pTarget != pPet)
		{
			pPet->SetInFront(pTarget);
			if (Player* player = pTarget->ToPlayer())
				pPet->SendUpdateToPlayer(player);
		}
		else if (Unit* unit_target2 = spell->m_targets.GetUnitTarget())
		{
			pPet->SetInFront(unit_target2);
			if (Player* player = unit_target2->ToPlayer())
				pPet->SendUpdateToPlayer(player);
		}

		if (Unit* powner = pPet->GetCharmerOrOwner())
			if (Player* player = powner->ToPlayer())
				pPet->SendUpdateToPlayer(player);

		castResult = SPELL_CAST_OK;
	}
	if (castResult == SPELL_CAST_OK)
	{
		pTarget = spell->m_targets.GetUnitTarget();

		//10% chance to play special pet attack talk, else growl
		//actually this only seems to happen on special spells, fire shield for imp, torment for voidwalker, but it's stupid to check every spell
		if (pPet->IsPet() && (((Pet*)pPet)->getPetType() == SUMMON_PET) && (pPet != pTarget) && (urand(0, 100) < 10))
			pPet->SendPetTalk((uint32)PET_TALK_SPECIAL_SPELL);
		else
		{
			pPet->SendPetAIReaction(me->GetPetGUID());
		}

		if (pTarget && !pPet->isPossessed() && !pPet->IsVehicle())
		{
			// This is true if pet has no target or has target but targets differs.
			if (pPet->GetVictim() != pTarget)
			{
				if (pPet->GetVictim())
					pPet->AttackStop();
				pPet->GetMotionMaster()->Clear();
				if (pPet->ToCreature()->IsAIEnabled)
					pPet->ToCreature()->AI()->AttackStart(pTarget);
			}
		}

		return spell->prepare(&(spell->m_targets));
	}
	else
	{
        //if (pPet->isPossessed() || pPet->IsVehicle()) /// @todo: confirm this check
        //    Spell::SendCastResult(me, spellInfo, 0, castResult);
        //else
            spell->SendPetCastResult(castResult);

        if (!pPet->GetSpellHistory()->HasCooldown(spellID))
            pPet->GetSpellHistory()->ResetCooldown(spellID, true);

		spell->finish(false);
		delete spell;

		// reset specific flags in case of spell fail. AI will reset other flags
		if (pPet->GetCharmInfo())
			pPet->GetCharmInfo()->SetIsCommandAttack(false);
	}

	return castResult;
}

void BotFieldAI::SettingPetAutoCastSpell(uint32 spellID, bool autoCast)
{
	Pet* pPet = me->GetPet();
	if (!pPet || !pPet->IsAlive())
		return;
	if (!spellID || !pPet->HasSpell(spellID))
		return;
	SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellID);
	if (!spellInfo || spellInfo->IsPassive() || !spellInfo->IsAutocastable())
		return;
	CharmInfo* charmInfo = pPet->GetCharmInfo();
	if (!charmInfo)
		return;
	if (pPet->IsPet())
		pPet->ToggleAutocast(spellInfo, autoCast);
	else
		pPet->GetCharmInfo()->ToggleCreatureAutocast(spellInfo, autoCast);

	charmInfo->SetSpellAutocast(spellInfo, autoCast);
}

bool BotFieldAI::NeedWaitSpecialSpell(uint32 diff)
{
	if (IsNotSelect(me))
	{
		me->StopMoving();
		return true;
	}
	if (HasAuraMechanic(me, Mechanics::MECHANIC_CHARM))
	{
		me->StopMoving();
		return true;
	}
	if (HasAuraMechanic(me, Mechanics::MECHANIC_DISORIENTED))
	{
		me->StopMoving();
		return true;
	}
	if (HasAuraMechanic(me, Mechanics::MECHANIC_FEAR))
	{
		//me->StopMoving();
		return true;
	}
	if (HasAuraMechanic(me, Mechanics::MECHANIC_SLEEP))
	{
		me->StopMoving();
		return true;
	}
	if (HasAuraMechanic(me, Mechanics::MECHANIC_STUN))
	{
		me->StopMoving();
		return true;
	}
	if (HasAuraMechanic(me, Mechanics::MECHANIC_FREEZE))
	{
		me->StopMoving();
		return true;
	}
	//if (HasAuraMechanic(me, Mechanics::MECHANIC_KNOCKOUT))
	//{
	//	me->StopMoving();
	//	return true;
	//}
	if (HasAuraMechanic(me, Mechanics::MECHANIC_POLYMORPH))
	{
		me->StopMoving();
		return true;
	}
	if (HasAuraMechanic(me, Mechanics::MECHANIC_BANISH))
	{
		me->StopMoving();
		return true;
	}
	if (HasAuraMechanic(me, Mechanics::MECHANIC_HORROR))
	{
		//me->StopMoving();
		return true;
	}
	if (HasAuraMechanic(me, Mechanics::MECHANIC_SAPPED))
	{
		me->StopMoving();
		return true;
	}
	return false;
}

bool BotFieldAI::NeedFlee()
{
	if (m_Flee.Fleeing())
		return true;
	if (IsMeleeBotAI())
	{
		if (me->GetHealthPct() < 25)
			return true;
	}
	return false;
}

void BotFieldAI::FleeMovement()
{
	if (/*me->IsStopped() && */!IsNotMovement())
	{
		NearUnitVec enemys = RangeEnemyListByTargetIsMe(NEEDFLEE_CHECKRANGE);
		Unit* selectEnemy = NULL;
		if (enemys.empty())
		{
			selectEnemy = me->GetSelectedUnit();
			if (!selectEnemy || me->GetDistance(selectEnemy->GetPosition()) > BOTAI_FLEE_JUDGE + 5.0f)
			{
				m_Flee.Clear();
				return;
			}
		}
		if (!selectEnemy && !enemys.empty())
			selectEnemy = enemys[urand(0, enemys.size() - 1)];
		m_Flee.UpdateFleeMovementByPVP(selectEnemy, m_Movement);
	}
	else
		m_Flee.Clear();
}

void BotFieldAI::ProcessFlee()
{
	FleeMovement();
}

void BotFieldAI::ProcessIDLE()
{
	if (!ProcessWarfare())
		m_IDLE.UpdateIDLEMovement(m_Movement);
	else
		m_IDLE.Clear();
}

void BotFieldAI::ProcessHealth()
{
	if (me->HasUnitState(UNIT_STATE_CASTING))
		return;
	NearUnitVec needHealth = SearchNeedHealth(BOTAI_SEARCH_RANGE * 1.5);
	if (needHealth.empty())
	{
		ProcessCombat(me->GetSelectedUnit());
		return;
	}
	Unit* healthPlayer = needHealth[urand(0, needHealth.size() - 1)];
	bool inView = me->IsWithinLOSInMap(healthPlayer);
	if (inView)
	{
		if (me->GetDistance(healthPlayer) > BOTAI_RANGESPELL_DISTANCE - 3)
		{
			if (!IsNotMovement())
				m_Movement->MovementTo(healthPlayer->GetPositionX(), healthPlayer->GetPositionY(), healthPlayer->GetPositionZ());
			return;
		}
		else
		{
			m_Movement->ClearMovement();
			me->SetInFront(healthPlayer);
			//me->SetFacingToObject(healthPlayer);
			ProcessHealthSpell(healthPlayer);
			return;
		}
	}
	else if (!IsNotMovement())
	{
		m_Movement->MovementTo(healthPlayer->GetGUID());
		return;
	}
	if (NeedFlee())
	{
		ProcessFlee();
		return;
	}
}

void BotFieldAI::ProcessCombat(Unit* pTarget)
{
	if (!pTarget)
		return;
	if (TargetIsStealth(pTarget->ToPlayer()))
	{
		me->SetSelection(ObjectGuid::Empty);
		return;
	}
	bool inView = me->IsWithinLOSInMap(pTarget);
	if (inView)
	{
		if (me->HasUnitState(UNIT_STATE_CASTING))
			return;
		if (m_IsRangeBot)
		{
			if (NeedFlee())
			{
				Unit* pVictim = me->GetVictim();
				if (!pVictim || pVictim != pTarget || !me->HasUnitState(UNIT_STATE_MELEE_ATTACKING))
					me->Attack(pTarget, true);
				ProcessFlee();
				if (!IsNotSelect(pTarget))
					ProcessMeleeSpell(pTarget);
			}
			else if (me->GetDistance(pTarget) <= BOTAI_RANGESPELL_DISTANCE)
			{
				Unit* pVictim = me->GetVictim();
				if (!pVictim || pVictim != pTarget)
					me->Attack(pTarget, false);
				if (!IsNotSelect(pTarget))// && !DoFaceToTarget(pTarget))
				{
					me->SetInFront(pTarget);
					me->SetFacingToObject(pTarget);
					if (me->getClass() != Classes::CLASS_HUNTER)
						m_Movement->ClearMovement();
					ChaseTarget(pTarget, false, BOTAI_RANGESPELL_DISTANCE);
					ProcessRangeSpell(pTarget);
					if (!me->HasUnitState(UNIT_STATE_CASTING))
						DoRangedAttackIfReady();
				}
			}
			else if (!IsNotMovement())
			{
				m_Movement->MovementToTarget();
			}
		}
		else
		{
			if (me->IsWithinMeleeRange(pTarget))
			{
				me->SetInFront(pTarget);
				me->SetFacingToObject(pTarget);
				//if (!DoFaceToTarget(pTarget))
				{
					Unit* pVictim = me->GetVictim();
					if (!pVictim || pVictim != pTarget)
						me->Attack(pTarget, true);
					if (!IsNotMovement())
						ChaseTarget(pTarget, true);
					if (!IsNotSelect(pTarget))
						ProcessMeleeSpell(pTarget);
				}
			}
			else
			{
				if (!IsNotMovement())
					m_Movement->MovementToTarget();
				if (me->GetDistance(pTarget) < BOTAI_RANGESPELL_DISTANCE && pTarget->IsAlive() && !me->IsVehicle())
				{
					ProcessRangeSpell(pTarget);
				}
			}
		}
	}
	else if (!IsNotMovement())
		m_Movement->MovementToTarget();
}

bool BotFieldAI::ProcessWarfare()
{
	if (m_WarfareTargetID != ObjectGuid::Empty)
	{
		Player* pTarget = ObjectAccessor::FindPlayer(m_WarfareTargetID);
		if (pTarget)
		{
			if (pTarget->GetMapId() == me->GetMapId() && !pTarget->InBattleground())
			{
				if (me->GetDistance(pTarget->GetPosition()) > BOTAI_RANGESPELL_DISTANCE)
				{
					m_Movement->MovementTo(pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 20);
					return true;
				}
			}
		}
		else
		{
			SetWarfareTarget(NULL);
			return false;
		}
	}
	return false;
}

void BotFieldAI::ChaseTarget(Unit* pTarget, bool isMelee, float range)
{
	if (IsNotSelect(pTarget))
		return;
	if (isMelee && pTarget->ToPlayer())
	{
		if (me->IsStopped())
		{
			Position targetPos = pTarget->GetPosition();
			float rndOffset = frand(-float(M_PI_4) * 0.75f, float(M_PI_4) * 0.75f);
			Position pos = me->GetFirstCollisionPosition(me->GetDistance(targetPos) + range, me->GetRelativeAngle(&targetPos) + rndOffset);
			m_Movement->MovementTo(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ());
			//me->GetMotionMaster()->MovePoint(0, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ());
		}
	}
	else
	{
		//if (me->isInBack(pTarget) && !IsNotMovement())// (fabsf(selfAngle) > M_PI_4)
		//{
		//	Movement::MoveSplineInit init(me);
		//	init.MoveTo(me->GetPositionX(), me->GetPositionY(), me->GetPositionZMinusOffset());
		//	init.SetFacing(pTarget);
		//	init.SetOrientationFixed(true);
		//	init.Launch();
		//}
		//else// if (!IsNotMovement())
		if (me->GetDistance(pTarget->GetPosition()) > range)
		{
			me->GetMotionMaster()->Clear();
			me->GetMotionMaster()->MoveChase(pTarget, range);
		}
		else
			m_Movement->ClearMovement();
	}
}

void BotFieldAI::SearchCreatureListFromRange(Unit* center, NearCreatureVec& nearCreatures, float range, bool selfFaction)
{
	NearCreatureList nearCreature;
	Trinity::AllWorldObjectsInRange checker(center, range);
	Trinity::CreatureListSearcher<Trinity::AllWorldObjectsInRange> searcher(center, nearCreature, checker);
	//center->VisitNearbyGridObject(range, searcher);
	for (Creature* pCreature : nearCreature)
	{
		if (!pCreature->IsAlive() || pCreature->IsPet())// || pCreature->IsTotem())
			continue;
		if (pCreature->IsInEvadeMode())
			continue;
		if (selfFaction && me->IsValidAttackTarget(pCreature))
			continue;
		if (!selfFaction && !me->IsValidAttackTarget(pCreature))
			continue;
		if (FieldBotMgr::FIELDBOT_CREATURE || selfFaction)
		{
			nearCreatures.push_back(pCreature);
		}
		else if (!selfFaction)
		{
			ObjectGuid targetGUID = pCreature->GetTarget();
			if (targetGUID != ObjectGuid::Empty)
				nearCreatures.push_back(pCreature);
		}
	}
}

NearUnitVec BotFieldAI::SearchFriend(float range)
{
	NearPlayerList playersNearby;
	NearUnitVec friendNearby;
	me->GetPlayerListInGrid(playersNearby, range);
	for (Player* pVisionPlayer : playersNearby)
	{
		if (!IsNotSelect(pVisionPlayer) && pVisionPlayer->GetTeamId() == me->GetTeamId())
		{
			friendNearby.push_back(pVisionPlayer);
		}
	}
	//NearCreatureVec creatures;
	//SearchCreatureListFromRange(me, creatures, range, true);
	//for (Creature* pCreature : creatures)
	//	friendNearby.push_back(pCreature);
	return friendNearby;
}

NearPlayerVec BotFieldAI::SearchFarFriend(float minRange, float maxRange, bool isIDLE)
{
	NearPlayerList playersNearby;
	NearPlayerVec friendNearby;
	me->GetPlayerListInGrid(playersNearby, maxRange);
	for (Player* pVisionPlayer : playersNearby)
	{
		if (!IsNotSelect(pVisionPlayer) && pVisionPlayer->GetTeamId() == me->GetTeamId())
		{
			if (me->GetDistance(pVisionPlayer->GetPosition()) > minRange)
			{
				if (isIDLE)
				{
					if (pVisionPlayer->GetSelectedUnit() == NULL || !pVisionPlayer->IsInCombat())
						friendNearby.push_back(pVisionPlayer);
				}
				else
					friendNearby.push_back(pVisionPlayer);
			}
		}
	}
	return friendNearby;
}

NearPlayerVec BotFieldAI::ExistFriendAttacker(float range /* = BOTAI_RANGESPELL_DISTANCE */)
{
	NearPlayerList playersNearby;
	NearPlayerVec friendNearby;
	me->GetPlayerListInGrid(playersNearby, range);
	for (Player* pVisionPlayer : playersNearby)
	{
		if (!IsNotSelect(pVisionPlayer) && pVisionPlayer->GetTeamId() == me->GetTeamId() && IsAttacker())
		{
			friendNearby.push_back(pVisionPlayer);
		}
	}

	return friendNearby;
}

NearUnitVec BotFieldAI::SearchNeedHealth(float range /* = BOTAI_SEARCH_RANGE */)
{
	NearPlayerList playersNearby;
	NearUnitVec lifeTo25, life25To55, life55To80;
	me->GetPlayerListInGrid(playersNearby, range);
	for (Player* pVisionPlayer : playersNearby)
	{
		if (!IsNotSelect(pVisionPlayer) && pVisionPlayer->GetTeamId() == me->GetTeamId())
		{
			float healthPct = pVisionPlayer->GetHealthPct();
			if (healthPct < 25)
				lifeTo25.push_back(pVisionPlayer);
			else if (healthPct >= 25 && healthPct < 55)
				life25To55.push_back(pVisionPlayer);
			else if (healthPct >= 55 && healthPct < 80)
				life55To80.push_back(pVisionPlayer);
		}
	}
	NearCreatureVec creatures;
	SearchCreatureListFromRange(me, creatures, range, true);
	for (Creature* pCreature : creatures)
	{
		float healthPct = pCreature->GetHealthPct();
		if (healthPct < 25)
			lifeTo25.push_back(pCreature);
		else if (healthPct >= 25 && healthPct < 55)
			life25To55.push_back(pCreature);
		else if (healthPct >= 55 && healthPct < 80)
			life55To80.push_back(pCreature);
	}

	uint32 rate = urand(0, 99);
	if (rate >= 85 && life55To80.size() > 0)
		return life55To80;
	else if (rate < 85 && rate >= 55 && life25To55.size() > 0)
		return life25To55;
	else if (lifeTo25.size() > 0)
		return lifeTo25;
	else if (life25To55.size() > 0)
		return life25To55;
	return life55To80;
}

NearUnitVec BotFieldAI::SearchLifePctByFriendRange(Unit* pTarget, float lifePct, float range /* = NEEDFLEE_CHECKRANGE */)
{
	NearPlayerList playersNearby;
	NearUnitVec lifePctPlayers;
	pTarget->GetPlayerListInGrid(playersNearby, range);
	for (Player* pVisionPlayer : playersNearby)
	{
		if (!IsNotSelect(pVisionPlayer) && pVisionPlayer->GetTeamId() == me->GetTeamId())
		{
			float healthPct = pVisionPlayer->GetHealthPct();
			if (healthPct <= lifePct)
				lifePctPlayers.push_back(pVisionPlayer);
		}
	}
	//NearCreatureVec creatures;
	//SearchCreatureListFromRange(me, creatures, range, true);
	//for (Creature* pCreature : creatures)
	//{
	//	float healthPct = pCreature->GetHealthPct();
	//	if (healthPct <= lifePct)
	//		lifePctPlayers.push_back(pCreature);
	//}

	return lifePctPlayers;
}

Unit* BotFieldAI::RandomRangeEnemyByCasting(float range)
{
	NearUnitVec enemyPlayers;
	NearPlayerList playersNearby;
	me->GetPlayerListInGrid(playersNearby, range);
	for (Player* pVisionPlayer : playersNearby)
	{
		if (!IsNotSelect(pVisionPlayer) && pVisionPlayer->GetTeamId() != me->GetTeamId())
		{
			if (pVisionPlayer->HasUnitState(UNIT_STATE_CASTING))
				enemyPlayers.push_back(pVisionPlayer);
		}
	}
	NearCreatureVec creatures;
	SearchCreatureListFromRange(me, creatures, range, false);
	for (Creature* pCreature : creatures)
	{
		if (pCreature->HasUnitState(UNIT_STATE_CASTING))
			enemyPlayers.push_back(pCreature);
	}
	if (!enemyPlayers.empty())
	{
		uint32 index = urand(0, enemyPlayers.size() - 1);
		return enemyPlayers[index];
	}
	return NULL;
}

NearUnitVec BotFieldAI::RangeEnemyListByHasAura(uint32 aura, float range)
{
	NearUnitVec enemyPlayers;
	NearPlayerList playersNearby;
	me->GetPlayerListInGrid(playersNearby, range);
	for (Player* pVisionPlayer : playersNearby)
	{
		if (!IsNotSelect(pVisionPlayer) && pVisionPlayer->GetTeamId() != me->GetTeamId())
		{
			if (TargetIsStealth(pVisionPlayer))
				continue;
			if (aura == 0 || pVisionPlayer->HasAura(aura))
				enemyPlayers.push_back(pVisionPlayer);
		}
	}
	NearCreatureVec creatures;
	SearchCreatureListFromRange(me, creatures, range, false);
	for (Creature* pCreature : creatures)
	{
		if (aura == 0 || pCreature->HasAura(aura))
			enemyPlayers.push_back(pCreature);
	}
	return enemyPlayers;
}

NearUnitVec BotFieldAI::RangeEnemyListByNonAura(uint32 aura, float range)
{
	NearUnitVec enemyPlayers;
	if (aura == 0)
		return enemyPlayers;
	NearPlayerList playersNearby;
	me->GetPlayerListInGrid(playersNearby, range);
	for (Player* pVisionPlayer : playersNearby)
	{
		if (!IsNotSelect(pVisionPlayer) && pVisionPlayer->GetTeamId() != me->GetTeamId())
		{
			if (TargetIsStealth(pVisionPlayer))
				continue;
			if (!pVisionPlayer->HasAura(aura))
				enemyPlayers.push_back(pVisionPlayer);
		}
	}
	NearCreatureVec creatures;
	SearchCreatureListFromRange(me, creatures, range, false);
	for (Creature* pCreature : creatures)
	{
		if (!pCreature->HasAura(aura))
			enemyPlayers.push_back(pCreature);
	}
	return enemyPlayers;
}

NearUnitVec BotFieldAI::RangeEnemyListByTargetIsMe(float range)
{
	NearUnitVec enemyPlayers;
	NearPlayerList playersNearby;
	me->GetPlayerListInGrid(playersNearby, range);
	for (Player* pVisionPlayer : playersNearby)
	{
		if (!IsNotSelect(pVisionPlayer) && pVisionPlayer->GetTeamId() != me->GetTeamId())
		{
			if (TargetIsStealth(pVisionPlayer))
				continue;
			Unit* pUnit = pVisionPlayer->GetSelectedUnit();
			if (pUnit && pUnit->GetGUID() == me->GetGUID())
				enemyPlayers.push_back(pVisionPlayer);
		}
	}
	NearCreatureVec creatures;
	SearchCreatureListFromRange(me, creatures, range, false);
	for (Creature* pCreature : creatures)
	{
		if (pCreature->GetTarget() == me->GetGUID())
			enemyPlayers.push_back(pCreature);
	}
	return enemyPlayers;
}

NearUnitVec BotFieldAI::RangeListByTargetIsTarget(Unit* pTarget, float range)
{
	NearUnitVec enemyPlayers;
	NearPlayerList playersNearby;
	pTarget->GetPlayerListInGrid(playersNearby, range);
	for (Player* pVisionPlayer : playersNearby)
	{
		if (!IsNotSelect(pVisionPlayer) && pVisionPlayer->GetTeamId() == me->GetTeamId())
		{
			if (TargetIsStealth(pVisionPlayer))
				continue;
			if (pVisionPlayer->GetSelectedUnit() == pTarget)
				enemyPlayers.push_back(pVisionPlayer);
		}
	}
	NearCreatureVec creatures;
	SearchCreatureListFromRange(pTarget, creatures, range, false);
	for (Creature* pCreature : creatures)
	{
		if (pCreature->GetTarget() == pTarget->GetGUID())
			enemyPlayers.push_back(pCreature);
	}
	return enemyPlayers;
}

NearUnitVec BotFieldAI::RangeEnemyListByTargetRange(Unit* pTarget, float range)
{
	NearUnitVec enemyPlayers;
	NearPlayerList playersNearby;
	pTarget->GetPlayerListInGrid(playersNearby, range);
	for (Player* pVisionPlayer : playersNearby)
	{
		if (!IsNotSelect(pVisionPlayer) && pVisionPlayer->GetTeamId() != me->GetTeamId())
		{
			if (TargetIsStealth(pVisionPlayer))
				continue;
			enemyPlayers.push_back(pVisionPlayer);
		}
	}
	NearCreatureVec creatures;
	SearchCreatureListFromRange(pTarget, creatures, range, false);
	for (Creature* pCreature : creatures)
		enemyPlayers.push_back(pCreature);
	return enemyPlayers;
}

NearUnitVec BotFieldAI::SearchFarEnemy(float minRange, float maxRange)
{
	NearUnitVec enemyNearby;
	NearCreatureVec creatures;
	SearchCreatureListFromRange(me, creatures, maxRange, false);
	for (Creature* pCreature : creatures)
	{
		if (me->GetDistance(pCreature->GetPosition()) > minRange)
			enemyNearby.push_back(pCreature);
	}
	return enemyNearby;
}

bool BotFieldAI::IsMeleeBotAI()
{
	switch (me->getClass())
	{
	case CLASS_WARRIOR:
	case CLASS_PALADIN:
	case CLASS_ROGUE:
	case CLASS_DEATH_KNIGHT:
	case CLASS_SHAMAN:
	case CLASS_DRUID:
		return true;
	case CLASS_MAGE:
	case CLASS_WARLOCK:
	case CLASS_PRIEST:
	case CLASS_HUNTER:
		return false;
	}
	return true;
}

bool BotFieldAI::IsRangeBotAI()
{
	switch (me->getClass())
	{
	case CLASS_WARRIOR:
	case CLASS_PALADIN:
	case CLASS_ROGUE:
	case CLASS_DEATH_KNIGHT:
		return false;
	case CLASS_MAGE:
	case CLASS_WARLOCK:
	case CLASS_PRIEST:
	case CLASS_HUNTER:
	case CLASS_SHAMAN:
	case CLASS_DRUID:
		return true;
	}
	return false;
}

bool BotFieldAI::IsHealerBotAI()
{
	switch (me->getClass())
	{
	case CLASS_WARRIOR:
	case CLASS_ROGUE:
	case CLASS_DEATH_KNIGHT:
	case CLASS_MAGE:
	case CLASS_WARLOCK:
	case CLASS_HUNTER:
		return false;
	case CLASS_PALADIN:
	case CLASS_PRIEST:
	case CLASS_SHAMAN:
	case CLASS_DRUID:
		return true;
	}
	return false;
}

Unit* BotFieldAI::GetBotAIValidSelectedUnit()
{
	Unit* pTarget = me->GetSelectedUnit();
	bool isValid = true;
	if (!pTarget)
		isValid = false;
	if (isValid && !pTarget->IsVisible())
		isValid = false;
	if (isValid && !me->InSamePhase(pTarget->GetPhaseShift()))
		isValid = false;
	if (isValid && IsNotSelect(pTarget))
		isValid = false;
	if (isValid && pTarget->ToCreature() && pTarget->ToCreature()->IsInEvadeMode())
		isValid = false;
	if (!isValid)
	{
		me->AttackStop();
		me->SetSelection(ObjectGuid::Empty);
		return NULL;
	}
	return pTarget;
}

bool BotFieldAI::TargetIsRange(Player* pTarget)
{
	if (!pTarget)
		return false;
	switch (pTarget->getClass())
	{
	case CLASS_WARRIOR:
	case CLASS_PALADIN:
	case CLASS_ROGUE:
	case CLASS_DEATH_KNIGHT:
		return false;
	case CLASS_MAGE:
	case CLASS_WARLOCK:
	case CLASS_HUNTER:
	case CLASS_PRIEST:
	case CLASS_SHAMAN:
	case CLASS_DRUID:
		return true;
	}
	return false;
}

bool BotFieldAI::TargetIsMagic(Player* pTarget)
{
	if (!pTarget)
		return false;
	switch (pTarget->getClass())
	{
	case CLASS_WARRIOR:
	case CLASS_ROGUE:
	case CLASS_DEATH_KNIGHT:
		return false;
	case CLASS_PALADIN:
	case CLASS_MAGE:
	case CLASS_WARLOCK:
	case CLASS_HUNTER:
	case CLASS_PRIEST:
	case CLASS_SHAMAN:
	case CLASS_DRUID:
		return true;
	}
	return false;
}

bool BotFieldAI::TargetIsCastMagic(Player* pTarget)
{
	if (!pTarget)
		return false;
	switch (pTarget->getClass())
	{
	case CLASS_WARRIOR:
	case CLASS_ROGUE:
	case CLASS_DEATH_KNIGHT:
	case CLASS_PALADIN:
	case CLASS_HUNTER:
		return false;
	case CLASS_MAGE:
	case CLASS_WARLOCK:
	case CLASS_PRIEST:
	case CLASS_SHAMAN:
	case CLASS_DRUID:
		return true;
	}
	return false;
}

bool BotFieldAI::TargetIsStealth(Player* pTarget)
{
	if (!pTarget)
		return false;
	// (1784     Ǳ   || 5215   ³  Ǳ   || 66   ʦ     || 58984   ҹ    )
	if (pTarget->HasAura(1784) || pTarget->HasAura(5215) ||
		pTarget->HasAura(66) || pTarget->HasAura(58984))
	{
		if (!me->CanSeeOrDetect(pTarget, false, true)) //    Ǳ  
			return true;
	}
	return false;
}
