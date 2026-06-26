/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
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

#ifndef TRINITY_PLAYERAI_H
#define TRINITY_PLAYERAI_H

#include "UnitAI.h"

class Creature;
class Spell;

class TC_GAME_API PlayerAI : public UnitAI
{
public:


    // ==========================================
    // 【AI 导演系统专属控制接口】
    // ==========================================
    void SetDirectorSleep(bool sleep) { m_isDirectorSleeping = sleep; }
    bool IsDirectorSleeping() const { return m_isDirectorSleeping; }
    void SetDirectorDrafted(bool drafted) { m_isDirectorDrafted = drafted; }
    // 【修改】加上 map 参数
    void SetDirectorAnchor(uint32 map, float x, float y, float z) {
        m_directorAnchorMapId = map;
        m_directorAnchorX = x;
        m_directorAnchorY = y;
        m_directorAnchorZ = z;
    }


    explicit PlayerAI(Player* player);

    Creature* GetCharmer() const;

    void OnCharmed(bool /*apply*/) override { } // charm AI application for players is handled by Unit::SetCharmedBy / Unit::RemoveCharmedBy

    // helper functions to determine player info
    uint16 GetSpec(Player const* who = nullptr) const;
    static bool IsPlayerHealer(Player const* who);
    bool IsHealer(Player const* who = nullptr) const { return (!who || who == me) ? _isSelfHealer : IsPlayerHealer(who); }
    static bool IsPlayerRangedAttacker(Player const* who);
    bool IsRangedAttacker(Player const* who = nullptr) const { return (!who || who == me) ? _isSelfRangedAttacker : IsPlayerRangedAttacker(who); }

protected:

    // ==========================================
    // 【AI 导演系统变量】默认所有机器人出生即休眠！
    // ==========================================
    bool m_isDirectorSleeping = true; 
    uint32 m_directorCheckTimer = 3000;

    
    // 【手动停留开关】
    bool m_isCommandStopped = false;
   // 【新增】：群演卖身契标记 (默认是 false，代表原生自由人)
    bool m_isDirectorDrafted = false;

   // 【新增】：提线木偶漫步系统
    float m_directorAnchorX = 0.0f;
    float m_directorAnchorY = 0.0f;
    float m_directorAnchorZ = 0.0f;
    uint32 m_directorWanderTimer = 0; // 专属溜达计时器
    uint32 m_directorAnchorMapId = 0; // 【新增】剧组所在地图
   
    // 【新增】：高级 RPG 交互系统
    uint32 m_directorInteractTimer = 0;  // 驻留交互倒计时
    uint64 m_directorInteractGuid = 0;   // 正在交互的目标 GUID



    struct TargetedSpell : public std::pair<Spell*, Unit*>
    {
        TargetedSpell() : pair<Spell*, Unit*>() { }
        TargetedSpell(Spell* first, Unit* second) : pair<Spell*, Unit*>(first, second) { }
        explicit operator bool() { return !!first; }
    };
    typedef std::pair<TargetedSpell, uint32> PossibleSpell;
    typedef std::vector<PossibleSpell> PossibleSpellVector;

    Player* const me;
    void SetIsRangedAttacker(bool state) { _isSelfRangedAttacker = state; } // this allows overriding of the default ranged attacker detection

    enum SpellTarget
    {
        TARGET_NONE,
        TARGET_VICTIM,
        TARGET_CHARMER,
        TARGET_SELF
    };
    /* Check if the specified spell can be cast on that target.
       Caller is responsible for cleaning up created Spell object from pointer. */
    TargetedSpell VerifySpellCast(uint32 spellId, Unit* target);
    /* Check if the specified spell can be cast on that target.
    Caller is responsible for cleaning up created Spell object from pointer. */
    TargetedSpell VerifySpellCast(uint32 spellId, SpellTarget target);

    /* Helper method - checks spell cast, then pushes it onto provided vector if valid. */
    template<typename T> inline void VerifyAndPushSpellCast(PossibleSpellVector& spells, uint32 spellId, T target, uint32 weight)
    {
        if (TargetedSpell spell = VerifySpellCast(spellId, target))
            spells.push_back({ spell,weight });
    }

    /* Helper method - selects one spell from the vector and returns it, while deleting everything else.
       This invalidates the vector, and empties it to prevent accidental misuse. */
    TargetedSpell SelectSpellCast(PossibleSpellVector& spells);
    /* Helper method - casts the included spell at the included target */
    void DoCastAtTarget(TargetedSpell spell);

    virtual Unit* SelectAttackTarget() const;
    void DoRangedAttackIfReady();
    void DoAutoAttackIfReady();

    // Cancels all shapeshifts that the player could voluntarily cancel
    void CancelAllShapeshifts();

private:
    uint16 const _selfSpec;
    bool const _isSelfHealer;
    bool _isSelfRangedAttacker;
};

class TC_GAME_API SimpleCharmedPlayerAI : public PlayerAI
{
public:
    SimpleCharmedPlayerAI(Player* player) : PlayerAI(player), _castCheckTimer(2500), _chaseCloser(false), _forceFacing(true), _isFollowing(false) { }
    void UpdateAI(uint32 diff) override;
    void OnCharmed(bool isNew) override;

protected:
    Unit* SelectAttackTarget() const override;

private:
    TargetedSpell SelectAppropriateCastForSpec();
    uint32 _castCheckTimer;
    bool _chaseCloser;
    bool _forceFacing;
    bool _isFollowing;
};

#endif
