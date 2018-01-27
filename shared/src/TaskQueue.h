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

#ifndef FlightOS_TaskQueue_H_INCLUDED
#define FlightOS_TaskQueue_H_INCLUDED

#include "Threading.h"
#include "ThreadSafeQueue.h"
#include "Task.h"

#include <vector>

namespace FlightOS
{
  class TaskQueue
  {
  public:
    class WorkerThread : public Thread
    {
    public:
      WorkerThread();
      int run();

      friend class TaskQueue;

    protected:
      Task* mTask;
      TaskQueue* mOwner;
      int mId;

    };
    friend class WorkerThread;
    TaskQueue(uint32 id);

    int initialize(int iWorkers);
    int update();
    int shutdown();

    void add( Task* pTask );
    void push( Task* pTask );
    void pushWorking( Task* pTask );
    void pause( Task* pTask, uint64 pauseTime );
    
    long getWorkerCount() const { return (long)mWorkers.size(); }
    long getNumWorking() const { return mNumWorking; }
    long getNumPending() const { return (long)mPending.size(); }
    long getCompleteCount() const { return (long)mComplete.size(); }

    uint32 getId() const { return mId; }

    typedef std::vector< WorkerThread > WorkerVec;
    typedef ThreadSafeQueue< Task* > WorkQueue;

    

    class ScheduledTask
    {
    public:
      ScheduledTask() : 
        mTask( NULL ),
        mStartTime( 0 )
      {
      }

      ScheduledTask( Task* task, uint64 starttime ) : 
        mTask( task ),
        mStartTime( starttime )
      {
      }

      Task* mTask;
      uint64 mStartTime;

      bool operator< ( const ScheduledTask& rhs ) const
      {
        return mStartTime < rhs.mStartTime;
      }
    };

    typedef ThreadSafePriorityQueue< ScheduledTask > TaskPauseQueue;

  private:
    void completeTask(Task* pTask);
    void workerWait();

    uint32 mId;
    Semaphore* mWorkReady;
    WorkerVec mWorkers;

    WorkQueue mPending;
    WorkQueue mComplete;
    WorkQueue mDelete;

    TaskPauseQueue mPaused;

    long mNumWorking;
  };

}


#endif