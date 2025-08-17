/*
* Copyright (C) 2017-2018 AshamaneProject <https://github.com/AshamaneProject>
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

#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "Containers.h"
#include "ObjectAccessor.h"
#include "Pet.h"
#include "Log.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "Item.h"
#include "SpellScript.h"
#include "SpellMgr.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "TemporarySummon.h"
#include "GridNotifiers.h"

enum SpellIds
{
    SPELL_DH_FURY_OF_THE_ILLIDARI_AT                = 202796,
    SPELL_DH_FURY_OF_THE_ILLIDARI_MH                = 201628,
    SPELL_DH_FURY_OF_THE_ILLIDARI_OH                = 201789,
    SPELL_DRUID_ASHAMANE_FRENZY                     = 210722,
    SPELL_DRUID_ASHAMANE_FRENZY_DAMAGE              = 210723,
    SPELL_DRUID_NEW_MOON                            = 202767,
    SPELL_DRUID_NEW_MOON_OVERRIDE                   = 202787,
    SPELL_DRUID_HALF_MOON                           = 202768,
    SPELL_DRUID_HALF_MOON_OVERRIDE                  = 202788,
    SPELL_DRUID_FULL_MOON                           = 202771,
    SPELL_DRUID_FULL_MOON_OVERRIDE                  = 202789,
    SPELL_MAGE_BRAIN_FREEZE_AURA                    = 190446,
    SPELL_MAGE_IMMOLATION                           = 211918,
    SPELL_MAGE_PHOENIX_FLAMES                       = 194466,
    SPELL_MAGE_PHOENIX_FLAMES_TRIGGER               = 224637,
    SPELL_PRIEST_CALL_OF_THE_VOID                   = 193371,
    SPELL_PRIEST_CALL_OF_THE_VOID_SUMMON            = 193470,
    SPELL_PRIEST_MIND_FLAY                          = 193473,
    SPELL_SHAMAN_CARESS_OF_THE_TIDEMOTHER           = 207354,
    SPELL_SHAMAN_HEALING_STREAM_TOTEM               = 5394,
    SPELL_SHAMAN_REINCARNATION                      = 20608,
    SPELL_SHAMAN_SERVANT_OF_THE_QUEEN               = 207357,
    SPELL_SHAMAN_FURY_OF_THE_STORMS                 = 191717,
    SPELL_SHAMAN_SUMMON_LIGHTHING_ELEMENTAL         = 191716,
    SPELL_WARLOCK_DEADWIND_HARVERST                 = 216708,
    SPELL_WARLOCK_TORMENTED_SOULS                   = 216695,
    SPELL_WARLOCK_THALKIELS_CONSUMPTION_DAMAGE      = 211715,
    SPELL_WARLOCK_TEAR_CHAOS_BARRAGE                = 187394,
    SPELL_WARLOCK_TEAR_CHAOS_BOLT                   = 215279,
    SPELL_WARLOCK_TEAR_SHADOW_BOLT                  = 196657,
    SPELL_HUNTER_HATIS_BOND                         = 197344,
    SPELL_HUNTER_STORMBOUND                         = 197388,
    SPELL_HUNTER_BROKENOUND                         = 211117,
    SPELL_HUNTER_BESTIAL_WRATH                      = 19574,
    SPELL_HUNTER_TITANS_THUNDER                     = 207068,
    SPELL_HUNTER_TITANS_THUNDER_TRIGGER             = 207081,
    SPELL_HUNTER_TITANS_THUNDER_AURA                = 207094,
    SPELL_HUNTER_TITANS_THUNDER_DAMAGE              = 218635,
    SPELL_PALADIN_TYR_DELIVERANCE_HEAL              = 200654,
    SPELL_PRIEST_SPHERE_OF_INSANITY                 = 194179,
    SPELL_PRIEST_SPHERE_OF_INSANITY_AURA            = 194230,
    SPELL_PRIEST_SPHERE_OF_INSANITY_TRIGGER         = 194182,
    SPELL_PRIEST_SPHERE_OF_INSANITY_DAMAGE          = 194238,
    SPELL_PRIEST_SHADOW_WORD_PAIN                   = 589,
    SPELL_PRIEST_MIND_BLAST                         = 8092,
    SPELL_MAGE_MARK_OF_ALUNETH                      = 224968,
    SPELL_MAGE_MARK_OF_ALUNETH_AOE                  = 211088,
    SPELL_MAGE_MARK_OF_ALUNETH_DAMAGE               = 211076,
    SPELL_MAGE_MARK_OF_ALUNETH_SPEED                = 211056,
};

enum ShatteredSoulsSpells
{
     SPELL_DH_SHATTERED_SOULS = 204255,
    SPELL_DH_SHATTERED_SOULS_DEMON = 204256,
    SPELL_DH_LESSER_SOUL_SHARD = 203795,
     SPELL_DH_SHATTERED_SOULS_MISSILE = 209651,
    SPELL_DH_SOUL_FRAGMENT_HEAL_25_HAVOC = 178963,
    SPELL_DH_SOUL_FRAGMENT_DEMON_BONUS = 163073,
    SPELL_DH_SHATTERED_SOULS_AT_DEMON = 203795,
    SPELL_DH_SOUL_FRAGMENT_HEAL_VENGEANCE = 210042,
    SPELL_DH_LESSER_SOUL_SHARD_HEAL = 203794,
    SPELL_DH_CONSUME_SOUL_MISSILE = 210047,
    SPELL_DH_LESSER_SOUL_FRAGMENT_HAVOC = 228532,
    SPELL_DH_PAINBRINGER = 207387,
    SPELL_DH_PAINBRINGER_BUFF = 212988,
    SPELL_DH_DEVOUR_SOULS = 212821,
    SPELL_DH_CHARRED_WARBLADES_HEAL = 213011,
    SPELL_DH_SHATTER_THE_SOULS = 212827,
    SPELL_DH_FIERY_DEMISE_DEBUFF = 212818,
    SPELL_DH_COVER_OF_DARKNESS = 227635,
    SPELL_DH_METAMORPHOSIS_VENGEANCE = 187827,
};

// Ebonbolt - 214634
class spell_arti_mage_ebonbolt : public SpellScript
{
    PrepareSpellScript(spell_arti_mage_ebonbolt);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        caster->CastSpell(caster, SPELL_MAGE_BRAIN_FREEZE_AURA, true);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_arti_mage_ebonbolt::HandleDummy, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
    }
};

// Reap Souls - 216698
class spell_arti_warl_reap_souls : public SpellScript
{
    PrepareSpellScript(spell_arti_warl_reap_souls);

    void HandleHit(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        caster->CastSpell(caster, SPELL_WARLOCK_DEADWIND_HARVERST, true);
    }

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();
        if (!caster)
            return SPELL_FAILED_DONT_REPORT;

        if (!caster->HasAura(SPELL_WARLOCK_TORMENTED_SOULS))
            return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

        return SPELL_CAST_OK;
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_arti_warl_reap_souls::CheckCast);
        OnEffectHitTarget += SpellEffectFn(spell_arti_warl_reap_souls::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// Deadwind harvest - 216708
class spell_arti_warl_deadwind_harvest : public AuraScript
{
    PrepareAuraScript(spell_arti_warl_deadwind_harvest);

    void CalcAmount(AuraEffect const* /*aurEff*/, int32& /*amount*/, bool& /*canBeRecalculated*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        int32 stackAmount = 0;
        if (Aura* aur = caster->GetAura(SPELL_WARLOCK_TORMENTED_SOULS))
            stackAmount = aur->GetStackAmount();

        int32 duration = GetAura()->GetDuration() * stackAmount;
        GetAura()->SetMaxDuration(duration);
        GetAura()->SetDuration(duration);

        caster->RemoveAurasDueToSpell(SPELL_WARLOCK_TORMENTED_SOULS);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_arti_warl_deadwind_harvest::CalcAmount, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
    }
};

// Phoenix Flames - 194466
class spell_arti_mage_phoenix_flames : public SpellScript
{
    PrepareSpellScript(spell_arti_mage_phoenix_flames);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MAGE_PHOENIX_FLAMES_TRIGGER, SPELL_MAGE_PHOENIX_FLAMES });
    }

    void HandleHit(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;

        caster->CastSpell(target, SPELL_MAGE_PHOENIX_FLAMES_TRIGGER, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_arti_mage_phoenix_flames::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// Phoenix Flames Trigger - 224637
class spell_arti_mage_phoenix_flames_trigger : public SpellScript
{
    PrepareSpellScript(spell_arti_mage_phoenix_flames_trigger);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MAGE_PHOENIX_FLAMES_TRIGGER, SPELL_MAGE_PHOENIX_FLAMES });
    }

    void HandleHit(SpellEffIndex /*effIndex*/)
    {
        Unit* target = GetHitUnit();
        Unit* originalTarget = GetExplTargetUnit();
        if (!target || !originalTarget)
            return;

        if (originalTarget == target)
            SetHitDamage(0);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_arti_mage_phoenix_flames_trigger::HandleHit, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// Immolation (artifact passive) - 211918
class spell_arti_mage_immolation : public AuraScript
{
    PrepareAuraScript(spell_arti_mage_immolation);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MAGE_IMMOLATION, SPELL_MAGE_PHOENIX_FLAMES });
    }

public:

    spell_arti_mage_immolation()
    {
        mod = new SpellModifier(GetAura());
        mod->op = SPELLMOD_CRITICAL_CHANCE;
        mod->type = SPELLMOD_FLAT;
        mod->spellId = SPELL_MAGE_IMMOLATION;
        mod->value = 200;
        mod->mask[3] = 0x20000000;
    }

private:

    SpellModifier* mod = nullptr;

    void HandleApply(AuraEffect const* /*aurEffect*/, AuraEffectHandleModes /*mode*/)
    {
        Player* player = GetCaster()->ToPlayer();
        if (!player)
            return;

        player->AddSpellMod(mod, true);
    }

    void HandleRemove(AuraEffect const* /*aurEffect*/, AuraEffectHandleModes /*mode*/)
    {
        Player* player = GetCaster()->ToPlayer();
        if (!player)
            return;

        player->AddSpellMod(mod, false);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_arti_mage_immolation::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_arti_mage_immolation::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// Fury of the Illidari - 201467
// AreaTriggerID - 5758
struct at_dh_fury_of_the_illidari : AreaTriggerAI
{
    int32 timeInterval;

    at_dh_fury_of_the_illidari(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger)
    {
        // How often should the action be executed
        timeInterval = 420; // Blaze it (blizzard wtf, 7 times under 3 seconds)
    }

    void OnUpdate(uint32 p_Time) override
    {
        Unit* caster = at->GetCaster();
        if (!caster || !caster->IsPlayer())
            return;

        // Check if we can handle actions
        timeInterval += p_Time;
        if (timeInterval < 420)
            return;

        caster->CastSpell(at->GetPosition(), SPELL_DH_FURY_OF_THE_ILLIDARI_MH, true);
        caster->CastSpell(at->GetPosition(), SPELL_DH_FURY_OF_THE_ILLIDARI_OH, true);

        timeInterval -= 420;
    }
};

//207357 - Servant of the queen
class spell_arti_sha_servant_of_the_queen : public AuraScript
{
    PrepareAuraScript(spell_arti_sha_servant_of_the_queen);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetSpellInfo())
            return false;

        if(eventInfo.GetSpellInfo()->Id == SPELL_SHAMAN_REINCARNATION)
            return true;
        else
            return false;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_arti_sha_servant_of_the_queen::CheckProc);
    }
};

//207354 - Caress of the tidemother
class spell_arti_sha_caress_of_the_tidemother : public AuraScript
{
    PrepareAuraScript(spell_arti_sha_caress_of_the_tidemother);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if(!eventInfo.GetSpellInfo())
            return false;

        if(eventInfo.GetSpellInfo()->Id == SPELL_SHAMAN_HEALING_STREAM_TOTEM)
            return true;
        else
            return false;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_arti_sha_caress_of_the_tidemother::CheckProc);
    }
};

// Dimensional Rift - 196586
class spell_arti_warl_dimensional_rift : public SpellScript
{
    PrepareSpellScript(spell_arti_warl_dimensional_rift);

    void HandleHit(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;

                                                //green //green //purple
        std::vector<uint32> spellVisualIds = { 219117, 219117, 219107 };
                                        // Chaos Tear  //Chaos Portal  //Shadowy Tear
        std::vector<uint32> summonIds = { 108493,        108493,          99887 };
        // Durations must be longer, because if the npc gets destroyed before the last projectile hits
        // it won't deal any damage.
        std::vector<uint32> durations = { 7000, 4500, 16000 };
        uint32 id = std::rand() % 3;
        Position pos = caster->GetPosition();
        caster->MovePosition(pos, (float)(std::rand() % 5) + 4.f, (float)rand_norm() * static_cast<float>(2 * M_PI));
        if (TempSummon* rift = caster->SummonCreature(summonIds[id], pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, durations[id]))
        {
            rift->CastSpell(rift, spellVisualIds[id], true);
            rift->SetOwnerGUID(caster->GetGUID());
            rift->SetTarget(target->GetGUID());
            // We use same ID and script for Chaos Portal and Chaos Tear as there are no more NPCs for this spell
            rift->SetArmor(id);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_arti_warl_dimensional_rift::HandleHit, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// Thal'kiel's Consumption - 211714
class spell_arti_warl_thalkiels_consumption : public SpellScript
{
    PrepareSpellScript(spell_arti_warl_thalkiels_consumption);

    int32 damage = 0;

    void HandleHit(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!target || !caster)
            return;

        caster->CastCustomSpell(SPELL_WARLOCK_THALKIELS_CONSUMPTION_DAMAGE, SPELLVALUE_BASE_POINT0, damage, target, TRIGGERED_FULL_MASK);
    }

    void SaveDamage(std::list<WorldObject*>& targets)
    {
        targets.remove_if([this](WorldObject* target)
        {
            if (!target->IsCreature())
                return true;
            if (!target->ToCreature()->IsPet() || target->ToCreature()->ToPet()->GetOwner() != GetCaster())
                return true;
            // Remove Gateways
            if (target->ToCreature()->GetCreatureType() != CREATURE_TYPE_DEMON)
                return true;

            return false;
        });

        int32 basePoints = GetSpellInfo()->GetEffect(EFFECT_1)->BasePoints;
        for (WorldObject* pet : targets)
            damage += pet->ToUnit()->CountPctFromMaxHealth(basePoints);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_arti_warl_thalkiels_consumption::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_arti_warl_thalkiels_consumption::SaveDamage, EFFECT_1, TARGET_UNIT_CASTER_PET);
    }
};

// Chaos Tear - 108493
struct npc_warl_chaos_tear : public ScriptedAI
{
    npc_warl_chaos_tear(Creature* p_Creature) : ScriptedAI(p_Creature) { }

    int32 timer = 0;
    int32 counter = 0;

    void UpdateAI(uint32 diff) override

    {
        timer += diff;
        switch (me->GetArmor())
        {
        case 0:
            if (counter >= 22)
                return;
            if (timer >= 250)
            {
                timer -= 250;
                ObjectGuid targetGuid = me->GetTarget();
                ObjectGuid casterGuid = me->GetOwnerGUID();
                if (targetGuid.IsEmpty() || casterGuid.IsEmpty())
                    return;
                Unit* caster = ObjectAccessor::GetUnit(*me, casterGuid);
                Unit* target = ObjectAccessor::GetUnit(*me, targetGuid);
                if (!caster || !target)
                    return;

                me->CastSpell(target, SPELL_WARLOCK_TEAR_CHAOS_BARRAGE, true, nullptr, nullptr, caster->GetGUID());
                counter++;
            }
            break;
        case 1:
            if (timer >= 1500)
            {
                timer -= 9000;
                ObjectGuid targetGuid = me->GetTarget();
                ObjectGuid casterGuid = me->GetOwnerGUID();
                if (targetGuid.IsEmpty() || casterGuid.IsEmpty())
                    return;
                Unit* caster = ObjectAccessor::GetUnit(*me, casterGuid);
                Unit* target = ObjectAccessor::GetUnit(*me, targetGuid);
                if (!caster || !target)
                    return;

                me->CastSpell(target, SPELL_WARLOCK_TEAR_CHAOS_BOLT, true, nullptr, nullptr, caster->GetGUID());
            }
            break;
        default:
            break;
        }
    }
};

// Shadowy Tear - 99887
struct npc_warl_shadowy_tear : public ScriptedAI
{
    npc_warl_shadowy_tear(Creature* p_Creature) : ScriptedAI(p_Creature) { }

    int32 timer = 0;
    int32 counter = 0;

    void UpdateAI(uint32 diff) override
    {
        if (counter >= 7)
            return;

        timer += diff;
        if (timer >= 2000)
        {
            timer -= 2000;
            ObjectGuid targetGuid = me->GetTarget();
            ObjectGuid casterGuid = me->GetOwnerGUID();
            if (targetGuid.IsEmpty() || casterGuid.IsEmpty())
                return;
            Unit* caster = ObjectAccessor::GetUnit(*me, casterGuid);
            Unit* target = ObjectAccessor::GetUnit(*me, targetGuid);
            if (!caster || !target)
                return;

            me->CastSpell(target, SPELL_WARLOCK_TEAR_SHADOW_BOLT, true, nullptr, nullptr, caster->GetGUID());
            counter++;
        }
    }
};

class spell_arti_dru_new_moon : public SpellScript
{
    PrepareSpellScript(spell_arti_dru_new_moon);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_NEW_MOON, SPELL_DRUID_NEW_MOON_OVERRIDE });
    }

    void RemoveOverride()
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        caster->RemoveAurasDueToSpell(SPELL_DRUID_FULL_MOON_OVERRIDE);
        caster->AddAura(SPELL_DRUID_NEW_MOON_OVERRIDE, caster);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_arti_dru_new_moon::RemoveOverride);
    }
};

// Half Moon - 202768
class spell_arti_dru_half_moon : public SpellScript
{
    PrepareSpellScript(spell_arti_dru_half_moon);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_HALF_MOON, SPELL_DRUID_HALF_MOON_OVERRIDE });
    }

    void RemoveOverride()
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        caster->RemoveAurasDueToSpell(SPELL_DRUID_NEW_MOON_OVERRIDE);
        caster->AddAura(SPELL_DRUID_HALF_MOON_OVERRIDE, caster);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_arti_dru_half_moon::RemoveOverride);
    }
};

//Full Moon - 202771
class spell_arti_dru_full_moon : public SpellScript
{
    PrepareSpellScript(spell_arti_dru_full_moon);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_FULL_MOON, SPELL_DRUID_FULL_MOON_OVERRIDE });
    }

    void RemoveOverride()
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        caster->RemoveAurasDueToSpell(SPELL_DRUID_HALF_MOON_OVERRIDE);
        //caster->AddAura(SPELL_DRUID_FULL_MOON_OVERRIDE, caster);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_arti_dru_full_moon::RemoveOverride);
    }
};

#define ASHAMANE_FRENZY_TARGET "ashamane_frenzy_target"

// 210722 - Ashamane's Frenzy
class spell_arti_dru_ashamane_frenzy : public SpellScript
{
    PrepareSpellScript(spell_arti_dru_ashamane_frenzy);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
            GetCaster()->Variables.Set(ASHAMANE_FRENZY_TARGET, target->GetGUID());
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_arti_dru_ashamane_frenzy::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 210722 - Ashamane's Frenzy
class aura_arti_dru_ashamane_frenzy : public AuraScript
{
    PrepareAuraScript(aura_arti_dru_ashamane_frenzy);

    void HandleDummyTick(AuraEffect const* aurEff)
    {
        if (Unit* caster = GetCaster())
            if (Unit* target = ObjectAccessor::GetUnit(*caster, caster->Variables.GetValue<ObjectGuid>(ASHAMANE_FRENZY_TARGET)))
                caster->CastSpell(target, SPELL_DRUID_ASHAMANE_FRENZY_DAMAGE, true, NULL, aurEff);
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* caster = GetCaster())
            caster->Variables.Remove(ASHAMANE_FRENZY_TARGET);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(aura_arti_dru_ashamane_frenzy::HandleDummyTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        AfterEffectRemove += AuraEffectRemoveFn(aura_arti_dru_ashamane_frenzy::OnRemove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 193371 - Call to the Void
class spell_arti_pri_call_of_the_void : public AuraScript
{
    PrepareAuraScript(spell_arti_pri_call_of_the_void);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_MIND_FLAY, SPELL_PRIEST_CALL_OF_THE_VOID, SPELL_PRIEST_CALL_OF_THE_VOID_SUMMON });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo() && eventInfo.GetSpellInfo()->Id == SPELL_PRIEST_MIND_FLAY;
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Unit* caster = GetCaster())
            caster->CastSpell(eventInfo.GetProcTarget(), SPELL_PRIEST_CALL_OF_THE_VOID_SUMMON, true, nullptr, aurEff);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_arti_pri_call_of_the_void::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_arti_pri_call_of_the_void::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 98167 - Void Tendril
struct npc_arti_priest_void_tendril : public Scripted_NoMovementAI
{
    npc_arti_priest_void_tendril(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature) { }

    void IsSummonedBy(Unit* summoner) override
    {
        auto channelTargets = summoner->GetChannelObjects();

        if (channelTargets.size() != 1)
        {
            me->DisappearAndDie();
            return;
        }

        ObjectGuid targetGuid = *(channelTargets.begin());
        if (Unit* target = ObjectAccessor::GetUnit(*me, targetGuid))
        {
            AttackStart(target);
            me->GetScheduler().Schedule(250ms, [this, targetGuid](TaskContext context)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (Unit* owner = me->GetOwner())
                    if (Unit* target = ObjectAccessor::GetUnit(*me, targetGuid))
                        me->CastCustomSpell(SPELL_PRIEST_MIND_FLAY, SPELLVALUE_BASE_POINT0, owner->GetTotalSpellPowerValue(SPELL_SCHOOL_MASK_SHADOW, false), target);

                context.Repeat();
            });
        }
    }
};

// 200653  - Tyr's Deliverance
class spell_arti_pal_tyr_deliverance : public SpellScript
{
    PrepareSpellScript(spell_arti_pal_tyr_deliverance);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PALADIN_TYR_DELIVERANCE_HEAL });
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
            GetCaster()->CastSpell(target, SPELL_PALADIN_TYR_DELIVERANCE_HEAL, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_arti_pal_tyr_deliverance::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 205495 - Stormkeeper
class aura_artifact_shaman_stormkeeper : public AuraScript
{
    PrepareAuraScript(aura_artifact_shaman_stormkeeper);

    void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetCaster()->HasAura(SPELL_SHAMAN_FURY_OF_THE_STORMS))
            GetCaster()->CastSpell(GetCaster(), SPELL_SHAMAN_SUMMON_LIGHTHING_ELEMENTAL, true);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_artifact_shaman_stormkeeper::AfterApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

// 194024 - Thrive in the Shadows
class spell_arti_pri_thrive : public SpellScriptLoader
{
public:
    spell_arti_pri_thrive() : SpellScriptLoader("spell_arti_pri_thrive") { }

    class spell_arti_pri_thrive_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_arti_pri_thrive_SpellScript);

        enum eSpells
        {
            SPELL_PRIEST_THRIVE         = 194024,
            SPELL_PRIEST_THRIVE_HEAL    = 194025
        };

        bool Validate(SpellInfo const* /*spell*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_THRIVE))
                return false;
            return true;
        }

        void HandleOnCast()
        {
            if (Unit* caster = GetCaster())
            {
                int32 basepoints0 = caster->GetMaxHealth() / 2 / 7;
                caster->CastCustomSpell(caster, SPELL_PRIEST_THRIVE_HEAL, &basepoints0, NULL, NULL, true, NULL);
            }
        }

        void Register()
        {
            OnCast += SpellCastFn(spell_arti_pri_thrive_SpellScript::HandleOnCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_arti_pri_thrive_SpellScript();
    }
};

// Sphere Of Insanity - 194182
class spell_arti_pri_sphere_of_insanity_summon : public SpellScriptLoader
{
public:
    spell_arti_pri_sphere_of_insanity_summon() : SpellScriptLoader("spell_arti_pri_sphere_of_insanity_summon") { }

    class spell_arti_pri_sphere_of_insanity_summon_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_arti_pri_sphere_of_insanity_summon_SpellScript);

        void HandleSummon(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            std::list<Creature*> list;

            for (auto itr = caster->m_Controlled.begin(); itr != caster->m_Controlled.end(); ++itr)
                if ((*itr)->GetEntry() == GetEffectInfo()->MiscValue && (*itr)->GetTypeId() == TYPEID_UNIT && (*itr)->IsAlive())
                    list.push_back((*itr)->ToCreature());

            if (list.empty())
                return;

            for (Creature* sphere : list)
                sphere->DespawnOrUnsummon();

            Position offset = { 0.0f, 0.0f, 2.0f, 0.0f };
            const_cast<WorldLocation*>(GetExplTargetDest())->RelocateOffset(offset);
            GetHitDest()->RelocateOffset(offset);
        }

        void Register() override
        {
            OnEffectLaunch += SpellEffectFn(spell_arti_pri_sphere_of_insanity_summon_SpellScript::HandleSummon, EFFECT_0, SPELL_EFFECT_SUMMON);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_arti_pri_sphere_of_insanity_summon_SpellScript();
    }
};

// Sphere Of Insanity - 194230
class spell_arti_pri_sphere_of_insanity : public SpellScriptLoader
{
public:
    spell_arti_pri_sphere_of_insanity() : SpellScriptLoader("spell_arti_pri_sphere_of_insanity") { }

    class spell_arti_pri_sphere_of_insanity_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_arti_pri_sphere_of_insanity_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo
            ({
                SPELL_PRIEST_SPHERE_OF_INSANITY_TRIGGER,
                SPELL_PRIEST_SPHERE_OF_INSANITY_DAMAGE,
                SPELL_PRIEST_SHADOW_WORD_PAIN,
            });
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (!eventInfo.GetActor() || !eventInfo.GetDamageInfo() || !eventInfo.GetDamageInfo()->GetDamage())
                return false;

            if (eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_PRIEST_MIND_BLAST || eventInfo.GetDamageInfo()->GetSpellInfo()->Id == 15407 ||
                eventInfo.GetDamageInfo()->GetSpellInfo()->Id == 73510)
                return true;

            return false;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* caster = eventInfo.GetActor();
            int32 damage = eventInfo.GetDamageInfo()->GetDamage();

            std::list<Creature*> list;

            for (auto itr = caster->m_Controlled.begin(); itr != caster->m_Controlled.end(); ++itr)
                if ((*itr)->GetEntry() == 98680 && (*itr)->GetTypeId() == TYPEID_UNIT && (*itr)->IsAlive())
                    list.push_back((*itr)->ToCreature());

            if (list.empty())
                return;

            Creature* sphere = list.front();
            if (!sphere)
                return;

            damage = CalculatePct(damage, sSpellMgr->AssertSpellInfo(SPELL_PRIEST_SPHERE_OF_INSANITY_TRIGGER)->GetEffect(EFFECT_2)->BasePoints);

            std::list<Unit*> targetList;
            sphere->GetAttackableUnitListInRange(targetList, 100.f);
            if (!targetList.empty())
            {
                targetList.remove_if(Trinity::UnitAuraCheck(false, SPELL_PRIEST_SHADOW_WORD_PAIN, caster->GetGUID()));
                if (!targetList.empty())
                    for (Unit* target : targetList)
                        sphere->CastCustomSpell(target, SPELL_PRIEST_SPHERE_OF_INSANITY_DAMAGE, &damage, NULL, NULL, true, NULL, NULL, caster->GetGUID());
            }
        }

        void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            std::list<Creature*> list;

            for (auto itr = caster->m_Controlled.begin(); itr != caster->m_Controlled.end(); ++itr)
                if ((*itr)->GetEntry() == 98680 && (*itr)->GetTypeId() == TYPEID_UNIT && (*itr)->IsAlive())
                    list.push_back((*itr)->ToCreature());

            if (list.empty())
                return;

            for (Creature* sphere : list)
                sphere->DespawnOrUnsummon();
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_arti_pri_sphere_of_insanity_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_arti_pri_sphere_of_insanity_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            AfterEffectRemove += AuraEffectRemoveFn(spell_arti_pri_sphere_of_insanity_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_arti_pri_sphere_of_insanity_AuraScript();
    }
};

// 197344 - hatis-bond
class aura_artifact_hunter_hatis_bond : public AuraScript
{
    PrepareAuraScript(aura_artifact_hunter_hatis_bond);
     void HandleApply(AuraEffect const* /*aurEffect*/, AuraEffectHandleModes /*mode*/)
    {
        Player* player = GetCaster()->ToPlayer();
        if (!player)
            return;
		
		if (Creature* hati = player->GetHati())
            hati->DespawnOrUnsummon();
        if (player->GetPet() && !player->GetHati() && !player->HasAura(SPELL_HUNTER_BROKENOUND))
            player->CastSpell(player, SPELL_HUNTER_STORMBOUND, true);
        /*		
         std::list<Creature*> clists = player->FindNearestCreatures(100324, 20.0f);
        for (Creature* target : clists)
            if (target->GetCharmerOrOwnerGUID() == player->GetGUID())
                target->DespawnOrUnsummon();
         if (player->GetPet() && !player->GetSummonedCreatureByEntry(100324) && !player->HasAura(SPELL_HUNTER_BROKENOUND))
            player->CastSpell(player, SPELL_HUNTER_STORMBOUND, true);
		*/
        //if (Creature* hatis = player->GetSummonedCreatureByEntry(100324))
        //{
        //    player->SetHatiGUID(hatis->GetGUID());
            //if (!hatis->IsPet())
            //   hatis->AddUnitTypeMask(UNIT_MASK_PET);
            //if (!hatis->IsHunterPet())
            //    hatis->AddUnitTypeMask(UNIT_MASK_HUNTER_PET);

            //if (hatis->IsAIEnabled && hatis->ToPet())
            //{


             //   TC_LOG_ERROR("server.worldserver", "====================(hatis->IsAIEnabled && hatis->ToPet()====================== ");
                /*
                hatis->ToPet()->ClearUnitState(UNIT_STATE_FOLLOW);
                if (hatis->ToPet()->GetVictim())
                    hatis->ToPet()->AttackStop();
                hatis->GetMotionMaster()->Clear();
                hatis->ToPet()->GetCharmInfo()->SetIsCommandAttack(true);
                hatis->ToPet()->GetCharmInfo()->SetIsAtStay(false);
                hatis->ToPet()->GetCharmInfo()->SetIsReturning(false);
                hatis->ToPet()->GetCharmInfo()->SetIsFollowing(false);
                */
                //caster->ToCreature()->AI()->AttackStart(target);
            //}
        //}
    }
     void HandleRemove(AuraEffect const* /*aurEffect*/, AuraEffectHandleModes /*mode*/)
    {
        Player* player = GetCaster()->ToPlayer();
        if (!player)
            return;
        if (Creature* hati = player->GetHati())
            hati->DespawnOrUnsummon();
        /*
        std::list<Creature*> clists = player->FindNearestCreatures(100324, 20.0f);
        for (Creature* target : clists)
            if (target->GetCharmerOrOwnerGUID() == player->GetGUID())
                target->DespawnOrUnsummon();*/
    }
     void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(aura_artifact_hunter_hatis_bond::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(aura_artifact_hunter_hatis_bond::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};
 // 211117 - broken-bond
class aura_artifact_hunter_broken_bond : public AuraScript
{
    PrepareAuraScript(aura_artifact_hunter_broken_bond);
     void HandleApply(AuraEffect const* /*aurEffect*/, AuraEffectHandleModes /*mode*/)
    {
        int32 dur = 30000;
        if (Aura* aur = GetCaster()->GetAura(SPELL_HUNTER_BROKENOUND ))
            if (dur)
                aur->SetDuration(dur);
         Player* player = GetCaster()->ToPlayer();
        if (!player)
            return;
        if (Creature* hati = player->GetHati())
            hati->DespawnOrUnsummon();
    }
     void HandleRemove(AuraEffect const* /*aurEffect*/, AuraEffectHandleModes /*mode*/)
    {       
        Player* player = GetCaster()->ToPlayer();
        if (!player)
            return;
         if (Creature* hati = player->GetHati())
            hati->DespawnOrUnsummon();
        if (player->GetPet() && !player->GetHati() && !player->HasAura(SPELL_HUNTER_BROKENOUND))
            player->CastSpell(player, SPELL_HUNTER_STORMBOUND, true);
    }
     void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(aura_artifact_hunter_broken_bond::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(aura_artifact_hunter_broken_bond::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};
 class playerscript_hunter_summon_pet_trigger : public PlayerScript
{
public:
    playerscript_hunter_summon_pet_trigger() : PlayerScript("playerscript_hunter_summon_pet_trigger") { skipupdate = false; }
     void RemoveHati(Player* player)
    {
        if (player->getClass() == CLASS_HUNTER)
        {
			if (Creature* hati = player->GetHati())
                hati->DespawnOrUnsummon();
            /*
            std::list<Creature*> clists = player->FindNearestCreatures(100324, 20.0f);
            for (Creature* target : clists)
                if (target->GetCharmerOrOwnerGUID() == player->GetGUID())
                    target->DespawnOrUnsummon();
				*/
        }      
    }
     void AddHati(Player* player)
    {
        if (player->getClass() == CLASS_HUNTER && player->HasAura(SPELL_HUNTER_HATIS_BOND) && !player->HasAura(SPELL_HUNTER_BROKENOUND))
            if (player->GetPet() && !player->GetHati())
                player->CastSpell(player, SPELL_HUNTER_STORMBOUND, true);
        /*    if (player->GetPet() && !player->GetSummonedCreatureByEntry(100324))
            {
                player->CastSpell(player, SPELL_HUNTER_STORMBOUND, true);
                if (Creature* hatis = player->GetSummonedCreatureByEntry(100324))
                {
                 
                    if (hatis->IsAIEnabled && hatis->ToPet())
                    {
                        hatis->ToPet()->ClearUnitState(UNIT_STATE_FOLLOW);
                        if (hatis->ToPet()->GetVictim())
                            hatis->ToPet()->AttackStop();
                        hatis->GetMotionMaster()->Clear();
                        hatis->ToPet()->GetCharmInfo()->SetIsCommandAttack(true);
                        hatis->ToPet()->GetCharmInfo()->SetIsAtStay(false);
                        hatis->ToPet()->GetCharmInfo()->SetIsReturning(false);
                        hatis->ToPet()->GetCharmInfo()->SetIsFollowing(false);
                        //caster->ToCreature()->AI()->AttackStart(target);
                    }
                    
                }
            }*/
        skipupdate = false;           
    }
     void OnUnsummonPetTemporary(Player* player) 
    {
        skipupdate = true;
        RemoveHati(player);
    }
     void OnResummonPetTemporary(Player* player)
	{       
        AddHati(player);
    }

    void OnItemLevelChange(Player* player)	 
    {
        skipupdate = true;
        RemoveHati(player);
        AddHati(player);
    }
     void OnSuccessfulSpellCast(Player* player, Spell* spell)
    {
        switch (spell->GetSpellInfo()->Id)
        {
        case 982:
        ///Call Pet
        case 883:
        case 83242:
        case 83243:
        case 83244:
        case 83245:
            RemoveHati(player);
            AddHati(player);
            break;
        case 2641:
            RemoveHati(player);
            break;
			case SPELL_HUNTER_BESTIAL_WRATH:
            if (Creature* hati = player->GetHati())
                hati->CastSpell(hati, 186254, true);
            break;
        }
    }
     void OnLogin(Player* player, bool /*firstLogin*/)
    {
        RemoveHati(player);
    }
     void OnUpdate(Player* player, uint32 diff)
    {
        if (checkTimer <= diff)
        {
            if(!skipupdate)
                AddHati(player);
            checkTimer = 4000;
        }
        else checkTimer -= diff;
    }
    uint32 checkTimer = 4000;
    bool skipupdate;
};

/*
SPELL_HUNTER_TITANS_THUNDER                     = 207068,
SPELL_HUNTER_TITANS_THUNDER_TRIGGER             = 207081,
SPELL_HUNTER_TITANS_THUNDER_AURA                = 207094,
SPELL_HUNTER_TITANS_THUNDER_DAMAGE              = 218635,
*/
///SPELL_HUNTER_TITANS_THUNDER 207068
class spell_arti_hun_titans_thunder : public SpellScript
{
    PrepareSpellScript(spell_arti_hun_titans_thunder);

    void HandleAfterCast()
    {
        if (Player* player = GetCaster()->ToPlayer())
        {
            if (Pet* pet = player->GetPet())
                pet->CastSpell(pet, SPELL_HUNTER_TITANS_THUNDER_AURA, true);
                    
            if (Creature* hati = player->GetHati())
                hati->CastSpell(hati, SPELL_HUNTER_TITANS_THUNDER_AURA, true);
                    
        } 
    }
    
    void Register()
    {
        AfterCast += SpellCastFn(spell_arti_hun_titans_thunder::HandleAfterCast);
    }
};
///SPELL_HUNTER_TITANS_THUNDER_AURA (On Pet) 207094 
class aura_arti_hun_titans_thunder : public AuraScript
{
    PrepareAuraScript(aura_arti_hun_titans_thunder);
    uint32 waitTime = 0;
    void OnUpdate(uint32 diff)
    {
        if (waitTime > diff)
        {
            waitTime -= diff;
            return;
        }
        waitTime = 1000;
        Unit* caster = GetCaster();
        Unit* target = caster->GetVictim();
        if (target)
            caster->CastSpell(target, SPELL_HUNTER_TITANS_THUNDER_DAMAGE, true);
    }

    void OnTick(const AuraEffect* aurEff)
    {     
        Unit* caster = GetCaster();
        Unit* target = caster->GetVictim();
        if(target)
            caster->CastSpell(target, SPELL_HUNTER_TITANS_THUNDER_DAMAGE, true);
    }

    void Register() override
    {
        OnAuraUpdate += AuraUpdateFn(aura_arti_hun_titans_thunder::OnUpdate);
        //OnEffectPeriodic += AuraEffectPeriodicFn(aura_arti_hun_titans_thunder::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};
//239042
class aura_concordance_of_the_legionfall : public AuraScript
{
    PrepareAuraScript(aura_concordance_of_the_legionfall);
    enum Spells
    {
        SPELL_VERSATILITY = 243096,
        SPELL_STRENGTH = 242583,
        SPELL_AGILITY = 242584,
        SPELL_INTELLECT = 242586,
    };
    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetSpellInfo()->Id == 239042)
            return true;

        return false;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();

        if (Unit* caster = GetCaster())
        {
            _rank = 0;
            if (Item* artifact = GetOwner()->ToPlayer()->GetItemByGuid(GetAura()->GetCastItemGUID()))               
                if (artifact->GetTotalPurchasedArtifactPowers() >= 52)
                    _rank = artifact->GetTotalPurchasedArtifactPowers() - 52;

            _amount = 4000 + 300 * _rank;

            switch (caster->ToPlayer()->GetSpecializationId())
            {
            case TALENT_SPEC_MAGE_ARCANE:
            case TALENT_SPEC_MAGE_FIRE:
            case TALENT_SPEC_MAGE_FROST:
            case TALENT_SPEC_PRIEST_DISCIPLINE:
            case TALENT_SPEC_PRIEST_HOLY:
            case TALENT_SPEC_PRIEST_SHADOW:
            case TALENT_SPEC_SHAMAN_RESTORATION:
            case TALENT_SPEC_SHAMAN_ELEMENTAL:
            case TALENT_SPEC_WARLOCK_AFFLICTION:
            case TALENT_SPEC_WARLOCK_DEMONOLOGY:
            case TALENT_SPEC_WARLOCK_DESTRUCTION:
            case TALENT_SPEC_DRUID_RESTORATION:
            case TALENT_SPEC_DRUID_BALANCE:
            case TALENT_SPEC_PALADIN_HOLY:
            case TALENT_SPEC_MONK_BATTLEDANCER:
                caster->CastSpell(caster, SPELL_INTELLECT, true);
                break;
            case TALENT_SPEC_WARRIOR_ARMS:
            case TALENT_SPEC_WARRIOR_FURY:
            case TALENT_SPEC_DEATHKNIGHT_FROST:
            case TALENT_SPEC_DEATHKNIGHT_UNHOLY:
            case TALENT_SPEC_PALADIN_RETRIBUTION:
                caster->CastCustomSpell(SPELL_STRENGTH, SPELLVALUE_BASE_POINT0, _amount, caster, true, nullptr, aurEff);
                break;
            case TALENT_SPEC_HUNTER_BEASTMASTER:
            case TALENT_SPEC_HUNTER_MARKSMAN:
            case TALENT_SPEC_HUNTER_SURVIVAL:
            case TALENT_SPEC_ROGUE_ASSASSINATION:
            case TALENT_SPEC_ROGUE_COMBAT:
            case TALENT_SPEC_ROGUE_SUBTLETY:
            case TALENT_SPEC_DEMON_HUNTER_HAVOC:
            case TALENT_SPEC_DRUID_CAT:
            case TALENT_SPEC_SHAMAN_ENHANCEMENT:
            case TALENT_SPEC_MONK_MISTWEAVER:
                caster->CastSpell(caster, SPELL_AGILITY, true);
                break; 
            case TALENT_SPEC_DEMON_HUNTER_VENGEANCE:
            case TALENT_SPEC_WARRIOR_PROTECTION:
            case TALENT_SPEC_PALADIN_PROTECTION:
            case TALENT_SPEC_DRUID_BEAR:
            case TALENT_SPEC_DEATHKNIGHT_BLOOD:
            case TALENT_SPEC_MONK_BREWMASTER:
                caster->CastCustomSpell(SPELL_VERSATILITY, SPELLVALUE_BASE_POINT0, _amount, caster, true, nullptr, aurEff);
                break;
            default:
                break;
            }
        }
    }

    void Register() override
    {
        //DoCheckProc += AuraCheckProcFn(aura_concordance_of_the_legionfall::CheckProc);
        OnEffectProc += AuraEffectProcFn(aura_concordance_of_the_legionfall::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
    uint32 _rank;
    int64 _amount;
};

class aura_arti_mage_mark_of_aluneth : public AuraScript
{
    PrepareAuraScript(aura_arti_mage_mark_of_aluneth);

    void HandleDummyTick(AuraEffect const* aurEff)
    {
        if (Unit* caster = GetCaster())
            if(Unit* target = GetTarget())
                caster->CastSpell(target, SPELL_MAGE_MARK_OF_ALUNETH_AOE, true, NULL, aurEff);
    }

    void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetTarget();

        if (!caster || !target)
            return;

        uint32 power = caster->GetMaxPower(Powers::POWER_MANA);
        int32 bp = CalculatePct(power, int32(sSpellMgr->GetSpellInfo(SPELL_MAGE_MARK_OF_ALUNETH_DAMAGE)->GetEffect(EFFECT_1)->BasePoints));
        caster->CastCustomSpell(SPELL_MAGE_MARK_OF_ALUNETH_DAMAGE, SPELLVALUE_BASE_POINT0, bp, target, true, nullptr, aurEff);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(aura_arti_mage_mark_of_aluneth::HandleDummyTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        AfterEffectRemove += AuraEffectRemoveFn(aura_arti_mage_mark_of_aluneth::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};
//211088,211076
class spell_arti_mage_mark_of_aluneth_aoe : public SpellScript
{
    PrepareSpellScript(spell_arti_mage_mark_of_aluneth_aoe);

    void HandleHit(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* hitunit = GetHitUnit();

        if (!caster || !hitunit)
            return;

        caster->CastSpell(hitunit, SPELL_MAGE_MARK_OF_ALUNETH_SPEED, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_arti_mage_mark_of_aluneth_aoe::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_dh_artifact_soul_carver : public SpellScriptLoader
{
public:
    spell_dh_artifact_soul_carver() : SpellScriptLoader("spell_dh_artifact_soul_carver") { }

     class spell_dh_artifact_soul_carver_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dh_artifact_soul_carver_SpellScript);

         void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

             uint32 soulsToShatter = GetEffectInfo(EFFECT_3)->BasePoints;
            for (uint32 i = 0; i < soulsToShatter; ++i)
                caster->CastCustomSpell(SPELL_DH_SHATTERED_SOULS_MISSILE, SPELLVALUE_BASE_POINT0, SPELL_DH_SHATTERED_SOULS_AT_DEMON, caster, true);
        }

         void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_dh_artifact_soul_carver_SpellScript::HandleHit, EFFECT_2, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
        }
    };

     class spell_dh_artifact_soul_carver_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dh_artifact_soul_carver_AuraScript);

         void PeriodicTick(AuraEffect const* /*aurEff*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

             caster->CastCustomSpell(SPELL_DH_SHATTERED_SOULS_MISSILE, SPELLVALUE_BASE_POINT0, SPELL_DH_SHATTERED_SOULS_AT_DEMON, caster, true);
        }

         void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dh_artifact_soul_carver_AuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

     AuraScript* GetAuraScript() const
    {
        return new spell_dh_artifact_soul_carver_AuraScript();
    }

     SpellScript* GetSpellScript() const
    {
        return new spell_dh_artifact_soul_carver_SpellScript();
    }
};

 // 213010 - Charred Warblades
class spell_dh_artifact_charred_warblades : public SpellScriptLoader
{
public:
    spell_dh_artifact_charred_warblades() : SpellScriptLoader("spell_dh_artifact_charred_warblades") { }

     class spell_dh_artifact_charred_warblades_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dh_artifact_charred_warblades_AuraScript);

         void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            Unit* caster = GetCaster();
            if (!caster || !eventInfo.GetDamageInfo())
                return;

             if (!eventInfo.GetDamageInfo()->GetDamage() || !(eventInfo.GetDamageInfo()->GetSchoolMask() & SPELL_SCHOOL_MASK_FIRE))
                return;

             int32 heal = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
            caster->CastCustomSpell(SPELL_DH_CHARRED_WARBLADES_HEAL, SPELLVALUE_BASE_POINT0, heal, caster, true);
        }

         void Register()
        {
            OnEffectProc += AuraEffectProcFn(spell_dh_artifact_charred_warblades_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

     AuraScript* GetAuraScript() const
    {
        return new spell_dh_artifact_charred_warblades_AuraScript();
    }
};

 // 213017 - Fueled by Pain
class spell_dh_artifact_fueled_by_pain : public SpellScriptLoader
{
public:
    spell_dh_artifact_fueled_by_pain() : SpellScriptLoader("spell_dh_artifact_fueled_by_pain") { }

     class spell_dh_artifact_fueled_by_pain_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dh_artifact_fueled_by_pain_AuraScript);

         void OnProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

             int32 duration = aurEff->GetAmount() * IN_MILLISECONDS;
            if (Aura* aur = caster->AddAura(SPELL_DH_METAMORPHOSIS_VENGEANCE, caster))
            {
                aur->SetMaxDuration(duration);
                aur->RefreshDuration();
            }
        }

         bool CheckProc(ProcEventInfo& eventInfo)
        {
            return eventInfo.GetSpellInfo() && (eventInfo.GetSpellInfo()->Id == SPELL_DH_SOUL_FRAGMENT_HEAL_VENGEANCE || eventInfo.GetSpellInfo()->Id == SPELL_DH_LESSER_SOUL_SHARD_HEAL);
        }

         void Register()
        {
            DoCheckProc += AuraCheckProcFn(spell_dh_artifact_fueled_by_pain_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_dh_artifact_fueled_by_pain_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

     AuraScript* GetAuraScript() const
    {
        return new spell_dh_artifact_fueled_by_pain_AuraScript();
    }
};

 // 212817 - Fiery Demise
class spell_dh_artifact_fiery_demise : public SpellScriptLoader
{
public:
    spell_dh_artifact_fiery_demise() : SpellScriptLoader("spell_dh_artifact_fiery_demise") { }

     class spell_dh_artifact_fiery_demise_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dh_artifact_fiery_demise_AuraScript);

         void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            Unit* caster = GetCaster();
            Unit* target = eventInfo.GetActionTarget();
            if (!caster || !target || !caster->IsValidAttackTarget(target))
                return;

             caster->CastCustomSpell(SPELL_DH_FIERY_DEMISE_DEBUFF, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), target, true);
        }

         void Register()
        {
            OnEffectProc += AuraEffectProcFn(spell_dh_artifact_fiery_demise_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

     AuraScript* GetAuraScript() const
    {
        return new spell_dh_artifact_fiery_demise_AuraScript();
    }
};

void AddSC_artifact_spell_scripts()
{
    RegisterSpellScript(spell_arti_dru_new_moon);
    RegisterSpellScript(spell_arti_dru_half_moon);
    RegisterSpellScript(spell_arti_dru_full_moon);
    RegisterSpellAndAuraScriptPair(spell_arti_dru_ashamane_frenzy, aura_arti_dru_ashamane_frenzy);

    RegisterAuraScript(spell_arti_mage_immolation);
    RegisterSpellScript(spell_arti_mage_phoenix_flames);
    RegisterSpellScript(spell_arti_mage_phoenix_flames_trigger);

    RegisterAuraScript(spell_arti_sha_servant_of_the_queen);
    RegisterAuraScript(spell_arti_sha_caress_of_the_tidemother);

    RegisterAuraScript(spell_arti_warl_deadwind_harvest);
    RegisterSpellScript(spell_arti_warl_reap_souls);
    RegisterSpellScript(spell_arti_warl_dimensional_rift);
    RegisterCreatureAI(npc_warl_chaos_tear);
    RegisterCreatureAI(npc_warl_shadowy_tear);
    RegisterSpellScript(spell_arti_warl_thalkiels_consumption);
    new spell_arti_pri_thrive;
    new spell_arti_pri_sphere_of_insanity_summon;
    new spell_arti_pri_sphere_of_insanity;
    RegisterAuraScript(aura_artifact_hunter_hatis_bond);
    RegisterAuraScript(aura_artifact_hunter_broken_bond);
    new playerscript_hunter_summon_pet_trigger();
    RegisterSpellScript(spell_arti_hun_titans_thunder);
    RegisterAuraScript(aura_arti_hun_titans_thunder);
    RegisterAuraScript(aura_concordance_of_the_legionfall);
    RegisterAuraScript(aura_arti_mage_mark_of_aluneth);
    RegisterSpellScript(spell_arti_mage_mark_of_aluneth_aoe);
    new spell_dh_artifact_soul_carver();
    new spell_dh_artifact_charred_warblades();
    new spell_dh_artifact_fueled_by_pain();
    new spell_dh_artifact_fiery_demise();

    /// AreaTrigger scripts
    RegisterAreaTriggerAI(at_dh_fury_of_the_illidari);

    RegisterSpellScript(spell_arti_mage_ebonbolt);

    RegisterAuraScript(spell_arti_pri_call_of_the_void);
    RegisterCreatureAI(npc_arti_priest_void_tendril);

    RegisterSpellScript(spell_arti_pal_tyr_deliverance);
    
    RegisterAuraScript(aura_artifact_shaman_stormkeeper);
}
