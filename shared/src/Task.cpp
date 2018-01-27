/*************************************************************************************
  g3::Task
  Copyright (c) 2013 John Rohrssen
  Email: johnrohrssen@gmail.com
*************************************************************************************/

#include "g3Types.h"
#include "g3Task.h"

namespace g3
{

static uint32 sTaskId=1;

Task::Task() : 
	mbStarted(false), 
	mId(sTaskId++), 
	mPauseMs(0),
	mPriority(0),
	mDeleteWhenComplete(false),
	mWorkState( eWsNone ),
  mDone(false)

{

}

}
