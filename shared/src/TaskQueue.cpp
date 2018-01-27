/*************************************************************************************
g3::TaskQueue
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#include "g3Util.h"
#include "g3System.h"
#include "g3TaskQueue.h"

namespace g3
{

  static int workerId = 0;
  TaskQueue::WorkerThread::WorkerThread()
    : mOwner( NULL ),
    mTask(0),
    mId(workerId++)
  {

  }

  int TaskQueue::WorkerThread::run()
  {

#ifdef DEBUG
    char temp[256];
    sprintf_s(temp, "%u: worker: %d", mOwner->getId(), mId );
    setName( temp );
#endif

    while(mbActive)
    {
      mOwner->mPending.pop(mTask);

      if( mTask == NULL )
      {
        mOwner->workerWait();
        continue;
      }

      Atomic::increment(&mOwner->mNumWorking);

      if( !mTask->started() )
      {
        mTask->mWorkState = Task::eWsWorking;
        mTask->start();
      }

      Task::UpdateResult res = mTask->update();

      switch( res )
      {
      case Task::eComplete: 
        mTask->finish();
        mTask->mDone = true;
        mTask->mWorkState = Task::eWsComplete;
        mOwner->mComplete.push( mTask );
        break;

      case Task::eYield:
      case Task::eContinue:
        mOwner->pushWorking( mTask );
        break;

      case Task::ePause:
        mOwner->pause( mTask, mTask->getPauseMs() );
        break;

      case Task::eDelete:
        mTask->mDeleteWhenComplete = true;
        break;

      case Task::eWait:
        // do nothing
        mTask->mWorkState = Task::eWsWaiting;
        break;
      }

      Atomic::decrement(&mOwner->mNumWorking);
      mTask = NULL;
    }

    return 1;
  }

  TaskQueue::TaskQueue(uint32 id)
    : mNumWorking(0)
    , mId(id)
  {

  }

  int TaskQueue::initialize( int iWorkers )
  {
    mWorkReady = new Semaphore(iWorkers);
    mWorkers.resize(iWorkers );

    for( int i=0; i<iWorkers; ++i)
    {
      mWorkers[i].mOwner = this;
      mWorkers[i].start();
    }

    return 1;
  }

  int TaskQueue::update()
  {
    // update paused tasks
    if( !mPaused.empty() )
    {
      uint64 now = Singletons::get<System>()->getSystemTimeMs();
      while(true)
      {
        ScheduledTask front;
        if( !mPaused.pop( front ) )
          break;

        if( front.mStartTime > now )
        {
          mPaused.push( front );
          break;
        }

        mPending.push( front.mTask );
      }
    }

    // signal worker threads there is work to do
    if( !mPending.empty() )
      mWorkReady->signal( (int)mPending.size() );

    // fire off completion callbacks
    while( !mComplete.empty() )
    {
      // complete on the main thread
      Task* pTask = NULL;
      if( mComplete.pop(pTask) && pTask )
      {
        pTask->complete();
        pTask->mWorkState = Task::eWsNone;

        if( pTask->deleteWhenComplete() )
        {
          delete pTask;
          pTask = NULL;
        }
      }
    }

    return 1;
  }

  void TaskQueue::add( Task* pTask )
  {
    pTask->mWorkState = Task::eWsPending;
    mPending.push( pTask );
    mWorkReady->signal( (int)mPending.size() );
  }

  void TaskQueue::push( Task* pTask )
  {
    pTask->mWorkState = Task::eWsPending;
    mPending.push( pTask );
  }

  void TaskQueue::pushWorking( Task* pTask )
  {
    pTask->mWorkState = Task::eWsPending;
    mPending.push( pTask );
  }

  void TaskQueue::pause( Task* pTask, uint64 pauseTime  )
  {
    pTask->mWorkState = Task::eWsPending;
    mPaused.push( ScheduledTask( pTask, pauseTime ) );
  }

  int TaskQueue::shutdown()
  {
    if( mWorkReady != NULL )
    {
      delete mWorkReady;
      mWorkReady = NULL;
    }

    for( uint32 i=0; i<mWorkers.size(); ++i)
    {
      mWorkers[i].stop();
    }

    return 1;
  }

  void TaskQueue::workerWait()
  {
    mWorkReady->wait();
  }

}
