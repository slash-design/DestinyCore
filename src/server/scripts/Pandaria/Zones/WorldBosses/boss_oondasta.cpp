/*
 * Copyright (C) 2017-2018 AshamaneProject <https://github.com/AshamaneProject>
 * Copyright (C) 2016 Firestorm Servers <https://firestorm-servers.com>
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

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "CreatureTextMgr.h"
#include "SpellScript.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "Player.h"

enum eTexts
{
    SAY_INTRO_01                = 0,
    SAY_INTRO_02                = 1,

    SAY_INTRO_END               = 0
};

enum eSpells
{
    SPELL_CRUSH                 = 137504,
    SPELL_ALPHA_MALE            = 138391,
    SPELL_FRILL_BLAST           = 137505,
    SPELL_GROWING_FURY          = 137502,
    SPELL_PIERCING_ROAR         = 137457,
    SPELL_SPIRITFIRE_BEAM       = 137511,

    SPELL_KILL_DOHAMAN          = 138859
};

enum eEvents
{
    EVENT_CRUSH                 = 1,
    EVENT_FRILL_BLAST,
    EVENT_GROWING_FURY,
    EVENT_PIERCING_ROAR,
    EVENT_SPIRITFIRE_BEAM,

    EVENT_INTRO_01,
    EVENT_INTRO_02,
    EVENT_INTRO_END
};

enum eCreatures
{
    NPC_DOHAMAN_THE_BEAST_LORD  = 69926
};

class boss_oondasta : public CreatureScript
{
    public:
        boss_oondasta() : CreatureScript("boss_oondasta") { }

        struct boss_oondastaAI : public ScriptedAI
        {
            boss_oondastaAI(Creature* creature) : ScriptedAI(creature), summons(me) { }

            EventMap _events;
            EventMap _introEvents;
            SummonList summons;
            bool _introEnd;

            void InitializeAI() override
            {
                if (!me->isDead())
                    Reset();
            }

            void Reset() override
            {
                _introEnd = false;

                me->RemoveAurasDueToSpell(SPELL_CRUSH);
                me->RemoveAura(SPELL_GROWING_FURY);

                _events.Reset();
                _introEvents.Reset();
                summons.DespawnAll();

                if (!me->HasAura(SPELL_ALPHA_MALE))
                    me->AddAura(SPELL_ALPHA_MALE, me);

                if (!_introEnd)
                {
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);

                    me->SummonCreature(NPC_DOHAMAN_THE_BEAST_LORD, 6038.54f, 1110.18f, 55.88f, 2.975316f, TEMPSUMMON_MANUAL_DESPAWN);
                }
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (!_introEnd && me->IsWithinDistInMap(who, 50.0f) && who->GetTypeId() == TYPEID_PLAYER && who->IsAlive())
                {
                    if (Creature* dohaman_the_beast_lord = me->FindNearestCreature(NPC_DOHAMAN_THE_BEAST_LORD, 50.0f, true))
                        me->SetFacingToObject(dohaman_the_beast_lord);

                    _introEvents.ScheduleEvent(EVENT_INTRO_01, 2s);
                    _introEnd = true;
                }
            }

            void EnterCombat(Unit* /*who*/) override
            {
                _events.ScheduleEvent(EVENT_SPIRITFIRE_BEAM, 15s);
                _events.ScheduleEvent(EVENT_PIERCING_ROAR, 20s);
                _events.ScheduleEvent(EVENT_FRILL_BLAST, 40s);
                _events.ScheduleEvent(EVENT_CRUSH, 60s);

                _events.ScheduleEvent(EVENT_GROWING_FURY, 30s);
            }

            void EnterEvadeMode(EvadeReason /*why*/) override
            {
                Reset();
                me->DeleteThreatList();
                me->CombatStop(false);

                me->GetMotionMaster()->MoveTargetedHome();
            }

            void JustDied(Unit* /*killer*/)
            {
                summons.DespawnAll();
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
                summon->setActive(true);
            }

            void JustReachedHome() override
            {
                if (!me->HasAura(SPELL_ALPHA_MALE))
                    me->AddAura(SPELL_ALPHA_MALE, me);
            }

            void UpdateAI(uint32 diff) override
            {
                _introEvents.Update(diff);

                while (uint32 eventId = _introEvents.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INTRO_01:
                            if (Creature* dohaman_the_beast_lord = me->FindNearestCreature(NPC_DOHAMAN_THE_BEAST_LORD, 50.0f, true))
                                dohaman_the_beast_lord->AI()->Talk(SAY_INTRO_01);

                            _introEvents.ScheduleEvent(EVENT_INTRO_02, 8s);
                            break;
                        case EVENT_INTRO_02:
                            if (Creature* dohaman_the_beast_lord = me->FindNearestCreature(NPC_DOHAMAN_THE_BEAST_LORD, 50.0f, true))
                            {
                                dohaman_the_beast_lord->SetFacingToObject(me);
                                dohaman_the_beast_lord->AI()->Talk(SAY_INTRO_02);
                            }

                            me->HandleEmoteCommand(391);
                            _introEvents.ScheduleEvent(EVENT_INTRO_END, 6s);
                            break;
                        case EVENT_INTRO_END:
                            if (Creature* dohaman_the_beast_lord = me->FindNearestCreature(NPC_DOHAMAN_THE_BEAST_LORD, 50.0f, true))
                            {
                                me->SetFacingToObject(dohaman_the_beast_lord);
                                dohaman_the_beast_lord->CastSpell(dohaman_the_beast_lord, SPELL_KILL_DOHAMAN, true);
                            }

                            me->AI()->Talk(SAY_INTRO_END);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                            me->SetReactState(REACT_AGGRESSIVE);
                            break;
                    }
                }

                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (_events.ExecuteEvent())
                {
                    case EVENT_CRUSH:
                        DoCastVictim(SPELL_CRUSH);
                        _events.ScheduleEvent(EVENT_CRUSH, 25s, 30s);
                        break;
                    case EVENT_FRILL_BLAST:
                        DoCast(me, SPELL_FRILL_BLAST);
                        _events.ScheduleEvent(EVENT_FRILL_BLAST, 25s, 30s);
                        break;
                    case EVENT_PIERCING_ROAR:
                        DoCast(me, SPELL_PIERCING_ROAR);
                        _events.ScheduleEvent(EVENT_PIERCING_ROAR, 25s, 30s);
                        break;
                    case EVENT_SPIRITFIRE_BEAM:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me)))
                            DoCast(target, SPELL_SPIRITFIRE_BEAM);
                        _events.ScheduleEvent(EVENT_SPIRITFIRE_BEAM, 25s, 30s);
                        break;
                    case EVENT_GROWING_FURY:
                        DoCast(me, SPELL_GROWING_FURY);
                        _events.ScheduleEvent(EVENT_GROWING_FURY, 30s);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new boss_oondastaAI(creature);
        }
};

class TankCheck
{
    public:
        explicit TankCheck(Unit* _caster) : caster(_caster) { }

        bool operator()(WorldObject* object)
        {
            if (object->GetTypeId() != TYPEID_PLAYER)
                return true;

            if (object->ToPlayer()->GetRoleForGroup() != ROLE_TANK)
                return true;

            return false;
        }

    private:
        Unit* caster;
};

class spell_alpha_male_threat: public SpellScriptLoader
{
    public:
        spell_alpha_male_threat() : SpellScriptLoader("spell_alpha_male_threat") { }

        class spell_alpha_male_threat_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_alpha_male_threat_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                targets.remove_if(TankCheck(GetCaster()));
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_alpha_male_threat_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_alpha_male_threat_SpellScript();
        }
};

void AddSC_boss_oondasta()
{
    new boss_oondasta();
    new spell_alpha_male_threat();
}
