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

#include "Util.h"
#include "Platform.h"
#include "TaskQueue.h"

namespace FlightOS
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
        if (mTask->completeOnMainThread())
        {
          mTask->mWorkState = Task::eWsComplete;
          mOwner->mComplete.push(mTask);
        }
        else
        {
          mOwner->completeTask(mTask);
        }
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
    // SHOULD: process on another thread
    // update paused tasks
    if( !mPaused.empty() )
    {
      uint64 now = System::module<Platform>()->getSystemTimeMs();
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

    // SHOULD: process on another thread
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
        completeTask(pTask);
      }
    }

    return 1;
  }

  void TaskQueue::completeTask(Task* pTask)
  {
    pTask->complete();
    pTask->mWorkState = Task::eWsNone;

    if (pTask->deleteWhenComplete())
    {
      delete pTask;
      pTask = NULL;
    }
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
