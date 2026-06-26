# Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.

CollectSourceFiles(
  ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE_SOURCES
  # Exclude
  ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders
)

if (USE_COREPCH)
  set(PRIVATE_PCH_HEADER PrecompiledHeaders/gamePCH.h)
endif ()

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DTRINITY_API_EXPORT_GAME)

# 强制告诉Eluna这是Trinity架构，防止API识别错乱
add_definitions(-DELUNA_TRINITY)

CollectIncludeDirectories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC_INCLUDES
  # Exclude
  ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders
)

add_library(game-interface INTERFACE)

target_include_directories(game-interface
  INTERFACE
    ${PUBLIC_INCLUDES})

target_link_libraries(game-interface
  INTERFACE
    shared
    Detour)

# ==========================================
# [关键修复]: 提取 Lua C 源码，并开启免检通道
# ==========================================
file(GLOB_RECURSE LUA_C_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/LuaEngine/lua_src/*.c")
if(LUA_C_SOURCES)
    set_source_files_properties(${LUA_C_SOURCES} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
endif()

# 注意：整个文件里，add_library(game ...) 只能出现这一次！
add_library(game
  ${PRIVATE_SOURCES})

target_include_directories(game
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/dep
    ${CMAKE_SOURCE_DIR}/dep/efsw/include  # [新增这一行
    ${CMAKE_CURRENT_SOURCE_DIR}/LuaEngine/lua_src # 确保 Eluna 能找到 lua.h
)

target_link_libraries(game
  PRIVATE
    trinity-core-interface
    efsw
  PUBLIC
    game-interface
)

set_target_properties(game
    PROPERTIES
      FOLDER
        "server")

if( BUILD_SHARED_LIBS )
  if( UNIX )
    install(TARGETS game
      LIBRARY
        DESTINATION lib)
  elseif( WIN32 )
    install(TARGETS game
      RUNTIME
        DESTINATION "${CMAKE_INSTALL_PREFIX}")
  endif()
endif()

if (USE_COREPCH)
  add_cxx_pch(game ${PRIVATE_PCH_HEADER})
endif ()