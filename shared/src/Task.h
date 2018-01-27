/*************************************************************************************
  g3::Task
  Copyright (c) 2013 John Rohrssen
  Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3_Task_H_INCLUDED
#define g3_Task_H_INCLUDED

#include "g3Types.h"
#include "g3Util.h"

namespace g3
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

  void wake();
  bool isDone() const { return mDone; }
	bool deleteWhenComplete() const { return mDeleteWhenComplete; }

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