/*************************************************************************************
  g3::ThreadSafeQueue
  Copyright (c) 2013 John Rohrssen
  Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3_ThreadSafeQueue_H_INCLUDED
#define g3_ThreadSafeQueue_H_INCLUDED

#include "g3Threading.h"
#include "g3ObjectPool.h"

#include <queue>

namespace g3
{
template< class T >
class ThreadSafeQueue
{
public:
	ThreadSafeQueue()
	{

	}

	void push( T& v)
	{
		mLock.grab();
		mQueue.push(v);
		mLock.release();
	}

	T& front()
	{
		return mQueue.top();
	}

	bool pop(T& v)
	{
    bool ret = false;
		mLock.grab();
    if( !mQueue.empty() )
    {
		  v = mQueue.front();
		  mQueue.pop();
      ret = true;
    }
		mLock.release();
		return ret;
	}

	bool pop()
	{
    bool ret = false;
		mLock.grab();
    if( !mQueue.empty() )
    {
		  mQueue.pop();
      ret = true;
    }
		mLock.release();
		return ret;
	}

	size_t size() const { return mQueue.size(); }

	bool empty() const { return mQueue.empty(); }

private:
	typedef std::queue<T> TQueue;
	TQueue mQueue;
	Lock mLock;
};

template< class T >
class ThreadSafePriorityQueue
{
public:
	ThreadSafePriorityQueue()
	{

	}

	void push( T& v)
	{
		mLock.grab();
		mQueue.push(v);
		mLock.release();
	}

	T& front()
	{
		return mQueue.top();
	}

	bool pop(T& v)
	{
    bool ret = false;
		mLock.grab();
    if( !mQueue.empty() )
    {
      v = mQueue.top();
		  mQueue.pop();
      ret = true;
    }
		mLock.release();
		return ret;
	}

	bool pop()
	{
    bool ret = false;
		mLock.grab();
    if( !mQueue.empty() )
    {
		  mQueue.pop();
      ret = true;
    }
		mLock.release();
		return ret;
	}

	size_t size() const { return mQueue.size(); }

	bool empty() const { return mQueue.empty(); }

private:
	typedef std::priority_queue<T> TQueue;
	TQueue mQueue;
	Lock mLock;
};
}


#endif