/*************************************************************************************
g3::Threading
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3_Threading_H_INCLUDED
#define g3_Threading_H_INCLUDED

#ifdef WIN32
#include <windows.h>
#endif

namespace g3
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