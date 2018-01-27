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

#ifndef FlightOS_ThreadSafeQueue_H_INCLUDED
#define FlightOS_ThreadSafeQueue_H_INCLUDED

#include "Threading.h"
#include "ObjectPool.h"

#include <queue>

namespace FlightOS
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