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
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "Creature.h"
#include "ObjectAccessor.h"
#include "Object.h"
#include "CellImpl.h"
#include "GridNotifiersImpl.h"
#include "SpellInfo.h"
#include "Vehicle.h"
#include "WorldSession.h"

// 32911 Gershala Nightwhisper
class npc_gershala_nightwhisper_32911 : public CreatureScript
{
public:
    npc_gershala_nightwhisper_32911() : CreatureScript("npc_gershala_nightwhisper_32911") { }

    enum eQuest
    {
        QUEST_THE_LAST_WAVE_OF_SURVIVORS = 13518,
        NPC_GERSHALA_NIGHTWHISPER = 32911,
        PLAYER_GUID = 99999,
        NPC_RESCUE_SENTINEL = 32969,
        ACTION_START_RESCUE = 101,
    };

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
        if (player->GetQuestStatus(QUEST_THE_LAST_WAVE_OF_SURVIVORS) == QUEST_STATUS_INCOMPLETE)
            if (!player->GetReqKillOrCastCurrentCount(QUEST_THE_LAST_WAVE_OF_SURVIVORS, NPC_GERSHALA_NIGHTWHISPER))
                if (Creature* npc = creature->SummonCreature(NPC_RESCUE_SENTINEL, 7422.14f, 151.08f, 1.70f, 0.328f, TEMPSUMMON_TIMED_DESPAWN, 120000))
                {
                    player->KilledMonsterCredit(NPC_GERSHALA_NIGHTWHISPER);
                    npc->setFaction(35);
                    npc->GetAI()->SetGUID(creature->GetGUID(), NPC_GERSHALA_NIGHTWHISPER);
                    npc->GetAI()->SetGUID(player->GetGUID(), PLAYER_GUID);
                    npc->GetAI()->DoAction(ACTION_START_RESCUE);
                    return true;
                }

        return false;
    }
};

// 33093 Cerellean Whiteclaw
class npc_cerellean_whiteclaw_33093 : public CreatureScript
{
public:
    npc_cerellean_whiteclaw_33093() : CreatureScript("npc_cerellean_whiteclaw_33093") { }

    enum eQuest
    {
        QUEST_THE_LAST_WAVE_OF_SURVIVORS = 13518,
        NPC_CERELLEAN_WHITECLAW = 33093,
        PLAYER_GUID = 99999,
        NPC_RESCUE_SENTINEL = 32969,
        ACTION_START_RESCUE = 101,
    };

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
        if (player->GetQuestStatus(QUEST_THE_LAST_WAVE_OF_SURVIVORS) == QUEST_STATUS_INCOMPLETE)
            if (!player->GetReqKillOrCastCurrentCount(QUEST_THE_LAST_WAVE_OF_SURVIVORS, NPC_CERELLEAN_WHITECLAW))
                if (Creature* npc = creature->SummonCreature(NPC_RESCUE_SENTINEL, 7375.88f, 113.33f, 12.38f, 1.865f, TEMPSUMMON_TIMED_DESPAWN, 120000))
                {
                    player->KilledMonsterCredit(NPC_CERELLEAN_WHITECLAW);
                    npc->setFaction(35);
                    npc->GetAI()->SetGUID(creature->GetGUID(), NPC_CERELLEAN_WHITECLAW);
                    npc->GetAI()->SetGUID(player->GetGUID(), PLAYER_GUID);
                    npc->GetAI()->DoAction(ACTION_START_RESCUE);
                    return true;
                }

        return false;
    }
};

// 33094 Volcor
class npc_volcor_33094 : public CreatureScript
{
public:
    npc_volcor_33094() : CreatureScript("npc_volcor_33094") { }

    enum eQuest
    {
        QUEST_THE_LAST_WAVE_OF_SURVIVORS = 13518,
        NPC_VOLCOR = 33094,
        PLAYER_GUID = 99999,
        NPC_RESCUE_SENTINEL = 32969,
        ACTION_START_RESCUE = 101,
    };

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
        if (player->GetQuestStatus(QUEST_THE_LAST_WAVE_OF_SURVIVORS) == QUEST_STATUS_INCOMPLETE)
            if (!player->GetReqKillOrCastCurrentCount(QUEST_THE_LAST_WAVE_OF_SURVIVORS, NPC_VOLCOR))
                if (Creature* npc = creature->SummonCreature(NPC_RESCUE_SENTINEL, 7431.35f, 87.78f, 2.16f, 1.212f, TEMPSUMMON_TIMED_DESPAWN, 120000))
                {
                    player->KilledMonsterCredit(NPC_VOLCOR);
                    npc->setFaction(35);
                    npc->GetAI()->SetGUID(creature->GetGUID(), NPC_VOLCOR);
                    npc->GetAI()->SetGUID(player->GetGUID(), PLAYER_GUID);
                    npc->GetAI()->DoAction(ACTION_START_RESCUE);
                    return true;
                }

        return false;
    }
};

// 33095 Shaldyn
class npc_shaldyn_33095 : public CreatureScript
{
public:
    npc_shaldyn_33095() : CreatureScript("npc_shaldyn_33095") { }

    enum eQuest
    {
        QUEST_THE_LAST_WAVE_OF_SURVIVORS = 13518,
        NPC_SHALDYN = 33095,
        PLAYER_GUID = 99999,
        NPC_RESCUE_SENTINEL = 32969,
        ACTION_START_RESCUE = 101,
    };

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
        if (player->GetQuestStatus(QUEST_THE_LAST_WAVE_OF_SURVIVORS) == QUEST_STATUS_INCOMPLETE)
            if (!player->GetReqKillOrCastCurrentCount(QUEST_THE_LAST_WAVE_OF_SURVIVORS, NPC_SHALDYN))
                if (Creature* npc = creature->SummonCreature(NPC_RESCUE_SENTINEL, 7289.61f, 215.36f, 0.53f, 1.299f, TEMPSUMMON_TIMED_DESPAWN, 120000))
                {
                    player->KilledMonsterCredit(NPC_SHALDYN);
                    npc->setFaction(35);
                    npc->GetAI()->SetGUID(creature->GetGUID(), NPC_SHALDYN);
                    npc->GetAI()->SetGUID(player->GetGUID(), PLAYER_GUID);
                    npc->GetAI()->DoAction(ACTION_START_RESCUE);
                    return true;
                }

        return false;
    }
};


enum CliffspringAnimalCage
{
    QUEST_BEARER_OF_GOOD_FORTUNE = 13557,
    NPC_UNCORRUPTED_DUSKRAT = 33023,
    NPC_UNCORRUPTED_THISTLE_BEAR = 33024,
    GO_SECURE_DUSKRAT_CAGE = 194133,
    GO_SECURE_BEAR_CAGE = 194124,
    POINT_INIT = 1,
    EVENT_DESPAWN = 1,
};

class npc_uncorrupted_animal : public CreatureScript
{
public:
    npc_uncorrupted_animal() : CreatureScript("npc_uncorrupted_animal") { }

    struct npc_uncorrupted_animalAI : public ScriptedAI
    {
        npc_uncorrupted_animalAI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        void Initialize()
        {
            _playerGUID.Clear();
            _movementComplete = false;
        }

        void Reset() override
        {
            if (GameObject* cage = me->FindNearestGameObject(GO_SECURE_DUSKRAT_CAGE, 5.0f))
            {
                cage->SetLootState(GO_JUST_DEACTIVATED);
                cage->SetGoState(GO_STATE_READY);
            }
            if (GameObject* cage = me->FindNearestGameObject(GO_SECURE_BEAR_CAGE, 5.0f))
            {
                cage->SetLootState(GO_JUST_DEACTIVATED);
                cage->SetGoState(GO_STATE_READY);
            }
            _events.Reset();
            Initialize();
        }

        void StartMoving(Player* owner)
        {
            if (owner)
            {   
                _playerGUID = owner->GetGUID();
            }
            Position pos = me->GetNearPosition(3.0f, 0.0f);
            me->GetMotionMaster()->MovePoint(POINT_INIT, pos);
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type != POINT_MOTION_TYPE || id != POINT_INIT)
                return;

            if (Player* _player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                _player->KilledMonsterCredit(NPC_UNCORRUPTED_THISTLE_BEAR, ObjectGuid::Empty);

            _movementComplete = true;
            _events.ScheduleEvent(EVENT_DESPAWN, 3500);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!_movementComplete)
                return;

            _events.Update(diff);

            if (_events.ExecuteEvent() == EVENT_DESPAWN)
                me->DespawnOrUnsummon();
        }

    private:
        ObjectGuid _playerGUID;
        EventMap _events;
        bool _movementComplete;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_uncorrupted_animalAI(creature);
    }
};

class go_secure_animal_cage : public GameObjectScript
{
public:
    go_secure_animal_cage() : GameObjectScript("go_secure_animal_cage") { }

    bool OnGossipHello(Player* player, GameObject* go) override
    {
        go->SetGoState(GO_STATE_READY);
        if (player->GetQuestStatus(QUEST_BEARER_OF_GOOD_FORTUNE) == QUEST_STATUS_INCOMPLETE)
        {
            if (Creature* uncorrupted_bear = go->FindNearestCreature(NPC_UNCORRUPTED_THISTLE_BEAR, 5.0f, true))
            {
                go->ResetDoorOrButton();
                ENSURE_AI(npc_uncorrupted_animal::npc_uncorrupted_animalAI, uncorrupted_bear->AI())->StartMoving(player);
                return false;
            }
            if (Creature* uncorrupted_duskrat = go->FindNearestCreature(NPC_UNCORRUPTED_DUSKRAT, 5.0f, true))
            {
                go->ResetDoorOrButton();
                ENSURE_AI(npc_uncorrupted_animal::npc_uncorrupted_animalAI, uncorrupted_duskrat->AI())->StartMoving(player);
                return false;
            }
        }
        return true;
    }
};

void AddSC_darkshore()
{
    new npc_gershala_nightwhisper_32911();
    new npc_cerellean_whiteclaw_33093();
    new npc_volcor_33094();
    new npc_shaldyn_33095();
   // new npc_lordanel_sentinel_32969();
    new npc_uncorrupted_animal();
    new go_secure_animal_cage();
}
