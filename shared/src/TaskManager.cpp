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
#include "System.h"
#include "TaskManager.h"

namespace FlightOS
{

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

  // alias for adding
  void TaskManager::wake(Task* pTask)
  {
    add(pTask);
  }

  // add to end of task's queue and signal workers
  void TaskManager::add( Task* pTask )
  {
    if( pTask->mWorkState != Task::eWsNone &&
      pTask->mWorkState != Task::eWsWaiting)
      return;

    TaskQueue* q = mQueues[pTask->getQueue()];
    q->add( pTask );
  }

  // add to end of task's queue, doesn't signal workers
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
