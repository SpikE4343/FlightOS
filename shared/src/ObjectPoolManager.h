/*************************************************************************************
g3::EngineModule

Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3ObjectPoolManager_h_INCLUDED
#define g3ObjectPoolManager_h_INCLUDED

#include <memory>
#include <shared_mutex>


#include "g3Types.h"
#include "g3Threading.h"
#include "g3EngineModule.h"
#include "g3ObjectPool.h"
#include "g3ImUiWindow.h"

namespace g3
{

  class IImuUiWindow;

  class ObjectPoolManager
    : public EngineModule
  {
  protected:
    
  public:

    template< class TPoolType>
    TPoolType* grab()
    {
      TypeId id = TypeReference<TPoolType>::getId();

      // lock for reading, prevent writing
      mPoolsLock.lock_shared();

      auto iter = mPools.find(id);
      ObjectPool<TPoolType>* pool = NULL;
      if (iter != mPools.end())
      {
        pool = (ObjectPool<TPoolType>*)(iter->second);
        mPoolsLock.unlock_shared();
      }
      else
      {
        // release reading lock
        mPoolsLock.unlock_shared();

        // Create new pool
        pool = new ObjectPool<TPoolType>(1);

        PoolsLock write(mPoolsLock);
        mPools[id] = pool;
      }

      return (TPoolType*)(pool->grab());
    }

    template< class TPoolType>
    void release(TPoolType* target)
    {
      TypeId id = TypeReference<TPoolType>::getId();

      // lock for reading, prevent writing
      mPoolsLock.lock_shared();

      auto iter = mPools.find(id);
      ObjectPool<TPoolType>* pool = NULL;
      if (iter == mPools.end())
      {
        // no pool for this type, should it be deleted?
        return;
      }
      
      pool = (ObjectPool<TPoolType>*)(iter->second);
      mPoolsLock.unlock_shared();

      pool->release(target);
    }

    virtual void DrawUI(float dt) {}

    ObjectPoolManager() {};
    virtual ~ObjectPoolManager() 
    {
      removeAll();
    };

  private:

    void removeAll() {}
    int prune() { return 0; }

    typedef std::unordered_map<TypeId, IObjectPool*> PoolTypeHash;
    PoolTypeHash mPools;

    typedef std::shared_lock<std::shared_timed_mutex> PoolsSharedLock;
    typedef std::shared_lock<std::shared_timed_mutex> PoolsLock;
    std::shared_timed_mutex mPoolsLock;
  };

} // g3

#endif

