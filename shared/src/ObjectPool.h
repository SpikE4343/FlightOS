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
  NodeObject() : mNext(0) {}
  NodeObject* mNext;
};


template< class T>
class ObjectPool
{
public:
	ObjectPool(int size)
  {
    for( int i=0; i < size; ++i )
    {
      mFreeList.push_back( new T() );
    }
  }

  T* grab()
  {
    if ( mFreeList.empty() )
    {
      T* t = new T();
      t->iRef++;
      return t;
    }

    mListLock.grab();
    T* t = mFreeList.front();
    t->iRef++;
    mFreeList.pop_front();
    mListLock.release();
    return t;
  }

  void release(T*t)
  {
    mListLock.grab();
    Atomic::decrement((long*)&t->iRef);
    if( t->iRef <= 0)
    {
      t->reset();
      t->iRef = 0;
      mFreeList.push_back(t);
    }
    mListLock.release();
  }
	
private:
	typedef std::list< T* > NodeList;
  NodeList mFreeList;
  Lock mListLock;

	NodeObject* mHead;
};




}


#endif