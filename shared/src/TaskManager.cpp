/*************************************************************************************
g3::TaskManager
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#include "g3Util.h"
#include "g3System.h"
#include "g3TaskManager.h"

namespace g3
{

  g3SingletonDef(TaskManager);

  TaskManager::TaskManager()
    : mNumWorking(0)
    , mNumPending(0)
  {
  }

  int TaskManager::initialize( int iWorkers )
  {
    createQueue( 0, iWorkers );

    return 1;
  }

  int TaskManager::update()
  {
    mNumWorking = 0;
    mNumPending = 0;

    mQueuesLock.grab();
    for( auto it = mQueues.begin(); it != mQueues.end(); ++it )
    {
      TaskQueue* q = it->second;
      q->update();

      mNumWorking += q->getNumWorking();
      mNumPending += q->getNumPending();
    }
    mQueuesLock.release();

    return 1;
  }

  TaskQueue* TaskManager::createQueue( uint32 id, int numWorkerThreads )
  {
    TaskQueue* q = new TaskQueue(id);
    q->initialize( numWorkerThreads );
    mQueuesLock.grab();
    mQueues[id] = q;
    mQueuesLock.release();

    return q;
  }

  void TaskManager::add( Task* pTask )
  {
    if( pTask->mWorkState != Task::eWsNone &&
      pTask->mWorkState != Task::eWsWaiting)
      return;

    TaskQueue* q = mQueues[pTask->getQueue()];
    q->add( pTask );
  }

  void TaskManager::push( Task* pTask )
  {
    if( pTask->mWorkState != Task::eWsNone &&
      pTask->mWorkState != Task::eWsWaiting)
      return;

    TaskQueue* q = mQueues[pTask->getQueue()];
    q->push( pTask );
  }

  void TaskManager::pause( Task* pTask, uint64 pauseTime  )
  {
    pTask->mWorkState = Task::eWsPending;

    TaskQueue* q = mQueues[pTask->getQueue()];
    q->pause( pTask, pauseTime );
  }

  int TaskManager::shutdown()
  {
    mQueuesLock.grab();

    for( auto it = mQueues.begin(); it != mQueues.end(); ++it )
    {
      TaskQueue* q = it->second;
      q->shutdown();
    }

    mQueuesLock.release();
    return 1;
  }

  void TaskManager::DrawUI(float s )
  {
    if (ImGui::CollapsingHeader("TaskManager"))
    {
      ImGui::LabelText("Working", "%u", mNumWorking);
      ImGui::LabelText("Pending", "%u", mNumPending);
      ImGui::LabelText("Queues", "%u", mQueues.size());

      if( ImGui::CollapsingHeader( "Queues" ) )
      {
        for( auto it : mQueues )
        {
          auto* queue = it.second;
          ImGui::Text( "[%u]", it.first );

          ImGui::LabelText( "Pending", "%u", queue->getNumPending() );
          ImGui::LabelText( "Working", "%u", queue->getNumWorking() );
          //ImGui::ProgressFloat( "Working", queue->getNumWorking() / (float)queue->getWorkerCount() );
        }
      }
    }
  }

}
