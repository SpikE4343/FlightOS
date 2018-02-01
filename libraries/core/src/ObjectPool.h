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

#ifndef FlightOS_ObjectPool_H_INCLUDED
#define FlightOS_ObjectPool_H_INCLUDED

#include "Util.h"
#include "Threading.h"

#include <list>

namespace FlightOS
{

class RefObject
{
public:
  RefObject() : iRef(0) {}
  virtual void reset()=0;
  int iRef;
};

class NodeObject : public RefObject
{
public:
  NodeObject() : mNodeNext(NULL) {}

private:
  NodeObject* mNodeNext;
};

class IObjectPool
{
public:
  
};

template< class T>
class ObjectPool : public IObjectPool
{
public:
	ObjectPool(int size)
  {
    Guard g(mListLock);
    for( int i=0; i < size; ++i )
    {
      mFreeList.push_back( new T() );
    }
  }

  T* grab()
  {
    // avoid locking if not needed
    // might generate more instances than required
    // TODO: shared_lock?
    if ( mFreeList.empty() )
    {
      T* t = new T();
      ++t->iRef;
      return t;
    }

    Guard g(mListLock);
    T* t = mFreeList.front();
    ++t->iRef;
    mFreeList.pop_front();
    return t;
  }

  void release(T*t)
  {
    // is atomic needed here?
    if (Atomic::decrement((long*)&t->iRef) > 0)
    {
      return;
    }
    
    t->reset();
    t->iRef = 0;

    Guard g(mListLock);
    mFreeList.push_back(t);
  }
	
private:
	typedef std::list< T* > NodeList;
  NodeList mFreeList;
  Lock mListLock;

	NodeObject* mHead;
};




}


#endif