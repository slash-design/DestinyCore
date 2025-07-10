/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#include "Opcodes.h"
#include "SharedDefines.h"
#include "WorldSession.h"
#include "WorldPacket.h"

#include <map>
#include <set>
#include <list>

#define PLAYABLE_RACES_COUNT 15
#define PLAYABLE_CLASSES_COUNT 11

uint8 raceExpansion[PLAYABLE_RACES_COUNT][2] =
{
    { RACE_TAUREN,            EXP_VANILLA             },
    { RACE_UNDEAD_PLAYER,     EXP_VANILLA             },
    { RACE_ORC,               EXP_VANILLA             },
    { RACE_GNOME,             EXP_VANILLA             },
    { RACE_GOBLIN,            EXP_BC },
    { RACE_HUMAN,             EXP_VANILLA             },
    { RACE_TROLL,             EXP_VANILLA             },
    { RACE_PANDAREN_NEUTRAL,  EXP_BC },
    { RACE_DRAENEI,           EXP_BC },
    { RACE_WORGEN,            EXP_BC },
    { RACE_BLOODELF,          EXP_BC },
    { RACE_NIGHTELF,          EXP_VANILLA             },
    { RACE_DWARF,             EXP_VANILLA             },
    { RACE_PANDAREN_ALLI, EXP_BC },
    { RACE_PANDAREN_HORDE,    EXP_BC },
};

uint8 classExpansion[PLAYABLE_CLASSES_COUNT][2] =
{
    { CLASS_MONK,         EXP_PANDARIA      },
    { CLASS_WARRIOR,      EXP_VANILLA                },
    { CLASS_PALADIN,      EXP_VANILLA                },
    { CLASS_HUNTER,       EXP_VANILLA                },
    { CLASS_ROGUE,        EXP_VANILLA                },
    { CLASS_PRIEST,       EXP_VANILLA                },
    { CLASS_SHAMAN,       EXP_VANILLA                },
    { CLASS_MAGE,         EXP_VANILLA                },
    { CLASS_WARLOCK,      EXP_VANILLA                },
    { CLASS_DRUID,        EXP_VANILLA                },
    { CLASS_DEATH_KNIGHT, EXP_WOTLK },
};


void WorldSession::SendAuthResponse(uint8 code, bool queued, uint32 queuePos)
{
    std::map<uint32, std::string> realmNamesToSend;

    RealmNameMap::const_iterator iter = realmNameStore.find(realmID);
    if (iter != realmNameStore.end()) // Add local realm
        realmNamesToSend[realmID] = iter->second;

    TC_LOG_DEBUG("network", "SMSG_AUTH_RESPONSE");
    WorldPacket packet(SMSG_AUTH_RESPONSE, 113);

    packet.WriteBit(code == AUTH_OK);

    if (code == AUTH_OK)
    {
        packet.WriteBits(realmNamesToSend.size(), 21); // Send current realmId

        for (std::map<uint32, std::string>::const_iterator itr = realmNamesToSend.begin(); itr != realmNamesToSend.end(); itr++)
        {
            packet.WriteBits(itr->second.size(), 8);
            std::string normalized = itr->second;
            normalized.erase(std::remove_if(normalized.begin(), normalized.end(), ::isspace), normalized.end());
            packet.WriteBits(normalized.size(), 8);
            packet.WriteBit(itr->first == realmID); // Home realm
        }

        packet.WriteBits(PLAYABLE_CLASSES_COUNT, 23);
        packet.WriteBits(0, 21);
        packet.WriteBit(0);
        packet.WriteBit(0);
        packet.WriteBit(0);
        packet.WriteBit(0);
        packet.WriteBits(PLAYABLE_RACES_COUNT, 23);
        packet.WriteBit(0);
    }

    packet.WriteBit(queued);

    if (queued)
        packet.WriteBit(1);

    packet.FlushBits();

    if (queued)
        packet << uint32(queuePos);

    if (code == AUTH_OK)
    {
        for (std::map<uint32, std::string>::const_iterator itr = realmNamesToSend.begin(); itr != realmNamesToSend.end(); itr++)
        {
            packet << uint32(itr->first);
            packet.WriteString(itr->second);
            std::string normalized = itr->second;
            normalized.erase(std::remove_if(normalized.begin(), normalized.end(), ::isspace), normalized.end());
            packet.WriteString(normalized);
        }

        for (int i = 0; i < PLAYABLE_RACES_COUNT; i++)
        {
            packet << uint8(raceExpansion[i][1]);
            packet << uint8(raceExpansion[i][0]);
        }

        for (int i = 0; i < PLAYABLE_CLASSES_COUNT; i++)
        {
            packet << uint8(classExpansion[i][1]);
            packet << uint8(classExpansion[i][0]);
        }

        packet << uint32(0);
        packet << uint8(Expansion()); // Active Expansion
        packet << uint32(0);
        packet << uint32(0); // unk time in ms
        packet << uint8(Expansion()); // Server Expansion
        packet << uint32(0);
        packet << uint32(0);
        packet << uint32(0);
    }

    packet << uint8(code);

    SendPacket(&packet);
}


void WorldSession::SendClientCacheVersion(uint32 version)
{
    WorldPacket data(SMSG_CLIENTCACHE_VERSION, 4);
    data << uint32(version);
    SendPacket(&data);
}

void WorldSession::SendBattlePay()
{
    WorldPacket data(SMSG_BATTLE_PAY_GET_DISTRIBUTION_LIST_RESPONSE, 7);
    data << uint32(0);
    data.WriteBits(0, 19);
    data.FlushBits();
    SendPacket(&data);
}

void WorldSession::SendDisplayPromo(int32 promo)
{
    WorldPacket data(SMSG_DISPLAY_PROMOTION, 7);
    data << promo;
    SendPacket(&data);
}