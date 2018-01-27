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

#include "Types.h"
#include "Task.h"

namespace FlightOS
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
