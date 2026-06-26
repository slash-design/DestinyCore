
#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "SpellScript.h"
#include "SpellAuras.h"
#include "ScriptedGossip.h"
#include "MotionMaster.h"
#include <G3D/Quat.h>

enum DarkmoonFaireYells
{
    // Selina
    SAY_SELINA_WELCOME           = 0,
};

class npc_darkmoon_faire_mystic_mage : public CreatureScript
{
public:
    npc_darkmoon_faire_mystic_mage() : CreatureScript("npc_darkmoon_faire_mystic_mage") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        char const* GOSSIP_BUTTON;
        char const* BOX_TEXT;

        switch (LocaleConstant currentlocale = player->GetSession()->GetSessionDbcLocale())
        {
            case LOCALE_esES:
            case LOCALE_esMX:
                GOSSIP_BUTTON         = "Ll vame a la zona de escala de la feria.";
                BOX_TEXT              = "Viajar a la zona de escala de la feria te costar :";
                break;
            default:
                GOSSIP_BUTTON         = "Take me to the faire staging area.";
                BOX_TEXT              = "Travel to the faire staging area will cost:";
                break;
        };

        AddGossipItemFor(player, 0, GOSSIP_BUTTON, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1, BOX_TEXT, 25, false);
        SendGossipMenuFor(player, 18269, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 uiAction)
    {
        if (player->HasEnoughMoney(uint64(25)))
        {
            player->ModifyMoney(uint64(-25));

            player->PlayerTalkClass->ClearMenus();
            CloseGossipMenuFor(player);

            if (player->GetTeam() == ALLIANCE)
                creature->CastSpell(player, 101260, false);
            else
                creature->CastSpell(player, 103582, false);

            return true;
        }
        else
            player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, creature, 0, 0);

        return false;
    }
};

enum SelinaDourmanEvent
{
    EVENT_RENEW_SELINA_TEXT = 1,
};

class npc_selina_dourman : public CreatureScript
{
public:
    npc_selina_dourman() : CreatureScript("npc_selina_dourman") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_selina_dourmanAI (creature);
    }

    struct npc_selina_dourmanAI : public ScriptedAI
    {
        npc_selina_dourmanAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;

        bool Talked;

        void Reset()
        {
            Talked = false;
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (who->GetTypeId() != TYPEID_PLAYER)
                return;

            if (who->GetExactDist(me) <= 20.0f && !Talked)
            {
                Talked = true;
                Talk(SAY_SELINA_WELCOME);
                events.ScheduleEvent(EVENT_RENEW_SELINA_TEXT, 60000);
            }
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_RENEW_SELINA_TEXT:
                        Talked = false;
                        break;
                }
            }
        }
    };

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        char const* GOSSIP_BUTTON_1;
        char const* GOSSIP_BUTTON_2;
        char const* GOSSIP_BUTTON_3;
        char const* GOSSIP_BUTTON_4;
        char const* GOSSIP_BUTTON_5;

        switch (LocaleConstant currentlocale = player->GetSession()->GetSessionDbcLocale())
        {
            case LOCALE_esES:
            case LOCALE_esMX:
                GOSSIP_BUTTON_1         = " Gu a del aventurero de la Luna Negra?";
                GOSSIP_BUTTON_2         = " Qu  puedo comprar?";
                GOSSIP_BUTTON_3         = " Vales para la Feria de la Luna Negra?";
                GOSSIP_BUTTON_4         = " Cartas de la Luna Negra?";
                GOSSIP_BUTTON_5         = " Atracciones?";
                break;
            default:
                GOSSIP_BUTTON_1         = "Darkmoon Adventurer's Guide?";
                GOSSIP_BUTTON_2         = "What can I purchase?";
                GOSSIP_BUTTON_3         = "Darkmoon Faire Prize Tickets?";
                GOSSIP_BUTTON_4         = "Darkmoon Cards?";
                GOSSIP_BUTTON_5         = "Attractions?";
                break;
        };

        AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);

        SendGossipMenuFor(player, 23004, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
    {
        char const* GOSSIP_BUTTON_1;
        char const* GOSSIP_BUTTON_2;
        char const* GOSSIP_BUTTON_3;
        char const* GOSSIP_BUTTON_4;
        char const* GOSSIP_BUTTON_5;
        char const* GOSSIP_BUTTON_6;
        char const* GOSSIP_BUTTON_7;
        char const* GOSSIP_BUTTON_8;
        char const* GOSSIP_BUTTON_9;
        char const* GOSSIP_BUTTON_10;
        char const* GOSSIP_BUTTON_11;
        char const* GOSSIP_BUTTON_12;
        char const* GOSSIP_BUTTON_13;

        switch (LocaleConstant currentlocale = player->GetSession()->GetSessionDbcLocale())
        {
            case LOCALE_esES:
            case LOCALE_esMX:
                GOSSIP_BUTTON_1         = " Gu a del aventurero de la Luna Negra?";
                GOSSIP_BUTTON_2         = " Qu  puedo comprar?";
                GOSSIP_BUTTON_3         = " Vales para la Feria de la Luna Negra?";
                GOSSIP_BUTTON_4         = " Cartas de la Luna Negra?";
                GOSSIP_BUTTON_5         = " Atracciones?";
                GOSSIP_BUTTON_6         = " Me puedes dar una gu a del aventurero de la Luna Negra?";
                GOSSIP_BUTTON_7         = "Cu ntame m s.";
                GOSSIP_BUTTON_8         = " Tonques?";
                GOSSIP_BUTTON_9         = " Ca  n?";
                GOSSIP_BUTTON_10        = " Golpear al gnoll?";
                GOSSIP_BUTTON_11        = " Lanzamiento de anillos?";
                GOSSIP_BUTTON_12        = " Galer a de tiro?";
                GOSSIP_BUTTON_13        = " Clarividente?";
                break;
            default:
                GOSSIP_BUTTON_1         = "Darkmoon Adventurer's Guide?";
                GOSSIP_BUTTON_2         = "What can I purchase?";
                GOSSIP_BUTTON_3         = "Darkmoon Faire Prize Tickets?";
                GOSSIP_BUTTON_4         = "Darkmoon Cards?";
                GOSSIP_BUTTON_5         = "Attractions?";
                GOSSIP_BUTTON_6         = "May I have another Darkmoon Adventurer's Guide?";
                GOSSIP_BUTTON_7         = "Tell me more.";
                GOSSIP_BUTTON_8         = "Tonk Challenge?";
                GOSSIP_BUTTON_9         = "Cannon?";
                GOSSIP_BUTTON_10        = "Whack-a-gnoll?";
                GOSSIP_BUTTON_11        = "Ring Toss?";
                GOSSIP_BUTTON_12        = "Shooting Gallery?";
				GOSSIP_BUTTON_13		= "Farseer?";
                break;
        };

        player->PlayerTalkClass->ClearMenus();

        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
            SendGossipMenuFor(player, 23005, creature->GetGUID());
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 2)
            SendGossipMenuFor(player, 23006, creature->GetGUID());

        if (action == GOSSIP_ACTION_INFO_DEF + 3)
            SendGossipMenuFor(player, 23007, creature->GetGUID());

        if (action == GOSSIP_ACTION_INFO_DEF + 4)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
            SendGossipMenuFor(player, 23008, creature->GetGUID());
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 5)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_8, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_9, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_10, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_11, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_12, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_13, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
            SendGossipMenuFor(player, 23010, creature->GetGUID());
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 6)
        {
            player->PlayerTalkClass->ClearMenus();
            CloseGossipMenuFor(player);
            player->AddItem(71634, 1);
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 7)
            SendGossipMenuFor(player, 23009, creature->GetGUID());

        if (action == GOSSIP_ACTION_INFO_DEF + 8)
        {
            SendGossipMenuFor(player, 23011, creature->GetGUID());
            //player->PlayerTalkClass->SendPointOfInterest();
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 9)
        {
            SendGossipMenuFor(player, 23012, creature->GetGUID());
            //player->PlayerTalkClass->SendPointOfInterest();
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 10)
        {
            SendGossipMenuFor(player, 23013, creature->GetGUID());
            //player->PlayerTalkClass->SendPointOfInterest();
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 11)
        {
            SendGossipMenuFor(player, 23014, creature->GetGUID());
            //player->PlayerTalkClass->SendPointOfInterest();
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 12)
        {
            SendGossipMenuFor(player, 23015, creature->GetGUID());
            //player->PlayerTalkClass->SendPointOfInterest();
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 13)
        {
            SendGossipMenuFor(player, 23016, creature->GetGUID());
            //player->PlayerTalkClass->SendPointOfInterest();
        }

        return true;
    }
};

enum WhackAGNollEvents
{
    EVENT_WHACKAGNOLL_START_GAME_FIELD_1  = 1,
    EVENT_WHACKAGNOLL_START_GAME_FIELD_2  = 2,
    EVENT_WHACKAGNOLL_START_GAME_FIELD_3  = 3,
    EVENT_WHACKAGNOLL_FINISH_GAME         = 4,
};

class npc_mola : public CreatureScript
{
public:
    npc_mola() : CreatureScript("npc_mola") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_molaAI(creature);
    }

    struct npc_molaAI : public ScriptedAI
    {
        npc_molaAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;

        bool Active;

        void Reset()
        {
            Active = false;
        }

        void StartGame()
        {
            if (!Active)
            {
                events.ScheduleEvent(EVENT_WHACKAGNOLL_START_GAME_FIELD_1, 0);
                events.ScheduleEvent(EVENT_WHACKAGNOLL_START_GAME_FIELD_2, 0);
                events.ScheduleEvent(EVENT_WHACKAGNOLL_START_GAME_FIELD_3, 0);
                events.ScheduleEvent(EVENT_WHACKAGNOLL_FINISH_GAME, 60000);
            }
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_WHACKAGNOLL_START_GAME_FIELD_1:
                            switch (urand(0,2))
                            {
                                case 0:
                                    switch (urand(0,2))
                                    {
                                        case 0:
                                            if (Creature* summon = me->SummonCreature(54444, -3983.67f, 6300.26f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                        case 1:
                                            if (Creature* summon = me->SummonCreature(54466, -3983.67f, 6300.26f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                        case 2:
                                            if (Creature* summon = me->SummonCreature(54549, -3983.67f, 6300.26f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                    }
                                    break;
                                case 1:
                                    switch (urand(0,2))
                                    {
                                        case 0:
                                            if (Creature* summon = me->SummonCreature(54444, -3989, 6296.27f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                        case 1:
                                            if (Creature* summon = me->SummonCreature(54466, -3989, 6296.27f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                        case 2:
                                            if (Creature* summon = me->SummonCreature(54549, -3989, 6296.27f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                    }
                                    break;
                                case 2:
                                    switch (urand(0,2))
                                    {
                                        case 0:
                                            if (Creature* summon = me->SummonCreature(54444, -3994.54f, 6292.83f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                        case 1:
                                            if (Creature* summon = me->SummonCreature(54466, -3994.54f, 6292.83f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                        case 2:
                                            if (Creature* summon = me->SummonCreature(54549, -3994.54f, 6292.83f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                    }
                                    break;
                            }
                            events.ScheduleEvent(EVENT_WHACKAGNOLL_START_GAME_FIELD_1, 5000);
                            break;
                        case EVENT_WHACKAGNOLL_START_GAME_FIELD_2:
                            switch (urand(0,2))
                            {
                                case 0:
                                    switch (urand(0,2))
                                    {
                                        case 0:
                                            if (Creature* summon = me->SummonCreature(54444, -3979.43f, 6295.16f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                        case 1:
                                            if (Creature* summon = me->SummonCreature(54466, -3979.43f, 6295.16f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                        case 2:
                                            if (Creature* summon = me->SummonCreature(54549, -3979.43f, 6295.16f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                    }
                                    break;
                                case 1:
                                    switch (urand(0,2))
                                    {
                                        case 0:
                                            if (Creature* summon = me->SummonCreature(54444, -3985.14f, 6291.34f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                        case 1:
                                            if (Creature* summon = me->SummonCreature(54466, -3985.14f, 6291.34f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                        case 2:
                                            if (Creature* summon = me->SummonCreature(54549, -3985.14f, 6291.34f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                    }
                                    break;
                                case 2:
                                    switch (urand(0,2))
                                    {
                                        case 0:
                                            if (Creature* summon = me->SummonCreature(54444, -3990.78f, 6287.58f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                        case 1:
                                            if (Creature* summon = me->SummonCreature(54466, -3990.78f, 6287.58f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                        case 2:
                                            if (Creature* summon = me->SummonCreature(54549, -3990.78f, 6287.58f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))                                                    summon->CastSpell(summon, 102136, false);
                                                break;
                                    }
                                    break;
                            }
                            events.ScheduleEvent(EVENT_WHACKAGNOLL_START_GAME_FIELD_2, 5000);
                            break;
                        case EVENT_WHACKAGNOLL_START_GAME_FIELD_3:
                            switch (urand(0,2))
                            {
                                case 0:
                                    switch (urand(0,2))
                                    {
                                        case 0:
                                            if (Creature* summon = me->SummonCreature(54444, -3975.98f, 6289.3f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                        case 1:
                                            if (Creature* summon = me->SummonCreature(54466, -3975.98f, 6289.3f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                        case 2:
                                            if (Creature* summon = me->SummonCreature(54549, -3975.98f, 6289.3f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                    }
                                    break;
                                case 1:
                                    switch (urand(0,2))
                                    {
                                        case 0:
                                            if (Creature* summon = me->SummonCreature(54444, -3981.68f, 6285.7f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                        case 1:
                                            if (Creature* summon = me->SummonCreature(54466, -3981.68f, 6285.7f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                        case 2:
                                            if (Creature* summon = me->SummonCreature(54549, -3981.68f, 6285.7f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                    }
                                    break;
                                case 2:
                                    switch (urand(0,2))
                                    {
                                        case 0:
                                            if (Creature* summon = me->SummonCreature(54444, -3986.82f, 6282.48f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                        case 1:
                                            if (Creature* summon = me->SummonCreature(54466, -3986.82f, 6282.48f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                        case 2:
                                            if (Creature* summon = me->SummonCreature(54549, -3986.82f, 6282.48f, 13.11f, 3.82f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                                summon->CastSpell(summon, 102136, false);
                                            break;
                                    }
                                    break;
                            }
                            events.ScheduleEvent(EVENT_WHACKAGNOLL_START_GAME_FIELD_3, 5000);
                            break;
                    case EVENT_WHACKAGNOLL_FINISH_GAME:
                        Active = false;
                        events.CancelEvent(EVENT_WHACKAGNOLL_START_GAME_FIELD_1);
                        events.CancelEvent(EVENT_WHACKAGNOLL_START_GAME_FIELD_2);
                        events.CancelEvent(EVENT_WHACKAGNOLL_START_GAME_FIELD_3);
                        break;
                    }
            }
        }
    };

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        char const* GOSSIP_BUTTON_1;
        char const* GOSSIP_BUTTON_2;

        switch (LocaleConstant currentlocale = player->GetSession()->GetSessionDbcLocale())
        {
            case LOCALE_esES:
            case LOCALE_esMX:
                GOSSIP_BUTTON_1         = " C mo puedo jugar a Golpea al gnoll?";
                GOSSIP_BUTTON_2         = " Listo para aporrear! |cFF0000FF(Ficha de juego de la Luna Negra)|r";
                break;
            default:
                GOSSIP_BUTTON_1         = "How do I play Whack-a-gnoll?";
                GOSSIP_BUTTON_2         = "Ready to whack! |cFF0000FF(Darkmoon Game Token)|r";
                break;
        };

        AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        if (!player->HasAura(101612) && !player->HasAura(110230) && !player->HasAura(102121) && !player->HasAura(102178) && !player->HasAura(102058) && !player->HasAura(101871))
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

        SendGossipMenuFor(player, 54601, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
    {
        char const* GOSSIP_BUTTON_1;
        char const* GOSSIP_BUTTON_2;
        char const* GOSSIP_BUTTON_3;

        switch (LocaleConstant currentlocale = player->GetSession()->GetSessionDbcLocale())
        {
            case LOCALE_esES:
            case LOCALE_esMX:
                GOSSIP_BUTTON_1         = " C mo puedo jugar a Golpea al gnoll?";
                GOSSIP_BUTTON_2         = " Listo para aporrear! |cFF0000FF(Ficha de juego de la Luna Negra)|r";
                GOSSIP_BUTTON_3         = "Comprendo.";
                break;
            default:
                GOSSIP_BUTTON_1         = "How do I play Whack-a-gnoll?";
                GOSSIP_BUTTON_2         = "Ready to whack! |cFF0000FF(Darkmoon Game Token)|r";
                GOSSIP_BUTTON_3         = "Alright.";
                break;
        };

        player->PlayerTalkClass->ClearMenus();

        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            SendGossipMenuFor(player, 54602, creature->GetGUID());
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 2)
        {
            if (player->HasItemCount(71083))
            {
                CloseGossipMenuFor(player);

                player->DestroyItemCount(71083, 1, true);
                player->RemoveAurasByType(SPELL_AURA_MOUNTED);

                if (!player->HasAura(101612) && !player->HasAura(110230))
                {
                    player->AddAura(101612, player);
                    player->AddAura(110230, player);

                    CAST_AI(npc_mola::npc_molaAI, creature->AI())->StartGame();
                    CAST_AI(npc_mola::npc_molaAI, creature->AI())->Active = true;
                }
                return true;
            }
            else
                SendGossipMenuFor(player, 54603, creature->GetGUID());
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 3)
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            if (!player->HasAura(101612) && !player->HasAura(110230) && !player->HasAura(102121) && !player->HasAura(102178) && !player->HasAura(102058) && !player->HasAura(101871))
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

            SendGossipMenuFor(player, 54601, creature->GetGUID());
        }

        return true;
    }
};

enum TheHumanoidCannonBall
{
    EVENT_CANNONBALL_START_GAME_BOOM      = 1,
    EVENT_CANNONBALL_JUMP                 = 2,
    EVENT_CANNONBALL_CHECK                = 3,
    EVENT_CANNONBALL_FINISH_GAME          = 4,
};

class npc_maxima_blastenheimer : public CreatureScript
{
public:
    npc_maxima_blastenheimer() : CreatureScript("npc_maxima_blastenheimer") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_maxima_blastenheimerAI(creature);
    }

    struct npc_maxima_blastenheimerAI : public ScriptedAI
    {
        npc_maxima_blastenheimerAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;

        void StartGame()
        {
            events.ScheduleEvent(EVENT_CANNONBALL_START_GAME_BOOM, 5000);
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CANNONBALL_START_GAME_BOOM:
                        if (Creature* cannontrigger = me->FindNearestCreature(54222, 25.0f, true))
                        {
                            cannontrigger->CastSpell(cannontrigger, 102115, false);

                            if (Player* player = cannontrigger->SelectNearestPlayer(5.0f))
                                player->RemoveAurasDueToSpell(74653);
                        }
                        events.ScheduleEvent(EVENT_CANNONBALL_JUMP, 0);
                        break;
                    case EVENT_CANNONBALL_JUMP:
                        if (Creature* cannontrigger = me->FindNearestCreature(54222, 25.0f, true))
                            if (Player* player = cannontrigger->SelectNearestPlayer(5.0f))
                                cannontrigger->CastSpell(player, 102116, false);

                        events.ScheduleEvent(EVENT_CANNONBALL_FINISH_GAME, 2000);
                        break;
                    case EVENT_CANNONBALL_FINISH_GAME:
                        me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        break;
                }
            }
        }
    };

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        char const* GOSSIP_BUTTON_1;
        char const* GOSSIP_BUTTON_2;

        switch (LocaleConstant currentlocale = player->GetSession()->GetSessionDbcLocale())
        {
            case LOCALE_esES:
            case LOCALE_esMX:
                GOSSIP_BUTTON_1         = " C mo se usa el ca  n?";
                GOSSIP_BUTTON_2         = " L nzame! |cFF0000FF(Ficha de juego de la Luna Negra)|r";
                break;
            default:
                GOSSIP_BUTTON_1         = "How do I use the cannon?";
                GOSSIP_BUTTON_2         = "Launch me! |cFF0000FF(Darkmoon Game Token)|r";
                break;
        };

        AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        if (!player->HasAura(101612) && !player->HasAura(110230) && !player->HasAura(102121) && !player->HasAura(102178) && !player->HasAura(102058) && !player->HasAura(101871))
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

        SendGossipMenuFor(player, 23000, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
    {
        char const* GOSSIP_BUTTON_1;
        char const* GOSSIP_BUTTON_2;
        char const* GOSSIP_BUTTON_3;

        switch (LocaleConstant currentlocale = player->GetSession()->GetSessionDbcLocale())
        {
            case LOCALE_esES:
            case LOCALE_esMX:
                GOSSIP_BUTTON_1         = " C mo uso el ca  n?";
                GOSSIP_BUTTON_2         = " L nzame! |cFF0000FF(Ficha de juego de la Luna Negra)|r";
                GOSSIP_BUTTON_3         = "Comprendo.";
                break;
            default:
                GOSSIP_BUTTON_1         = " How do I use the cannon?";
                GOSSIP_BUTTON_2         = "Launch me! |cFF0000FF(Darkmoon Game Token)|r";
                GOSSIP_BUTTON_3         = "Alright.";
                break;
        };

        player->PlayerTalkClass->ClearMenus();

        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            SendGossipMenuFor(player, 23001, creature->GetGUID());
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 2)
        {
            if (player->HasItemCount(71083))
            {
                CloseGossipMenuFor(player);

                player->DestroyItemCount(71083, 1, true);
                player->RemoveAurasByType(SPELL_AURA_MOUNTED);

                player->AddAura(102121, player);
                player->AddAura(101994, player);
                player->AddAura(74653, player);
                player->AddAura(102112, player);

                player->NearTeleportTo(-4021.01f, 6299.91f, 17.5f, 3.03f, false);

                CAST_AI(npc_maxima_blastenheimer::npc_maxima_blastenheimerAI, creature->AI())->StartGame();

                creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                return true;
            }
            else
                SendGossipMenuFor(player, 54603, creature->GetGUID());
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 3)
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            if (!player->HasAura(101612) && !player->HasAura(110230) && !player->HasAura(102121) && !player->HasAura(102178) && !player->HasAura(102058) && !player->HasAura(101871))
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

            SendGossipMenuFor(player, 23000, creature->GetGUID());
        }

        return true;
    }
};

class npc_darkmoon_cannon_trigger : public CreatureScript
{
public:
    npc_darkmoon_cannon_trigger() : CreatureScript("npc_darkmoon_cannon_trigger") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_darkmoon_cannon_triggerAI(creature);
    }

    struct npc_darkmoon_cannon_triggerAI : public ScriptedAI
    {
        npc_darkmoon_cannon_triggerAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;

        void Reset()
        {
            events.ScheduleEvent(EVENT_CANNONBALL_CHECK, 3000);
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CANNONBALL_CHECK:
                        AchievementEntry const *AchievCannonBall = sAchievementStore.LookupEntry(6021);

                        if (Player* player = me->SelectNearestPlayer(9.0f))
                        {
                            if (player->HasAura(101994))
                            {
                                player->RemoveAurasDueToSpell(101994);
                                player->RemoveAurasDueToSpell(102121);

                                if (me->GetExactDist(player) <= 3.0f)
                                {
                                    player->CompletedAchievement(AchievCannonBall);

                                    player->KilledMonsterCredit(54224, ObjectGuid::Empty);
                                    player->KilledMonsterCredit(54224, ObjectGuid::Empty);
                                    player->KilledMonsterCredit(54224, ObjectGuid::Empty);
                                    player->KilledMonsterCredit(54224, ObjectGuid::Empty);
                                    player->KilledMonsterCredit(54224, ObjectGuid::Empty);
                                }
                                else if (me->GetExactDist(player) <= 6.0f)
                                {
                                    player->CompletedAchievement(AchievCannonBall);

                                    player->KilledMonsterCredit(54224, ObjectGuid::Empty);
                                    player->KilledMonsterCredit(54224, ObjectGuid::Empty);
                                    player->KilledMonsterCredit(54224, ObjectGuid::Empty);
                                }
                                else if (me->GetExactDist(player) <= 9.0f)
                                    player->KilledMonsterCredit(54224, ObjectGuid::Empty);
                            }
                            else
                            {
                                player->GetMotionMaster()->MoveJump(-4456.84f, 6211.07f, -1.571f, 0.0f, 20.0f, 20.0f);
                            }
                        }
                        events.ScheduleEvent(EVENT_CANNONBALL_CHECK, 1000);
                        break;
                }
            }
        }
    };
};

class npc_teleportologist_fozlebub : public CreatureScript
{
public:
    npc_teleportologist_fozlebub() : CreatureScript("npc_teleportologist_fozlebub") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        char const* GOSSIP_BUTTON;
        char const* BOX_TEXT;

        switch (LocaleConstant currentlocale = player->GetSession()->GetSessionDbcLocale())
        {
            case LOCALE_esES:
            case LOCALE_esMX:
                GOSSIP_BUTTON         = "Teletransp rtame al ca  n.";
                BOX_TEXT              = "El teletransporte al ca  n te costar :";
                break;
            default:
                GOSSIP_BUTTON         = "Teleport me to the cannon.";
                BOX_TEXT              = "Teleportation to the cannon will cost:";
                break;
        };

        AddGossipItemFor(player, 0, GOSSIP_BUTTON, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1, BOX_TEXT, 1000, false);
        SendGossipMenuFor(player, 23017, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 uiAction)
    {
        if (player->HasEnoughMoney(uint64(1000)))
        {
            player->ModifyMoney(uint64(-1000));

            player->PlayerTalkClass->ClearMenus();
            CloseGossipMenuFor(player);

            creature->CastSpell(player, 109244, false);

            return true;
        }
        else
            player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, creature, 0, 0);

        return false;
    }
};

enum TonkCommander
{
    EVENT_TONKCOMMANDER_START_GAME_1      = 1,
    EVENT_TONKCOMMANDER_START_GAME_2      = 2,
    EVENT_TONKCOMMANDER_START_GAME_3      = 3,
    EVENT_TONKCOMMANDER_FINISH_GAME       = 4,
};

class npc_finlay_coolshot : public CreatureScript
{
public:
    npc_finlay_coolshot() : CreatureScript("npc_finlay_coolshot") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_finlay_coolshotAI(creature);
    }

    struct npc_finlay_coolshotAI : public ScriptedAI
    {
        npc_finlay_coolshotAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;

        bool Active;

        void Reset()
        {
            Active = false;
        }

        void StartGame()
        {
            if (!Active)
            {
                events.ScheduleEvent(EVENT_TONKCOMMANDER_START_GAME_1, 0);
                events.ScheduleEvent(EVENT_TONKCOMMANDER_START_GAME_2, 0);
                events.ScheduleEvent(EVENT_TONKCOMMANDER_START_GAME_3, 0);
                events.ScheduleEvent(EVENT_TONKCOMMANDER_FINISH_GAME, 60000);
            }
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_TONKCOMMANDER_START_GAME_1:
                        switch (urand(0,16))
                        {
                            case 0:
                                me->SummonCreature(33081, -4141.6f, 6315.88f, 13.11f, 4.91587f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 1:
                                me->SummonCreature(33081, -4146.43f, 6309.72f, 13.11f, 5.9655f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 2:
                                me->SummonCreature(33081, -4143.39f, 6314.97f, 13.11f, 5.87662f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 3:
                                me->SummonCreature(33081, -4128.46f, 6312.61f, 13.11f, 0.0794366f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 4:
                                me->SummonCreature(33081, -4123.75f, 6305.47f, 13.11f, 3.03825f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 5:
                                me->SummonCreature(33081, -4136.6f, 6301.53f, 13.1176f, 1.45875f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 6:
                                me->SummonCreature(33081, -4139.58f, 6308.16f, 13.1176f, 1.04362f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 7:
                                me->SummonCreature(33081, -4142.23f, 6291.32f, 13.1167f, 0.986097f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 8:
                                me->SummonCreature(33081, -4136.95f, 6289.13f, 13.1167f, 1.68022f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 9:
                                me->SummonCreature(33081, -4126.09f, 6292.85f, 13.1167f, 2.36872f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 10:
                                me->SummonCreature(33081, -4139.19f, 6287.29f, 13.1167f, 1.47097f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 11:
                                me->SummonCreature(33081, -4148.21f, 6301.88f, 13.1165f, 0.0309724f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 12:
                                me->SummonCreature(33081, -4142.29f, 6299.38f, 13.1165f, 0.0309724f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 13:
                                me->SummonCreature(33081, -4124.66f, 6299.65f, 13.1165f, 2.8671f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 14:
                                me->SummonCreature(33081, -4133.37f, 6308.8f, 13.1165f, 1.07716f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 15:
                                me->SummonCreature(33081, -4132.44f, 6291.18f, 13.1165f, 2.16906f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 16:
                                me->SummonCreature(33081, -4137.18f, 6295.24f, 13.1165f, 4.25818f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                        }
                        events.ScheduleEvent(EVENT_TONKCOMMANDER_START_GAME_1, 5000);
                        break;
                    case EVENT_TONKCOMMANDER_START_GAME_2:
                        switch (urand(0,16))
                        {
                            case 0:
                                me->SummonCreature(33081, -4141.6f, 6315.88f, 13.11f, 4.91587f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 1:
                                me->SummonCreature(33081, -4146.43f, 6309.72f, 13.11f, 5.9655f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 2:
                                me->SummonCreature(33081, -4143.39f, 6314.97f, 13.11f, 5.87662f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 3:
                                me->SummonCreature(33081, -4128.46f, 6312.61f, 13.11f, 0.0794366f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 4:
                                me->SummonCreature(33081, -4123.75f, 6305.47f, 13.11f, 3.03825f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 5:
                                me->SummonCreature(33081, -4136.6f, 6301.53f, 13.1176f, 1.45875f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 6:
                                me->SummonCreature(33081, -4139.58f, 6308.16f, 13.1176f, 1.04362f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 7:
                                me->SummonCreature(33081, -4142.23f, 6291.32f, 13.1167f, 0.986097f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 8:
                                me->SummonCreature(33081, -4136.95f, 6289.13f, 13.1167f, 1.68022f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 9:
                                me->SummonCreature(33081, -4126.09f, 6292.85f, 13.1167f, 2.36872f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 10:
                                me->SummonCreature(33081, -4139.19f, 6287.29f, 13.1167f, 1.47097f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 11:
                                me->SummonCreature(33081, -4148.21f, 6301.88f, 13.1165f, 0.0309724f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 12:
                                me->SummonCreature(33081, -4142.29f, 6299.38f, 13.1165f, 0.0309724f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 13:
                                me->SummonCreature(33081, -4124.66f, 6299.65f, 13.1165f, 2.8671f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 14:
                                me->SummonCreature(33081, -4133.37f, 6308.8f, 13.1165f, 1.07716f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 15:
                                me->SummonCreature(33081, -4132.44f, 6291.18f, 13.1165f, 2.16906f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 16:
                                me->SummonCreature(33081, -4137.18f, 6295.24f, 13.1165f, 4.25818f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                        }
                        events.ScheduleEvent(EVENT_TONKCOMMANDER_START_GAME_2, 5000);
                        break;
                    case EVENT_TONKCOMMANDER_START_GAME_3:
                        switch (urand(0,16))
                        {
                            case 0:
                                me->SummonCreature(33081, -4141.6f, 6315.88f, 13.11f, 4.91587f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 1:
                                me->SummonCreature(33081, -4146.43f, 6309.72f, 13.11f, 5.9655f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 2:
                                me->SummonCreature(33081, -4143.39f, 6314.97f, 13.11f, 5.87662f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 3:
                                me->SummonCreature(33081, -4128.46f, 6312.61f, 13.11f, 0.0794366f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 4:
                                me->SummonCreature(33081, -4123.75f, 6305.47f, 13.11f, 3.03825f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 5:
                                me->SummonCreature(33081, -4136.6f, 6301.53f, 13.1176f, 1.45875f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 6:
                                me->SummonCreature(33081, -4139.58f, 6308.16f, 13.1176f, 1.04362f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 7:
                                me->SummonCreature(33081, -4142.23f, 6291.32f, 13.1167f, 0.986097f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 8:
                                me->SummonCreature(33081, -4136.95f, 6289.13f, 13.1167f, 1.68022f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 9:
                                me->SummonCreature(33081, -4126.09f, 6292.85f, 13.1167f, 2.36872f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 10:
                                me->SummonCreature(33081, -4139.19f, 6287.29f, 13.1167f, 1.47097f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 11:
                                me->SummonCreature(33081, -4148.21f, 6301.88f, 13.1165f, 0.0309724f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 12:
                                me->SummonCreature(33081, -4142.29f, 6299.38f, 13.1165f, 0.0309724f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 13:
                                me->SummonCreature(33081, -4124.66f, 6299.65f, 13.1165f, 2.8671f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 14:
                                me->SummonCreature(33081, -4133.37f, 6308.8f, 13.1165f, 1.07716f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 15:
                                me->SummonCreature(33081, -4132.44f, 6291.18f, 13.1165f, 2.16906f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                            case 16:
                                me->SummonCreature(33081, -4137.18f, 6295.24f, 13.1165f, 4.25818f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                                break;
                        }
                        events.ScheduleEvent(EVENT_TONKCOMMANDER_START_GAME_3, 5000);
                        break;
                    case EVENT_TONKCOMMANDER_FINISH_GAME:
                        Active = false;
                        events.CancelEvent(EVENT_TONKCOMMANDER_START_GAME_1);
                        events.CancelEvent(EVENT_TONKCOMMANDER_START_GAME_2);
                        events.CancelEvent(EVENT_TONKCOMMANDER_START_GAME_3);
                        break;
                }
            }
        }
    };

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        char const* GOSSIP_BUTTON_1;
        char const* GOSSIP_BUTTON_2;

        switch (LocaleConstant currentlocale = player->GetSession()->GetSessionDbcLocale())
        {
            case LOCALE_esES:
            case LOCALE_esMX:
                GOSSIP_BUTTON_1         = " C mo puedo jugar a la batalla de tonques?";
                GOSSIP_BUTTON_2         = " Estoy listo para jugar! |cFF0000FF(Ficha de juego de la Luna Negra)|r";
                break;
            default:
                GOSSIP_BUTTON_1         = "How do I play the Tonk Challenge?";
                GOSSIP_BUTTON_2         = "Ready to play! |cFF0000FF(Darkmoon Game Token)|r";
                break;
        };

        AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        if (!player->HasAura(101612) && !player->HasAura(110230) && !player->HasAura(102121) && !player->HasAura(102178) && !player->HasAura(102058) && !player->HasAura(101871))
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

        SendGossipMenuFor(player, 54605, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
    {
        char const* GOSSIP_BUTTON_1;
        char const* GOSSIP_BUTTON_2;
        char const* GOSSIP_BUTTON_3;

        switch (LocaleConstant currentlocale = player->GetSession()->GetSessionDbcLocale())
        {
            case LOCALE_esES:
            case LOCALE_esMX:
                GOSSIP_BUTTON_1         = " C mo puedo jugar a la batalla de tonques?";
                GOSSIP_BUTTON_2         = " Estoy listo para jugar! |cFF0000FF(Ficha de juego de la Luna Negra)|r";
                GOSSIP_BUTTON_3         = "Comprendo.";
                break;
            default:
                GOSSIP_BUTTON_1         = "How do I play the Tonk Challenge?";
                GOSSIP_BUTTON_2         = "Ready to play! |cFF0000FF(Darkmoon Game Token)|r";
                GOSSIP_BUTTON_3         = "Alright.";
                break;
        };

        player->PlayerTalkClass->ClearMenus();

        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            SendGossipMenuFor(player, 54606, creature->GetGUID());
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 2)
        {
            if (player->HasItemCount(71083))
            {
                CloseGossipMenuFor(player);

                player->DestroyItemCount(71083, 1, true);
                player->RemoveAurasByType(SPELL_AURA_MOUNTED);
                player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);

                player->AddAura(102178, player);

                if (Creature* summon = creature->SummonCreature(54588, -4131.37f, 6317.32f, 13.11f, 4.31f, TEMPSUMMON_TIMED_DESPAWN, 60000))
                    player->CastSpell(summon, 46598, false);

                CAST_AI(npc_finlay_coolshot::npc_finlay_coolshotAI, creature->AI())->StartGame();
                CAST_AI(npc_finlay_coolshot::npc_finlay_coolshotAI, creature->AI())->Active = true;

                return true;
            }
            else
                SendGossipMenuFor(player, 54603, creature->GetGUID());
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 3)
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            if (!player->HasAura(101612) && !player->HasAura(110230) && !player->HasAura(102121) && !player->HasAura(102178) && !player->HasAura(102058) && !player->HasAura(101871))
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

            SendGossipMenuFor(player, 54605, creature->GetGUID());
        }

        return true;
    }
};

class npc_jessica_rogers : public CreatureScript
{
public:
    npc_jessica_rogers() : CreatureScript("npc_jessica_rogers") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        char const* GOSSIP_BUTTON_1;
        char const* GOSSIP_BUTTON_2;

        switch (LocaleConstant currentlocale = player->GetSession()->GetSessionDbcLocale())
        {
            case LOCALE_esES:
            case LOCALE_esMX:
                GOSSIP_BUTTON_1         = " C mo puedo jugar al lanzamiento de anillo?";
                GOSSIP_BUTTON_2         = " Estoy listo para jugar! |cFF0000FF(Ficha de juego de la Luna Negra)|r";
                break;
            default:
                GOSSIP_BUTTON_1         = "How do I play the Ring Toss?";
                GOSSIP_BUTTON_2         = "Ready to play! |cFF0000FF(Darkmoon Game Token)|r";
                break;
        };

        AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        if (!player->HasAura(101612) && !player->HasAura(110230) && !player->HasAura(102121) && !player->HasAura(102178) && !player->HasAura(102058) && !player->HasAura(101871))
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

        SendGossipMenuFor(player, 54485, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
    {
        char const* GOSSIP_BUTTON_1;
        char const* GOSSIP_BUTTON_2;
        char const* GOSSIP_BUTTON_3;

        switch (LocaleConstant currentlocale = player->GetSession()->GetSessionDbcLocale())
        {
            case LOCALE_esES:
            case LOCALE_esMX:
                GOSSIP_BUTTON_1         = " C mo puedo jugar al lanzamiento de anillo?";
                GOSSIP_BUTTON_2         = " Estoy listo para jugar! |cFF0000FF(Ficha de juego de la Luna Negra)|r";
                GOSSIP_BUTTON_3         = "Comprendo.";
                break;
            default:
                GOSSIP_BUTTON_1         = "How do I play the Ring Toss?";
                GOSSIP_BUTTON_2         = "Ready to play! |cFF0000FF(Darkmoon Game Token)|r";
                GOSSIP_BUTTON_3         = "Alright.";
                break;
        };

        player->PlayerTalkClass->ClearMenus();

        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            SendGossipMenuFor(player, 54486, creature->GetGUID());
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 2)
        {
            if (player->HasItemCount(71083))
            {
                CloseGossipMenuFor(player);

                player->DestroyItemCount(71083, 1, true);
                player->RemoveAurasByType(SPELL_AURA_MOUNTED);

                player->AddAura(102058, player);
                player->SetPower(POWER_ALTERNATE_POWER, 10);

                return true;
            }
            else
                SendGossipMenuFor(player, 54603, creature->GetGUID());
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 3)
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            if (!player->HasAura(101612) && !player->HasAura(110230) && !player->HasAura(102121) && !player->HasAura(102178) && !player->HasAura(102058) && !player->HasAura(101871))
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

            SendGossipMenuFor(player, 54485, creature->GetGUID());
        }

        return true;
    }
};

enum ShootGallery
{
    EVENT_SHOOTGALLERY_START_GAME        = 1,
    EVENT_SHOOTGALLERY_FINISH_GAME       = 2,
};

class npc_rinling : public CreatureScript
{
public:
    npc_rinling() : CreatureScript("npc_rinling") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_rinlingAI(creature);
    }

    struct npc_rinlingAI : public ScriptedAI
    {
        npc_rinlingAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;

        bool Active;

        void Reset()
        {
            Active = false;
        }

        void StartGame()
        {
            if (!Active)
            {
                events.ScheduleEvent(EVENT_SHOOTGALLERY_START_GAME, 0);
                events.ScheduleEvent(EVENT_SHOOTGALLERY_FINISH_GAME, 60000);
            }
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SHOOTGALLERY_START_GAME:
                        switch (urand(0,2))
                        {
                            case 0:
                                if (Creature* summon = me->SummonCreature(54231, -4072.19f, 6356.46f, 13.35f, 4.21f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                    summon->CastSpell(summon, 102341, false);

                                me->SummonCreature(54225, -4070.09f, 6354.87f, 12.57f, 4.21f, TEMPSUMMON_TIMED_DESPAWN, 5000);
                                me->SummonCreature(54225, -4068.41f, 6353.09f, 13.24f, 4.21f, TEMPSUMMON_TIMED_DESPAWN, 5000);
                                break;
                            case 1:
                                me->SummonCreature(54225, -4072.19f, 6356.46f, 13.35f, 4.21f, TEMPSUMMON_TIMED_DESPAWN, 5000);

                                if (Creature* summon = me->SummonCreature(54231, -4070.09f, 6354.87f, 12.57f, 4.21f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                    summon->CastSpell(summon, 102341, false);

                                me->SummonCreature(54225, -4068.41f, 6353.09f, 12.24f, 4.21f, TEMPSUMMON_TIMED_DESPAWN, 5000);
                                break;
                            case 2:
                                me->SummonCreature(54225, -4072.19f, 6356.46f, 13.35f, 4.21f, TEMPSUMMON_TIMED_DESPAWN, 5000);
                                me->SummonCreature(54225, -4070.09f, 6354.87f, 12.57f, 4.21f, TEMPSUMMON_TIMED_DESPAWN, 5000);

                                if (Creature* summon = me->SummonCreature(54231, -4068.41f, 6353.09f, 13.24f, 4.21f, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                    summon->CastSpell(summon, 102341, false);

                                break;
                        }
                        events.ScheduleEvent(EVENT_SHOOTGALLERY_START_GAME, 5000);
                        break;
                    case EVENT_SHOOTGALLERY_FINISH_GAME:
                        Active = false;
                        events.CancelEvent(EVENT_SHOOTGALLERY_START_GAME);
                        break;
                }
            }
        }
    };

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        char const* GOSSIP_BUTTON_1;
        char const* GOSSIP_BUTTON_2;

        switch (LocaleConstant currentlocale = player->GetSession()->GetSessionDbcLocale())
        {
            case LOCALE_esES:
            case LOCALE_esMX:
                GOSSIP_BUTTON_1         = " C mo funciona la galer a de tiro?";
                GOSSIP_BUTTON_2         = " Estoy listo para disparar! |cFF0000FF(Ficha de juego de la Luna Negra)|r";
                break;
            default:
                GOSSIP_BUTTON_1         = "How does the Shooting Gallery work?";
                GOSSIP_BUTTON_2         = "Let's shoot! |cFF0000FF(Darkmoon Game Token)|r";
                break;
        };

        AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        if (!player->HasAura(101612) && !player->HasAura(110230) && !player->HasAura(102121) && !player->HasAura(102178) && !player->HasAura(102058) && !player->HasAura(101871))
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

        SendGossipMenuFor(player, 23002, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
    {
        char const* GOSSIP_BUTTON_1;
        char const* GOSSIP_BUTTON_2;
        char const* GOSSIP_BUTTON_3;

        switch (LocaleConstant currentlocale = player->GetSession()->GetSessionDbcLocale())
        {
            case LOCALE_esES:
            case LOCALE_esMX:
                GOSSIP_BUTTON_1         = " C mo funciona la galer a de tiro?";
                GOSSIP_BUTTON_2         = " Estoy listo para disparar! |cFF0000FF(Ficha de juego de la Luna Negra)|r";
                GOSSIP_BUTTON_3         = "Comprendo.";
                break;
            default:
                GOSSIP_BUTTON_1         = "How does the Shooting Gallery work?";
                GOSSIP_BUTTON_2         = "Let's shoot! |cFF0000FF(Darkmoon Game Token)|r";
                GOSSIP_BUTTON_3         = "Alright.";
                break;
        };

        player->PlayerTalkClass->ClearMenus();

        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            SendGossipMenuFor(player, 23003, creature->GetGUID());
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 2)
        {
            if (player->HasItemCount(71083))
            {
                CloseGossipMenuFor(player);

                player->DestroyItemCount(71083, 1, true);
                player->RemoveAurasByType(SPELL_AURA_MOUNTED);

                player->AddAura(101871, player);

                CAST_AI(npc_rinling::npc_rinlingAI, creature->AI())->StartGame();
                CAST_AI(npc_rinling::npc_rinlingAI, creature->AI())->Active = true;

                return true;
            }
            else
                SendGossipMenuFor(player, 54603, creature->GetGUID());
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 3)
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            if (!player->HasAura(101612) && !player->HasAura(110230) && !player->HasAura(102121) && !player->HasAura(102178) && !player->HasAura(102058) && !player->HasAura(101871))
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_BUTTON_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

            SendGossipMenuFor(player, 23002, creature->GetGUID());
        }

        return true;
    }
};

class spell_gen_repair_damaged_tonk : public SpellScriptLoader
{
    public:
        spell_gen_repair_damaged_tonk() : SpellScriptLoader("spell_gen_repair_damaged_tonk") {}

        class spell_gen_repair_damaged_tonk_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_repair_damaged_tonk_SpellScript);

            SpellCastResult CheckRequirement()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Player* player = GetCaster()->ToPlayer();
                Creature* target = GetHitCreature();

                if (!target)
                    return;

                player->KilledMonsterCredit(54504, ObjectGuid::Empty);

                Position myPos = target->GetPosition();
                target->SummonCreature(55356, myPos, TEMPSUMMON_TIMED_DESPAWN, 30000);
                target->DespawnOrUnsummon();
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_repair_damaged_tonk_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnCheckCast += SpellCheckCastFn(spell_gen_repair_damaged_tonk_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_repair_damaged_tonk_SpellScript();
        }
};

class spell_gen_shoe_baby : public SpellScriptLoader
{
    public:
        spell_gen_shoe_baby() : SpellScriptLoader("spell_gen_shoe_baby") {}

        class spell_gen_shoe_baby_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_shoe_baby_SpellScript);

            SpellCastResult CheckRequirement()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Player* player = GetCaster()->ToPlayer();
                player->KilledMonsterCredit(54510, ObjectGuid::Empty);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_shoe_baby_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnCheckCast += SpellCheckCastFn(spell_gen_shoe_baby_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_shoe_baby_SpellScript();
        }
};

class spell_cook_crunchy_fog : public SpellScriptLoader
{
    public:
        spell_cook_crunchy_fog() : SpellScriptLoader("spell_cook_crunchy_fog") {}

        class spell_cook_crunchy_fog_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_cook_crunchy_fog_SpellScript);

            SpellCastResult CheckRequirement()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Player* player = GetCaster()->ToPlayer();
                player->AddItem(72058, 1);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_cook_crunchy_fog_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnCheckCast += SpellCheckCastFn(spell_cook_crunchy_fog_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_cook_crunchy_fog_SpellScript();
        }
};

enum InjuriedCarnieYells
{
    SAY_GREETINGS_1 = 0,
    SAY_GREETINGS_2 = 1,
    SAY_GREETINGS_3 = 2,
    SAY_GREETINGS_4 = 3,
    SAY_GREETINGS_5 = 4,
};

class spell_heal_injuried_carnie : public SpellScriptLoader
{
    public:
        spell_heal_injuried_carnie() : SpellScriptLoader("spell_heal_injuried_carnie") {}

        class spell_heal_injuried_carnie_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_heal_injuried_carnie_SpellScript);

            SpellCastResult CheckRequirement()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                return SPELL_CAST_OK;
            }

            void HandleHealPct(SpellEffIndex /*effIndex*/)
            {
                Player* player = GetCaster()->ToPlayer();
                Creature* target = GetHitCreature();

                player->KilledMonsterCredit(54518, ObjectGuid::Empty);

                if (!target)
                    return;

                switch (urand(0,4))
                {
                    case 0:
                        target->AI()->Talk(SAY_GREETINGS_1);
                        target->HandleEmoteCommand(0);
                        target->GetMotionMaster()->MoveRandom(30);
                        target->DespawnOrUnsummon(8000);
                        break;
                    case 1:
                        target->AI()->Talk(SAY_GREETINGS_2);
                        target->HandleEmoteCommand(0);
                        target->GetMotionMaster()->MoveRandom(30);
                        target->DespawnOrUnsummon(8000);
                        break;
                    case 2:
                        target->AI()->Talk(SAY_GREETINGS_3);
                        target->HandleEmoteCommand(0);
                        target->GetMotionMaster()->MoveRandom(30);
                        target->DespawnOrUnsummon(8000);
                        break;
                    case 3:
                        target->AI()->Talk(SAY_GREETINGS_4);
                        target->HandleEmoteCommand(0);
                        target->GetMotionMaster()->MoveRandom(30);
                        target->DespawnOrUnsummon(8000);
                        break;
                    case 4:
                        target->AI()->Talk(SAY_GREETINGS_5);
                        target->HandleEmoteCommand(0);
                        target->GetMotionMaster()->MoveRandom(30);
                        target->DespawnOrUnsummon(8000);
                        break;
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_heal_injuried_carnie_SpellScript::HandleHealPct, EFFECT_0, SPELL_EFFECT_HEAL_PCT);
                OnCheckCast += SpellCheckCastFn(spell_heal_injuried_carnie_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_heal_injuried_carnie_SpellScript();
        }
};

class spell_put_up_darkmoon_banner : public SpellScriptLoader
{
    public:
        spell_put_up_darkmoon_banner() : SpellScriptLoader("spell_put_up_darkmoon_banner") {}

        class spell_put_up_darkmoon_banner_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_put_up_darkmoon_banner_SpellScript);

            SpellCastResult CheckRequirement()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Player* player = GetCaster()->ToPlayer();
                player->KilledMonsterCredit(54545, ObjectGuid::Empty);

                if (Creature* loosestones = GetCaster()->FindNearestCreature(54545, 10.0f, true))
                {
                    loosestones->SummonGameObject(179965, Position(loosestones->GetPositionX(), loosestones->GetPositionY(), loosestones->GetPositionZ(), loosestones->GetOrientation()), QuaternionData(), 0);
                    loosestones->DespawnOrUnsummon(30000);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_put_up_darkmoon_banner_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnCheckCast += SpellCheckCastFn(spell_put_up_darkmoon_banner_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_put_up_darkmoon_banner_SpellScript();
        }
};

class spell_darkmoon_deathmatch : public SpellScriptLoader
{
    public:
        spell_darkmoon_deathmatch() : SpellScriptLoader("spell_darkmoon_deathmatch") {}

        class spell_darkmoon_deathmatch_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_darkmoon_deathmatch_SpellScript);

            SpellCastResult CheckRequirement()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                return SPELL_CAST_OK;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();

                if (caster->GetPositionY() <= 6400.0f)
                    caster->CastSpell(caster, 108919, false);
                else
                    caster->CastSpell(caster, 108923, false);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_darkmoon_deathmatch_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
                OnCheckCast += SpellCheckCastFn(spell_darkmoon_deathmatch_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_darkmoon_deathmatch_SpellScript();
        }
};

// 101604  (wrong=101612)
class spell_whack_a_gnoll_sap : public SpellScriptLoader
{
    public:
        spell_whack_a_gnoll_sap() : SpellScriptLoader("spell_whack_a_gnoll_sap") {}

        class spell_whack_a_gnoll_sap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_whack_a_gnoll_sap_SpellScript);

            SpellCastResult CheckRequirement()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Player* player = GetCaster()->ToPlayer();

                if (Creature* gnoll = GetCaster()->FindNearestCreature(54444, 1.0f, true))
                {
                    gnoll->SetHealth(0);
                    gnoll->setDeathState(JUST_DIED);

                    player->SetPower(POWER_ALTERNATE_POWER, player->GetPower(POWER_ALTERNATE_POWER) + 1);
                    player->KilledMonsterCredit(54505, ObjectGuid::Empty);
                }

                if (Creature* baby = GetCaster()->FindNearestCreature(54466, 1.0f, true))
                {
                    baby->SetHealth(0);
                    baby->setDeathState(JUST_DIED);

                    player->CastSpell(player, 101679, false);
                }

                if (Creature* gnoll = GetCaster()->FindNearestCreature(54549, 1.0f, true))
                {
                    gnoll->SetHealth(0);
                    gnoll->setDeathState(JUST_DIED);

                    player->SetPower(POWER_ALTERNATE_POWER, player->GetPower(POWER_ALTERNATE_POWER) + 3);
                    player->KilledMonsterCredit(54505, ObjectGuid::Empty);
                    player->KilledMonsterCredit(54505, ObjectGuid::Empty);
                    player->KilledMonsterCredit(54505, ObjectGuid::Empty);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_whack_a_gnoll_sap_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnCheckCast += SpellCheckCastFn(spell_whack_a_gnoll_sap_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_whack_a_gnoll_sap_SpellScript();
        }
};

class spell_shoot_gallery_shoot : public SpellScriptLoader
{
    public:
        spell_shoot_gallery_shoot() : SpellScriptLoader("spell_shoot_gallery_shoot") {}

        class spell_shoot_gallery_shoot_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_shoot_gallery_shoot_SpellScript);

            SpellCastResult CheckRequirement()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                return SPELL_CAST_OK;
            }

            void OnHit()
            {
                Player* player = GetCaster()->ToPlayer();

                if (GetHitCreature()->GetEntry() == 54231)
                {
                    player->KilledMonsterCredit(54231, ObjectGuid::Empty);

                    AchievementEntry const *AchievShooter = sAchievementStore.LookupEntry(6022);
                    player->CompletedAchievement(AchievShooter);
                    player->SetPower(POWER_ALTERNATE_POWER, player->GetPower(POWER_ALTERNATE_POWER) + 1);
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_shoot_gallery_shoot_SpellScript::OnHit);
                OnCheckCast += SpellCheckCastFn(spell_shoot_gallery_shoot_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_shoot_gallery_shoot_SpellScript();
        }
};

// 181458 (wrong=102058)
class spell_ring_toss : public SpellScriptLoader
{
    public:
        spell_ring_toss() : SpellScriptLoader("spell_ring_toss") {}

        class spell_ring_toss_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ring_toss_SpellScript);

            SpellCastResult CheckRequirement()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                Player* player = GetCaster()->ToPlayer();

                if (Creature* dubenko = caster->FindNearestCreature(54490, 100.0f, true))
                {
                    caster->CastSpell(dubenko, 101697, false);
                    dubenko->CastSpell(dubenko, 101737, false);
                }

                player->KilledMonsterCredit(54495, ObjectGuid::Empty);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_ring_toss_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnCheckCast += SpellCheckCastFn(spell_ring_toss_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ring_toss_SpellScript();
        }
};

//
// BRUTAL HACK - Puesto hasta que se arregle el achievementcriteria, que me he peleado con  l y no consigo arreglarlo.
//

class item_darkmoon_faire_fireworks : public ItemScript
{
public:
    item_darkmoon_faire_fireworks() : ItemScript("item_darkmoon_faire_fireworks") { }

    bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const& /*targets*/, ObjectGuid /*castId*/) override
    {
        AchievementEntry const *AchievFireworksAlliance = sAchievementStore.LookupEntry(6030);
        AchievementEntry const *AchievFireworksHorde = sAchievementStore.LookupEntry(6031);

        if (player->GetTeam() == ALLIANCE)
        {
            player->CompletedAchievement(AchievFireworksAlliance);
            player->CastSpell(player, 103740, false);
        }
        else
        {
            player->CompletedAchievement(AchievFireworksHorde);
            player->CastSpell(player, 103740, false);
        }

        return true;
    }
};

void AddSC_event_darkmoon_faire()
{
    new npc_darkmoon_faire_mystic_mage();
    new npc_selina_dourman();
    new npc_mola();
    new npc_maxima_blastenheimer();
    new npc_darkmoon_cannon_trigger();
    new npc_teleportologist_fozlebub();
    new npc_finlay_coolshot();
    new npc_jessica_rogers();
    new npc_rinling();
    new spell_gen_repair_damaged_tonk();
    new spell_gen_shoe_baby();
    new spell_cook_crunchy_fog();
    new spell_heal_injuried_carnie();
    new spell_put_up_darkmoon_banner();
    new spell_darkmoon_deathmatch();
    new spell_whack_a_gnoll_sap();
    new spell_shoot_gallery_shoot();
    new spell_ring_toss();
    new item_darkmoon_faire_fireworks();
}
