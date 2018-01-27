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

#ifndef FlightOS_Threading_H_INCLUDED
#define FlightOS_Threading_H_INCLUDED

#ifdef WIN32
#include <windows.h>
#endif

namespace FlightOS
{
  class Thread
  {
  public:
    Thread();
    static void sleep(long ms );
    static int getCurrentThreadId();
    static int Thread_Starter(void* start_data);

    void setName(const char* name );
    int start();
    int stop();
    virtual int run()=0;

  protected:
    int miThreadId;
    bool mbActive;
  };

  class Lock
  {
  public:
    Lock();

    bool tryGrab();
    bool grab();
    bool release();
  private:
    volatile long iLock;
  };

  class Guard
  {
  public:
    Guard(Lock& lock )
      : mLock(lock)
    {
      mLock.grab();
    }

    ~Guard()
    {
      mLock.release();
    }

  private:
    Lock& mLock;
  };

  class Semaphore
  {
  public:
    Semaphore(int iMaxCount=1);
    ~Semaphore();

    void wait();
    void signal(int iCount=1);
  private:
#ifdef WIN32
    HANDLE mhOsHdl;
#endif
  };

  class Condition
  {
  public:
    Condition();
    ~Condition();

    void wait();
    void signal();
    void signalAll();
  };


  namespace Atomic
  {
    long increment( volatile long* value );
    long decrement( volatile long* value );
    long compareAndSwap( volatile long* value, long set, long compare );

    long add( volatile long* value, long num );
  }

}

#endif