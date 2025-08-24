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

#ifndef TRINITY_ARENA_H
#define TRINITY_ARENA_H

#include "ArenaScore.h"
#include "Battleground.h"

enum ArenaBroadcastTexts
{
    ARENA_TEXT_START_ONE_MINUTE             = 15740,
    ARENA_TEXT_START_THIRTY_SECONDS         = 15741,
    ARENA_TEXT_START_FIFTEEN_SECONDS        = 15739,
    ARENA_TEXT_START_BATTLE_HAS_BEGUN       = 15742,
};

enum ArenaSpellIds
{
    SPELL_ALLIANCE_GOLD_FLAG                = 32724,
    SPELL_ALLIANCE_GREEN_FLAG               = 32725,
    SPELL_HORDE_GOLD_FLAG                   = 35774,
    SPELL_HORDE_GREEN_FLAG                  = 35775,

    SPELL_LAST_MAN_STANDING                 = 26549  // Achievement Credit
};

enum ArenaWorldStates
{
    ARENA_WORLD_STATE_ALIVE_PLAYERS_GREEN   = 3600,
    ARENA_WORLD_STATE_ALIVE_PLAYERS_GOLD    = 3601
};

class TC_GAME_API Arena : public Battleground
{
    protected:
        Arena();

        void AddPlayer(Player* player) override;
        void RemovePlayer(Player* player, ObjectGuid /*guid*/, uint32 /*team*/) override;

        void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet) override;
        void UpdateArenaWorldState();

        void HandleKillPlayer(Player* player, Player* killer) override;

        void BuildPvPLogDataPacket(WorldPackets::Battleground::PVPLogData& pvpLogData) const override;

        using PLAYERS = std::list<Player*>;
        void CommandCombat(TeamId commandTeam, PLAYERS& alliances, PLAYERS& hordes);
        bool NeedHarassHealer(PLAYERS& selfPlayer, PLAYERS& enemyPlayer);
        bool HasFullSuppressSpell(PLAYERS& selfPlayer);
        bool CanFullSuppressPlayer(PLAYERS& enemyPlayers);
        bool SuppressRealPlayer(PLAYERS& selfPlayer, PLAYERS& enemyPlayer);
        bool SuppressHealerPlayer(PLAYERS& selfPlayer, PLAYERS& enemyPlayer);
        bool SuppressMightinessPlayer(PLAYERS& selfPlayer, PLAYERS& enemyPlayer);
        void FollowEnemyHealer(PLAYERS& selfPlayer, PLAYERS& enemyPlayer);
        PLAYERS::iterator GetListHealer(PLAYERS& players);
        PLAYERS::iterator GetListMeleer(PLAYERS& players);
        PLAYERS::iterator GetListRanger(PLAYERS& players);
        bool HasAuraMechanic(Unit* pTarget, Mechanics mask);
        bool CanSelectTarget(Player* pTarget);
        bool MeleeTargetIsSuppress(Player* pTarget);
        bool TargetIsOverSuppress(Player* pTarget);
        bool IsHealerBot(Player* pTarget);
        bool IsMeleeBot(Player* pTarget);
        bool IsAllMeleeBot(Player* pTarget);
        bool IsRangeBot(Player* pTarget);
        bool IsMightiness(Player* pTarget);
        bool CanFullHealerEnemy(PLAYERS& players);
        bool CanFullHealerEnemy2(PLAYERS& players);
        bool ExistCtrlSpellCasting(PLAYERS& players);
        bool ExistRealPlayerByRange(PLAYERS& players);
        bool ExistControler(PLAYERS& players);
        bool ExistMightinessClasses(PLAYERS& players);
        bool ExistMightinessHealer(PLAYERS& players);
        Player* FindPriorAttackTarget(PLAYERS& players);
        void NormalTactics(PLAYERS& comLists, PLAYERS& canSelectEnemys);

        void RemovePlayerAtLeave(ObjectGuid guid, bool transport, bool sendPacket) override;
        void CheckWinConditions() override;
        void EndBattleground(uint32 winner) override;
        void InsureArenaAllPlayerFlag();
        void InsureArenaFlag(Player* player, bool init = false);
        bool AssignTactics(PLAYERS& comLists, PLAYERS& enemyLists);
        void Update(uint32 diff) override;

private:
    uint32 m_UpdateTick;
    bool m_StartTryMount;
    TeamId m_LastCommandTeam;

        ArenaGroupScore _ArenaGroupScores[BG_TEAMS_COUNT];
};

#endif // TRINITY_ARENA_H
