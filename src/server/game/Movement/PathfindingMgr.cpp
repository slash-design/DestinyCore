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

#include "Log.h"
#include "PathfindingMgr.h"
#include "World.h"
#include "WorldSession.h"
#include "BotAI.h"
#include "DisableMgr.h"
#include <thread>

PFThread::PFThread(PathfindingMgr* pfMgr, uint32 threadIndex) :
    m_pfMgr(pfMgr),
    m_pMMap(MMAP::MMapFactory::createOrGetMMapManager()),
    _navMesh(NULL),
    _navMeshQuery(NULL),
    m_ThreadIndex(threadIndex),
    m_IsIDLE(true),
    m_IsRun(false),
    _thread(0),
    timespan(8),
    m_pfParameter(0)
{
}

PFThread::~PFThread()
{
    DestroyThread();
}

void PFThread::StartThread()
{
    DestroyThread();
    m_IsRun = true;
    m_IsIDLE = true;
    _thread = new std::thread(&PFThread::ThreadRun, this);
}

void PFThread::ThreadRun()
{
    while (m_IsRun)
    {
        std::this_thread::sleep_for(timespan);
        if (!m_pfParameter || m_IsIDLE)
            continue;

        bool result = ExecturePathfinding(false);
        if (!result)
        {
            ExecturePathfinding(true);
        }
        //Pathfinding path(m_pfParameter, _navMesh, _navMeshQuery);
        //bool result = path.CalculatePath(m_pfParameter->targetPosX, m_pfParameter->targetPosY, m_pfParameter->targetPosZ);
        //if (!result || (path.GetPathType() & PATHFIND_NOPATH))
        //{
        //	TC_LOG_ERROR("PFThread::ThreadRun", "Path not find.");
        //	m_pfParameter->findOK = false;
        //}
        //else
        //	m_pfParameter->findOK = true;
        //m_pfParameter->finishPaths.clear();
        //const std::vector<G3D::Vector3>& points = path.GetPath();
        //for (std::vector<G3D::Vector3>::const_iterator itPoints = points.begin();
        //	itPoints != points.end();
        //	itPoints++)
        //{
        //	m_pfParameter->finishPaths.push_back(*itPoints);
        //}
        //m_pfParameter->destPosition = path.GetActualEndPosition();

        //m_pfMgr->AddFinishPFParameter(m_pfParameter);
        m_pfParameter = NULL;
        m_IsIDLE = true;
    }
}

void PFThread::DestroyThread()
{
    m_IsRun = false;
    if (_thread)
    {
        _thread->join();
        delete _thread;
        _thread = 0;
    }
    m_pfParameter = NULL;
    m_IsIDLE = false;
}

void PFThread::AddPathParameter(PathParameter* pfParameter)
{
    if (!pfParameter)
        return;

    m_pfParameter = pfParameter;
    //if (_navMeshQuery)
    //{
    //	m_pMMap->unloadMapInstance(m_pfParameter->mapID, m_pfParameter->instID);
    //}
    bool canPathfinding = DisableMgr::IsPathfindingEnabled(m_pfParameter->mapID);
    if (m_pMMap && canPathfinding)
    {
        _navMesh = m_pMMap->GetNavMesh(m_pfParameter->mapID);
        _navMeshQuery = m_pMMap->GetNavMeshQuery(m_pfParameter->mapID, m_pfParameter->instID);
    }
    else
        TC_LOG_ERROR("PFThread::AddPathParameter", "Pathfinding not get nav mesh.");
    m_IsIDLE = false;
}

bool PFThread::ExecturePathfinding(bool force)
{
    Pathfinding path(m_pfParameter, _navMesh, _navMeshQuery);
    if (m_pfParameter->offset < 0)
        m_pfParameter->offset *= -1;
    float x = m_pfParameter->targetPosX + ((m_pfParameter->offset != 0) ? frand(m_pfParameter->offset * (-1), m_pfParameter->offset) : 0);
    float y = m_pfParameter->targetPosY + ((m_pfParameter->offset != 0) ? frand(m_pfParameter->offset * (-1), m_pfParameter->offset) : 0);
    float z = m_pfParameter->targetPosZ;
    path.UpdateAllowedPositionZ(x, y, z);
    bool result = path.CalculatePath(x, y, z);
    if (!result || (path.GetPathType() & PATHFIND_NOPATH))
    {
        m_pfMgr->OutputPathfindingError();
        m_pfParameter->findOK = false;
    }
    else
        m_pfParameter->findOK = true;

    if (m_pfParameter->findOK || force)
    {
        m_pfParameter->finishPaths.clear();
        const std::vector<G3D::Vector3>& points = path.GetPath();
        for (std::vector<G3D::Vector3>::const_iterator itPoints = points.begin();
            itPoints != points.end();
            itPoints++)
        {
            G3D::Vector3 point = (G3D::Vector3)(*itPoints);
            if (!m_pfParameter->findOK)
                path.UpdateAllowedPositionZ(point.x, point.y, point.z);
            m_pfParameter->finishPaths.push_back(point);
        }
        m_pfParameter->destPosition = path.GetActualEndPosition();

        if (!m_pfParameter->findOK && m_pfParameter->finishPaths.size() > 2)
            m_pfParameter->finishPaths.erase(m_pfParameter->finishPaths.begin());

        m_pfMgr->AddFinishPFParameter(m_pfParameter);
    }

    return m_pfParameter->findOK;
}

//////////////////////////////////////////////////////////////////////////

PathfindingMgr::PathfindingMgr() :
    pathErrorCount(0)
{
}

PathfindingMgr::~PathfindingMgr()
{
    ClearPFThreads();
}

PathfindingMgr* PathfindingMgr::instance()
{
    static PathfindingMgr instance;
    return &instance;
}

void PathfindingMgr::OutputPathfindingError()
{
    TC_LOG_ERROR("PFThread::ThreadRun", "Path not find6. (Count %d)", ++pathErrorCount);
}

void PathfindingMgr::InitializePFMgr()
{
    ClearPFThreads();
    uint32 threadCount = GetCPUNumber();
    if (threadCount < 2)
        threadCount = 2;
    if (threadCount > 4)
        threadCount = 4;
    //TC_LOG_WARN("PathfindingMgr", "Create waypoint find thread %d.", threadCount);
    threadCount = 1;

    for (uint32 i = 0; i < threadCount; i++)
    {
        PFThread* pPF = new PFThread(this, i);
        m_pfThreads.push_back(pPF);
        pPF->StartThread();
    }
}

int PathfindingMgr::GetCPUNumber() const
{
    unsigned int cpuCount = std::thread::hardware_concurrency();
    if (cpuCount == 0)
        cpuCount = 1;
    return static_cast<int>(cpuCount);
}

void PathfindingMgr::ClearPFThreads()
{
    for (PFThreads::iterator itPF = m_pfThreads.begin(); itPF != m_pfThreads.end(); itPF++)
    {
        PFThread* pf = *itPF;
        pf->DestroyThread();
        delete pf;
    }
    m_pfThreads.clear();
}

void PathfindingMgr::AddPFParameter(PathParameter* pfParameter)
{
    if (pfParameter)
    {
        std::lock_guard<std::mutex> lock(m_addfpQueueLock);
        m_WaitPFQueue.push(pfParameter);
    }
}

void PathfindingMgr::AddFinishPFParameter(PathParameter* pfParameter)
{
    if (!pfParameter)
        return;
    std::lock_guard<std::mutex> lock(m_fpQueueLock);
    m_FinishPFQueue.push(pfParameter);
}

void PathfindingMgr::Update()
{
    {
        std::lock_guard<std::mutex> lock(m_addfpQueueLock);
        if (!m_WaitPFQueue.empty())
        {
            for (PFThreads::iterator itPF = m_pfThreads.begin(); itPF != m_pfThreads.end(); itPF++)
            {
                PFThread* pf = *itPF;
                if (!m_WaitPFQueue.empty() && pf->IsIDLE())
                {
                    PathParameter* pfParameter = m_WaitPFQueue.front();
                    m_WaitPFQueue.pop();
                    pf->AddPathParameter(pfParameter);
                }
            }
        }
    }

    PFParameterQueue finishQueue;
    while (true)
    {
        std::lock_guard<std::mutex> lock(m_fpQueueLock);
        if (m_FinishPFQueue.empty())
            break;
        PathParameter* pfParameter = m_FinishPFQueue.front();
        m_FinishPFQueue.pop();
        finishQueue.push(pfParameter);
    }
    while (!finishQueue.empty())
    {
        PathParameter* pfParameter = finishQueue.front();
        finishQueue.pop();
        ProcessFinishPFParameter(pfParameter);
    }
}

void PathfindingMgr::ProcessFinishPFParameter(PathParameter* pfParameter)
{
    bool isProcess = false;
    Player* player = ObjectAccessor::FindPlayer(pfParameter->unitGUID);
    if (player)
    {
        UnitAI* pAI = player->GetAI();
        if (pAI)
        {
            BotBGAI* pBotAI = dynamic_cast<BotBGAI*>(pAI);
            if (pBotAI)
            {
                pBotAI->PushFinishQueue(pfParameter);
                isProcess = true;
            }
        }
    }
    //if (!isProcess)
    //    TC_LOG_WARN("Pathfinding", "PathfindingMgr::ProcessFinishPFParameter SessionID %u GUID %u un process.\n", pfParameter->sessionID, pfParameter->unitGUID);

    //if (pfParameter)
    //	delete pfParameter;
}
