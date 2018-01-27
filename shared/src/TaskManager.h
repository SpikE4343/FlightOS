/*************************************************************************************
g3::TaskManager
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3_TaskManager_H_INCLUDED
#define g3_TaskManager_H_INCLUDED

#include "g3Threading.h"
#include "g3ThreadSafeQueue.h"
#include "g3Task.h"
#include "g3TaskQueue.h"
#include "g3Singletons.h"
#include "g3ImUi.h"

#include <vector>
#include <unordered_map>

namespace g3
{
  class TaskManager : public Singleton, public IImUiWindow
  {
    g3SingletonDecl;
  public:
    static const int kGeneralTaskQueueId = 0;
    static const int kAssetTaskQueueId = 1;
    static const int kAppTaskQueueId = 2;
    static const int kGameTaskQueueId = 3;
    static const int kNetworkTaskQueueId = 4;

    TaskManager();

    static TaskManager* getInstance();

    int initialize( int iWorkers );
    int update();
    int shutdown();

    void add( Task* pTask );
    void push( Task* pTask );
    void pause( Task* pTask, uint64 pauseTime );

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