#include "ScriptMgr.h"
#include "Player.h"
#include "Log.h"
#include "WorldSession.h"
#include "World.h"
#include "Map.h"
#include "AI/PlayerAI/BotAI.h" 
#include "AI/PlayerAI/PlayerAI.h"
#include "Phasing/PhasingHandler.h"

// ==========================================
// 地图等级鉴定中心 (Zone Level Dictionary)
// ==========================================
struct ZoneLevelRange { uint32 minLevel; uint32 maxLevel; };

ZoneLevelRange GetZoneRealLevel(uint32 zoneId)
{
    // 这里列举了魔兽经典的地图真实等级。你可以根据需要自由添加！
    switch (zoneId)
    {
        // --- 新手村 (1-10) ---
        case 12:   return { 1, 10 };  // 艾尔文森林
        case 14:   return { 1, 10 };  // 杜隆塔尔
        case 85:   return { 1, 10 };  // 提瑞斯法林地
        case 215:  return { 1, 10 };  // 莫高雷
        case 1:    return { 1, 10 };  // 丹莫罗
        case 141:  return { 1, 10 };  // 泰达希尔
        case 3430: return { 1, 10 };  // 永歌森林
        case 3524: return { 1, 10 };  // 秘蓝岛

        // --- 前期野外 (10-20) ---
        case 17:   return { 10, 25 }; // 贫瘠之地
        case 40:   return { 10, 20 }; // 西部荒野
        case 130:  return { 10, 20 }; // 银松森林
        case 38:   return { 10, 20 }; // 洛克莫丹
        case 148:  return { 10, 20 }; // 黑海岸
        case 3433: return { 10, 20 }; // 塔伦米尔/幽魂之地

        // --- 中期野外 (30-50) ---
        case 33:   return { 30, 45 }; // 荆棘谷
        case 440:  return { 40, 50 }; // 塔纳利斯
        case 400:  return { 50, 55 }; // 安戈洛环形山

        // --- 外域 (58-70) ---
        case 3483: return { 58, 63 }; // 地狱火半岛
        case 3518: return { 64, 68 }; // 纳格兰

        // --- 诺森德 (68-80) ---
        case 3537: return { 68, 72 }; // 北风苔原
        case 3536: return { 68, 72 }; // 嚎风峡湾
        case 210:  return { 77, 80 }; // 冰冠冰川

        // 默认返回值：如果字典里没写，返回 0，系统会走另一种默认逻辑
        default:   return { 0, 0 }; 
    }
}


// ==========================================
// AI 导演核心调度器 (单例)
// ==========================================
class AIDirectorMgr
{
public:
    static AIDirectorMgr* GetInstance()
    {
        static AIDirectorMgr instance;
        return &instance;
    }

    // 【新增】：5秒全屏雷达扫描补给线
    void Update(uint32 diff)
    {
        m_radarTimer += diff;
        // 每 5 秒雷达扫一圈
        if (m_radarTimer >= 5000)
        {
            m_radarTimer = 0;
            
            SessionMap const& sessions = sWorld->GetAllSessions();
            for (auto const& pair : sessions)
            {
                Player* p = pair.second->GetPlayer();
                // 找到真实玩家，并且不在副本、不在飞行的
                if (p && p->IsInWorld() && !p->IsPlayerBot() && !p->GetMap()->IsDungeon() && !p->IsInFlight())
                {
                    // 检测玩家周围 50 码内有几个清醒的群演
                    uint32 activeBotsNearby = 0;
                    for (auto const& botPair : sessions)
                    {
                        Player* b = botPair.second->GetPlayer();
                        if (b && b->IsPlayerBot() && b->GetMapId() == p->GetMapId())
                        {
                            if (p->GetDistance(b) <= 50.0f)
                            {
                                PlayerAI* bai = dynamic_cast<PlayerAI*>(b->GetAI());
                                if (bai && !bai->IsDirectorSleeping())
                                    activeBotsNearby++;
                            }
                        }
                    }

                    // 主城要求热闹 (比如 15 个)，野外要求真实 (比如 5 个)
                    bool isInCity = p->HasFlag(PLAYER_FLAGS,PLAYER_FLAGS_RESTING);
                    uint32 desiredBots = isInCity ? 15 : 5;

                    // 如果雷达发现周围群演不够，立刻呼叫空投支援！
                    if (activeBotsNearby < desiredBots)
                    {
                        uint32 needCount = desiredBots - activeBotsNearby;
                        DispatchActorsToPlayer(p, needCount);
                    }
                }
            }
        }
    }

    // 核心投放逻辑：在玩家周围部署群演
    void DispatchActorsToPlayer(Player* realPlayer, uint32 actorsNeeded = 50)
    {
        if (!realPlayer || !realPlayer->IsInWorld()) return;

        uint32 currentMapId = realPlayer->GetMapId();
        uint32 currentZoneId = realPlayer->GetZoneId();
        uint32 playerLevel = realPlayer->getLevel();
        uint32 teamId = realPlayer->GetTeamId(); 
        
        bool isInCity = realPlayer->HasFlag(PLAYER_FLAGS,PLAYER_FLAGS_RESTING);
        
        // 查字典，获取当前地图的真实等级
        ZoneLevelRange zoneLvl = GetZoneRealLevel(currentZoneId);

        uint32 currentDispatched = 0;

        SessionMap const& sessions = sWorld->GetAllSessions();
        for (auto const& pair : sessions)
        {
            if (currentDispatched >= actorsNeeded) break; 

            Player* bot = pair.second->GetPlayer();
            if (!bot || !bot->IsInWorld() || !bot->IsPlayerBot() || bot->isDead() || bot->IsInCombat())
                continue;

            PlayerAI* botAI = dynamic_cast<PlayerAI*>(bot->GetAI());
            if (!botAI) continue;

            // 1. 只挑选当前正在休眠的机器人
            if (!botAI->IsDirectorSleeping()) continue;
            // 2. 绝对不抓有队伍的机器人！
            if (bot->GetGroup()) continue;
            // 3. 阵营海关！
            if (bot->GetTeamId() != teamId) continue;
            // 4. 衣着防呆
            if (!bot->EquipIsTidiness() || (bot->getLevel() >= 10 && bot->GetSpecializationId() == 0)) continue;

            // ================= 等级智能过滤 =================
            uint32 botLevel = bot->getLevel();
            
            if (isInCity)
            {
                // 如果在主城：任何等级都可以出现 (老少皆宜，大城气象)
            }
            else 
            {
                // 如果在野外：启用极其真实的等级过滤网
                int destinyRoll = urand(1, 100);
                
                if (destinyRoll <= 90) // 90% 的群演必须严格符合地图等级
                {
                    if (zoneLvl.minLevel != 0) 
                    {
                        // 字典里有的地图：严格按字典等级过滤 (允许上下浮动 2 级)
                        if (botLevel < (zoneLvl.minLevel > 2 ? zoneLvl.minLevel - 2 : 1) || botLevel > zoneLvl.maxLevel + 2) 
                            continue;
                    }
                    else 
                    {
                        // 字典里没写的地图：以前的原生逻辑，按玩家等级上下 5 级过滤
                        if (botLevel + 5 < playerLevel || botLevel > playerLevel + 5) 
                            continue;
                    }
                }
                else 
                {
                    // 10% 的群演是“路过的大佬”或者“满级采药大军”
                    // 只要大于等于地图等级都可以，制造惊喜感
                    if (botLevel < playerLevel) 
                        continue;
                }
            }
            // ===============================================

            // 【验尸官打印】：空投日志
            TC_LOG_ERROR("server", ">>> [导演筹备] 为玩家 [%s] 空投 %u级 群演 [%s]! Map:%u, Zone:%u",
                realPlayer->GetName().c_str(), botLevel, bot->GetName().c_str(), currentMapId, currentZoneId);

            // ================= 视野外散布空投计算 (防叠罗汉机制) =================
            // 绝不砸在玩家头顶！在玩家周围 20~45 码外进行环形空投，让他们自己“走入”视线！
            float dropAngle = frand(0.0f, 6.28f);
            float dropDist  = frand(20.0f, 45.0f);
            
            float x = realPlayer->GetPositionX() + dropDist * std::cos(dropAngle);
            float y = realPlayer->GetPositionY() + dropDist * std::sin(dropAngle);
            float z = realPlayer->GetPositionZ(); 
            float o = frand(0.0f, 6.28f);

            // ================= 场务执行 =================
            if (bot->IsInFlight()) continue; 
            
            bot->InterruptNonMeleeSpells(false);
            bot->SetStandState(UNIT_STAND_STATE_STAND);
            bot->StopMoving();
            bot->GetMotionMaster()->Clear();
            bot->Dismount();

            // 传送到计算好的散布点
            bool teleSuccess = bot->TeleportTo(currentMapId, x, y, z, o);
            if (!teleSuccess && bot->GetMapId() == currentMapId)
            {
                bot->Relocate(x, y, z, o);
                teleSuccess = true;
            }

            if (!teleSuccess) continue; 

            // 完美位面继承
            PhasingHandler::InheritPhaseShift(bot, realPlayer);
            bot->UpdateObjectVisibility(true); 

            botAI->SetDirectorDrafted(true);
            botAI->SetDirectorAnchor(currentMapId, realPlayer->GetPositionX(), realPlayer->GetPositionY(), realPlayer->GetPositionZ()); 

            currentDispatched++;
        }
    }

private:
    uint32 m_radarTimer = 0; // 雷达计时器
};

// ==========================================
// 玩家事件监听器 (保留：切地图瞬间爆发空投)
// ==========================================
class AIDirectorPlayerScript : public PlayerScript
{
public:
    AIDirectorPlayerScript() : PlayerScript("AIDirectorPlayerScript") { }

    void OnUpdateZone(Player* player, Area* oldArea, Area* newArea) override
    {
        if (!player || player->IsPlayerBot()) return;
        TC_LOG_INFO("server", "[AI Director] 侦测到真实玩家 [%s] 跨越了区域边界。开始调拨群演...", player->GetName().c_str());
        
        // 切图时，瞬间调拨 50 人填满视野
        AIDirectorMgr::GetInstance()->DispatchActorsToPlayer(player, 50);
    }

    void OnLogin(Player* player, bool firstLogin) override
    {
        if (!player || player->IsPlayerBot()) return;
        TC_LOG_INFO("server", "[AI Director] 真实玩家 [%s] 登录游戏。正在预热周边环境...", player->GetName().c_str());
        
        AIDirectorMgr::GetInstance()->DispatchActorsToPlayer(player, 50);
    }
};

// ==========================================
// 世界事件监听器 (全局 5秒 雷达心脏)
// ==========================================
class AIDirectorWorldScript : public WorldScript
{
public:
    AIDirectorWorldScript() : WorldScript("AIDirectorWorldScript") { }

    void OnStartup() override
    {
        TC_LOG_INFO("server", "[AI Director] 导演系统已上线！动态视锥与全域 AI 场务调度就绪...");
    }

    // 底层主循环每次心跳都会触发这个
    void OnUpdate(uint32 diff) override
    {
        AIDirectorMgr::GetInstance()->Update(diff);
    }
};

void AddSC_ai_director()
{
    new AIDirectorPlayerScript();
    new AIDirectorWorldScript();
}