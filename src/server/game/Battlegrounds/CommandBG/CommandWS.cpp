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

#include "CommandWS.h"
#include "BotAI.h"
#include "BattlegroundWS.h"

CommandWS::CommandWS(Battleground* pBG, TeamId team) :
    CommandBG(pBG, team),
    lastUpdateTick(COMMANDBG_UPDATE_TICK / 5)
{
    if (team == TeamId::TEAM_ALLIANCE)
    {
        m_AIWPEntrys.push_back(7);
        m_AIWPEntrys.push_back(8);
        m_AIWPEntrys.push_back(9);
        m_AIWPEntrys.push_back(10);

        m_AIWPEntrys.push_back(11);
        m_AIWPEntrys.push_back(12);
        m_AIWPEntrys.push_back(13);
        m_AIWPEntrys.push_back(14);
    }
    else
    {
        m_AIWPEntrys.push_back(11);
        m_AIWPEntrys.push_back(12);
        m_AIWPEntrys.push_back(13);
        m_AIWPEntrys.push_back(14);

        m_AIWPEntrys.push_back(7);
        m_AIWPEntrys.push_back(8);
        m_AIWPEntrys.push_back(9);
        m_AIWPEntrys.push_back(10);
    }
}

CommandWS::~CommandWS()
{
}

void CommandWS::Initialize()
{
    CommandBG::Initialize();
}

const Creature* CommandWS::GetMatchGraveyardNPC(const Player* player)
{
    if (!player || !player->InBattleground())
        return NULL;
    Creature* pCreature = NULL;
    if (player->GetTeamId() == TeamId::TEAM_ALLIANCE)
        pCreature = player->GetBattleground()->GetBGCreature(BG_WS_CreatureTypes::WS_SPIRIT_MAIN_ALLIANCE);
    else
        pCreature = player->GetBattleground()->GetBGCreature(BG_WS_CreatureTypes::WS_SPIRIT_MAIN_HORDE);
    return pCreature;
}

void CommandWS::Update(uint32 diff)
{
    CommandBG::Update(diff);

    if (m_pBattleground && m_pBattleground->GetStatus() == BattlegroundStatus::STATUS_IN_PROGRESS)
    {
        lastUpdateTick -= int32(diff);
        if (lastUpdateTick <= 0)
        {
            switch (CommandBG::g_CommandModelType)
            {
            case CommandModel::CM_Dispersibility:
                RndStartCommand();
                lastUpdateTick = COMMANDBG_UPDATE_TICK * 10;
                break;
            case CommandModel::CM_GroupFocus:
                ProcessGroupFocus(m_BGKeyWaypoints[AIWP_SELF_PORT2], m_BGKeyWaypoints[AIWP_ENEMY_PORT2]);
                lastUpdateTick = COMMANDBG_UPDATE_TICK;
                break;
            case CommandModel::CM_Regulation:
                ProcessRegulation();
                lastUpdateTick = COMMANDBG_UPDATE_TICK / 5;
                break;
            default:
                lastUpdateTick = COMMANDBG_UPDATE_TICK;
                break;
            }
        }
    }
}

bool CommandWS::CanUpMount(Player* player)
{
    if (EnemyBGFlagPicker() == player->GetGUID())
        return false;
    return true;
}

void CommandWS::InsureAttackAndDefance(uint32 count)
{
    uint32 index = 0;
    for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
    {
        if (index < count)
        {
            itGuid->second = true;
        }
        else
        {
            itGuid->second = false;
        }
        ++index;
    }
}

void CommandWS::RndStartCommand()
{
    for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
    {
        BotBGAI* pBotAI = GetBotBGAI(itGuid->first);
        if (!pBotAI)
            continue;
        BGCommandInfo comInfo(BGCommandType::BGCT_GuardPoint);
        comInfo.rndOffset = 5.0f;
        uint32 rnd = irand(0, 3);
        switch (rnd)
        {
        case 0:
            comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_SELF_PORT2];
            break;
        case 1:
            comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_SELF_PORT3];
            break;
        case 2:
            comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_ENEMY_PORT2];
            break;
        case 3:
            comInfo.targetAIWP = m_BGKeyWaypoints[AIWP_ENEMY_PORT3];
            break;
        }
        pBotAI->GetAIMovement()->AcceptCommand(comInfo.targetAIWP);
    }
}

void CommandWS::ProcessRegulation()
{
    InsureAttackAndDefance(7);
    ObjectGuid needGuaredTarget = EnemyBGFlagPicker();
    ObjectGuid needAttackTarget = SelfBGFlagPicker();
    bool pickedFlagTarget = (needAttackTarget.IsEmpty()) ? true : false;

    if (pickedFlagTarget)
    {
        if (needGuaredTarget.IsEmpty())
        {
            ProcessAllPicked();
        }
        else
        {
            ProcessAllGuared(needGuaredTarget);
        }
    }
    else
    {
        if (needGuaredTarget.IsEmpty())
        {
            ProcessAllAttack(needAttackTarget);
        }
        else
        {
            ProcessAttackAndGuard(needAttackTarget, needGuaredTarget);
        }
    }
}

void CommandWS::TryPickEnemyFlag(uint64 guid)
{
    if (!m_pBattleground)
        return;
    BattlegroundWS* pBGWS = dynamic_cast<BattlegroundWS*>(m_pBattleground);
    if (!pBGWS)
        return;
    uint8 flagState = pBGWS->GetFlagState((m_TeamID == TEAM_ALLIANCE) ? HORDE : ALLIANCE);
    if (flagState == BG_WS_FLAG_STATE_ON_PLAYER || flagState == BG_WS_FLAG_STATE_WAIT_RESPAWN)
        return;
    Player* player = GetBGPlayer(guid);
    if (!player)
        return;
    GameObject* pObject = NULL;
    if (flagState == BG_WS_FLAG_STATE_ON_BASE)
    {
        pObject = pBGWS->GetBGObject((m_TeamID == TEAM_ALLIANCE) ? BG_WS_ObjectTypes::BG_WS_OBJECT_H_FLAG : BG_WS_ObjectTypes::BG_WS_OBJECT_A_FLAG);
        if (pObject && !pObject->isSpawned())
            return;
    }
    else
    {
        if (m_TeamID == TEAM_ALLIANCE)
            pObject = SearchDropedFlag(guid, TeamId::TEAM_HORDE);
        else
            pObject = SearchDropedFlag(guid, TeamId::TEAM_ALLIANCE);
    }
    if (!pObject)
        return;
    if (player->GetDistance(pObject->GetPosition()) > 5)
        return;
    if (BotBGAI* pAI = GetBotBGAI(guid))
    {
        pAI->Dismount();
        if (!pAI->CanUseBGObject())
            return;
    }
    pObject->Use(player);
}

void CommandWS::TryPickSelfFlag(uint64 guid)
{
    if (!m_pBattleground)
        return;
    BattlegroundWS* pBGWS = dynamic_cast<BattlegroundWS*>(m_pBattleground);
    if (!pBGWS)
        return;
    uint8 flagState = pBGWS->GetFlagState((m_TeamID == TEAM_ALLIANCE) ? ALLIANCE : HORDE);
    if (flagState != BG_WS_FLAG_STATE_ON_GROUND)
        return;
    Player* player = GetBGPlayer(guid);
    if (!player)
        return;
    GameObject* pObject = NULL;// pBGWS->GetBGObject((m_TeamID == TEAM_ALLIANCE) ? BG_WS_ObjectTypes::BG_WS_OBJECT_A_FLAG : BG_WS_ObjectTypes::BG_WS_OBJECT_H_FLAG);
    if (m_TeamID == TEAM_ALLIANCE)
        pObject = SearchDropedFlag(guid, TeamId::TEAM_ALLIANCE);
    else
        pObject = SearchDropedFlag(guid, TeamId::TEAM_HORDE);
    if (!pObject)
        return;
    if (player->GetDistance(pObject->GetPosition()) > 5)
        return;
    if (BotBGAI* pAI = GetBotBGAI(guid))
    {
        pAI->Dismount();
    }
    pObject->Use(player);
}

void CommandWS::TryCaptureFlag(uint64 guid)
{
    if (!m_pBattleground)
        return;
    BattlegroundWS* pBGWS = dynamic_cast<BattlegroundWS*>(m_pBattleground);
    if (!pBGWS)
        return;
    if (pBGWS->GetFlagState((m_TeamID == TEAM_ALLIANCE) ? HORDE : ALLIANCE) != BG_WS_FLAG_STATE_ON_PLAYER)
        return;
    if (pBGWS->GetFlagState((m_TeamID == TEAM_ALLIANCE) ? ALLIANCE : HORDE) != BG_WS_FLAG_STATE_ON_BASE)
        return;
    Player* player = GetBGPlayer(guid);
    if (!player)
        return;
    GameObject* pObject = pBGWS->GetBGObject((m_TeamID == TEAM_ALLIANCE) ? BG_WS_ObjectTypes::BG_WS_OBJECT_A_FLAG : BG_WS_ObjectTypes::BG_WS_OBJECT_H_FLAG);
    if (!pObject)
        return;
    if (player->GetDistance(pObject->GetPosition()) > 4)
        return;
    if (BotBGAI* pAI = GetBotBGAI(guid))
    {
        pAI->Dismount();
    }
    pBGWS->HandleAreaTrigger(player, (m_TeamID == TEAM_ALLIANCE) ? 3646 : 3647, true);
}

void CommandWS::ProcessAllPicked() // ??????????????
{
    ObjectGuid defanceTarget = GetBGFlagFromSelf();
    AIWaypoint* defancePoint = (defanceTarget.IsEmpty()) ? m_BGKeyWaypoints[AIWP_SELF_FLAG] : NULL;
    Position defanceTargetPos;
    if (defancePoint)
    {
        defanceTargetPos.m_positionX = defancePoint->posX;
        defanceTargetPos.m_positionY = defancePoint->posY;
        defanceTargetPos.m_positionZ = defancePoint->posZ;
    }
    //else
    //{
    //    defanceTargetPos = GetPositionByGuid(defanceTarget.GetRawValue());
    //}

    ObjectGuid attackTarget = GetBGFlagFromEnemy();
    AIWaypoint* attackPoint = (attackTarget.IsEmpty()) ? m_BGKeyWaypoints[AIWP_ENEMY_FLAG] : NULL;
    Position attackTargetPos;
    if (attackPoint)
    {
        attackTargetPos.m_positionX = attackPoint->posX;
        attackTargetPos.m_positionY = attackPoint->posY;
        attackTargetPos.m_positionZ = attackPoint->posZ;
    }
    //else
    //{
    //    attackTargetPos = GetPositionByGuid(attackTarget.GetRawValue());
    //}

    PlayerGUIDs commandedPlayers;
    if (!attackTarget.IsEmpty())
    {
        for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
        {
            GameObject* pFlagObject = SearchDropedFlag(itGuid->first, (m_TeamID == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE);
            if (pFlagObject)
            {
                BotBGAI* pBotAI = GetBotBGAI(itGuid->first);
                if (!pBotAI)
                    continue;
                pBotAI->GetAIMovement()->AcceptCommand(pFlagObject->GetGUID());
                TryPickEnemyFlag(itGuid->first);
                commandedPlayers.insert(itGuid->first);
            }
        }
    }
    if (!defanceTarget.IsEmpty())
    {
        for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
        {
            if (commandedPlayers.find(itGuid->first) != commandedPlayers.end())
                continue;
            GameObject* pFlagObject = SearchDropedFlag(itGuid->first, (m_TeamID == TEAM_ALLIANCE) ? TEAM_ALLIANCE : TEAM_HORDE);
            if (pFlagObject)
            {
                BotBGAI* pBotAI = GetBotBGAI(itGuid->first);
                if (!pBotAI)
                    continue;
                pBotAI->GetAIMovement()->AcceptCommand(pFlagObject->GetGUID());
                TryPickSelfFlag(itGuid->first);
                commandedPlayers.insert(itGuid->first);
            }
        }
    }
    for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
    {
        if (commandedPlayers.find(itGuid->first) != commandedPlayers.end())
            continue;
        BotBGAI* pBotAI = GetBotBGAI(itGuid->first);
        if (!pBotAI)
            continue;
        pBotAI->SetNeedFindpathSearch();
        if (itGuid->second)
        {
            pBotAI->GetAIMovement()->AcceptCommand(m_BGKeyWaypoints[AIWP_ENEMY_FLAG]);
            TryPickEnemyFlag(itGuid->first);
        }
        else
        {
            pBotAI->GetAIMovement()->AcceptCommand(m_BGKeyWaypoints[AIWP_SELF_FLAG]);
        }
    }
}

void CommandWS::ProcessAllGuared(ObjectGuid guaredGuid) // ????????,?????????
{
    ObjectGuid defanceTarget = GetBGFlagFromSelf();
    AIWaypoint* defancePoint = (defanceTarget.IsEmpty()) ? m_BGKeyWaypoints[AIWP_SELF_FLAG] : NULL;
    Position defanceTargetPos;
    if (defancePoint)
    {
        defanceTargetPos.m_positionX = defancePoint->posX;
        defanceTargetPos.m_positionY = defancePoint->posY;
        defanceTargetPos.m_positionZ = defancePoint->posZ;
    }
    //else
    //{
    //    defanceTargetPos = GetPositionByGuid(defanceTarget.GetRawValue());
    //}

    PlayerGUIDs commandedPlayers;
    if (!defanceTarget.IsEmpty())
    {
        for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
        {
            GameObject* pFlagObject = SearchDropedFlag(itGuid->first, (m_TeamID == TEAM_ALLIANCE) ? TEAM_ALLIANCE : TEAM_HORDE);
            if (pFlagObject)
            {
                BotBGAI* pBotAI = GetBotBGAI(itGuid->first);
                if (!pBotAI)
                    continue;
                pBotAI->GetAIMovement()->AcceptCommand(pFlagObject->GetGUID());
                TryPickSelfFlag(itGuid->first);
                commandedPlayers.insert(itGuid->first);
            }
        }
    }
    for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
    {
        if (commandedPlayers.find(itGuid->first) != commandedPlayers.end())
            continue;
        BotBGAI* pBotAI = GetBotBGAI(itGuid->first);
        if (!pBotAI)
            continue;
        pBotAI->SetNeedFindpathSearch();
        if (guaredGuid == itGuid->first)
        {
            AIWaypoint* pWaypoint = m_BGKeyWaypoints[AIWP_SELF_FLAG];
            pBotAI->GetAIMovement()->AcceptCommand(pWaypoint, true);
            if (defanceTarget.IsEmpty())
            {
                TryCaptureFlag(itGuid->first);
            }
            continue;
        }
        if (defancePoint && !itGuid->second)
            pBotAI->GetAIMovement()->AcceptCommand(defancePoint);
        else
            pBotAI->GetAIMovement()->AcceptCommand(guaredGuid);
    }
}

void CommandWS::ProcessAllAttack(ObjectGuid& attackGuid) // ??????,?????????
{
    ObjectGuid attackTarget = GetBGFlagFromEnemy();
    AIWaypoint* attackPoint = (attackTarget.IsEmpty()) ? m_BGKeyWaypoints[AIWP_ENEMY_FLAG] : NULL;
    Position attackTargetPos;
    if (attackPoint)
    {
        attackTargetPos.m_positionX = attackPoint->posX;
        attackTargetPos.m_positionY = attackPoint->posY;
        attackTargetPos.m_positionZ = attackPoint->posZ;
    }
    //else
    //{
    //    attackTargetPos = GetPositionByGuid(attackTarget.GetRawValue());
    //}

    PlayerGUIDs commandedPlayers;
    if (!attackTarget.IsEmpty())
    {
        for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
        {
            GameObject* pFlagObject = SearchDropedFlag(itGuid->first, (m_TeamID == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE);
            if (pFlagObject)
            {
                BotBGAI* pBotAI = GetBotBGAI(itGuid->first);
                if (!pBotAI)
                    continue;
                pBotAI->GetAIMovement()->AcceptCommand(pFlagObject->GetGUID());
                TryPickEnemyFlag(itGuid->first);
                commandedPlayers.insert(itGuid->first);
            }
        }
    }
    for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
    {
        if (commandedPlayers.find(itGuid->first) != commandedPlayers.end())
            continue;
        BotBGAI* pBotAI = GetBotBGAI(itGuid->first);
        if (!pBotAI)
            continue;
        pBotAI->SetNeedFindpathSearch();
        Position playerPos = GetPositionByGuid(itGuid->first).GetPosition();
        /*if (playerPos.IsInDist(&GetPositionByGuid(attackGuid), COMMAND_POINT_IFDISTANCE))
            pBotAI->GetAIMovement()->AcceptCommand(attackGuid);
        else
        {
            if (attackPoint)
            {
                pBotAI->GetAIMovement()->AcceptCommand(attackPoint);
            }
            else
            {
                pBotAI->GetAIMovement()->AcceptCommand(attackTarget);
            }
            TryPickEnemyFlag(itGuid->first);
        }*/
    }
}

void CommandWS::ProcessAttackAndGuard(ObjectGuid attackGuid, ObjectGuid guaredGuid) // ???????
{
    for (PlayerStatus::iterator itGuid = m_PlayerGUIDs.begin(); itGuid != m_PlayerGUIDs.end(); itGuid++)
    {
        BotBGAI* pBotAI = GetBotBGAI(itGuid->first);
        if (!pBotAI)
            continue;
        pBotAI->SetNeedFindpathSearch();
        if (guaredGuid == itGuid->first)
        {
            AIWaypoint* pWaypoint = m_BGKeyWaypoints[AIWP_SELF_FLAG];
            pBotAI->GetAIMovement()->AcceptCommand(pWaypoint);
            continue;
        }
        /*Position playerPos = GetPositionByGuid(itGuid->first);
        if (playerPos.IsInDist(GetPositionByGuid(attackGuid), COMMAND_POINT_IFDISTANCE))
        {
            pBotAI->GetAIMovement()->AcceptCommand(attackGuid);
        }
        else
        {
            if (itGuid->second)
                pBotAI->GetAIMovement()->AcceptCommand(attackGuid);
            else
                pBotAI->GetAIMovement()->AcceptCommand(guaredGuid);
        }*/
    }
}

ObjectGuid CommandWS::SelfBGFlagPicker()
{
    if (!m_pBattleground)
        return ObjectGuid::Empty;
    BattlegroundWS* pBGWS = dynamic_cast<BattlegroundWS*>(m_pBattleground);
    if (!pBGWS)
        return ObjectGuid::Empty;
    if (m_TeamID == TEAM_ALLIANCE)
    {
        if (pBGWS->GetFlagState(ALLIANCE) == BG_WS_FLAG_STATE_ON_PLAYER)
        {
            return pBGWS->GetFlagPickerGUID(TEAM_ALLIANCE);
        }
    }
    else if (m_TeamID == TEAM_HORDE)
    {
        if (pBGWS->GetFlagState(HORDE) == BG_WS_FLAG_STATE_ON_PLAYER)
        {
            return pBGWS->GetFlagPickerGUID(TEAM_HORDE);
        }
    }
    return ObjectGuid::Empty;
}

ObjectGuid CommandWS::EnemyBGFlagPicker()
{
    if (!m_pBattleground)
        return ObjectGuid::Empty;
    BattlegroundWS* pBGWS = dynamic_cast<BattlegroundWS*>(m_pBattleground);
    if (!pBGWS)
        return ObjectGuid::Empty;
    if (m_TeamID == TEAM_ALLIANCE)
    {
        if (pBGWS->GetFlagState(HORDE) == BG_WS_FLAG_STATE_ON_PLAYER)
        {
            return pBGWS->GetFlagPickerGUID(TEAM_HORDE);
        }
    }
    else if (m_TeamID == TEAM_HORDE)
    {
        if (pBGWS->GetFlagState(ALLIANCE) == BG_WS_FLAG_STATE_ON_PLAYER)
        {
            return pBGWS->GetFlagPickerGUID(TEAM_ALLIANCE);
        }
    }
    return ObjectGuid::Empty;
}

ObjectGuid CommandWS::GetBGFlagFromSelf()
{
    if (!m_pBattleground)
        return ObjectGuid::Empty;
    BattlegroundWS* pBGWS = dynamic_cast<BattlegroundWS*>(m_pBattleground);
    if (!pBGWS)
        return ObjectGuid::Empty;
    if (pBGWS->GetFlagState((m_TeamID == TEAM_ALLIANCE) ? ALLIANCE : HORDE) == BG_WS_FLAG_STATE_ON_GROUND)
    {
        GameObject* pObject = m_pBattleground->GetBGObject((m_TeamID == TEAM_ALLIANCE) ? BG_WS_ObjectTypes::BG_WS_OBJECT_A_FLAG : BG_WS_ObjectTypes::BG_WS_OBJECT_H_FLAG);
        if (!pObject)
            return ObjectGuid::Empty;
        return pObject->GetGUID();
    }
    return ObjectGuid::Empty;
}

ObjectGuid CommandWS::GetBGFlagFromEnemy()
{
    if (!m_pBattleground)
        return ObjectGuid::Empty;
    BattlegroundWS* pBGWS = dynamic_cast<BattlegroundWS*>(m_pBattleground);
    if (!pBGWS)
        return ObjectGuid::Empty;
    if (pBGWS->GetFlagState((m_TeamID == TEAM_ALLIANCE) ? HORDE : ALLIANCE) == BG_WS_FLAG_STATE_ON_GROUND)
    {
        GameObject* pObject = m_pBattleground->GetBGObject((m_TeamID == TEAM_ALLIANCE) ? BG_WS_ObjectTypes::BG_WS_OBJECT_H_FLAG : BG_WS_ObjectTypes::BG_WS_OBJECT_A_FLAG);
        if (!pObject)
            return ObjectGuid::Empty;
        return pObject->GetGUID();
    }
    return ObjectGuid::Empty;
}

GameObject* CommandWS::SearchDropedFlag(uint64 guid, TeamId team)
{
    BotBGAI* pBotAI = GetBotBGAI(guid);
    if (!pBotAI)
        return NULL;
    uint32 goInfoEntry = (team == TEAM_ALLIANCE) ? BG_WS_ObjectEntry::BG_OBJECT_A_FLAG_GROUND_WS_ENTRY : BG_WS_ObjectEntry::BG_OBJECT_H_FLAG_GROUND_WS_ENTRY;
    NearObjectList objectList = pBotAI->SearchGameObject(COMMAND_POINT_IFDISTANCE / 2);
    for (GameObject* pObject : objectList)
    {
        if (pObject->GetGOInfo()->entry == goInfoEntry)
            return pObject;
    }
    return NULL;
}
