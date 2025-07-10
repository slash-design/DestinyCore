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
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "GitRevision.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "DatabaseEnv.h"
#include "Chat.h"
#include "Group.h"
#include "Guild.h"
#include "GuildFinderMgr.h"
#include "Language.h"
#include "Log.h"
#include "Opcodes.h"
#include "Player.h"
#include "PlayerDump.h"
#include "SharedDefines.h"
#include "SocialMgr.h"
#include "UpdateMask.h"
#include "Util.h"
#include "ScriptMgr.h"
#include "Battleground.h"
#include "AccountMgr.h"
#include "DBCStores.h"
#include "LFGMgr.h"

class LoginQueryHolder : public SQLQueryHolder
{
    private:
        uint32 m_accountId;
        uint64 m_guid;
    public:
        LoginQueryHolder(uint32 accountId, uint64 guid)
            : m_accountId(accountId), m_guid(guid) { }
        uint64 GetGuid() const { return m_guid; }
        uint32 GetAccountId() const { return m_accountId; }
        bool Initialize();
};

bool LoginQueryHolder::Initialize()
{
    SetSize(MAX_PLAYER_LOGIN_QUERY);

    bool res = true;
    uint32 lowGuid = GUID_LOPART(m_guid);
    PreparedStatement* stmt = NULL;

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADFROM, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GROUP_MEMBER);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADGROUP, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_INSTANCE);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADBOUNDINSTANCES, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_AURAS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADAURAS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_AURAS_EFFECTS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADAURAS_EFFECTS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SPELL);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADSPELLS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_MOUNTS);
    stmt->setUInt32(0, m_accountId);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACCOUNTMOUNTS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_QUESTSTATUS);
    stmt->setUInt32(0, m_accountId);
    stmt->setUInt32(1, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADQUESTSTATUS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_DAILYQUESTSTATUS);
    stmt->setUInt32(0, m_accountId);
    stmt->setUInt32(1, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADDAILYQUESTSTATUS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_WEEKLYQUESTSTATUS);
    stmt->setUInt32(0, m_accountId);
    stmt->setUInt32(1, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADWEEKLYQUESTSTATUS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SEASONALQUESTSTATUS);
    stmt->setUInt32(0, m_accountId);
    stmt->setUInt32(1, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADSEASONALQUESTSTATUS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_REPUTATION);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADREPUTATION, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_INVENTORY);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADINVENTORY, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_VOID_STORAGE);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADVOIDSTORAGE, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_ACTIONS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACTIONS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_MAILCOUNT);
    stmt->setUInt32(0, lowGuid);
    stmt->setUInt64(1, uint64(time(NULL)));
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADMAILCOUNT, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_MAILDATE);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADMAILDATE, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SOCIALLIST);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADSOCIALLIST, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_HOMEBIND);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADHOMEBIND, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SPELLCOOLDOWNS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADSPELLCOOLDOWNS, stmt);

    if (sWorld->getBoolConfig(CONFIG_DECLINED_NAMES_USED))
    {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_DECLINEDNAMES);
        stmt->setUInt32(0, lowGuid);
        res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADDECLINEDNAMES, stmt);
    }

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUILD_MEMBER);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADGUILD, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_ACHIEVEMENTS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACHIEVEMENTS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_ACHIEVEMENTS);
    stmt->setUInt32(0, m_accountId);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACCOUNTACHIEVEMENTS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_CRITERIAPROGRESS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADCRITERIAPROGRESS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_CRITERIAPROGRESS);
    stmt->setUInt32(0, m_accountId);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACCOUNTCRITERIAPROGRESS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_EQUIPMENTSETS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADEQUIPMENTSETS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_BGDATA);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADBGDATA, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_GLYPHS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADGLYPHS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_TALENTS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADTALENTS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PLAYER_ACCOUNT_DATA);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACCOUNTDATA, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SKILLS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADSKILLS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_RANDOMBG);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADRANDOMBG, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_BANNED);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADBANNED, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_QUESTSTATUSREW);
    stmt->setUInt32(0, m_accountId);
    stmt->setUInt32(1, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADQUESTSTATUSREW, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_INSTANCELOCKTIMES);
    stmt->setUInt32(0, m_accountId);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADINSTANCELOCKTIMES, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PLAYER_CURRENCY);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADCURRENCY, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CUF_PROFILES);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_CUF_PROFILES, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_BATTLE_PET_LIST);
    stmt->setUInt32(0, m_accountId);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_BATTLE_PETS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_BATTLE_PET_SLOTS);
    stmt->setUInt32(0, m_accountId);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_BATTLE_PET_SLOTS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PLAYER_ARCHAELOGY);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADARCHAELOGY, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PLAYER_ARCHAEOLOGY_FINDS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_ARCHAEOLOGY_FINDS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PERSONAL_RATE);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_PERSONAL_RATE, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PLAYER_VISUAL);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_VISUAL, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PLAYER_LOOTCOOLDOWN);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_LOOTCOOLDOWN, stmt);

    return res;
}

void WorldSession::HandleCharEnum(PreparedQueryResult result)
{
    uint32 charCount = 0;
    ByteBuffer bitBuffer;
    ByteBuffer dataBuffer;

    // Sended before SMSG_ENUM_CHARACTERS_RESULT
    // must be procceded before BuildEnumData, because of unsetting bosted character guid
    //if (m_charBooster->GetCurrentAction() == CHARACTER_BOOST_APPLIED)
    //    m_charBooster->HandleCharacterBoost();

    if (result)
    {
        _allowedCharsToLogin.clear();

        charCount = uint32(result->GetRowCount());
        bitBuffer.reserve(24 * charCount / 8);
        dataBuffer.reserve(charCount * 381);

        bitBuffer.WriteBits(0, 21); // factionChangeRestrictions - raceId / mask loop
        bitBuffer.WriteBits(charCount, 16);

        do
        {
            uint32 guidLow = (*result)[0].GetUInt32();

            TC_LOG_INFO("network", "Loading char guid %u from account %u.", guidLow, GetAccountId());

            Player::BuildEnumData(result, &dataBuffer, &bitBuffer);

            // Do not allow banned characters to log in
            if (!(*result)[20].GetUInt32())
                _allowedCharsToLogin.insert(guidLow);

            if (!sWorld->HasCharacterNameData(guidLow)) // This can happen if characters are inserted into the database manually. Core hasn't loaded name data yet.
                sWorld->AddCharacterNameData(guidLow, (*result)[1].GetString(), (*result)[4].GetUInt8(), (*result)[2].GetUInt8(), (*result)[3].GetUInt8(), (*result)[7].GetUInt8());
        } while (result->NextRow());

        bitBuffer.WriteBit(1); // Sucess
        bitBuffer.FlushBits();
    }
    else
    {
        bitBuffer.WriteBits(0, 21);
        bitBuffer.WriteBits(0, 16);
        bitBuffer.WriteBit(1); // Success
        bitBuffer.FlushBits();
    }

    WorldPacket data(SMSG_CHAR_ENUM, 7 + bitBuffer.size() + dataBuffer.size());

    data.append(bitBuffer);

    if (charCount)
        data.append(dataBuffer);

    SendPacket(&data);

    // Sended after SMSG_ENUM_CHARACTERS_RESULT
    //if (m_charBooster->GetCurrentAction() == CHARACTER_BOOST_ITEMS)
    //    m_charBooster->HandleCharacterBoost();
}

void WorldSession::HandleCharEnumOpcode(WorldPacket & recvData)
{
    // remove expired bans
    //PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_EXPIRED_BANS);
    //CharacterDatabase.Execute(stmt);
    PreparedStatement* stmt;

    /// get all the data necessary for loading all characters (along with their pets) on the account

    if (sWorld->getBoolConfig(CONFIG_DECLINED_NAMES_USED))
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ENUM_DECLINED_NAME);
    else
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ENUM);

    stmt->setUInt32(0, GetAccountId());

    _charEnumCallback = CharacterDatabase.AsyncQuery(stmt);
}

void WorldSession::HandleCharCreateOpcode(WorldPacket& recvData)
{
    uint8 hairStyle, face, facialHair, hairColor, race_, class_, skin, gender, outfitId;

    recvData >> outfitId >> hairStyle >> class_ >> skin;
    recvData >> face >> race_ >> facialHair >> gender >> hairColor;

    uint32 nameLength = recvData.ReadBits(6);
    uint8 unk = recvData.ReadBit();
    std::string name = recvData.ReadString(nameLength);

    if (unk)
        recvData.read_skip<uint32>();

    WorldPacket data(SMSG_CHAR_CREATE, 1);                  // returned with diff.values in all cases

    if (AccountMgr::IsPlayerAccount(GetSecurity()))
    {
        if (uint32 mask = sWorld->getIntConfig(WorldIntConfigs::CONFIG_CHARACTER_CREATING_DISABLED))
        {
            bool disabled = false;

            uint32 team = Player::TeamForRace(race_);
            switch (team)
            {
            case ALLIANCE:
                disabled = mask & (1 << 0);
                break;
            case HORDE:
                disabled = mask & (1 << 1);
                break;
            }

            if (disabled)
            {
                data << uint8(ResponseCodes::CHAR_CREATE_DISABLED);
                SendPacket(&data);
                return;
            }
        }
    }

    ChrClassesEntry const* classEntry = sChrClassesStore.LookupEntry(class_);
    if (!classEntry)
    {
        data << uint8(ResponseCodes::CHAR_CREATE_FAILED);
        SendPacket(&data);
        TC_LOG_ERROR("network", "Class (%u) not found in DBC while creating new char for account (ID: %u): wrong DBC files or cheater?", class_, GetAccountId());
        return;
    }

    ChrRacesEntry const* raceEntry = sChrRacesStore.LookupEntry(race_);
    if (!raceEntry)
    {
        data << uint8(ResponseCodes::CHAR_CREATE_FAILED);
        SendPacket(&data);
        TC_LOG_ERROR("network", "Race (%u) not found in DBC while creating new char for account (ID: %u): wrong DBC files or cheater?", race_, GetAccountId());
        return;
    }
    /*
    // prevent character creating Expansion race without Expansion account
    if (raceEntry->expansion > Expansion())
    {
    data << uint8(CHAR_CREATE_EXPANSION);
    SF_LOG_ERROR("network", "Expansion %u account:[%d] tried to Create character with expansion %u race (%u)", Expansion(), GetAccountId(), raceEntry->expansion, race_);
    SendPacket(&data);
    return;
    }

    // prevent character creating Expansion class without Expansion account
    if (classEntry->expansion > Expansion())
    {
    data << uint8(CHAR_CREATE_EXPANSION_CLASS);
    SF_LOG_ERROR("network", "Expansion %u account:[%d] tried to Create character with expansion %u class (%u)", Expansion(), GetAccountId(), classEntry->expansion, class_);
    SendPacket(&data);
    return;
    }*/

    if (AccountMgr::IsPlayerAccount(GetSecurity()))
    {
        uint32 raceMaskDisabled = sWorld->getIntConfig(WorldIntConfigs::CONFIG_CHARACTER_CREATING_DISABLED_RACEMASK);
        if ((1 << (race_ - 1)) & raceMaskDisabled)
        {
            data << uint8(ResponseCodes::CHAR_CREATE_DISABLED);
            SendPacket(&data);
            return;
        }
    }

    if (AccountMgr::IsPlayerAccount(GetSecurity()))
    {
        uint32 classMaskDisabled = sWorld->getIntConfig(WorldIntConfigs::CONFIG_CHARACTER_CREATING_DISABLED_CLASSMASK);
        if ((1 << (class_ - 1)) & classMaskDisabled)
        {
            data << uint8(ResponseCodes::CHAR_CREATE_DISABLED);
            SendPacket(&data);
            return;
        }
    }

    // prevent character creating with invalid name
    if (!normalizePlayerName(name))
    {
        data << uint8(ResponseCodes::CHAR_NAME_NO_NAME);
        SendPacket(&data);
        TC_LOG_ERROR("network", "Account:[%d] but tried to Create character with empty [name] ", GetAccountId());
        return;
    }

    // check name limitations
    uint8 res = ObjectMgr::CheckPlayerName(name, true);
    if (res != ResponseCodes::CHAR_NAME_SUCCESS)
    {
        data << uint8(res);
        SendPacket(&data);
        return;
    }

    if (AccountMgr::IsPlayerAccount(GetSecurity()) && sObjectMgr->IsReservedName(name))
    {
        data << uint8(ResponseCodes::CHAR_NAME_RESERVED);
        SendPacket(&data);
        return;
    }

    if (class_ == CLASS_DEATH_KNIGHT && AccountMgr::IsPlayerAccount(GetSecurity()))
    {
        // speedup check for heroic class disabled case
        uint32 heroic_free_slots = sWorld->getIntConfig(WorldIntConfigs::CONFIG_HEROIC_CHARACTERS_PER_REALM);
        if (heroic_free_slots == 0)
        {
            data << uint8(ResponseCodes::CHAR_CREATE_UNIQUE_CLASS_LIMIT);
            SendPacket(&data);
            return;
        }

        // speedup check for heroic class disabled case
        uint32 req_level_for_heroic = sWorld->getIntConfig(WorldIntConfigs::CONFIG_CHARACTER_CREATING_MIN_LEVEL_FOR_HEROIC_CHARACTER);
        if (req_level_for_heroic > sWorld->getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL))
        {
            data << uint8(ResponseCodes::CHAR_CREATE_LEVEL_REQUIREMENT);
            SendPacket(&data);
            return;
        }
    }

    delete _charCreateCallback.GetParam();  // Delete existing if any, to make the callback chain reset to stage 0
    _charCreateCallback.SetParam(new CharacterCreateInfo(name, race_, class_, gender, skin, face, hairStyle, hairColor, facialHair, outfitId, recvData));
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHECK_NAME);
    stmt->setString(0, name);
    _charCreateCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
}

void WorldSession::HandleCharCreateCallback(PreparedQueryResult result, CharacterCreateInfo* createInfo)
{
    /** This is a series of callbacks executed consecutively as a result from the database becomes available.
    This is much more efficient than synchronous requests on packet handler, and much less DoS prone.
    It also prevents data syncrhonisation errors.
    */
    switch (_charCreateCallback.GetStage())
    {
    case 0:
    {
        if (result)
        {
            WorldPacket data(SMSG_CHAR_CREATE, 1);
            data << uint8(ResponseCodes::CHAR_CREATE_NAME_IN_USE);
            SendPacket(&data);
            delete createInfo;
            _charCreateCallback.Reset();
            return;
        }

        ASSERT(_charCreateCallback.GetParam() == createInfo);

        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_SUM_REALM_CHARACTERS);
        stmt->setUInt32(0, GetAccountId());

        _charCreateCallback.FreeResult();
        _charCreateCallback.SetFutureResult(LoginDatabase.AsyncQuery(stmt));
        _charCreateCallback.NextStage();
    }
    break;
    case 1:
    {
        uint16 acctCharCount = 0;
        if (result)
        {
            Field* fields = result->Fetch();
            // SELECT SUM(x) is MYSQL_TYPE_NEWDECIMAL - needs to be read as string
            const char* ch = fields[0].GetCString();
            if (ch)
                acctCharCount = atoi(ch);
        }

        if (acctCharCount >= sWorld->getIntConfig(WorldIntConfigs::CONFIG_CHARACTERS_PER_ACCOUNT))
        {
            WorldPacket data(SMSG_CHAR_CREATE, 1);
            data << uint8(ResponseCodes::CHAR_CREATE_ACCOUNT_LIMIT);
            SendPacket(&data);
            delete createInfo;
            _charCreateCallback.Reset();
            return;
        }


        ASSERT(_charCreateCallback.GetParam() == createInfo);

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_SUM_CHARS);
        stmt->setUInt32(0, GetAccountId());

        _charCreateCallback.FreeResult();
        _charCreateCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
        _charCreateCallback.NextStage();
    }
    break;
    case 2:
    {
        if (result)
        {
            Field* fields = result->Fetch();
            createInfo->CharCount = uint8(fields[0].GetUInt64()); // SQL's COUNT() returns uint64 but it will always be less than uint8.Max

            if (createInfo->CharCount >= sWorld->getIntConfig(WorldIntConfigs::CONFIG_CHARACTERS_PER_REALM))
            {
                WorldPacket data(SMSG_CHAR_CREATE, 1);
                data << uint8(ResponseCodes::CHAR_CREATE_SERVER_LIMIT);
                SendPacket(&data);
                delete createInfo;
                _charCreateCallback.Reset();
                return;
            }
        }

        bool allowTwoSideAccounts = !sWorld->IsPvPRealm() || !AccountMgr::IsPlayerAccount(GetSecurity());
        uint32 skipCinematics = sWorld->getIntConfig(WorldIntConfigs::CONFIG_SKIP_CINEMATICS);

        _charCreateCallback.FreeResult();

        if (!allowTwoSideAccounts || skipCinematics == 1 || createInfo->Class == CLASS_DEATH_KNIGHT)
        {
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_CREATE_INFO);
            stmt->setUInt32(0, GetAccountId());
            stmt->setUInt32(1, (skipCinematics == 1 || createInfo->Class == CLASS_DEATH_KNIGHT) ? 10 : 1);
            _charCreateCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
            _charCreateCallback.NextStage();
            return;
        }

        _charCreateCallback.NextStage();
        HandleCharCreateCallback(PreparedQueryResult(NULL), createInfo);   // Will jump to case 3
    }
    break;
    case 3:
    {
        bool haveSameRace = false;
        uint32 heroicReqLevel = sWorld->getIntConfig(WorldIntConfigs::CONFIG_CHARACTER_CREATING_MIN_LEVEL_FOR_HEROIC_CHARACTER);
        bool hasHeroicReqLevel = (heroicReqLevel == 0);
        bool allowTwoSideAccounts = !sWorld->IsPvPRealm() || !AccountMgr::IsPlayerAccount(GetSecurity());
        uint32 skipCinematics = sWorld->getIntConfig(WorldIntConfigs::CONFIG_SKIP_CINEMATICS);
        bool checkHeroicReqs = createInfo->Class == CLASS_DEATH_KNIGHT && !AccountMgr::IsPlayerAccount(GetSecurity());

        if (result)
        {
            uint32 team = Player::TeamForRace(createInfo->Race);
            uint32 freeHeroicSlots = sWorld->getIntConfig(WorldIntConfigs::CONFIG_HEROIC_CHARACTERS_PER_REALM);

            Field* field = result->Fetch();
            uint8 accRace = field[1].GetUInt8();

            if (checkHeroicReqs)
            {
                uint8 accClass = field[2].GetUInt8();
                if (accClass == CLASS_DEATH_KNIGHT)
                {
                    if (freeHeroicSlots > 0)
                        --freeHeroicSlots;

                    if (freeHeroicSlots == 0)
                    {
                        WorldPacket data(SMSG_CHAR_CREATE, 1);
                        data << uint8(ResponseCodes::CHAR_CREATE_UNIQUE_CLASS_LIMIT);
                        SendPacket(&data);
                        delete createInfo;
                        _charCreateCallback.Reset();
                        return;
                    }
                }

                if (!hasHeroicReqLevel)
                {
                    uint8 accLevel = field[0].GetUInt8();
                    if (accLevel >= heroicReqLevel)
                        hasHeroicReqLevel = true;
                }
            }

            // need to check team only for first character
            /// @todo what to if account already has characters of both races?
            if (!allowTwoSideAccounts)
            {
                uint32 accTeam = 0;
                if (accRace > 0)
                    accTeam = Player::TeamForRace(accRace);

                if (accTeam != team)
                {
                    WorldPacket data(SMSG_CHAR_CREATE, 1);
                    data << uint8(ResponseCodes::CHAR_CREATE_PVP_TEAMS_VIOLATION);
                    SendPacket(&data);
                    delete createInfo;
                    _charCreateCallback.Reset();
                    return;
                }
            }

            // search same race for cinematic or same class if need
            /// @todo check if cinematic already shown? (already logged in?; cinematic field)
            while ((skipCinematics == 1 && !haveSameRace) || createInfo->Class == CLASS_DEATH_KNIGHT)
            {
                if (!result->NextRow())
                    break;

                field = result->Fetch();
                accRace = field[1].GetUInt8();

                if (!haveSameRace)
                    haveSameRace = createInfo->Race == accRace;

                if (checkHeroicReqs)
                {
                    uint8 acc_class = field[2].GetUInt8();
                    if (acc_class == CLASS_DEATH_KNIGHT)
                    {
                        if (freeHeroicSlots > 0)
                            --freeHeroicSlots;

                        if (freeHeroicSlots == 0)
                        {
                            WorldPacket data(SMSG_CHAR_CREATE, 1);
                            data << uint8(ResponseCodes::CHAR_CREATE_UNIQUE_CLASS_LIMIT);
                            SendPacket(&data);
                            delete createInfo;
                            _charCreateCallback.Reset();
                            return;
                        }
                    }

                    if (!hasHeroicReqLevel)
                    {
                        uint8 acc_level = field[0].GetUInt8();
                        if (acc_level >= heroicReqLevel)
                            hasHeroicReqLevel = true;
                    }
                }
            }
        }

        if (checkHeroicReqs && !hasHeroicReqLevel)
        {
            WorldPacket data(SMSG_CHAR_CREATE, 1);
            data << uint8(ResponseCodes::CHAR_CREATE_LEVEL_REQUIREMENT);
            SendPacket(&data);
            delete createInfo;
            _charCreateCallback.Reset();
            return;
        }

        if (createInfo->Data.rpos() < createInfo->Data.wpos())
        {
            uint8 unk;
            createInfo->Data >> unk;
            TC_LOG_DEBUG("network", "Character creation %s (account %u) has unhandled tail data: [%u]", createInfo->Name.c_str(), GetAccountId(), unk);
        }

        Player newChar(this);
        newChar.GetMotionMaster()->Initialize();
        if (!newChar.Create(sObjectMgr->GenerateLowGuid(HIGHGUID_PLAYER), createInfo))
        {
            // Player not create (race/class/etc problem?)
            newChar.CleanupsBeforeDelete();

            WorldPacket data(SMSG_CHAR_CREATE, 1);
            data << uint8(ResponseCodes::CHAR_CREATE_ERROR);
            SendPacket(&data);
            delete createInfo;
            _charCreateCallback.Reset();
            return;
        }

        if ((haveSameRace && skipCinematics == 1) || skipCinematics == 2)
            newChar.setCinematic(1);                          // not show intro

        newChar.SetAtLoginFlag(AT_LOGIN_FIRST);               // First login

        // Player created, save it now
        newChar.SaveToDB(true);
        createInfo->CharCount += 1;

        SQLTransaction trans = LoginDatabase.BeginTransaction();

        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_REALM_CHARACTERS_BY_REALM);
        stmt->setUInt32(0, GetAccountId());
        stmt->setUInt32(1, GetVirtualRealmID());
        trans->Append(stmt);

        stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_REALM_CHARACTERS);
        stmt->setUInt32(0, createInfo->CharCount);
        stmt->setUInt32(1, GetAccountId());
        stmt->setUInt32(2, GetVirtualRealmID());
        trans->Append(stmt);

        LoginDatabase.CommitTransaction(trans);

        WorldPacket data(SMSG_CHAR_CREATE, 1);
        data << uint8(ResponseCodes::CHAR_CREATE_SUCCESS);
        SendPacket(&data);

        std::string IP_str = GetRemoteAddress();
        TC_LOG_INFO("entities.player.character", "Account: %d (IP: %s) Create Character:[%s] (GUID: %u)", GetAccountId(), IP_str.c_str(), createInfo->Name.c_str(), newChar.GetGUIDLow());
        sScriptMgr->OnPlayerCreate(&newChar);
        sWorld->AddCharacterNameData(newChar.GetGUIDLow(), newChar.GetName(), newChar.getGender(), newChar.getRace(), newChar.getClass(), newChar.getLevel());

        newChar.CleanupsBeforeDelete();
        delete createInfo;
        _charCreateCallback.Reset();
    }
    break;
    }
}

void WorldSession::HandleCharDeleteOpcode(WorldPacket & recvData)
{
    ObjectGuid guid;
    recvData.ReadGuidMask<1, 4, 7, 5, 3, 2, 0, 6>(guid);
    recvData.ReadGuidBytes<2, 0, 4, 1, 5, 3, 7, 6>(guid);

    // can't delete loaded character
    if (ObjectAccessor::FindPlayer(guid))
        return;

    uint32 accountId = 0;
    std::string name;

    // is guild leader
    if (sGuildMgr->GetGuildByLeader(guid))
    {
        WorldPacket data(SMSG_CHAR_DELETE, 1);
        data << uint8(CHAR_DELETE_FAILED_GUILD_LEADER);
        SendPacket(&data);
        return;
    }

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_NAME_BY_GUID);
    stmt->setUInt32(0, GUID_LOPART(guid));

    if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
    {
        Field* fields = result->Fetch();
        accountId     = fields[0].GetUInt32();
        name          = fields[1].GetString();
    }

    // prevent deleting other players' characters using cheating tools
    if (accountId != GetAccountId())
        return;

    std::string IP_str = GetRemoteAddress();
    TC_LOG_INFO("char", "Account: %d (IP: %s) Delete Character:[%s] (GUID: %u)", GetAccountId(), IP_str.c_str(), name.c_str(), GUID_LOPART(guid));
    sScriptMgr->OnPlayerDelete(guid);
    sWorld->DeleteCharacterNameData(GUID_LOPART(guid));

    sGuildFinderMgr->RemoveAllMembershipRequestsFromPlayer(guid);
    Player::DeleteFromDB(guid, GetAccountId());
    sWorld->DeleteCharName(name);

    WorldPacket data(SMSG_CHAR_DELETE, 1);
    data << uint8(CHAR_DELETE_SUCCESS);
    SendPacket(&data);

    // reset timer.
    timeCharEnumOpcode = 0;
}

void WorldSession::HandlePlayerLoginOpcode(WorldPacket& recvData)
{
    // Prevent flood of CMSG_PLAYER_LOGIN
    if (++playerLoginCounter > 10)
    {
        TC_LOG_ERROR("opcode", "Player kicked due to flood of CMSG_PLAYER_LOGIN");
        KickPlayer();
        recvData.rfinish();
        return;
    }
    
    if (PlayerLoading() || GetPlayer() != NULL)
    {
        TC_LOG_ERROR("network", "Player tries to login again, AccountId = %d", GetAccountId());
        return;
    }

    m_playerLoading = true;
    ObjectGuid playerGuid;
    float unk;

    recvData >> unk;
    recvData.ReadGuidMask<6, 7, 1, 5, 2, 4, 3, 0>(playerGuid);
    recvData.ReadGuidBytes<7, 6, 0, 1, 4, 3, 2, 5>(playerGuid);

    TC_LOG_DEBUG("network", "WORLD: Recvd Player Logon Message");
    TC_LOG_DEBUG("network", "Character (Guid: %u) logging in, unk float: %f", GUID_LOPART(playerGuid), unk);

    if (!CharCanLogin(GUID_LOPART(playerGuid)))
    {
        TC_LOG_ERROR("network", "Account (%u) can't login with that character (%u).", GetAccountId(), GUID_LOPART(playerGuid));
        KickPlayer();
        return;
    }

    LoginQueryHolder *holder = new LoginQueryHolder(GetAccountId(), playerGuid);
    if (!holder->Initialize())
    {
        delete holder;                                      // delete all unprocessed queries
        m_playerLoading = false;
        return;
    }

    _charLoginCallback = CharacterDatabase.DelayQueryHolder((SQLQueryHolder*)holder);
}

void WorldSession::HandleLoadScreenOpcode(WorldPacket& recvPacket)
{
    TC_LOG_DEBUG("server", "WORLD: Recvd CMSG_LOAD_SCREEN");
    uint32 mapID;

    recvPacket >> mapID;
    recvPacket.ReadBit();
}

void WorldSession::HandlePlayerLogin(LoginQueryHolder* holder)
{
    uint64 playerGuid = holder->GetGuid();

    Player* pCurrChar = new Player(this);
     // for send server info and strings (config)
    ChatHandler chH = ChatHandler(pCurrChar);

    // "GetAccountId() == db stored account id" checked in LoadFromDB (prevent login not own character using cheating tools)
    if (!pCurrChar->LoadFromDB(GUID_LOPART(playerGuid), holder))
    {
        SetPlayer(NULL);
        KickPlayer();                                       // disconnect client, player no set to session and it will not deleted or saved at kick
        delete pCurrChar;                                   // delete it manually
        delete holder;                                      // delete all unprocessed queries
        m_playerLoading = false;
        return;
    }

    pCurrChar->GetMotionMaster()->Initialize();
    pCurrChar->SendDungeonDifficulty();

    WorldPacket data(SMSG_LOGIN_VERIFY_WORLD, 20);
    data << pCurrChar->GetPositionZ();
    data << pCurrChar->GetPositionY();
    data << pCurrChar->GetMapId();
    data << pCurrChar->GetOrientation();
    data << pCurrChar->GetPositionX();
    SendPacket(&data);

    // load player specific part before send times
    LoadAccountData(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOADACCOUNTDATA), PER_CHARACTER_CACHE_MASK);
    SendAccountDataTimes(PER_CHARACTER_CACHE_MASK);
    SendTutorialsData();

    data.Initialize(SMSG_FEATURE_SYSTEM_STATUS);
    data.WriteBit(0);                                                       // has travel pass
    data.WriteBit(sWorld->getBoolConfig(CONFIG_PURCHASE_SHOP_ENABLED));     // shop enabled
    data.WriteBit(0);                                                       // shop disabled by parental controls
    data.WriteBit(0);                                                       // has session time alert
    data.WriteBit(0);                                                       // can sor by text
    data.WriteBit(0);                                                       // voice chat enabled
    data.WriteBit(sWorld->getBoolConfig(CONFIG_PURCHASE_SHOP_ENABLED));     // shop available
    data.WriteBit(0);                                                       // raf status
    data.WriteBit(0);                                                       // quick ticket status
    data.WriteBit(0);                                                       // has item restoration button
    data.WriteBit(1);                                                       // byte2D
    data << uint32(realmID);                                                // realm id?
    data << uint32(0);                                                      // sor dword44
    data << uint32(0);                                                      // sor remaining
    data << uint32(43);                                                     // dword50
    data << uint8(2);                                                       // complain state
    SendPacket(&data);

    // Send MOTD
    {
        data.Initialize(SMSG_MOTD);
        uint32 bit_pos = data.bitwpos();
        data.WriteBits(0, 4);                    // placeholder

        uint32 linecount=0;
        std::string str_motd = sWorld->GetMotd();
        std::string::size_type pos, nextpos;

        pos = 0;
        bool first = false;
        while ((nextpos= str_motd.find('@', pos)) != std::string::npos)
        {
            if (nextpos != pos)
            {
                if (!first)
                {
                    first = true;
                    data.WriteBits(nextpos-pos, 7);
                }
                else
                    data.WriteBits(nextpos-pos+1, 7);
                ++linecount;
            }

            str_motd.replace(nextpos, 1, "");
            pos = nextpos+1;
        }

        if (pos<str_motd.length())
        {
            data.WriteBits(str_motd.length()-pos+1, 7);
            ++linecount;
        }

        data.FlushBits();
        data.PutBits<uint32>(bit_pos, linecount, 4);
        data << str_motd;                                // mess

        SendPacket(&data);
        TC_LOG_DEBUG("network", "WORLD: Sent motd (SMSG_MOTD)");

        // send server info
        if (sWorld->getIntConfig(CONFIG_ENABLE_SINFO_LOGIN) == 1)
            chH.PSendSysMessage("%s", GitRevision::GetFullVersion());

        TC_LOG_DEBUG("network", "WORLD: Sent server info");
    }

    SendTimeZoneInformation();

    //QueryResult* result = CharacterDatabase.PQuery("SELECT guildid, rank FROM guild_member WHERE guid = '%u'", pCurrChar->GetGUIDLow());
    if (PreparedQueryResult resultGuild = holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOADGUILD))
    {
        Field* fields = resultGuild->Fetch();
        pCurrChar->SetInGuild(fields[0].GetUInt32());
        pCurrChar->SetRank(fields[1].GetUInt8());
        if (Guild* guild = sGuildMgr->GetGuildById(pCurrChar->GetGuildId()))
            pCurrChar->SetGuildLevel(guild->GetLevel());
    }
    else if (pCurrChar->GetGuildId())                        // clear guild related fields in case wrong data about non existed membership
    {
        pCurrChar->SetInGuild(0);
        pCurrChar->SetRank(0);
        pCurrChar->SetGuildLevel(0);
    }

    //! 5.4.1
    // ToDo: add data from config
    data.Initialize(SMSG_ARENA_SEASON_WORLDSTATE, 4+4);
    data << uint32(sWorld->getIntConfig(CONFIG_ARENA_SEASON_ID));
    data << uint32(sWorld->getIntConfig(CONFIG_ARENA_SEASON_ID)-1);
    SendPacket(&data);

    //! 5.4.1
    data.Initialize(SMSG_HOTFIX_INFO);
    HotfixData const& hotfix = sObjectMgr->GetHotfixData();
    data.WriteBits(hotfix.size(), 20);
    data.FlushBits();
    for (uint32 i = 0; i < hotfix.size(); ++i)
    {
        data << uint32(hotfix[i].Timestamp);
        data << uint32(hotfix[i].Type);
        data << uint32(hotfix[i].Entry);
    }
    SendPacket(&data);

    pCurrChar->SendInitialPacketsBeforeAddToMap();

    //Show cinematic at the first time that player login
    if (!pCurrChar->getCinematic())
    {
        pCurrChar->setCinematic(1);

        if (ChrClassesEntry const* cEntry = sChrClassesStore.LookupEntry(pCurrChar->getClass()))
        {
            if (cEntry->CinematicSequence)
                pCurrChar->SendCinematicStart(cEntry->CinematicSequence);
            else if (ChrRacesEntry const* rEntry = sChrRacesStore.LookupEntry(pCurrChar->getRace()))
                pCurrChar->SendCinematicStart(rEntry->CinematicSequence);

            // send new char string if not empty
            if (!sWorld->GetNewCharString().empty())
                chH.PSendSysMessage("%s", sWorld->GetNewCharString().c_str());
        }
    }

    if (!pCurrChar->GetMap()->AddPlayerToMap(pCurrChar) || !pCurrChar->CheckInstanceLoginValid())
    {
        AreaTriggerStruct const* at = sObjectMgr->GetGoBackTrigger(pCurrChar->GetMapId());
        if (at)
            pCurrChar->TeleportTo(at->target_mapId, at->target_X, at->target_Y, at->target_Z, pCurrChar->GetOrientation());
        else
            pCurrChar->TeleportTo(pCurrChar->m_homebindMapId, pCurrChar->m_homebindX, pCurrChar->m_homebindY, pCurrChar->m_homebindZ, pCurrChar->GetOrientation());
    }

    sObjectAccessor->AddObject(pCurrChar);
    //TC_LOG_DEBUG("server", "Player %s added to Map.", pCurrChar->GetName());

    if (pCurrChar->GetGuildId() != 0)
    {
        if (Guild* guild = sGuildMgr->GetGuildById(pCurrChar->GetGuildId()))
            guild->SendLoginInfo(this);
        else
        {
            // remove wrong guild data
            TC_LOG_ERROR("server", "Player %s (GUID: %u) marked as member of not existing guild (id: %u), removing guild membership for player.", pCurrChar->GetName(), pCurrChar->GetGUIDLow(), pCurrChar->GetGuildId());
            pCurrChar->SetInGuild(0);
        }
    }

    pCurrChar->SendInitialPacketsAfterAddToMap();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_ONLINE);
    stmt->setUInt32(0, pCurrChar->GetGUIDLow());
    CharacterDatabase.Execute(stmt);

    stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_ACCOUNT_ONLINE);
    stmt->setUInt32(0, GetAccountId());
    LoginDatabase.Execute(stmt);

    pCurrChar->SetInGameTime(getMSTime());

    // announce group about member online (must be after add to player list to receive announce to self)
    if (Group* group = pCurrChar->GetGroup())
    {
        //pCurrChar->groupInfo.group->SendInit(this); // useless
        group->SendUpdate();
        group->ResetMaxEnchantingLevel();
    }

    // friend status
    sSocialMgr->SendFriendStatus(pCurrChar, FRIEND_ONLINE, pCurrChar->GetGUIDLow(), true);

    // Place character in world (and load zone) before some object loading
    pCurrChar->LoadCorpse();

    // setting Ghost+speed if dead
    if (pCurrChar->m_deathState != ALIVE)
    {
        // not blizz like, we must correctly save and load player instead...
        if (pCurrChar->getRace() == RACE_NIGHTELF)
            pCurrChar->CastSpell(pCurrChar, 20584, true, 0);// auras SPELL_AURA_INCREASE_SPEED(+speed in wisp form), SPELL_AURA_INCREASE_SWIM_SPEED(+swim speed in wisp form), SPELL_AURA_TRANSFORM (to wisp form)
        pCurrChar->CastSpell(pCurrChar, 8326, true, 0);     // auras SPELL_AURA_GHOST, SPELL_AURA_INCREASE_SPEED(why?), SPELL_AURA_INCREASE_SWIM_SPEED(why?)

        pCurrChar->SendMovementSetWaterWalking(true);
    }

    pCurrChar->ContinueTaxiFlight();
    pCurrChar->LoadPet();

    // Set FFA PvP for non GM in non-rest mode
    if (sWorld->IsFFAPvPRealm() && !pCurrChar->isGameMaster() && !pCurrChar->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_RESTING))
        pCurrChar->SetByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);

    if (pCurrChar->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_CONTESTED_PVP))
        pCurrChar->SetContestedPvP();

    // Apply at_login requests
    if (pCurrChar->HasAtLoginFlag(AT_LOGIN_RESET_SPELLS))
    {
        pCurrChar->resetSpells();
        SendNotification(LANG_RESET_SPELLS);
    }

    if (pCurrChar->HasAtLoginFlag(AT_LOGIN_RESET_TALENTS))
    {
        pCurrChar->ResetTalents(true);
        pCurrChar->SendTalentsInfoData(false);              // original talents send already in to SendInitialPacketsBeforeAddToMap, resend reset state
        SendNotification(LANG_RESET_TALENTS);
    }

    if (pCurrChar->HasAtLoginFlag(AT_LOGIN_FIRST))
        pCurrChar->RemoveAtLoginFlag(AT_LOGIN_FIRST);

    // show time before shutdown if shutdown planned.
    if (sWorld->IsShuttingDown())
        sWorld->ShutdownMsg(true, pCurrChar);

    if (sWorld->getBoolConfig(CONFIG_ALL_TAXI_PATHS))
        pCurrChar->SetTaxiCheater(true);

    if (pCurrChar->isGameMaster())
        SendNotification(LANG_GM_ON);

    // Send CUF profiles (new raid UI 4.2)
    SendLoadCUFProfiles();

    // Hackfix Remove Talent spell - Remove Glyph spell
    pCurrChar->learnSpell(111621, false); // Reset Glyph
    pCurrChar->learnSpell(113873, false); // Reset Talent

    std::string IP_str = GetRemoteAddress();
    TC_LOG_INFO("char", "Account: %d (IP: %s) Login Character:[%s] (GUID: %u) Level: %d",
        GetAccountId(), IP_str.c_str(), pCurrChar->GetName(), pCurrChar->GetGUIDLow(), pCurrChar->getLevel());

    if (!pCurrChar->IsStandState() && !pCurrChar->HasUnitState(UNIT_STATE_STUNNED))
        pCurrChar->SetStandState(UNIT_STAND_STATE_STAND);

    m_playerLoading = false;

    sScriptMgr->OnPlayerLogin(pCurrChar);

    // this part of code needs for handle restored characters via web
    if (!sWorld->GetCharacterNameData(pCurrChar->GetGUIDLow()))
        sWorld->AddCharacterNameData(pCurrChar->GetGUIDLow(), pCurrChar->GetName(), pCurrChar->getGender(), pCurrChar->getRace(), pCurrChar->getClass(), pCurrChar->getLevel());
    else
        sWorld->UpdateCharacterNameData(pCurrChar->GetGUIDLow(), pCurrChar->GetName());

    delete holder;
}

void WorldSession::HandleSetFactionAtWar(WorldPacket & recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_SET_FACTION_ATWAR");

    uint8 repListID;
    recvData >> repListID;

    GetPlayer()->GetReputationMgr().SetAtWar(repListID, true);
}

void WorldSession::HandleSetLfgBonusFaction(WorldPacket & recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_SET_LFG_BONUS_FACTION");

    uint32 factionID;
    recvData >> factionID;

    _player->SetLfgBonusFaction(factionID);
}

void WorldSession::HandleUnsetFactionAtWar(WorldPacket & recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_UNSET_FACTION_ATWAR");

    uint8 repListID;
    recvData >> repListID;

    GetPlayer()->GetReputationMgr().SetAtWar(repListID, false);
}

//I think this function is never used :/ I dunno, but i guess this opcode not exists
void WorldSession::HandleSetFactionCheat(WorldPacket & /*recvData*/)
{
    TC_LOG_ERROR("network", "WORLD SESSION: HandleSetFactionCheat, not expected call, please report.");
    GetPlayer()->GetReputationMgr().SendStates();
}

void WorldSession::HandleTutorialFlag(WorldPacket & recvData)
{
    uint32 data;
    recvData >> data;

    uint8 index = uint8(data / 32);
    if (index >= MAX_ACCOUNT_TUTORIAL_VALUES)
        return;

    uint32 value = (data % 32);

    uint32 flag = GetTutorialInt(index);
    flag |= (1 << value);
    SetTutorialInt(index, flag);
}

void WorldSession::HandleTutorialClear(WorldPacket& /*recvData*/)
{
    for (uint8 i = 0; i < MAX_ACCOUNT_TUTORIAL_VALUES; ++i)
        SetTutorialInt(i, 0xFFFFFFFF);
}

void WorldSession::HandleTutorialReset(WorldPacket& /*recvData*/)
{
    for (uint8 i = 0; i < MAX_ACCOUNT_TUTORIAL_VALUES; ++i)
        SetTutorialInt(i, 0x00000000);
}

void WorldSession::HandleSetWatchedFactionOpcode(WorldPacket & recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_SET_WATCHED_FACTION");

    uint32 factionID;
    recvData >> factionID;

    GetPlayer()->SetUInt32Value(PLAYER_FIELD_WATCHED_FACTION_INDEX, factionID);
}

void WorldSession::HandleSetFactionInactiveOpcode(WorldPacket & recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_SET_FACTION_INACTIVE");
    uint32 repListid;
    recvData >> repListid;
    bool inactive = recvData.ReadBit();

    _player->GetReputationMgr().SetInactive(repListid, inactive);
}

void WorldSession::HandleShowingHelmOpcode(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "CMSG_SHOWING_HELM for %s", _player->GetName());
    recvData.ReadBit(); // on / off
    _player->ToggleFlag(PLAYER_FLAGS, PLAYER_FLAGS_HIDE_HELM);
}

void WorldSession::HandleShowingCloakOpcode(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "CMSG_SHOWING_CLOAK for %s", _player->GetName());
    recvData.ReadBit(); // on / off
    _player->ToggleFlag(PLAYER_FLAGS, PLAYER_FLAGS_HIDE_CLOAK);
}

void WorldSession::HandleCharRenameOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;
    std::string newName;

    uint32 len = recvData.ReadBits(6);
    recvData.ReadGuidMask<0, 5, 7, 2, 4, 1, 6, 3>(guid);
    recvData.ReadGuidBytes<3, 6, 7, 2, 4, 5, 0, 1>(guid);
    newName = recvData.ReadString(len);

    // prevent character rename to invalid name
    if (!normalizePlayerName(newName))
    {
        WorldPacket data(SMSG_CHAR_RENAME, 2);
        data.WriteBit(0);
        data.WriteBit(1);
        data << uint8(CHAR_NAME_NO_NAME);
        SendPacket(&data);
        return;
    }

    uint8 res = ObjectMgr::CheckPlayerName(newName, true);
    if (res != CHAR_NAME_SUCCESS)
    {
        WorldPacket data(SMSG_CHAR_RENAME, 1+8+(newName.size()+1));
        data.WriteBit(1);
        data.WriteGuidMask<5, 2, 7, 0, 6, 1, 3, 4>(guid);
        data.WriteBit(0);
        data.WriteBits(newName.length(), 6);
        data.WriteString(newName);
        data.WriteGuidBytes<1, 3, 4, 6, 0, 7, 2, 5>(guid);
        data << uint8(res);
        SendPacket(&data);
        return;
    }

    // check name limitations
    if (AccountMgr::IsPlayerAccount(GetSecurity()) && sObjectMgr->IsReservedName(newName))
    {
        WorldPacket data(SMSG_CHAR_RENAME, 2);
        data.WriteBit(0);
        data.WriteBit(1);
        data << uint8(CHAR_NAME_RESERVED);
        SendPacket(&data);
        return;
    }

    // Ensure that the character belongs to the current account, that rename at login is enabled
    // and that there is no character with the desired new name
    _charRenameCallback.SetParam(newName);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_FREE_NAME);

    stmt->setUInt32(0, GUID_LOPART(guid));
    stmt->setUInt32(1, GetAccountId());
    stmt->setUInt16(2, AT_LOGIN_RENAME);
    stmt->setUInt16(3, AT_LOGIN_RENAME);
    stmt->setString(4, newName);

    _charRenameCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
}

void WorldSession::HandleChangePlayerNameOpcodeCallBack(PreparedQueryResult result, std::string newName)
{
    if (!result)
    {
        WorldPacket data(SMSG_CHAR_RENAME, 2);
        data.WriteBit(0);
        data.WriteBit(1);
        data << uint8(CHAR_CREATE_ERROR);
        SendPacket(&data);
        return;
    }

    Field* fields = result->Fetch();

    uint32 guidLow      = fields[0].GetUInt32();
    std::string oldName = fields[1].GetString();

    uint64 guid = MAKE_NEW_GUID(guidLow, 0, HIGHGUID_PLAYER);

    // Update name and at_login flag in the db
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_NAME);

    stmt->setString(0, newName);
    stmt->setUInt16(1, AT_LOGIN_RENAME);
    stmt->setUInt32(2, guidLow);

    CharacterDatabase.Execute(stmt);

    // Removed declined name from db
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_DECLINED_NAME);

    stmt->setUInt32(0, guidLow);

    CharacterDatabase.Execute(stmt);

    // Logging
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_NAME_LOG);

    stmt->setUInt32(0, guidLow);
    stmt->setString(1, oldName);
    stmt->setString(2, newName);

    CharacterDatabase.Execute(stmt);

    TC_LOG_INFO("char", "Account: %d (IP: %s) Character:[%s] (guid:%u) Changed name to: %s", GetAccountId(), GetRemoteAddress().c_str(), oldName.c_str(), guidLow, newName.c_str());

    WorldPacket data(SMSG_CHAR_RENAME, 1+8+(newName.size()+1));
    data.WriteBit(1);
    data.WriteGuidMask<5, 2, 7, 0, 6, 1, 3, 4>(guid);
    data.WriteBit(0);
    data.WriteBits(newName.length(), 6);
    data.WriteString(newName);
    data.WriteGuidBytes<1, 3, 4, 6, 0, 7, 2, 5>(guid);
    data << uint8(RESPONSE_SUCCESS);
    SendPacket(&data);

    sWorld->UpdateCharacterNameData(guidLow, newName);
}

void WorldSession::HandleSetPlayerDeclinedNames(WorldPacket& recvData)
{
    if (!sWorld->getBoolConfig(CONFIG_DECLINED_NAMES_USED))
    {
        recvData.rfinish();
        //! 5.4.1
        WorldPacket data(SMSG_SET_PLAYER_DECLINED_NAMES_RESULT, 4+8);
        data << uint32(0);                                      // OK
        data.WriteBits(0, 9);   //epmpty guid + unk bit
        data.FlushBits();
        SendPacket(&data);
        return;
    }

    ObjectGuid guid;
    DeclinedName declinedname;

    recvData.ReadGuidMask<5, 1, 3, 0, 6, 4>(guid);

    uint32 declinedNamesLength[MAX_DECLINED_NAME_CASES];
    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
        declinedNamesLength[i] = recvData.ReadBits(7);

    recvData.ReadGuidMask<2, 7>(guid);
    recvData.ReadGuidBytes<6, 1, 2>(guid);

    bool decl_checl = true;
    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
    {
        declinedname.name[i] = recvData.ReadString(declinedNamesLength[i]);
        if (!normalizePlayerName(declinedname.name[i]))
            decl_checl = false;
    }

    recvData.ReadGuidBytes<3, 4, 0, 5, 7>(guid);

    if (!decl_checl)
    {
        //! 5.4.1
        WorldPacket data(SMSG_SET_PLAYER_DECLINED_NAMES_RESULT, 4+8);
        data << uint32(1);
        data.WriteBit(guid);
        data.WriteGuidMask<0, 1, 3, 7, 5, 4, 2, 6>(guid);
        data.FlushBits();
        data.WriteGuidBytes<6, 4, 5, 0, 2, 7, 1, 3>(guid);
        SendPacket(&data);
        return;
    }

    // not accept declined names for unsupported languages
    std::string name;
    if (!ObjectMgr::GetPlayerNameByGUID(guid, name))
    {
        //! 5.4.1
        WorldPacket data(SMSG_SET_PLAYER_DECLINED_NAMES_RESULT, 4+8);
        data << uint32(1);
        data.WriteBit(guid);
        data.WriteGuidMask<0, 1, 3, 7, 5, 4, 2, 6>(guid);
        data.FlushBits();
        data.WriteGuidBytes<6, 4, 5, 0, 2, 7, 1, 3>(guid);
        SendPacket(&data);
        return;
    }

    std::wstring wname;
    if (!Utf8toWStr(name, wname) || !isCyrillicCharacter(wname[0]))
    {
        //! 5.4.1
        WorldPacket data(SMSG_SET_PLAYER_DECLINED_NAMES_RESULT, 4+8);
        data << uint32(1);
        data.WriteBit(guid);
        data.WriteGuidMask<0, 1, 3, 7, 5, 4, 2, 6>(guid);
        data.FlushBits();
        data.WriteGuidBytes<6, 4, 5, 0, 2, 7, 1, 3>(guid);
        SendPacket(&data);
        return;
    }

    if (!ObjectMgr::CheckDeclinedNames(wname, declinedname))
    {
        //5.4.1
        WorldPacket data(SMSG_SET_PLAYER_DECLINED_NAMES_RESULT, 4+8);
        data << uint32(1);
        data.WriteBit(guid);
        data.WriteGuidMask<0, 1, 3, 7, 5, 4, 2, 6>(guid);
        data.FlushBits();
        data.WriteGuidBytes<6, 4, 5, 0, 2, 7, 1, 3>(guid);
        SendPacket(&data);
        return;
    }

    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
        CharacterDatabase.EscapeString(declinedname.name[i]);

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_DECLINED_NAME);
    stmt->setUInt32(0, GUID_LOPART(guid));
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_DECLINED_NAME);
    stmt->setUInt32(0, GUID_LOPART(guid));

    for (uint8 i = 0; i < 5; ++i)
        stmt->setString(i+1, declinedname.name[i]);

    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);

    //! 5.4.1
    WorldPacket data(SMSG_SET_PLAYER_DECLINED_NAMES_RESULT, 4+8);
    data << uint32(0);                                      // OK
    data.WriteBits(0, 9);   //epmpty guid + unk bit
    data.FlushBits();
    SendPacket(&data);
}

void WorldSession::HandleAlterAppearance(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "CMSG_ALTER_APPEARANCE");

    uint32 Hair, Color, FacialHair, SkinColor;
    recvData >> Color >> SkinColor >> FacialHair >> Hair;

    BarberShopStyleEntry const* bs_hair = sBarberShopStyleStore.LookupEntry(Hair);

    if (!bs_hair || bs_hair->type != 0 || bs_hair->race != _player->getRace() || bs_hair->gender != _player->getGender())
        return;

    BarberShopStyleEntry const* bs_facialHair = sBarberShopStyleStore.LookupEntry(FacialHair);

    if (!bs_facialHair || bs_facialHair->type != 2 || bs_facialHair->race != _player->getRace() || bs_facialHair->gender != _player->getGender())
        return;

    BarberShopStyleEntry const* bs_skinColor = sBarberShopStyleStore.LookupEntry(SkinColor);

    if (bs_skinColor && (bs_skinColor->type != 3 || bs_skinColor->race != _player->getRace() || bs_skinColor->gender != _player->getGender()))
        return;

    GameObject* go = _player->FindNearestGameObjectOfType(GAMEOBJECT_TYPE_BARBER_CHAIR, 5.0f);
    if (!go)
    {
        WorldPacket data(SMSG_BARBER_SHOP_RESULT, 4);
        data << uint32(2);
        SendPacket(&data);
        return;
    }

    if (_player->getStandState() != UNIT_STAND_STATE_SIT_LOW_CHAIR + go->GetGOInfo()->barberChair.chairheight)
    {
        WorldPacket data(SMSG_BARBER_SHOP_RESULT, 4);
        data << uint32(2);
        SendPacket(&data);
        return;
    }

    uint32 cost = _player->GetBarberShopCost(bs_hair->hair_id, Color, bs_facialHair->hair_id, bs_skinColor);

    // 0 - ok
    // 1, 3 - not enough money
    // 2 - you have to sit on barber chair
    if (!_player->HasEnoughMoney((uint64)cost))
    {
        WorldPacket data(SMSG_BARBER_SHOP_RESULT, 4);
        data << uint32(1);                                  // no money
        SendPacket(&data);
        return;
    }
    else
    {
        WorldPacket data(SMSG_BARBER_SHOP_RESULT, 4);
        data << uint32(0);                                  // ok
        SendPacket(&data);
    }

    _player->ModifyMoney(-int64(cost));                     // it isn't free
    _player->UpdateAchievementCriteria(CRITERIA_TYPE_GOLD_SPENT_AT_BARBER, cost);

    _player->SetByteValue(PLAYER_BYTES, 2, uint8(bs_hair->hair_id));
    _player->SetByteValue(PLAYER_BYTES, 3, uint8(Color));
    _player->SetByteValue(PLAYER_BYTES_2, 0, uint8(bs_facialHair->hair_id));
    if (bs_skinColor)
        _player->SetByteValue(PLAYER_BYTES, 0, uint8(bs_skinColor->hair_id));

    _player->UpdateAchievementCriteria(CRITERIA_TYPE_VISIT_BARBER_SHOP, 1);

    _player->SetStandState(0);                              // stand up
}

void WorldSession::HandleRemoveGlyph(WorldPacket & recvData)
{
    uint32 slot;
    recvData >> slot;

    if (slot >= MAX_GLYPH_SLOT_INDEX)
    {
        TC_LOG_DEBUG("network", "Client sent wrong glyph slot number in opcode CMSG_REMOVE_GLYPH %u", slot);
        return;
    }

    if (uint32 glyph = _player->GetGlyph(_player->GetActiveSpec(), slot))
    {
        if (GlyphPropertiesEntry const* gp = sGlyphPropertiesStore.LookupEntry(glyph))
        {
            _player->removeSpell(gp->SpellId);
            //_player->RemoveAurasDueToSpell(gp->SpellId);
            _player->SetGlyph(slot, 0);
            _player->SendTalentsInfoData(false);
        }
    }
}

//! 5.4.1
void WorldSession::HandleCharCustomize(WorldPacket& recvData)
{
    ObjectGuid guid;
    std::string newName;
    std::string oldName;

    uint8 gender, skin, face, hairStyle, hairColor, facialHair;
    recvData >> skin >> face >> gender >> facialHair >> hairStyle >> hairColor;

    recvData.ReadGuidMask<6, 2, 1>(guid);
    uint32 newNameLen = recvData.ReadBits(6);
    recvData.ReadGuidMask<3, 5, 4, 0, 7>(guid);

    recvData.ReadGuidBytes<0, 3, 5, 7>(guid);
    newName = recvData.ReadString(newNameLen);
    recvData.ReadGuidBytes<4, 6, 1, 2>(guid);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_AT_LOGIN);
    stmt->setUInt32(0, GUID_LOPART(guid));
    PreparedQueryResult result = CharacterDatabase.Query(stmt);
    if (!result)
    {
        WorldPacket data(SMSG_CHAR_CUSTOMIZE, 1);
        data.WriteBits(0, 8);               //null guid
        data << uint8(CHAR_CREATE_ERROR);
        SendPacket(&data);
        return;
    }

    Field* fields = result->Fetch();
    uint32 at_loginFlags = fields[0].GetUInt16();
    oldName = fields[1].GetString();

    if (!(at_loginFlags & AT_LOGIN_CUSTOMIZE))
    {
        WorldPacket data(SMSG_CHAR_CUSTOMIZE, 1);
        data.WriteBits(0, 8);
        data << uint8(CHAR_CREATE_ERROR);
        SendPacket(&data);
        return;
    }

    // prevent character rename to invalid name
    if (!normalizePlayerName(newName))
    {
        WorldPacket data(SMSG_CHAR_CUSTOMIZE, 1);
        data.WriteBits(0, 8);
        data << uint8(CHAR_NAME_NO_NAME);
        SendPacket(&data);
        return;
    }

    uint8 res = ObjectMgr::CheckPlayerName(newName, true);
    if (res != CHAR_NAME_SUCCESS)
    {
        WorldPacket data(SMSG_CHAR_CUSTOMIZE, 1);
        data.WriteBits(0, 8);
        data << uint8(res);
        SendPacket(&data);
        return;
    }

    // check name limitations
    if (AccountMgr::IsPlayerAccount(GetSecurity()) && sObjectMgr->IsReservedName(newName))
    {
        WorldPacket data(SMSG_CHAR_CUSTOMIZE, 1);
        data.WriteBits(0, 8);
        data << uint8(CHAR_NAME_RESERVED);
        SendPacket(&data);
        return;
    }

    // character with this name already exist
    if (uint64 newguid = ObjectMgr::GetPlayerGUIDByName(newName))
    {
        if (newguid != guid)
        {
            WorldPacket data(SMSG_CHAR_CUSTOMIZE, 1);
            data.WriteBits(0, 8);
            data << uint8(CHAR_CREATE_NAME_IN_USE);
            SendPacket(&data);
            return;
        }
    }

    uint16 atLoginFlag = oldName.compare(newName) != 0 ? AT_LOGIN_CUSTOMIZE | AT_LOGIN_RENAME : AT_LOGIN_CUSTOMIZE;
    Player::Customize(guid, gender, skin, face, hairStyle, hairColor, facialHair);

    // we need direct pexecute we need update data before charenum
    CharacterDatabase.DirectPExecute("UPDATE `characters` set `name` = '%s', at_login = at_login & ~ %u WHERE `guid` = '%u'", newName.c_str(), atLoginFlag, GUID_LOPART(guid));

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_DECLINED_NAME);
    stmt->setUInt32(0, GUID_LOPART(guid));
    CharacterDatabase.Execute(stmt);

    sWorld->UpdateCharacterNameData(GUID_LOPART(guid), newName, gender);

    WorldPacket data(SMSG_CHAR_CUSTOMIZE, 1+8+(newName.size()+1)+6);
    data.WriteGuidMask<5, 4, 2, 3, 1, 7, 0, 6>(guid);
    data.WriteGuidBytes<0, 4, 5>(guid);
    data << uint8(RESPONSE_SUCCESS);
    data << uint8(skin);
    data << uint8(hairStyle);
    data << uint8(face);
    data << uint8(facialHair);
    data << uint8(gender);
    data << uint8(hairColor);
    data.WriteGuidBytes<6, 3, 2, 1, 7>(guid);
    data.WriteBits(newName.length(), 6);
    data.WriteString(newName);
    SendPacket(&data);
}

void WorldSession::HandleEquipmentSetSave(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "CMSG_EQUIPMENT_SET_SAVE");

    uint32 index;
    recvData >> index;
    if (index >= MAX_EQUIPMENT_SET_INDEX)                    // client set slots amount
    {
        recvData.rfinish();
        return;
    }

    EquipmentSet eqSet;
    eqSet.state = EQUIPMENT_SET_NEW;

    ObjectGuid setGuid;
    ObjectGuid itemGuids[EQUIPMENT_SLOT_END];
    for (uint32 i = 0; i < EQUIPMENT_SLOT_END; ++i)
        recvData.ReadGuidMask<1, 5, 2, 0, 3, 6, 4, 7>(itemGuids[i]);

    recvData.ReadGuidMask<3, 7, 4, 6, 5>(setGuid);
    uint32 nameLen = recvData.ReadBits(8);
    recvData.ReadGuidMask<0>(setGuid);
    uint32 iconLen = recvData.ReadBits(9);
    recvData.ReadGuidMask<2, 1>(setGuid);

    recvData.ReadGuidBytes<6>(setGuid);
    eqSet.IconName = recvData.ReadString(iconLen);
    for (uint32 i = 0; i < EQUIPMENT_SLOT_END; ++i)
    {
        recvData.ReadGuidBytes<1, 3, 7, 2, 0, 5, 4, 6>(itemGuids[i]);

        // equipment manager sends "1" (as raw GUID) for slots set to "ignore" (don't touch slot at equip set)
        if (itemGuids[i] == 1)
        {
            // ignored slots saved as bit mask because we have no free special values for Items[i]
            eqSet.IgnoreMask |= 1 << i;
            continue;
        }

        Item* item = _player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);

        if (!item && itemGuids[i])                          // cheating check 1
            return;

        if (item && item->GetGUID() != itemGuids[i])        // cheating check 2
            return;

        eqSet.Items[i] = GUID_LOPART(itemGuids[i]);
    }

    recvData.ReadGuidBytes<0, 4, 1>(setGuid);
    eqSet.Name = recvData.ReadString(nameLen);
    recvData.ReadGuidBytes<7, 2, 5, 3>(setGuid);

    eqSet.Guid = setGuid;

    _player->SetEquipmentSet(index, eqSet);
}

void WorldSession::HandleEquipmentSetDelete(WorldPacket &recvData)
{
    TC_LOG_DEBUG("network", "CMSG_EQUIPMENT_SET_DELETE");

    ObjectGuid setGuid;
    recvData.ReadGuidMask<4, 6, 7, 3, 5, 1, 2, 0>(setGuid);
    recvData.ReadGuidBytes<3, 1, 2, 4, 5, 7, 0, 6>(setGuid);

    _player->DeleteEquipmentSet(setGuid);
}

void WorldSession::HandleEquipmentSetUse(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "CMSG_EQUIPMENT_SET_USE");

    ObjectGuid itemGuid[EQUIPMENT_SLOT_END];

    for (uint32 i = 0; i < EQUIPMENT_SLOT_END; ++i)
        recvData >> Unused<uint8>() >> Unused<uint8>();     // bag, slot

    for (uint32 i = 0; i < EQUIPMENT_SLOT_END; ++i)
        recvData.ReadGuidMask<3, 0, 5, 1, 7, 2, 4, 6>(itemGuid[i]);

    uint32 dword10 = recvData.ReadBits(2);
    for (uint8 i = 0; i < dword10; ++i)
    {
        recvData.ReadBit();
        recvData.ReadBit();
    }

    for (uint32 i = 0; i < EQUIPMENT_SLOT_END; ++i)
        recvData.ReadGuidBytes<4, 7, 3, 0, 1, 2, 6, 5>(itemGuid[i]);

    recvData.rfinish();

    if (sObjectMgr->IsPlayerInLogList(GetPlayer()))
    {
        sObjectMgr->DumpDupeConstant(GetPlayer());
        TC_LOG_DEBUG("dupe", "---WorldSession::HandleEquipmentSetUse");
    }

    EquipmentSlots startSlot = _player->isInCombat() ? EQUIPMENT_SLOT_MAINHAND : EQUIPMENT_SLOT_START;
    for (uint32 i = 0; i < EQUIPMENT_SLOT_END; ++i)
    {
        if (i == EQUIPMENT_SLOT_RANGED || i < uint32(startSlot))
            continue;

        TC_LOG_DEBUG("player.items", "Item " UI64FMTD ": srcbag %u, srcslot %u", (uint64)itemGuid[i]);

        // check if item slot is set to "ignored" (raw value == 1), must not be unequipped then
        if (itemGuid[i] == 1)
            continue;

        Item* item = _player->GetItemByGuid(itemGuid[i]);

        uint16 dstpos = i | (INVENTORY_SLOT_BAG_0 << 8);

        if (!item)
        {
            Item* uItem = _player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
            if (!uItem)
                continue;

            ItemPosCountVec sDest;
            InventoryResult msg = _player->CanStoreItem(NULL_BAG, NULL_SLOT, sDest, uItem, false);
            if (msg == EQUIP_ERR_OK)
            {
                _player->RemoveItem(INVENTORY_SLOT_BAG_0, i, true);
                _player->StoreItem(sDest, uItem, true);
            }
            else
                _player->SendEquipError(msg, uItem, NULL);

            continue;
        }

        if (item->GetPos() == dstpos)
            continue;

        _player->SwapItem(item->GetPos(), dstpos);
    }

    WorldPacket data(SMSG_EQUIPMENT_SET_USE_RESULT, 1);
    data << uint8(0);                                       // 4 - inventory is full, 0 - ok, else failed
    SendPacket(&data);
}

//! 5.4.1
void WorldSession::HandleCharFactionOrRaceChange(WorldPacket& recvData)
{
    // TODO: Move queries to prepared statements
    ObjectGuid guid;
    std::string newName;
    uint8 gender, skin, face, hairStyle, hairColor, facialHair, race = 0;

    recvData >> race >> gender;
    bool byte15 = recvData.ReadBit();
    recvData.ReadGuidMask<1, 2, 4, 5, 0>(guid);
    bool byte56 = recvData.ReadBit();
    bool byte11 = recvData.ReadBit();
    recvData.ReadGuidMask<3>(guid);
    bool byte53 = recvData.ReadBit();
    bool byte13 = recvData.ReadBit();
    recvData.ReadGuidMask<6>(guid);
    uint32 len = recvData.ReadBits(6);
    bool isFactionChange = recvData.ReadBit();
    recvData.ReadGuidMask<7>(guid);

    recvData.ReadGuidBytes<2, 4, 6, 7, 3>(guid);
    newName = recvData.ReadString(len);
    recvData.ReadGuidBytes<0, 1, 5>(guid);

    facialHair = byte56 ? recvData.read<uint8>() : 0;
    hairColor = byte53 ? recvData.read<uint8>() : 0;
    face = byte13 ? recvData.read<uint8>() : 0;
    skin = byte15 ? recvData.read<uint8>() : 0;
    hairStyle = byte11 ? recvData.read<uint8>() : 0;

    uint32 lowGuid = GUID_LOPART(guid);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_CLASS_LVL_AT_LOGIN);

    stmt->setUInt32(0, lowGuid);

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
    {
        WorldPacket data(SMSG_CHAR_FACTION_CHANGE, 5);
        data << uint8(CHAR_CREATE_ERROR);
        data << uint64(0);
        SendPacket(&data);
        return;
    }

    Field* fields = result->Fetch();
    uint8  oldRace          = fields[0].GetUInt8();
    uint32 playerClass      = uint32(fields[1].GetUInt8());
    uint32 level            = uint32(fields[2].GetUInt8());
    uint32 at_loginFlags    = fields[3].GetUInt16();
    uint32 used_loginFlag   = isFactionChange ? AT_LOGIN_CHANGE_FACTION : AT_LOGIN_CHANGE_RACE;
    char const* knownTitlesStr = fields[4].GetCString();
    std::string oldName = fields[5].GetString();

    BattlegroundTeamId oldTeam = BG_TEAM_ALLIANCE;

    // Search each faction is targeted
    switch (oldRace)
    {
        case RACE_ORC:
        case RACE_GOBLIN:
        case RACE_TAUREN:
        case RACE_UNDEAD_PLAYER:
        case RACE_TROLL:
        case RACE_BLOODELF:
        case RACE_PANDAREN_HORDE:
        	oldTeam = BG_TEAM_HORDE;
            break;
        default:
            break;
    }

    // We need to correct race when it's pandaren
    // Because client always send neutral ID
    if (race == RACE_PANDAREN_NEUTRAL)
    {
    	if (used_loginFlag == AT_LOGIN_CHANGE_RACE)
    		race = oldTeam == BG_TEAM_ALLIANCE ? RACE_PANDAREN_ALLI : RACE_PANDAREN_HORDE;
    	else
    		race = oldTeam == BG_TEAM_ALLIANCE ? RACE_PANDAREN_HORDE : RACE_PANDAREN_ALLI;
    }

    if (!sObjectMgr->GetPlayerInfo(race, playerClass))
    {
        WorldPacket data(SMSG_CHAR_FACTION_CHANGE, 5);
        data << uint8(CHAR_CREATE_ERROR);
        data << uint64(0);
        SendPacket(&data);
        return;
    }

    if (!(at_loginFlags & used_loginFlag))
    {
        WorldPacket data(SMSG_CHAR_FACTION_CHANGE, 5);
        data << uint8(CHAR_CREATE_ERROR);
        data << uint64(0);
        SendPacket(&data);
        return;
    }

    if (AccountMgr::IsPlayerAccount(GetSecurity()))
    {
        uint32 raceMaskDisabled = sWorld->getIntConfig(CONFIG_CHARACTER_CREATING_DISABLED_RACEMASK);
        if ((1 << (race - 1)) & raceMaskDisabled)
        {
            WorldPacket data(SMSG_CHAR_FACTION_CHANGE, 5);
            data << uint8(CHAR_CREATE_ERROR);
            data << uint64(0);
            SendPacket(&data);
            return;
        }
    }

    // prevent character rename to invalid name
    if (!normalizePlayerName(newName))
    {
        WorldPacket data(SMSG_CHAR_FACTION_CHANGE, 5);
        data << uint8(CHAR_NAME_NO_NAME);
        data << uint64(0);
        SendPacket(&data);
        return;
    }

    uint8 res = ObjectMgr::CheckPlayerName(newName, true);
    if (res != CHAR_NAME_SUCCESS)
    {
        WorldPacket data(SMSG_CHAR_FACTION_CHANGE, 5);
        data << uint8(res);
        data << uint64(0);
        SendPacket(&data);
        return;
    }

    // check name limitations
    if (AccountMgr::IsPlayerAccount(GetSecurity()) && sObjectMgr->IsReservedName(newName))
    {
        WorldPacket data(SMSG_CHAR_FACTION_CHANGE, 5);
        data << uint8(CHAR_NAME_RESERVED);
        data << uint64(0);
        SendPacket(&data);
        return;
    }

    // character with this name already exist
    if (uint64 newguid = ObjectMgr::GetPlayerGUIDByName(newName))
    {
        if (newguid != guid)
        {
            WorldPacket data(SMSG_CHAR_FACTION_CHANGE, 5);
            data << uint8(CHAR_CREATE_NAME_IN_USE);
            data << uint64(0);
            SendPacket(&data);
            return;
        }
    }
    
    CharacterDatabase.EscapeString(newName);

    if (newName.compare(oldName) != 0)
        used_loginFlag |= AT_LOGIN_RENAME;

    Player::Customize(guid, gender, skin, face, hairStyle, hairColor, facialHair);
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
   
    // here's need directpexecute because need update data before charenum opcode
    CharacterDatabase.DirectPExecute("UPDATE characters SET `name` = '%s', `race` = '%u', `at_login` = at_login & ~ %u WHERE `guid` = '%u'", newName.c_str(), race, used_loginFlag, lowGuid);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_FACTION_OR_RACE_LOG);
    stmt->setUInt32(0, lowGuid);
    stmt->setUInt32(1, GetAccountId());
    stmt->setUInt8 (2, oldRace);
    stmt->setUInt8 (3, race);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_DECLINED_NAME);
    stmt->setUInt32(0, lowGuid);
    trans->Append(stmt);

    sWorld->UpdateCharacterNameData(GUID_LOPART(guid), newName, gender, race);

    BattlegroundTeamId team = BG_TEAM_ALLIANCE;

    // Search each faction is targeted
    switch (race)
    {
        case RACE_ORC:
        case RACE_GOBLIN:
        case RACE_TAUREN:
        case RACE_UNDEAD_PLAYER:
        case RACE_TROLL:
        case RACE_BLOODELF:
        case RACE_PANDAREN_HORDE:
            team = BG_TEAM_HORDE;
            break;
        default:
            break;
    }

    // Switch Languages
    // delete all languages first
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SKILL_LANGUAGES);
    stmt->setUInt32(0, lowGuid);
    trans->Append(stmt);

    // Now add them back
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_SKILL_LANGUAGE);
    stmt->setUInt32(0, lowGuid);

    // Faction specific languages
    if (team == BG_TEAM_HORDE)
        stmt->setUInt16(1, 109);
    else
        stmt->setUInt16(1, 98);

    trans->Append(stmt);

    // Race specific languages

    if (race != RACE_ORC && race != RACE_HUMAN)
    {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_SKILL_LANGUAGE);
        stmt->setUInt32(0, lowGuid);

        switch (race)
        {
            case RACE_DWARF:
                stmt->setUInt16(1, 111);
                break;
            case RACE_DRAENEI:
                stmt->setUInt16(1, 759);
                break;
            case RACE_GNOME:
                stmt->setUInt16(1, 313);
                break;
            case RACE_NIGHTELF:
                stmt->setUInt16(1, 113);
                break;
            case RACE_WORGEN:
                stmt->setUInt16(1, 791);
                break;
            case RACE_UNDEAD_PLAYER:
                stmt->setUInt16(1, 673);
                break;
            case RACE_TAUREN:
                stmt->setUInt16(1, 115);
                break;
            case RACE_TROLL:
                stmt->setUInt16(1, 315);
                break;
            case RACE_BLOODELF:
                stmt->setUInt16(1, 137);
                break;
            case RACE_GOBLIN:
                stmt->setUInt16(1, 792);
                break;
            case RACE_PANDAREN_ALLI:
                stmt->setUInt16(1, 906);
                break;
            case RACE_PANDAREN_HORDE:
                stmt->setUInt16(1, 907);
                break;
            case RACE_PANDAREN_NEUTRAL:
                stmt->setUInt16(1, 905);
                break;
        }

        trans->Append(stmt);
    }

    if (used_loginFlag & AT_LOGIN_CHANGE_FACTION)
    {
        // Delete all Flypaths
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_TAXI_PATH);
        stmt->setUInt32(0, lowGuid);
        trans->Append(stmt);

        if (level > 7)
        {
            // Update Taxi path
            // this doesn't seem to be 100% blizzlike... but it can't really be helped.
            std::ostringstream taximaskstream;
            uint32 numFullTaximasks = level / 7;
            if (numFullTaximasks > 11)
                numFullTaximasks = 11;
            if (team == BG_TEAM_ALLIANCE)
            {
                if (playerClass != CLASS_DEATH_KNIGHT)
                {
                    for (uint8 i = 0; i < numFullTaximasks; ++i)
                        taximaskstream << uint32(sAllianceTaxiNodesMask[i]) << ' ';
                }
                else
                {
                    for (uint8 i = 0; i < numFullTaximasks; ++i)
                        taximaskstream << uint32(sAllianceTaxiNodesMask[i] | sDeathKnightTaxiNodesMask[i]) << ' ';
                }
            }
            else
            {
                if (playerClass != CLASS_DEATH_KNIGHT)
                {
                    for (uint8 i = 0; i < numFullTaximasks; ++i)
                        taximaskstream << uint32(sHordeTaxiNodesMask[i]) << ' ';
                }
                else
                {
                    for (uint8 i = 0; i < numFullTaximasks; ++i)
                        taximaskstream << uint32(sHordeTaxiNodesMask[i] | sDeathKnightTaxiNodesMask[i]) << ' ';
                }
            }

            uint32 numEmptyTaximasks = 11 - numFullTaximasks;
            for (uint8 i = 0; i < numEmptyTaximasks; ++i)
                taximaskstream << "0 ";
            taximaskstream << '0';
            std::string taximask = taximaskstream.str();

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_TAXIMASK);
            stmt->setString(0, taximask);
            stmt->setUInt32(1, lowGuid);
            trans->Append(stmt);
        }

        // Delete all current quests
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_QUESTSTATUS);
        stmt->setUInt32(0, GUID_LOPART(guid));
        trans->Append(stmt);

        // Delete record of the faction old completed quests
        {
            std::ostringstream quests;
            ObjectMgr::QuestMap const& qTemplates = sObjectMgr->GetQuestTemplates();
            for (ObjectMgr::QuestMap::const_iterator iter = qTemplates.begin(); iter != qTemplates.end(); ++iter)
            {
                Quest *qinfo = iter->second;
                uint32 requiredRaces = qinfo->GetRequiredRaces();
                if (team == BG_TEAM_ALLIANCE)
                {
                    if (requiredRaces & RACEMASK_ALLIANCE)
                    {
                        quests << uint32(qinfo->GetQuestId());
                        quests << ',';
                    }
                }
                else // if (team == BG_TEAM_HORDE)
                {
                    if (requiredRaces & RACEMASK_HORDE)
                    {
                        quests << uint32(qinfo->GetQuestId());
                        quests << ',';
                    }
                }
            }

            std::string questsStr = quests.str();
            questsStr = questsStr.substr(0, questsStr.length() - 1);

            if (!questsStr.empty())
                trans->PAppend("DELETE FROM `character_queststatus_rewarded` WHERE guid='%u' AND quest IN (%s)", lowGuid, questsStr.c_str());
        }

        if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD))
        {
            // Reset guild
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUILD_MEMBER);

            stmt->setUInt32(0, lowGuid);

            PreparedQueryResult result = CharacterDatabase.Query(stmt);
            if (result)
                if (Guild* guild = sGuildMgr->GetGuildById((result->Fetch()[0]).GetUInt32()))
                    guild->DeleteMember(MAKE_NEW_GUID(lowGuid, 0, HIGHGUID_PLAYER));
        }

        if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_ADD_FRIEND))
        {
            // Delete Friend List
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SOCIAL_BY_GUID);
            stmt->setUInt32(0, lowGuid);
            trans->Append(stmt);

            stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SOCIAL_BY_FRIEND);
            stmt->setUInt32(0, lowGuid);
            trans->Append(stmt);

        }

        // Reset homebind and position
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_HOMEBIND);
        stmt->setUInt32(0, lowGuid);
        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_PLAYER_HOMEBIND);
        stmt->setUInt32(0, lowGuid);
        if (team == BG_TEAM_ALLIANCE)
        {
            stmt->setUInt16(1, 0);
            stmt->setUInt16(2, 1519);
            stmt->setFloat (3, -8867.68f);
            stmt->setFloat (4, 673.373f);
            stmt->setFloat (5, 97.9034f);
            Player::SavePositionInDB(0, -8867.68f, 673.373f, 97.9034f, 0.0f, 1519, lowGuid);
        }
        else
        {
            stmt->setUInt16(1, 1);
            stmt->setUInt16(2, 1637);
            stmt->setFloat (3, 1633.33f);
            stmt->setFloat (4, -4439.11f);
            stmt->setFloat (5, 15.7588f);
            Player::SavePositionInDB(1, 1633.33f, -4439.11f, 15.7588f, 0.0f, 1637, lowGuid);
        }
        trans->Append(stmt);

        // Achievement conversion
        for (std::map<uint32, uint32>::const_iterator it = sObjectMgr->FactionChange_Achievements.begin(); it != sObjectMgr->FactionChange_Achievements.end(); ++it)
        {
            uint32 achiev_alliance = it->first;
            uint32 achiev_horde = it->second;

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_ACHIEVEMENT_BY_ACHIEVEMENT);
            stmt->setUInt16(0, uint16(team == BG_TEAM_ALLIANCE ? achiev_alliance : achiev_horde));
            stmt->setUInt32(1, lowGuid);
            trans->Append(stmt);

            stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_ACHIEVEMENT);
            stmt->setUInt16(0, uint16(team == BG_TEAM_ALLIANCE ? achiev_alliance : achiev_horde));
            stmt->setUInt16(1, uint16(team == BG_TEAM_ALLIANCE ? achiev_horde : achiev_alliance));
            stmt->setUInt32(2, lowGuid);
            trans->Append(stmt);
        }

        // Item conversion
        for (std::map<uint32, uint32>::const_iterator it = sObjectMgr->FactionChange_Items.begin(); it != sObjectMgr->FactionChange_Items.end(); ++it)
        {
            uint32 item_alliance = it->first;
            uint32 item_horde = it->second;

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_INVENTORY_FACTION_CHANGE);
            stmt->setUInt32(0, (team == BG_TEAM_ALLIANCE ? item_alliance : item_horde));
            stmt->setUInt32(1, (team == BG_TEAM_ALLIANCE ? item_horde : item_alliance));
            stmt->setUInt32(2, guid);
            trans->Append(stmt);
        }

        // Spell conversion
        for (std::map<uint32, uint32>::const_iterator it = sObjectMgr->FactionChange_Spells.begin(); it != sObjectMgr->FactionChange_Spells.end(); ++it)
        {
            uint32 spell_alliance = it->first;
            uint32 spell_horde = it->second;

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SPELL_BY_SPELL);
            stmt->setUInt32(0, (team == BG_TEAM_ALLIANCE ? spell_alliance : spell_horde));
            stmt->setUInt32(1, lowGuid);
            trans->Append(stmt);

            stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_SPELL_FACTION_CHANGE);
            stmt->setUInt32(0, (team == BG_TEAM_ALLIANCE ? spell_alliance : spell_horde));
            stmt->setUInt32(1, (team == BG_TEAM_ALLIANCE ? spell_horde : spell_alliance));
            stmt->setUInt32(2, lowGuid);
            trans->Append(stmt);
        }

        // Reputation conversion
        for (std::map<uint32, uint32>::const_iterator it = sObjectMgr->FactionChange_Reputation.begin(); it != sObjectMgr->FactionChange_Reputation.end(); ++it)
        {
            uint32 reputation_alliance = it->first;
            uint32 reputation_horde = it->second;

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_REP_BY_FACTION);
            stmt->setUInt32(0, uint16(team == BG_TEAM_ALLIANCE ? reputation_alliance : reputation_horde));
            stmt->setUInt32(1, lowGuid);
            trans->Append(stmt);

            stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_REP_FACTION_CHANGE);
            stmt->setUInt16(0, uint16(team == BG_TEAM_ALLIANCE ? reputation_alliance : reputation_horde));
            stmt->setUInt16(1, uint16(team == BG_TEAM_ALLIANCE ? reputation_horde : reputation_alliance));
            stmt->setUInt32(2, lowGuid);
            trans->Append(stmt);
        }
        // Title conversion
        if (knownTitlesStr)
        {
            const uint32 ktcount = KNOWN_TITLES_SIZE * 2;
            uint32 knownTitles[ktcount];
            Tokenizer tokens(knownTitlesStr, ' ', ktcount);

            if (tokens.size() != ktcount)
                return;

            for (uint32 index = 0; index < ktcount; ++index)
                knownTitles[index] = atol(tokens[index]);

            for (std::map<uint32, uint32>::const_iterator it = sObjectMgr->FactionChange_Titles.begin(); it != sObjectMgr->FactionChange_Titles.end(); ++it)
            {
                uint32 title_alliance = it->first;
                uint32 title_horde = it->second;

                CharTitlesEntry const* atitleInfo = sCharTitlesStore.LookupEntry(title_alliance);
                CharTitlesEntry const* htitleInfo = sCharTitlesStore.LookupEntry(title_horde);
                // new team
                if (team == BG_TEAM_ALLIANCE)
                {
                    uint32 bitIndex = htitleInfo->bit_index;
                    uint32 index = bitIndex / 32;
                    uint32 old_flag = 1 << (bitIndex % 32);
                    uint32 new_flag = 1 << (atitleInfo->bit_index % 32);
                    if (knownTitles[index] & old_flag)
                    {
                        knownTitles[index] &= ~old_flag;
                        // use index of the new title
                        knownTitles[atitleInfo->bit_index / 32] |= new_flag;
                    }
                }
                else
                {
                    uint32 bitIndex = atitleInfo->bit_index;
                    uint32 index = bitIndex / 32;
                    uint32 old_flag = 1 << (bitIndex % 32);
                    uint32 new_flag = 1 << (htitleInfo->bit_index % 32);
                    if (knownTitles[index] & old_flag)
                    {
                        knownTitles[index] &= ~old_flag;
                        // use index of the new title
                        knownTitles[htitleInfo->bit_index / 32] |= new_flag;
                    }
                }

                std::ostringstream ss;
                for (uint32 index = 0; index < ktcount; ++index)
                    ss << knownTitles[index] << ' ';

                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_TITLES_FACTION_CHANGE);
                stmt->setString(0, ss.str().c_str());
                stmt->setUInt32(1, lowGuid);
                trans->Append(stmt);

                // unset any currently chosen title
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_RES_CHAR_TITLES_FACTION_CHANGE);
                stmt->setUInt32(0, lowGuid);
                trans->Append(stmt);
            }
        }

    }

    CharacterDatabase.CommitTransaction(trans);

    std::string IP_str = GetRemoteAddress();
    TC_LOG_DEBUG("server", "Account: %d (IP: %s), Character guid: %u Change Race/Faction to: %s", GetAccountId(), IP_str.c_str(), lowGuid, newName.c_str());

    WorldPacket data(SMSG_CHAR_FACTION_CHANGE, 1 + 8 + (newName.size() + 1) + 1 + 1 + 1 + 1 + 1 + 1 + 1);
    data << uint8(RESPONSE_SUCCESS);
    data << uint64(guid);
    data << newName;
    data << uint8(gender);
    data << uint8(skin);
    data << uint8(face);
    data << uint8(hairStyle);
    data << uint8(hairColor);
    data << uint8(facialHair);
    data << uint8(race);
    SendPacket(&data);
}

//! 5.4.1
void WorldSession::HandleRandomizeCharNameOpcode(WorldPacket& recvData)
{
    uint8 gender, race;

    recvData >> gender;
    recvData >> race;
    
    if (!Player::IsValidRace(race))
    {
        TC_LOG_ERROR("server", "Invalid race (%u) sent by accountId: %u", race, GetAccountId());
        return;
    }

    if (!Player::IsValidGender(gender))
    {
        TC_LOG_ERROR("server", "Invalid gender (%u) sent by accountId: %u", gender, GetAccountId());
        return;
    }

    std::string const* name = GetRandomCharacterName(race, gender);
    WorldPacket data(SMSG_RANDOMIZE_CHAR_NAME, 10);
    data.WriteBit(0); // unk
    data.WriteBits(name->size(), 6);
    data.WriteString(*name);
    SendPacket(&data);
}

//! 5.4.1
void WorldSession::HandleReorderCharacters(WorldPacket& recvData)
{
    uint32 charactersCount = recvData.ReadBits(9);

    std::vector<ObjectGuid> guids(charactersCount);
    uint8 position;

    for (uint8 i = 0; i < charactersCount; ++i)
        recvData.ReadGuidMask<6, 2, 7, 0, 4, 3, 5, 1>(guids[i]);

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    for (uint8 i = 0; i < charactersCount; ++i)
    {
        recvData.ReadGuidBytes<0>(guids[i]);
        recvData >> position;
        recvData.ReadGuidBytes<6, 4, 7, 1, 5, 3, 2>(guids[i]);

        //! WARNING!!!
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_LIST_SLOT);
        stmt->setUInt8(0, position);
        stmt->setUInt32(1, GUID_LOPART(guids[i]));
        trans->Append(stmt);
    }

    CharacterDatabase.CommitTransaction(trans);
}

void WorldSession::HandleSaveCUFProfiles(WorldPacket& recvData)
{
    uint32 profilesCount = recvData.ReadBits(19);

    if (profilesCount > MAX_CUF_PROFILES)
    {
        recvData.rfinish();
        return;
    }

    CUFProfile* profiles[MAX_CUF_PROFILES];
    uint8 strLengths[MAX_CUF_PROFILES];

    for (uint8 i = 0; i < profilesCount; ++i)
    {
        profiles[i] = new CUFProfile;
        profiles[i]->setOptionBit(CUF_OPT_GROUPS_TOGETHER_HORIZONT, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_ONLY_DISPELLABLE_EFFECTS, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_AUTO_IN_GROUPS_5PPL, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_THREAT_INDICATOR, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_SHOW_PETS, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_GROUPS_TOGETHER, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_RESOURCE_INDICATOR, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_AUTO_IN_GROUPS_15PPL, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_AUTO_FOR_SPEC_1, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_UNK_156, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_AUTO_IN_PVP, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_AUTO_IN_GROUPS_40PPL, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_AUTO_IN_GROUPS_2PPL, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_AUTO_FOR_SPEC_2, recvData.ReadBit());
        strLengths[i] = (uint8)recvData.ReadBits(7);
        profiles[i]->setOptionBit(CUF_OPT_SHOW_MAIN_TANK_ASSIST, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_UNK_157, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_AUTO_IN_GROUPS_10PPL, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_SHOW_NEG_EFFECTS, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_AUTO_IN_GROUPS_3PPL, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_UNK_145, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_AUTO_IN_GROUPS_25PPL, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_CLASS_COLORS, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_AUTO_IN_PVE, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_PREDICTED_HEALING, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_OPT_SHOW_BORDERS, recvData.ReadBit());
    }

    for (uint8 i = 0; i < profilesCount; ++i)
    {
        recvData >> profiles[i]->Unk146;
        recvData >> profiles[i]->Unk148;
        recvData >> profiles[i]->Unk150;
        recvData >> profiles[i]->frameHeight;
        recvData >> profiles[i]->Unk154;
        recvData >> profiles[i]->showHealthText;
        recvData >> profiles[i]->Unk152;
        recvData >> profiles[i]->frameWidth;
        recvData >> profiles[i]->sortBy;
        profiles[i]->profileName = recvData.ReadString(strLengths[i]);
        recvData >> profiles[i]->Unk147;

        // save current profile
        _player->SaveCUFProfile(i, profiles[i]);
    }

    // clear other profiles
    for (uint8 i = profilesCount; i < MAX_CUF_PROFILES; ++i)
        _player->SaveCUFProfile(i, NULL);
}

void WorldSession::SendLoadCUFProfiles()
{
    uint8 profilesCount = _player->GetCUFProfilesCount();

    WorldPacket data(SMSG_LOAD_CUF_PROFILES);
    ByteBuffer buffer;

    data.WriteBits(profilesCount, 19);
    for (uint8 i = 0; i < profilesCount; ++i)
    {
        CUFProfile * profile = _player->GetCUFProfile(i);

        if (!profile)
            continue;

        data.WriteBit(profile->getOptionBit(CUF_OPT_GROUPS_TOGETHER));
        data.WriteBit(profile->getOptionBit(CUF_OPT_GROUPS_TOGETHER_HORIZONT));
        data.WriteBit(profile->getOptionBit(CUF_OPT_AUTO_FOR_SPEC_2));
        data.WriteBit(profile->getOptionBit(CUF_OPT_AUTO_IN_PVP));
        data.WriteBit(profile->getOptionBit(CUF_OPT_AUTO_IN_PVE));
        data.WriteBit(profile->getOptionBit(CUF_OPT_AUTO_FOR_SPEC_1));
        data.WriteBit(profile->getOptionBit(CUF_OPT_UNK_157));
        data.WriteBit(profile->getOptionBit(CUF_OPT_ONLY_DISPELLABLE_EFFECTS));
        data.WriteBit(profile->getOptionBit(CUF_OPT_AUTO_IN_GROUPS_25PPL));
        data.WriteBit(profile->getOptionBit(CUF_OPT_SHOW_BORDERS));
        data.WriteBit(profile->getOptionBit(CUF_OPT_UNK_156));
        data.WriteBit(profile->getOptionBit(CUF_OPT_AUTO_IN_GROUPS_5PPL));
        data.WriteBit(profile->getOptionBit(CUF_OPT_RESOURCE_INDICATOR));
        data.WriteBit(profile->getOptionBit(CUF_OPT_SHOW_MAIN_TANK_ASSIST));
        data.WriteBit(profile->getOptionBit(CUF_OPT_AUTO_IN_GROUPS_2PPL));
        data.WriteBit(profile->getOptionBit(CUF_OPT_AUTO_IN_GROUPS_40PPL));
        data.WriteBit(profile->getOptionBit(CUF_OPT_SHOW_PETS));
        data.WriteBit(profile->getOptionBit(CUF_OPT_SHOW_NEG_EFFECTS));
        data.WriteBit(profile->getOptionBit(CUF_OPT_THREAT_INDICATOR));
        data.WriteBit(profile->getOptionBit(CUF_OPT_AUTO_IN_GROUPS_10PPL));
        data.WriteBits(profile->profileName.size(), 7);
        data.WriteBit(profile->getOptionBit(CUF_OPT_AUTO_IN_GROUPS_15PPL));
        data.WriteBit(profile->getOptionBit(CUF_OPT_UNK_145));
        data.WriteBit(profile->getOptionBit(CUF_OPT_CLASS_COLORS));
        data.WriteBit(profile->getOptionBit(CUF_OPT_PREDICTED_HEALING));
        data.WriteBit(profile->getOptionBit(CUF_OPT_AUTO_IN_GROUPS_3PPL));

        buffer.WriteString(profile->profileName);
        buffer << uint16(profile->Unk152);
        buffer << uint8(profile->showHealthText);
        buffer << uint8(profile->Unk148);
        buffer << uint16(profile->frameWidth);
        buffer << uint8(profile->Unk146);
        buffer << uint8(profile->sortBy);
        buffer << uint16(profile->Unk154);
        buffer << uint16(profile->frameHeight);
        buffer << uint16(profile->Unk150);
        buffer << uint8(profile->Unk147);
    }

    data.FlushBits();
    data.append(buffer);
    SendPacket(&data);
}
