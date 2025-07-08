/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "Common.h"
#include "Language.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "UpdateMask.h"
#include "NPCHandler.h"
#include "Pet.h"
#include "MapManager.h"
#include "GossipDef.h"

void WorldSession::SendNameQueryOpcode(uint64 guid)
{
    Player* player = ObjectAccessor::FindPlayer(guid);
    CharacterNameData const* nameData = sWorld->GetCharacterNameData(GUID_LOPART(guid));

    WorldPacket data(SMSG_NAME_QUERY_RESPONSE);
    data.WriteGuidMask<3, 2, 6, 0, 4, 1, 5, 7>(guid);

    data.WriteGuidBytes<7, 1, 2, 6, 3, 5>(guid);
    data << uint8(nameData ? 0 : 1);
    if (nameData)
    {
        data << uint8(nameData->m_gender);
        data << uint8(nameData->m_class);
        data << uint8(nameData->m_level);
        data << uint32(realmID);
        data << uint8(nameData->m_race);
        data << uint32(0);
    }
    data.WriteGuidBytes<4, 0>(guid);

    ObjectGuid guid28 = 0;
    ObjectGuid guid30 = guid;

    if (nameData)
    {
        data.WriteGuidMask<5>(guid30);
        data.WriteGuidMask<1, 6, 2>(guid28);
        data.WriteGuidMask<3, 7, 0, 6>(guid30);
        data.WriteGuidMask<0, 7>(guid28);
        data.WriteGuidMask<1>(guid30);
        data.WriteGuidMask<5>(guid28);

        DeclinedName const* names = player ? player->GetDeclinedNames() : NULL;
        for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            data.WriteBits(names ? names->name[i].length() : 0, 7);

        data.WriteGuidMask<2>(guid30);
        data.WriteBit(0);       // byte20
        data.WriteGuidMask<4>(guid30);
        data.WriteGuidMask<4>(guid28);
        data.WriteBits(nameData->m_name.size(), 6);
        data.WriteGuidMask<3>(guid28);

        for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            data.WriteString(names ? names->name[i] : "");

        data.WriteGuidBytes<3>(guid28);
        data.WriteGuidBytes<0>(guid30);
        data.WriteGuidBytes<5, 1>(guid28);
        data.WriteGuidBytes<7, 5>(guid30);
        data.WriteGuidBytes<3, 6>(guid28);
        data.WriteGuidBytes<2>(guid30);
        data.WriteGuidBytes<2>(guid28);
        data.WriteString(nameData->m_name);
        data.WriteGuidBytes<4, 1>(guid30);
        data.WriteGuidBytes<0>(guid28);
        data.WriteGuidBytes<3, 6>(guid30);
        data.WriteGuidBytes<7>(guid28);
    }

    SendPacket(&data);
}

void WorldSession::HandleNameQueryOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;

    recvData.ReadGuidMask<1, 3, 6, 7, 2, 5>(guid);
    bool bit14 = recvData.ReadBit();
    recvData.ReadGuidMask<0>(guid);
    bool bit1C = recvData.ReadBit();
    recvData.ReadGuidMask<4>(guid);

    recvData.ReadGuidBytes<4, 6, 7, 1, 2, 5, 0, 3>(guid);
    if (bit14)
        recvData.read_skip<uint32>();   // realm id 1
    if (bit1C)
        recvData.read_skip<uint32>();   // realm id 2

    // This is disable by default to prevent lots of console spam
    // TC_LOG_INFO("network", "HandleNameQueryOpcode %u", guid);

    SendNameQueryOpcode(guid);
}

void WorldSession::SendRealmNameQueryOpcode(uint32 realmId)
{
    RealmNameMap::const_iterator iter = realmNameStore.find(realmId);

    bool found = iter != realmNameStore.end();
    std::string realmName = found ? iter->second : "";

    WorldPacket data(SMSG_REALM_NAME_QUERY_RESPONSE);
    data << uint8(!found);
    data << uint32(realmId);

    if (found)
    {
        data.WriteBits(realmName.length(), 8);
        data.WriteBit(realmId == GetVirtualRealmID());
        data.WriteBits(realmName.length(), 8);
        data.FlushBits();

        data.WriteString(realmName);
        data.WriteString(realmName);
    }

    SendPacket(&data);
}

void WorldSession::HandleRealmNameQueryOpcode(WorldPacket& recvPacket)
{
    uint32 realmId;
    recvPacket >> realmId;
    SendRealmNameQueryOpcode(realmId);
}


void WorldSession::HandleQueryTimeOpcode(WorldPacket & /*recvData*/)
{
    SendQueryTimeResponse();
}

void WorldSession::SendQueryTimeResponse()
{
    WorldPacket data(SMSG_QUERY_TIME_RESPONSE, 4+4);
    data << uint32(sWorld->GetNextDailyQuestsResetTime() - time(NULL));
    data << uint32(time(NULL));
    SendPacket(&data);
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandleCreatureQueryOpcode(WorldPacket & recvData)
{
    uint32 entry;
    recvData >> entry;

    CreatureTemplate const* ci = sObjectMgr->GetCreatureTemplate(entry);
    if (ci)
    {
        std::string Name, SubName, Unk505;
        Name = ci->Name;
        SubName = ci->SubName;
        Unk505 = "";

        int loc_idx = GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            if (CreatureLocale const* cl = sObjectMgr->GetCreatureLocale(entry))
            {
                ObjectMgr::GetLocaleString(cl->Name, loc_idx, Name);
                ObjectMgr::GetLocaleString(cl->SubName, loc_idx, SubName);
            }
        }
        TC_LOG_DEBUG("network", "WORLD: CMSG_CREATURE_QUERY '%s' - Entry: %u.", ci->Name.c_str(), entry);
                                                            // guess size
        WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 100);
        data << uint32(entry);                              // creature entry
        data.WriteBit(1);

        data.WriteBits(Unk505.size() ? Unk505.size() + 1 : 0, 11);
        uint32 itemsCount = 0;
        for (uint32 i = 0; i < MAX_CREATURE_QUEST_ITEMS; ++i)
            if (ci->questItems[i])
                ++itemsCount;
        data.WriteBits(itemsCount, 22);
        data.WriteBits(ci->IconName.size() ? ci->IconName.size() + 1 : 0, 6);
        data.WriteBit(ci->RacialLeader);

        data.WriteBits(Name.size() ? Name.size() + 1 : 0, 11);
        for (int i = 0; i < 7; ++i)
            data.WriteBits(0, 11);
        data.WriteBits(SubName.size() ? SubName.size() + 1 : 0, 11);

        data << uint32(ci->family);                         // CreatureFamily.dbc
        data << uint32(ci->expansionUnknown);               // unknown meaning
        if (Unk505.size())
            data << Unk505;
        data << uint32(ci->type);                           // CreatureType.dbc
        if (SubName.size())
            data << SubName;
        data << uint32(ci->Modelid1);                       // Modelid1
        data << uint32(ci->Modelid4);                       // Modelid4
        for (uint32 i = 0; i < MAX_CREATURE_QUEST_ITEMS; ++i)
            if (ci->questItems[i])
                data << uint32(ci->questItems[i]);          // itemId[6], quest drop
        if (Name.size())
            data << Name;
        if (ci->IconName.size())
            data << ci->IconName;
        data << uint32(ci->type_flags);                     // flags
        data << uint32(ci->type_flags2);                    // unknown meaning
        data << float(ci->ModHealth);                       // dmg/hp modifier
        data << uint32(ci->rank);                           // Creature Rank (elite, boss, etc)
        data << uint32(ci->KillCredit[0]);                  // new in 3.1, kill credit
        data << uint32(ci->KillCredit[1]);                  // new in 3.1, kill credit
        data << float(ci->ModMana);                         // dmg/mana modifier
        data << uint32(ci->movementId);                     // CreatureMovementInfo.dbc
        data << uint32(ci->Modelid3);                       // Modelid3
        data << uint32(ci->Modelid2);                       // Modelid2

        SendPacket(&data);
        TC_LOG_DEBUG("network", "WORLD: Sent SMSG_CREATURE_QUERY_RESPONSE");
    }
    else
    {
        TC_LOG_DEBUG("network", "WORLD: CMSG_CREATURE_QUERY - NO CREATURE INFO! (ENTRY: %u)",
            entry);
        WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 4);
        data << uint32(entry | 0x80000000);                 // is there that mask?
        data.WriteBit(0);
        SendPacket(&data);
        TC_LOG_DEBUG("network", "WORLD: Sent SMSG_CREATURE_QUERY_RESPONSE");
    }
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandleGameObjectQueryOpcode(WorldPacket & recvData)
{
    uint32 entry;
    recvData >> entry;
    ObjectGuid guid;
    recvData.ReadGuidMask<6, 3, 1, 2, 0, 7, 5, 4>(guid);
    recvData.ReadGuidBytes<5, 0, 6, 7, 3, 4, 2, 1>(guid);

    const GameObjectTemplate* info = sObjectMgr->GetGameObjectTemplate(entry);
    if (info)
    {
        std::string Name;
        std::string IconName;
        std::string CastBarCaption;

        Name = info->name;
        IconName = info->IconName;
        CastBarCaption = info->castBarCaption;

        int loc_idx = GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            if (GameObjectLocale const* gl = sObjectMgr->GetGameObjectLocale(entry))
            {
                ObjectMgr::GetLocaleString(gl->Name, loc_idx, Name);
                ObjectMgr::GetLocaleString(gl->CastBarCaption, loc_idx, CastBarCaption);
            }
        }
        TC_LOG_DEBUG("network", "WORLD: CMSG_GAMEOBJECT_QUERY '%s' - Entry: %u. ", info->name.c_str(), entry);

        ByteBuffer buff;
        WorldPacket data (SMSG_GAMEOBJECT_QUERY_RESPONSE, 150);
        data << uint32(entry);

        buff << uint32(info->type);
        buff << uint32(info->displayId);
        buff << Name;
        buff << uint8(0) << uint8(0) << uint8(0);           // name2, name3, name4
        buff << IconName;                                   // 2.0.3, string. Icon name to use instead of default icon for go's (ex: "Attack" makes sword)
        buff << CastBarCaption;                             // 2.0.3, string. Text will appear in Cast Bar when using GO (ex: "Collecting")
        buff << info->unk1;

        buff.append<uint32>(info->raw.data, MAX_GAMEOBJECT_DATA);
        buff << float(info->size);                          // go size
        uint8 itemsCount = 0;
        for (uint32 i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS; ++i)
            if (info->questItems[i])
                ++itemsCount;
        buff << uint8(itemsCount);
        for (uint32 i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS; ++i)
            if (info->questItems[i])
                buff << uint32(info->questItems[i]);        // itemId[6], quest drop
        buff << int32(info->unkInt32);                      // 4.x, unknown

        data << uint32(buff.size());
        data.append(buff);
        data.WriteBit(1);

        SendPacket(&data);
        TC_LOG_DEBUG("network", "WORLD: Sent SMSG_GAMEOBJECT_QUERY_RESPONSE");
    }
    else
    {
        TC_LOG_DEBUG("network", "WORLD: CMSG_GAMEOBJECT_QUERY - Missing gameobject info for (GUID: %u, ENTRY: %u)",
            GUID_LOPART(guid), entry);

        WorldPacket data (SMSG_GAMEOBJECT_QUERY_RESPONSE, 4 + 4 + 1);
        data << uint32(entry);
        data << uint32(0);
        data.WriteBit(0);
        SendPacket(&data);

        TC_LOG_DEBUG("network", "WORLD: Sent SMSG_GAMEOBJECT_QUERY_RESPONSE");
    }
}

void WorldSession::HandleCorpseQueryOpcode(WorldPacket& /*recvData*/)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_CORPSE_QUERY");

    Corpse* corpse = GetPlayer()->GetCorpse();

    if (!corpse)
    {
        //! 5.4.1
        WorldPacket data(SMSG_CORPSE_QUERY);
        data << uint8(0);
        data << uint8(0);   // corpse not found
        data << float(0);
        data << float(0);
        data << int32(0);
        data << float(0);
        data << int32(0);
        SendPacket(&data);
        return;
    }

    uint32 mapid = corpse->GetMapId();
    float x = corpse->GetPositionX();
    float y = corpse->GetPositionY();
    float z = corpse->GetPositionZ();
    uint32 corpsemapid = mapid;

    // if corpse at different map
    if (mapid != _player->GetMapId())
    {
        // search entrance map for proper show entrance
        if (MapEntry const* corpseMapEntry = sMapStore.LookupEntry(mapid))
        {
            if (corpseMapEntry->IsDungeon() && corpseMapEntry->entrance_map >= 0)
            {
                // if corpse map have entrance
                if (Map const* entranceMap = sMapMgr->CreateBaseMap(corpseMapEntry->entrance_map))
                {
                    mapid = corpseMapEntry->entrance_map;
                    x = corpseMapEntry->entrance_x;
                    y = corpseMapEntry->entrance_y;
                    z = entranceMap->GetHeight(GetPlayer()->GetPhaseMask(), x, y, MAX_HEIGHT);
                }
            }
        }
    }
    ObjectGuid guid = corpse->GetGUID();

    //! 5.4.1
    WorldPacket data(SMSG_CORPSE_QUERY);
    data.WriteGuidMask<1, 0, 3, 7, 4, 6, 5, 2>(guid);
    data.WriteBit(1);               // corpse found

    data.FlushBits();

    data << float(y);
    data.WriteGuidBytes<6>(guid);
    data << float(x);
    data << int32(mapid);
    data.WriteGuidBytes<0, 7, 2, 4>(guid);
    data << float(z);
    data.WriteGuidBytes<1, 5, 3>(guid);
    data << int32(corpsemapid);

    SendPacket(&data);
}

void WorldSession::HandleNpcTextQueryOpcode(WorldPacket & recvData)
{
    uint32 textID;
    ObjectGuid guid;

    recvData >> textID;
    TC_LOG_DEBUG("network", "WORLD: CMSG_NPC_TEXT_QUERY ID '%u'", textID);

    recvData.ReadGuidMask<4, 7, 2, 5, 3, 0, 1, 6>(guid);
    recvData.ReadGuidBytes<6, 4, 1, 3, 2, 5, 7, 0>(guid);
    GetPlayer()->SetSelection(guid);

    GossipText const* pGossip = sObjectMgr->GetGossipText(textID);

    ByteBuffer buf;
    for (int i = 0; i < 8; ++i)
        if (i == 0)
            buf << uint32(0x3F800000);
        else
            buf << uint32(0);
    for (int i = 0; i < 8; ++i)
        if (i == 0)
            buf << uint32(textID);
        else
            buf << uint32(0);

    WorldPacket data(SMSG_NPC_TEXT_UPDATE, 100);
    data << uint32(textID);
    data << uint32(buf.size());
    data.append(buf);
    data.WriteBit(1);                                   // has data

    /*if (!pGossip)
    {
        for (uint32 i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            data << float(0);
            data << "Greetings $N";
            data << "Greetings $N";
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
        }
    }
    else
    {
        std::string Text_0[MAX_LOCALES], Text_1[MAX_LOCALES];
        for (int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            Text_0[i]=pGossip->Options[i].Text_0;
            Text_1[i]=pGossip->Options[i].Text_1;
        }

        int loc_idx = GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            if (NpcTextLocale const* nl = sObjectMgr->GetNpcTextLocale(textID))
            {
                for (int i = 0; i < MAX_LOCALES; ++i)
                {
                    ObjectMgr::GetLocaleString(nl->Text_0[i], loc_idx, Text_0[i]);
                    ObjectMgr::GetLocaleString(nl->Text_1[i], loc_idx, Text_1[i]);
                }
            }
        }

        for (int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            data << pGossip->Options[i].Probability;

            if (Text_0[i].empty())
                data << Text_1[i];
            else
                data << Text_0[i];

            if (Text_1[i].empty())
                data << Text_0[i];
            else
                data << Text_1[i];

            data << pGossip->Options[i].Language;

            for (int j = 0; j < MAX_GOSSIP_TEXT_EMOTES; ++j)
            {
                data << pGossip->Options[i].Emotes[j]._Delay;
                data << pGossip->Options[i].Emotes[j]._Emote;
            }
        }
    }*/

    SendPacket(&data);

    TC_LOG_DEBUG("network", "WORLD: Sent SMSG_NPC_TEXT_UPDATE");
}

void WorldSession::SendBroadcastTextDb2Reply(uint32 entry)
{
    ByteBuffer buff;
    WorldPacket data(SMSG_DB_REPLY);

    int loc_idx = GetSessionDbLocaleIndex();
    GossipText const* pGossip = sObjectMgr->GetGossipText(entry);
    uint32 localeEntry = entry;
    if (!pGossip)
    {
        pGossip = sObjectMgr->GetGossipText(DEFAULT_GOSSIP_MESSAGE);
        localeEntry = DEFAULT_GOSSIP_MESSAGE;
    }

    std::string Text_0, Text_1;
    if(pGossip)
    {
        Text_0 = pGossip->Options[0].Text_0;
        Text_1 = pGossip->Options[0].Text_1;
        
        if (loc_idx >= 0)
        {
            if (NpcTextLocale const* nl = sObjectMgr->GetNpcTextLocale(localeEntry))
            {
                ObjectMgr::GetLocaleString(nl->Text_0[0], loc_idx, Text_0);
                ObjectMgr::GetLocaleString(nl->Text_1[0], loc_idx, Text_1);
            }
        }
    }
    
    uint16 size1 = Text_0.length();
    uint16 size2 = Text_1.length();

    data << uint32(sObjectMgr->GetHotfixDate(entry, DB2_REPLY_BROADCAST_TEXT));
    data << uint32(DB2_REPLY_BROADCAST_TEXT);

    buff << uint32(entry);
    buff << uint32(0);
    buff << uint16(size1);
    if (size1)
        buff << std::string( Text_0);
    buff << uint16(size2);
    if (size2)
        buff << std::string(Text_1);
    buff << uint32(0);
    buff << uint32(0);
    buff << uint32(0);
    buff << uint32(0);
    buff << uint32(0);
    buff << uint32(0);
    buff << uint32(0); // sound Id
    buff << uint32(pGossip ? pGossip->Options[0].Emotes[0]._Delay : 0); // Delay
    buff << uint32(pGossip ? pGossip->Options[0].Emotes[0]._Emote : 0); // Emote

    data << uint32(buff.size());
    data.append(buff);
    data << uint32(entry);

    SendPacket(&data);
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandlePageTextQueryOpcode(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_PAGE_TEXT_QUERY");

    ObjectGuid itemGuid;
    uint32 pageID;
    recvData >> pageID;
    recvData.ReadGuidMask<3, 2, 5, 1, 4, 7, 6, 0>(itemGuid);
    recvData.ReadGuidBytes<4, 6, 1, 5, 7, 3, 2, 0>(itemGuid);

    while (pageID)
    {
        PageText const* pageText = sObjectMgr->GetPageText(pageID);
                                                            // guess size
        WorldPacket data(SMSG_PAGE_TEXT_QUERY_RESPONSE, 50);
        data << uint32(pageID);

        if (!pageText)
        {
            data.WriteBit(0);
            pageID = 0;
        }
        else
        {
            std::string Text = pageText->Text;

            int loc_idx = GetSessionDbLocaleIndex();
            if (loc_idx >= 0)
                if (PageTextLocale const* player = sObjectMgr->GetPageTextLocale(pageID))
                    ObjectMgr::GetLocaleString(player->Text, loc_idx, Text);

            data.WriteBit(1);
            data.WriteBits(Text.size(), 12);
            data.WriteString(Text);
            data << uint32(pageText->NextPage);
            data << uint32(pageID);
            pageID = pageText->NextPage;
        }
        SendPacket(&data);

        TC_LOG_DEBUG("network", "WORLD: Sent SMSG_PAGE_TEXT_QUERY_RESPONSE");
    }
}

void WorldSession::HandleCorpseMapPositionQuery(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Recv CMSG_CORPSE_MAP_POSITION_QUERY");

    // Read guid, useless
    recvData.rfinish();

    WorldPacket data(SMSG_CORPSE_MAP_POSITION_QUERY_RESPONSE, 4+4+4+4);
    data << float(0);
    data << float(0);
    data << float(0);
    data << float(0);
    SendPacket(&data);
}

void WorldSession::HandleQuestPOIQuery(WorldPacket& recvData)
{
    uint32 count;
    recvData >> count; // quest count, max=50

    WorldPacket data(SMSG_QUEST_POI_QUERY_RESPONSE, 557);
    data << uint32(count);
    data.WriteBits(count, 20);

    ByteBuffer buff;
    for (uint32 i = 0; i < 50; ++i)
    {
        uint32 questId;
        recvData >> questId; // quest id

        if (i >= count)
            continue;

        bool questOk = false;

        uint16 questSlot = _player->FindQuestSlot(questId);

        if (questSlot != MAX_QUEST_LOG_SIZE)
            questOk =_player->GetQuestSlotQuestId(questSlot) == questId;

        if (!questOk)
        {
            data.WriteBits(0, 18);
            buff << uint32(0);
            buff << uint32(questId);
            continue;
        }

        QuestPOIVector const* POI = sObjectMgr->GetQuestPOIVector(questId);
        if (!POI)
        {
            data.WriteBits(0, 18);
            buff << uint32(0);
            buff << uint32(questId);
            continue;
        }

        data.WriteBits(POI->size(), 18);

        for (QuestPOIVector::const_iterator itr = POI->begin(); itr != POI->end(); ++itr)
        {
            data.WriteBits(itr->points.size(), 21); // POI points count

            buff << uint32(itr->AreaId);            // areaid
            for (std::vector<QuestPOIPoint>::const_iterator itr2 = itr->points.begin(); itr2 != itr->points.end(); ++itr2)
            {
                buff << int32(itr2->y);             // POI point y
                buff << int32(itr2->x);             // POI point x
            }

            buff << int32(itr->ObjectiveIndex);     // objective index
            buff << uint32(0);
            buff << uint32(itr->Unk4);              // unknown
            buff << uint32(itr->Id);                // POI index
            buff << uint32(itr->MapId);             // mapid
            buff << uint32(0);
            buff << uint32(itr->Unk2);              // floor
            buff << uint32(itr->points.size());     // POI points count
            buff << uint32(itr->Unk3);              // unknown
            buff << uint32(0);
        }

        buff << uint32(POI->size());                // POI count
        buff << uint32(questId);                    // quest ID
    }

    data.FlushBits();
    if (!buff.empty())
        data.append(buff);

    SendPacket(&data);
}
