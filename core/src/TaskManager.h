//=====================================================================
// This file is part of FlightOS.
//
// FlightOS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// FlightOS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with FlightOS.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef FlightOS_TaskManager_H_INCLUDED
#define FlightOS_TaskManager_H_INCLUDED

#include "Threading.h"
#include "ThreadSafeQueue.h"
#include "Task.h"
#include "TaskQueue.h"
#include "ImUi.h"

#include <vector>
#include <unordered_map>

namespace FlightOS
{
  class TaskManager : public SystemModule
  {
  public:
    static const int kGeneralTaskQueueId = 0;
    static const int kAssetTaskQueueId = 1;
    static const int kAppTaskQueueId = 2;
    static const int kGameTaskQueueId = 3;
    static const int kNetworkTaskQueueId = 4;

    TaskManager();

    int initialize( int iWorkers );
    int update();
    int shutdown();

    
    void add( Task* pTask );
    void push( Task* pTask );
    void pause( Task* pTask, uint64 pauseTime );
    void wake( Task* pTask );

    long getNumWorking() const { return mNumWorking; }
    long getNumPending() const { return mNumPending; }

    TaskQueue* createQueue( uint32 id, int numWorkerThreads );

    void DrawUI(float dt );

    typedef std::unordered_map< uint32, TaskQueue* > TaskQueueMap;

  private:
    Lock mQueuesLock;
    TaskQueueMap mQueues;

    long mNumWorking;
    long mNumPending;
  };

}


#endif