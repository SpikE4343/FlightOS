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

#ifndef FlightOS_ObjectPoolManager_h_INCLUDED
#define FlightOS_ObjectPoolManager_h_INCLUDED

#include <memory>
#include <shared_mutex>


#include "Types.h"
#include "Threading.h"
#include "EngineModule.h"
#include "ObjectPool.h"
#include "ImUiWindow.h"

namespace FlightOS
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

