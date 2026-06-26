/*
 * DestinyCore 7.3.5 - AI Bot Bridge (Ultimate Commander Edition)
 * 完美修复头文件、正则表达式抓取、多频道路由与并发控制
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "Group.h"
#include "Guild.h" 
#include "GuildMgr.h"          // 用于获取公会名称
#include "World.h"             // 引入 sWorld 和 SessionMap
#include "WorldSession.h"      // 引入 WorldSession 解决 ChatHandler 连带报错
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "QuestDef.h"
#include "ObjectMgr.h"
#include <boost/asio.hpp>
#include <sstream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <vector>

struct AIReplyMsg {
    ObjectGuid botGuid;
    std::string text;
    uint32 chatType;
    ObjectGuid senderGuid;
};

std::mutex g_aiReplyMutex;
std::vector<AIReplyMsg> g_aiReplyQueue;

class cs_mod_ollama : public PlayerScript
{
public:
    cs_mod_ollama() : PlayerScript("cs_mod_ollama") { }

    std::string DecodeUnicodeEscape(const std::string& input) const
    {
        std::string output;
        for (size_t i = 0; i < input.length(); )
        {
            if (input[i] == '\\' && i + 1 < input.length() && input[i+1] == 'u' && i + 5 < input.length())
            {
                std::string hexStr = input.substr(i + 2, 4);
                try {
                    int codePoint = std::stoi(hexStr, nullptr, 16);
                    if (codePoint <= 0x7F) { output += static_cast<char>(codePoint); } 
                    else if (codePoint <= 0x7FF) {
                        output += static_cast<char>(0xC0 | ((codePoint >> 6) & 0x1F));
                        output += static_cast<char>(0x80 | (codePoint & 0x3F));
                    } else if (codePoint <= 0xFFFF) {
                        output += static_cast<char>(0xE0 | ((codePoint >> 12) & 0x0F));
                        output += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
                        output += static_cast<char>(0x80 | (codePoint & 0x3F));
                    }
                } catch (...) { output += "\\u" + hexStr; }
                i += 6;
            }
            else if (input[i] == '\\' && i + 1 < input.length() && input[i+1] == 'n') { output += '\n'; i += 2; }
            else if (input[i] == '\\' && i + 1 < input.length() && input[i+1] == '"') { output += '"'; i += 2; }
            else { output += input[i]; i++; }
        }
        return output;
    }

    std::string EscapeJsonString(const std::string& input) const
    {
        std::ostringstream ss;
        for (char ch : input)
        {
            unsigned char c = (unsigned char)ch;
            if (c == '"') ss << "\\\"";
            else if (c == '\\') ss << "\\\\";
            else if (c == '\b') ss << "\\b";
            else if (c == '\f') ss << "\\f";
            else if (c == '\n') ss << "\\n";
            else if (c == '\r') ss << "\\r";
            else if (c == '\t') ss << "\\t";
            else if (c < 32) { }
            else ss << ch;
        }
        return ss.str();
    }

    std::string GetClassName(uint8 classId) const
    {
        switch (classId)
        {
            case 1: return "warrior"; case 2: return "paladin"; case 3: return "hunter";
            case 4: return "rogue"; case 5: return "priest"; case 6: return "death knight";
            case 7: return "shaman"; case 8: return "mage"; case 9: return "warlock";
            case 10: return "monk"; case 11: return "druid"; case 12: return "demon hunter";
            default: return "unknown";
        }
    }

    std::string GetRaceName(uint8 raceId) const
    {
        switch (raceId)
        {
            case 1: return "Human"; case 2: return "Orc"; case 3: return "Dwarf";
            case 4: return "Night Elf"; case 5: return "Undead"; case 6: return "Tauren";
            case 7: return "Gnome"; case 8: return "Troll"; case 9: return "Goblin";
            case 10: return "Blood Elf"; case 11: return "Draenei"; case 22: return "Worgen";
            case 24: case 25: case 26: return "Pandaren";
            default: return "Unknown";
        }
    }

    // 包含 7.3.5 全职业 36 系专精 ID，并带有职业兜底机制
    std::string GetSpecName(uint32 specId, uint8 classId) const
    {
        switch (specId)
        {
            // 法师 Mage
            case 62: return "arcane"; case 63: return "fire"; case 64: return "frost";
            // 圣骑士 Paladin
            case 65: return "holy"; case 66: return "protection"; case 70: return "retribution";
            // 战士 Warrior
            case 71: return "arms"; case 72: return "fury"; case 73: return "protection";
            // 德鲁伊 Druid
            case 102: return "balance"; case 103: return "feral combat"; case 104: return "guardian"; case 105: return "restoration";
            // 死亡骑士 DK
            case 250: return "blood"; case 251: return "frost"; case 252: return "unholy";
            // 猎人 Hunter
            case 253: return "beast mastery"; case 254: return "marksmanship"; case 255: return "survival";
            // 牧师 Priest
            case 256: return "discipline"; case 257: return "holy"; case 258: return "shadow";
            // 盗贼 Rogue (7.3.5 的狂徒对应 3.3.5 的战斗，这里传 combat 以兼容你的 Python 设定)
            case 259: return "assassination"; case 260: return "combat"; case 261: return "subtlety";
            // 萨满 Shaman
            case 262: return "elemental"; case 263: return "enhancement"; case 264: return "restoration";
            // 术士 Warlock
            case 265: return "affliction"; case 266: return "demonology"; case 267: return "destruction";
            // 武僧 Monk
            case 268: return "brewmaster"; case 269: return "windwalker"; case 270: return "mistweaver";
            // 恶魔猎手 DH
            case 577: return "havoc"; case 581: return "vengeance";
            default: 
                // 【终极兜底】：如果机器人还没学专精（或者核心返回 0），根据职业强制给一个默认天赋，防止 AI 懵逼
                switch (classId) {
                    case 1: return "arms"; case 2: return "holy"; case 3: return "marksmanship";
                    case 4: return "combat"; case 5: return "holy"; case 6: return "unholy";
                    case 7: return "restoration"; case 8: return "frost"; case 9: return "destruction";
                    case 10: return "windwalker"; case 11: return "restoration"; case 12: return "havoc";
                    default: return "unknown";
                }
        }
    }

    std::string GetGuildNameSafe(Player* p) const
    {
        if (!p) return "无";
        if (uint32 guildId = p->GetGuildId())
        {
            if (Guild* guild = sGuildMgr->GetGuildById(guildId))
                return guild->GetName();
        }
        return "无";
    }

    std::string BuildEnvironmentSnapshot(Player* bot, Player* sender, const std::string& msg, bool isWhisper)
    {
        std::ostringstream prompt;
        std::string botClass = GetClassName(bot->getClass());
        std::string senderClass = sender ? GetClassName(sender->getClass()) : "unknown";
        std::string botGuild = GetGuildNameSafe(bot);
        std::string senderGuild = GetGuildNameSafe(sender);

       prompt << "你是一名魔兽世界军团再临(7.3.5)版本的玩家。在当前版本中，“天赋”就是指你的“专精”(Specialization)。当别人问你的专精或天赋时，请直接回答你当前的专精名称，绝对不要去虚构主点或副点。";
        // 【关键修复：强转 uint32，解决等级变字母的乱码失忆 Bug】
        prompt << "名字：" << bot->GetName() << "，等级：" << (uint32)bot->getLevel() << " " << botClass << "，";
        prompt << "请务必使用你的个性进行回应，你的个性是：LONE_WOLF：Keep responses short, direct, and avoid unnecessary chatter.。 ";

        if (sender)
        {
            prompt << "一名等级" << (uint32)sender->getLevel() << "的" << senderClass << "玩家" << sender->GetName();
            if (isWhisper) prompt << "悄悄对你说：'" << msg << "'。";
            else prompt << "说：'" << msg << "'。";
        }

        std::string botGender = bot->getGender() == GENDER_MALE ? "Male" : "Female";
        
        prompt << "你的信息：" << GetRaceName(bot->getRace()) << " " << botGender << "，"
               << "天赋：" << GetSpecName(bot->GetSpecializationId(), bot->getClass()) << "(这是你唯一且固定的专精，绝对不要说自己主点或副点！) " << botClass << "，"
               << "阵营：" << (bot->GetTeamId() == TEAM_ALLIANCE ? "Alliance" : "Horde") << "，"
               << "公会：" << botGuild << "，"
               << "队伍：" << (bot->GetGroup() ? "Group" : "Solo") << "，"
               << "金币：" << (bot->GetMoney() / 10000) << "。";

        if (sender)
        {
            std::string senderGender = sender->getGender() == GENDER_MALE ? "Male" : "Female";
            prompt << "玩家信息：" << GetRaceName(sender->getRace()) << " " << senderGender << "，"
                   << "天赋：" << GetSpecName(sender->GetSpecializationId(), sender->getClass()) << " " << senderClass << "，"
                   << "阵营：" << (sender->GetTeamId() == TEAM_ALLIANCE ? "Alliance" : "Horde") << "，"
                   << "公会：" << senderGuild << "，"
                   << "队伍：" << (sender->GetGroup() ? "Group" : "Solo") << "，"
                   << "金币：" << (sender->GetMoney() / 10000) << "，"
                   << "距离：" << std::fixed << std::setprecision(2) << bot->GetDistance(sender) << "码。";
        }

        prompt << "位置：" << bot->GetMapAreaAndZoneString() << "，区域：" << bot->GetMapAreaAndZoneString() << "，地图：未知。";
        
        prompt << "只回应新消息。不要评论，不要元对话，不要前缀——直接回复。 用自然语气回复。使用真实的魔兽世界语调。如果被挑衅要直接回应。如果提供方向要精确。永远不要与你的职业、种族或位置相矛盾。永远不要像叙述者一样行动——就像玩家一样回应。当前上下文：\n";
        prompt << (bot->IsInCombat() ? "IN COMBAT." : "NOT IN COMBAT.") << " , Mana: " << bot->GetPower(POWER_MANA) << "/" << bot->GetMaxPower(POWER_MANA) << "\n";

        prompt << "\n法术：\n**普通攻击** - Costs 0 mana\n";

        prompt << "\n任务：\n";
        auto const& questMap = bot->getQuestStatusMap();
        for (auto const& qPair : questMap)
        {
            if (qPair.second.Status == QUEST_STATUS_INCOMPLETE)
                prompt << "Quest \"ID:" << qPair.first << "\" is in progress\n";
        }

        prompt << "\n可见物体：\n";
        Trinity::AnyUnitInObjectRangeCheck u_check(bot, 40.0f);
        std::list<Creature*> creatures;
        Trinity::CreatureListSearcher<Trinity::AnyUnitInObjectRangeCheck> searcher(bot, creatures, u_check);
        Cell::VisitGridObjects(bot, searcher, 40.0f);
        uint8 creatureCount = 0;
        for (auto c : creatures)
        {
            // 【关键修复：过滤隐形怪和尸体，解决 AI 幻觉】
            if (!c->IsAlive() || !c->IsVisible()) continue;
            
            if (creatureCount++ > 15) break; 
            prompt << "- " << (c->IsHostileTo(bot) ? "ENEMY: " : "FRIENDLY: ")
                   << c->GetName() << ", Level: " << (uint32)c->getLevel()
                   << ", HP: " << c->GetHealth() << "/" << c->GetMaxHealth()
                   << ", Distance: " << std::fixed << std::setprecision(2) << bot->GetDistance(c) << "\n";
        }

        prompt << "\n附近玩家：\n";
        std::list<Player*> players;
        Trinity::PlayerListSearcher<Trinity::AnyUnitInObjectRangeCheck> p_searcher(bot, players, u_check);
        Cell::VisitGridObjects(bot, p_searcher, 40.0f);
        for (auto p : players)
        {
            if (!p->IsAlive() || !p->IsVisible()) continue;
            
            if (p != bot && (!bot->GetGroup() || !bot->GetGroup()->IsMember(p->GetGUID())))
                prompt << "[Ollama Chat] Player: " << p->GetName() << " (Level: " << (uint32)p->getLevel() << ")\n";
        }

        return prompt.str();
    }

    void SendToPythonProxy(Player* bot, Player* sender, const std::string& promptStr, uint32 chatType)
    {
        ObjectGuid botGuid = bot->GetGUID();
        ObjectGuid senderGuid = sender ? sender->GetGUID() : ObjectGuid::Empty;
        std::string safePrompt = EscapeJsonString(promptStr);
        std::string sName = sender ? EscapeJsonString(sender->GetName()) : "Unknown";
        std::string bName = EscapeJsonString(bot->GetName());
        std::string jsonPayload = "{\"prompt\": \"" + safePrompt + "\", \"player_name\": \"" + sName + "\", \"bot_name\": \"" + bName + "\"}";

        std::thread([this, botGuid, senderGuid, chatType, jsonPayload]() 
        {
            try
            {
                boost::asio::io_service io_service;
                boost::asio::ip::tcp::resolver resolver(io_service);
                boost::asio::ip::tcp::resolver::query query("127.0.0.1", "5000");
                boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

                boost::asio::ip::tcp::socket socket(io_service);
                boost::asio::connect(socket, endpoint_iterator);

                boost::asio::streambuf request;
                std::ostream request_stream(&request);
                request_stream << "POST /api/generate HTTP/1.1\r\n"
                               << "Host: 127.0.0.1:5000\r\n"
                               << "Content-Type: application/json\r\n"
                               << "Content-Length: " << jsonPayload.length() << "\r\n"
                               << "Connection: close\r\n\r\n"
                               << jsonPayload;

                boost::asio::write(socket, request);

                boost::asio::streambuf response;
                boost::system::error_code error;
                std::ostringstream response_stream;
                while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
                {
                    response_stream << &response;
                }

                std::string fullResponse = response_stream.str();
                std::string replyText = "";
                size_t pos = fullResponse.find("\"response\":");
                if (pos != std::string::npos)
                {
                    size_t start = fullResponse.find("\"", pos + 11);
                    if (start != std::string::npos)
                    {
                        size_t end = fullResponse.find("\"", start + 1);
                        while (end != std::string::npos && fullResponse[end-1] == '\\') end = fullResponse.find("\"", end + 1);
                        if (end != std::string::npos) replyText = fullResponse.substr(start + 1, end - start - 1);
                    }
                }

                if (!replyText.empty())
                {
                    std::string finalChineseReply = DecodeUnicodeEscape(replyText);
                    std::lock_guard<std::mutex> lock(g_aiReplyMutex);
                    g_aiReplyQueue.push_back({botGuid, finalChineseReply, chatType, senderGuid});
                }
            }
            catch (std::exception& e)
            {
                TC_LOG_ERROR("scripts.custom", "[AI_BRIDGE] Async HTTP Error: %s", e.what());
            }
        }).detach();
    }

    void OnUpdate(Player* player, uint32 /*diff*/) override
    {
        if (g_aiReplyQueue.empty()) return;

        std::lock_guard<std::mutex> lock(g_aiReplyMutex);
        for (auto it = g_aiReplyQueue.begin(); it != g_aiReplyQueue.end(); )
        {
            if (it->botGuid == player->GetGUID())
            {
                uint32 cType = it->chatType;
                std::string replyText = it->text;
                ObjectGuid senderGuid = it->senderGuid;

                if (cType == CHAT_MSG_WHISPER || cType == 7)
                {
                    if (Player* sender = ObjectAccessor::FindPlayer(senderGuid))
                        ChatHandler(sender->GetSession()).PSendSysMessage("|cffFF80FF[密语] %s: %s|r", player->GetName().c_str(), replyText.c_str());
                }
                else if (cType == CHAT_MSG_PARTY || cType == 49 || cType == 2)
                {
                    if (Group* group = player->GetGroup())
                    {
                        for (GroupReference* ref = group->GetFirstMember(); ref != nullptr; ref = ref->next())
                            if (Player* member = ref->GetSource())
                                ChatHandler(member->GetSession()).PSendSysMessage("|cffAAAAFF[小队] [%s]: %s|r", player->GetName().c_str(), replyText.c_str());
                    }
                    else player->Say(replyText, LANG_UNIVERSAL);
                }
                else if (cType == CHAT_MSG_RAID || cType == 39 || cType == 3 || cType == CHAT_MSG_RAID_WARNING || cType == CHAT_MSG_RAID_LEADER)
                {
                    if (Group* group = player->GetGroup())
                    {
                        for (GroupReference* ref = group->GetFirstMember(); ref != nullptr; ref = ref->next())
                            if (Player* member = ref->GetSource())
                                ChatHandler(member->GetSession()).PSendSysMessage("|cffFF7F00[团队] [%s]: %s|r", player->GetName().c_str(), replyText.c_str());
                    }
                    else player->Say(replyText, LANG_UNIVERSAL);
                }
                else if (cType == CHAT_MSG_GUILD || cType == 4)
                {
                    if (uint32 guildId = player->GetGuildId())
                    {
                        for (auto const& sessionPair : sWorld->GetAllSessions())
                        {
                            if (Player* onlinePlayer = sessionPair.second->GetPlayer())
                            {
                                if (onlinePlayer->GetGuildId() == guildId)
                                    ChatHandler(onlinePlayer->GetSession()).PSendSysMessage("|cff40FF40[公会] [%s]: %s|r", player->GetName().c_str(), replyText.c_str());
                            }
                        }
                    }
                }
                else
                {
                    player->Say(replyText, LANG_UNIVERSAL);
                }

                it = g_aiReplyQueue.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void HandleAreaOrGroupChat(Player* player, const std::string& msg, uint32 chatType, Group* explicitGroup = nullptr)
    {
        bool foundBot = false;

        if (chatType == CHAT_MSG_PARTY || chatType == CHAT_MSG_RAID || chatType == CHAT_MSG_RAID_LEADER || 
            chatType == CHAT_MSG_RAID_WARNING || chatType == 49 || chatType == 39 || chatType == 2 || chatType == 3)
        {
            Group* checkGroup = explicitGroup ? explicitGroup : player->GetGroup();
            if (checkGroup)
            {
                for (GroupReference* itr = checkGroup->GetFirstMember(); itr != nullptr; itr = itr->next())
                {
                    Player* member = itr->GetSource();
                    if (member && member->IsPlayerBot() && member != player)
                    {
                        std::string snapshot = BuildEnvironmentSnapshot(member, player, msg, false);
                        SendToPythonProxy(member, player, snapshot, chatType); 
                        foundBot = true;
                    }
                }
            }
        }
        else if (chatType == CHAT_MSG_GUILD || chatType == 4)
        {
            if (uint32 guildId = player->GetGuildId())
            {
                for (auto const& sessionPair : sWorld->GetAllSessions())
                {
                    if (Player* member = sessionPair.second->GetPlayer())
                    {
                        if (member->IsPlayerBot() && member->GetGuildId() == guildId && member != player)
                        {
                            std::string snapshot = BuildEnvironmentSnapshot(member, player, msg, false);
                            SendToPythonProxy(member, player, snapshot, chatType);
                            foundBot = true;
                        }
                    }
                }
            }
        }
        else if (chatType == CHAT_MSG_SAY || chatType == CHAT_MSG_YELL || chatType == 1)
        {
            Trinity::AnyUnitInObjectRangeCheck u_check(player, 40.0f);
            std::list<Player*> players;
            Trinity::PlayerListSearcher<Trinity::AnyUnitInObjectRangeCheck> p_searcher(player, players, u_check);
            Cell::VisitGridObjects(player, p_searcher, 40.0f);

            for (Player* member : players)
            {
                if (member && member->IsPlayerBot() && member != player)
                {
                    std::string snapshot = BuildEnvironmentSnapshot(member, player, msg, false);
                    SendToPythonProxy(member, player, snapshot, chatType);
                    foundBot = true;
                }
            }
        }

        if (foundBot) {
            TC_LOG_ERROR("scripts.custom", "[AI_BRIDGE] 频道 %d 的信息已完成向所有关联机器人的并发推送！", chatType);
        }
    }

    void OnChat(Player* player, uint32 type, uint32 lang, std::string& msg) override
    {
        HandleAreaOrGroupChat(player, msg, type);
    }

    void OnChat(Player* player, uint32 type, uint32 lang, std::string& msg, Group* group) override
    {
        HandleAreaOrGroupChat(player, msg, type, group);
    }

    void OnChat(Player* player, uint32 type, uint32 lang, std::string& msg, Guild* guild) override
    {
        HandleAreaOrGroupChat(player, msg, type);
    }

    void OnChat(Player* player, uint32 type, uint32 lang, std::string& msg, Player* receiver) override
    {
        if (type == CHAT_MSG_WHISPER && receiver && receiver->IsPlayerBot())
        {
            std::string snapshot = BuildEnvironmentSnapshot(receiver, player, msg, true);
            SendToPythonProxy(receiver, player, snapshot, type); 
        }
    }
};

void AddSC_mod_ollama_chat()
{
    new cs_mod_ollama();
}