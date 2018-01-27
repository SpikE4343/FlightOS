/*************************************************************************************
g3::TaskQueue
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3_TaskQueue_H_INCLUDED
#define g3_TaskQueue_H_INCLUDED

#include "g3Threading.h"
#include "g3ThreadSafeQueue.h"
#include "g3Task.h"

#include <vector>

namespace g3
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
    void workerWait();
    long getWorkerCount() const { return (long)mWorkers.size(); }
    long getNumWorking() const { return mNumWorking; }
    long getNumPending() const { return (long)mPending.size(); }
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