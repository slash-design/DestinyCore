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

#ifndef _PATHFINDINGMGR_H_
#define _PATHFINDINGMGR_H_

#include "Pathfinding.h"
#include "Map.h"
#include "MMapFactory.h"
#include "MMapManager.h"
#include <mutex>
#include <thread>
//#include <boost/thread/thread.hpp>

class PathfindingMgr;
typedef std::queue<PathParameter*> PFParameterQueue;

class PFThread
{
public:
    PFThread(PathfindingMgr* pfMgr, uint32 threadIndex);
    ~PFThread();

    void StartThread();
    void ThreadRun();
    void DestroyThread();

    void AddPathParameter(PathParameter* pfParameter);
    bool ExecturePathfinding(bool force);

    bool IsIDLE() { return m_IsIDLE && !m_pfParameter; }

private:
    PathfindingMgr* m_pfMgr;
    MMAP::MMapManager* m_pMMap;
    dtNavMesh const* _navMesh;              // the nav mesh
    dtNavMeshQuery const* _navMeshQuery;    // the nav mesh query used to find the path
    uint32 m_ThreadIndex;
    bool m_IsIDLE;
    bool m_IsRun;
    std::thread* _thread;
    std::chrono::milliseconds timespan;

    PathParameter* m_pfParameter;
};

class TC_GAME_API PathfindingMgr
{
    typedef std::vector<PFThread*> PFThreads;

private:
    PathfindingMgr();
    ~PathfindingMgr();

public:
    PathfindingMgr(PathfindingMgr const&) = delete;
    PathfindingMgr(PathfindingMgr&&) = delete;

    PathfindingMgr& operator= (PathfindingMgr const&) = delete;
    PathfindingMgr& operator= (PathfindingMgr&&) = delete;

    static PathfindingMgr* instance();

    void OutputPathfindingError();
    void InitializePFMgr();
    void ClearPFThreads();
    void AddPFParameter(PathParameter* pfParameter);
    void AddFinishPFParameter(PathParameter* pfParameter);
    void Update();

private:
    int GetCPUNumber() const;
    void ProcessFinishPFParameter(PathParameter* pfParameter);

private:
    PFThreads m_pfThreads;
    PFParameterQueue m_WaitPFQueue;
    PFParameterQueue m_FinishPFQueue;

    std::mutex m_fpQueueLock;
    std::mutex m_addfpQueueLock;

    uint32 pathErrorCount;
};

#define sFPMgr PathfindingMgr::instance()

#endif // _PATHFINDINGMGR_H_
