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

#ifndef FlightOS_Task_H_INCLUDED
#define FlightOS_Task_H_INCLUDED

#include "Types.h"
#include "Util.h"

namespace FlightOS
{
class Task
{
public:
  
  enum UpdateResult
  {
    eContinue,
    eYield,
		ePause,
    eComplete,
    eWait,
		eDelete
  };

	enum Priority
	{
		eNormalPriority = 0,
		eHighPriority = -1
	};

	enum WorkState
	{
		eWsNone,
		eWsPending,
    eWsWaiting,
		eWsWorking,
		eWsComplete
	};

	Task();
  virtual ~Task() {}

  typedef Event< void, Task* > TaskEvent;

	bool started() const { return mbStarted; }
	// worker 
  virtual int start(){return 1;}
	virtual UpdateResult update()=0;
  virtual int finish(){return 1;}
	virtual uint32 getQueue() const { return 0; }

	// main
  virtual int complete(){return 1;}
  TaskEvent Complete;

  bool isDone() const { return mDone; }
	bool deleteWhenComplete() const { return mDeleteWhenComplete; }
  virtual bool completeOnMainThread() const { return true; }

	uint64 getPauseMs() const { return mPauseMs; }
	int getPriority() const { return mPriority; }
	void setPriority( int priority ) { mPriority = priority; }



	WorkState getWorkState() const { return mWorkState; }

	bool operator< ( const Task& rhs )
	{
		return mPriority < rhs.mPriority;
	}

  friend class TaskManager;
	friend class TaskQueue;
protected:

	void pause( uint64 pausems ) { mPauseMs = pausems; }

	bool mbStarted;
  bool mDone;
	bool mDeleteWhenComplete;
  uint32 mId;
	uint64 mPauseMs;
	int		mPriority;

private:

	WorkState mWorkState;
};

}


#endif