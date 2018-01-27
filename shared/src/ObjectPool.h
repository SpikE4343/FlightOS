/*************************************************************************************
  g3::MemoryBuffer
  Copyright (c) 2013 John Rohrssen
  Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3_ObjectPool_H_INCLUDED
#define g3_ObjectPool_H_INCLUDED

#include "g3Util.h"
#include "g3Threading.h"

#include <list>

namespace g3
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