/*
* Copyright (C) 2010 - 2024 Eluna Lua Engine <https://elunaluaengine.github.io/>
* Copyright (C) 2022 - 2022 Hour of Twilight <https://www.houroftwilight.net/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#include "ElunaCompat.h"
#include "ElunaConfig.h"
#include "ElunaLoader.h"
#include "ElunaUtility.h"
#include "LuaEngine.h" 
#include <fstream>
#include <sstream>
#include <thread>
#include <charconv>

// [恢复]: 强制声明全局唯一引擎 sEluna
extern class Eluna* sEluna;

#if defined USING_BOOST
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

#if defined ELUNA_WINDOWS
#include <Windows.h>
#endif

#if defined ELUNA_TRINITY || ELUNA_MANGOS
#include "MapManager.h"
#elif defined ELUNA_CMANGOS
#include "Maps/MapManager.h"
#endif

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

ElunaLoader::ElunaLoader() : m_cacheState(SCRIPT_CACHE_NONE)
{
}

ElunaLoader* ElunaLoader::instance()
{
    static ElunaLoader instance;
    return &instance;
}

ElunaLoader::~ElunaLoader()
{
    if (m_reloadThread.joinable())
        m_reloadThread.join();
}

void ElunaLoader::ReloadScriptCache()
{
    // [关键修复]：如果是第一次启动，直接强行“同步加载”，确保把文件读完再继续！
    if (m_cacheState == SCRIPT_CACHE_NONE)
    {
        LoadScripts();
        return;
    }

    if (m_cacheState != SCRIPT_CACHE_READY)
    {
        ELUNA_LOG_DEBUG("[Eluna]: Script cache not ready, skipping reload");
        return;
    }

    if (m_reloadThread.joinable())
        m_reloadThread.join();

    m_cacheState = SCRIPT_CACHE_REINIT;

    m_reloadThread = std::thread(&ElunaLoader::LoadScripts, this);
    ELUNA_LOG_DEBUG("[Eluna]: Script cache reload thread started");
}

void ElunaLoader::LoadScripts()
{
    if (m_cacheState != SCRIPT_CACHE_REINIT && m_cacheState != SCRIPT_CACHE_NONE)
        return;

    m_cacheState = SCRIPT_CACHE_LOADING;

    uint32 oldMSTime = ElunaUtil::GetCurrTime();

    std::string lua_folderpath = sElunaConfig->GetConfig(CONFIG_ELUNA_SCRIPT_PATH);
    const std::string& lua_path_extra = sElunaConfig->GetConfig(CONFIG_ELUNA_REQUIRE_PATH_EXTRA);
    const std::string& lua_cpath_extra = sElunaConfig->GetConfig(CONFIG_ELUNA_REQUIRE_CPATH_EXTRA);
    
#if !defined ELUNA_WINDOWS
    if (lua_folderpath[0] == '~')
        if (const char* home = getenv("HOME"))
            lua_folderpath.replace(0, 1, home);
#endif

    printf("\n======================================================\n");
    printf("[Eluna 物理打印]: 正在搜索此目录下的 Lua 脚本: %s\n", lua_folderpath.c_str());
    printf("======================================================\n");

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    m_requirePath.clear();
    m_requirecPath.clear();

    ReadFiles(L, lua_folderpath);

    lua_close(L);

    CombineLists();

    if (!lua_path_extra.empty())
        m_requirePath += lua_path_extra;

    if (!lua_cpath_extra.empty())
        m_requirecPath += lua_cpath_extra;

    if (!m_requirePath.empty())
        m_requirePath.erase(m_requirePath.end() - 1);

    if (!m_requirecPath.empty())
        m_requirecPath.erase(m_requirecPath.end() - 1);

    printf("[Eluna 物理打印]: 成功加载并编译了 %u 个脚本！耗时: %u 毫秒\n", uint32(m_scriptCache.size()), ElunaUtil::GetTimeDiff(oldMSTime));
    printf("======================================================\n\n");

    m_cacheState = SCRIPT_CACHE_READY;
}

int ElunaLoader::LoadBytecodeChunk(lua_State* /*L*/, uint8* bytes, size_t len, BytecodeBuffer* buffer)
{
    buffer->insert(buffer->end(), bytes, bytes + len);
    return 0;
}

void ElunaLoader::ReadFiles(lua_State* L, std::string path)
{
    std::string lua_folderpath = sElunaConfig->GetConfig(CONFIG_ELUNA_SCRIPT_PATH);

    ELUNA_LOG_DEBUG("[Eluna]: ReadFiles from path `%s`", path.c_str());

    fs::path someDir(path);
    fs::directory_iterator end_iter;

    if (fs::exists(someDir) && fs::is_directory(someDir) && !fs::is_empty(someDir))
    {
        m_requirePath +=
            path + "/?.lua;" +
            path + "/?.ext;" +
            path + "/?.moon;";

        m_requirecPath +=
            path + "/?.dll;" +
            path + "/?.so;";

        for (fs::directory_iterator dir_iter(someDir); dir_iter != end_iter; ++dir_iter)
        {
            std::string fullpath = dir_iter->path().generic_string();
#if defined ELUNA_WINDOWS
            DWORD dwAttrib = GetFileAttributes(fullpath.c_str());
            if (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_HIDDEN))
                continue;
#else
            std::string name = dir_iter->path().filename().generic_string().c_str();
            if (name[0] == '.')
                continue;
#endif

            if (fs::is_directory(dir_iter->status()))
            {
                ReadFiles(L, fullpath);
                continue;
            }

            if (fs::is_regular_file(dir_iter->status()))
            {
                int32 mapId = -1;

                std::string subfolder = dir_iter->path().generic_string();
                subfolder = subfolder.erase(0, lua_folderpath.size() + 1);

                auto [ptr, ec] = std::from_chars(subfolder.data(), subfolder.data() + subfolder.size(), mapId);

                if (ec == std::errc::invalid_argument || ec == std::errc::result_out_of_range || mapId < -1)
                    mapId = -1;

                std::string filename = dir_iter->path().filename().generic_string();
                size_t filesize = fs::file_size(dir_iter->path());
                ProcessScript(L, filename, filesize, fullpath, mapId);
            }
        }
    }
}

bool ElunaLoader::CompileScript(lua_State* L, LuaScript& script)
{
    int err = 0;
    if (script.fileext == ".moon")
    {
        std::string str = "return require('moonscript').loadfile([[" + script.filepath+ "]])";
        err = luaL_dostring(L, str.c_str());
    } else
        err = luaL_loadfile(L, script.filepath.c_str());

    if (err != 0)
    {
        ELUNA_LOG_ERROR("[Eluna]: CompileScript failed to load the Lua script `%s`.", script.filename.c_str());
        Eluna::Report(L);
        return false;
    }
    ELUNA_LOG_DEBUG("[Eluna]: CompileScript loaded Lua script `%s`", script.filename.c_str());

    err = lua_dump(L, (lua_Writer)LoadBytecodeChunk, &script.bytecode);
    if (err || script.bytecode.empty())
    {
        ELUNA_LOG_ERROR("[Eluna]: CompileScript failed to dump the Lua script `%s` to bytecode.", script.filename.c_str());
        Eluna::Report(L);
        return false;
    }
    ELUNA_LOG_DEBUG("[Eluna]: CompileScript dumped Lua script `%s` to bytecode.", script.filename.c_str());

    lua_pop(L, 1);
    return true;
}

void ElunaLoader::ProcessScript(lua_State* L, std::string filename, const size_t& filesize, const std::string& fullpath, int32 mapId)
{
    ELUNA_LOG_DEBUG("[Eluna]: ProcessScript checking file `%s`", fullpath.c_str());

    std::size_t extDot = filename.find_last_of('.');
    if (extDot == std::string::npos)
        return;
    std::string ext = filename.substr(extDot);
    filename = filename.substr(0, extDot);

    if (ext != ".lua" && ext != ".ext" && ext != ".moon")
        return;
    bool extension = ext == ".ext";

    LuaScript script;
    script.fileext = ext;
    script.filename = filename;
    script.filepath = fullpath;
    script.modulepath = fullpath.substr(0, fullpath.length() - filename.length() - ext.length());
    script.bytecode.reserve(filesize);
    script.mapId = mapId;

    if (!CompileScript(L, script))
        return;

    if (extension)
        m_extensions.push_back(script);
    else
        m_scripts.push_back(script);

    ELUNA_LOG_DEBUG("[Eluna]: ProcessScript processed `%s` successfully", fullpath.c_str());
}

static bool ScriptPathComparator(const LuaScript& first, const LuaScript& second)
{
    return first.filepath < second.filepath;
}

void ElunaLoader::CombineLists()
{
    m_extensions.sort(ScriptPathComparator);
    m_scripts.sort(ScriptPathComparator);

    m_scriptCache.clear();
    m_scriptCache.reserve(m_extensions.size() + m_scripts.size());

    std::move(m_extensions.begin(), m_extensions.end(), std::back_inserter(m_scriptCache));
    std::move(m_scripts.begin(), m_scripts.end(), std::back_inserter(m_scriptCache));

    m_extensions.clear();
    m_scripts.clear();
}

void ElunaLoader::ReloadElunaForMap(int mapId)
{
    ReloadScriptCache();

    if (mapId != RELOAD_CACHE_ONLY)
    {
        if (mapId == RELOAD_GLOBAL_STATE || mapId == RELOAD_ALL_STATES)
            if (Eluna* e = sEluna) // [恢复]: 用 sEluna 替换 sWorld->GetEluna()
                e->ReloadEluna();

#if defined ELUNA_TRINITY || defined ELUNA_AZEROTHCORE
        sMapMgr->DoForAllMaps([&](Map* map)
#else
        sMapMgr.DoForAllMaps([&](Map* map)
#endif
            {
                if (mapId == RELOAD_ALL_STATES || mapId == static_cast<int>(map->GetId()))
                    if (Eluna* e = sEluna) // [恢复]: 用 sEluna 替换 map->GetEluna()
                        e->ReloadEluna();
            }
        );
    }
}