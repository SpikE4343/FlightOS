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
#ifndef FlightOS_Util_H_INCLUDED
#define FlightOS_Util_H_INCLUDED

#include <stddef.h>
#include <assert.h>
#include <unordered_map>
#include <stdlib.h>

#include "Types.h"
#include "Threading.h"
//#include "ObjectPoolManager.h"

#define g3Assert(x) assert(x)
#define g3AssetNotNull(x) assert(x != NULL);

namespace FlightOS
{
  //class Task; 
  namespace String
  {
    std::string replace(std::string& str, const std::string find, const std::string& newValue);
  };

  template< typename Ret, typename Arg1 >
  class Delegate
  {
  public:
    typedef Ret(*CallFuncPtr)(void*, Arg1);

    template< class Obj, Ret(Obj::*FuncPtr)(Arg1)>
    static Ret Call(void* obj, Arg1 request)
    {
      Obj * o = (Obj*)obj;
      return (o->*FuncPtr)(request);
    }

    template< class Obj, Ret(Obj::*FuncPtr)(Arg1)>
    static Delegate Method(Obj* obj)
    {
      Delegate<Ret, Arg1> del;
      del.mObj = obj;
      // template overload Call for this object type
      del.mCall = &Call< Obj, FuncPtr >;
      return del;
    }

    Ret operator() (Arg1 arg)
    {
      return (*mCall)(mObj, arg);
    }

    CallFuncPtr getCall() { return mCall; }

  private:
    void* mObj;
    CallFuncPtr mCall;
  };

  template< typename Ret, typename Arg1 >
  class Event
  {
  public:
    typedef Delegate< Ret, Arg1 > Callback;
    typedef std::unordered_map< uint64, Callback > CallbacksMap;
    Event()
    {

    }

    template< class Obj >
    static uint64 genId(Obj* obj, void* cbf)
    {
      int64 id = (intptr_t)obj;
      id <<= 32;
      id |= (uint32)(intptr_t)cbf;

      return id;
    }

    template< class Obj, Ret(Obj::*MethodPtr)(Arg1) >
    void add(Obj*obj)
    {
      Callback cb = Callback::Method< Obj, MethodPtr >(obj);

      // ERROR: deadlock created when called while signaling
      Guard cbGuard(mCallbackLock);
      mCallbacks[genId<Obj>(obj, (void*)cb.getCall())] = cb;
    }

    template< class Obj, Ret(Obj::*MethodPtr)(Arg1) >
    void remove(Obj*obj)
    {
      Callback cb = Callback::Method< Obj, MethodPtr >(obj);

      // ERROR: deadlock created when called while signaling
      Guard cbGuard(mCallbackLock);
      mCallbacks.erase(genId<Obj>(obj, (void*)cb.getCall()));
    }

    void signal(Arg1 arg)
    {
      Guard cbGuard(mCallbackLock);
      CallbacksMap::iterator it = mCallbacks.begin();
      while (it != mCallbacks.end())
      {
        Callback& cb = it->second;
        ++it;
        cb(arg);
      }
    }

    bool isValid() const { return !mCallbacks.empty(); }

    // TODO: add call queue and process async by pooling callback call tasks
    //       add signalSync that fires all callbacks in the same frame
    CallbacksMap mCallbacks;
    Lock mCallbackLock;
  };

  /*template< typename Ret, typename Arg1 >
  class EventAsync
    : public Event<Ret, Arg1>
  {
    class SignalTask : public Task
    {
    public:
      SignalTask()
        : mCallback(NULL)
        , mArgument(NULL)
      {

      }

      void set(Callback* cb, Arg1* arg1)
      {
        mCallback(cb);
        mArgument(arg1);
      }

      UpdateResult update()
      {
        if (mCallback != NULL)
        {
          mCallback(mArgument);
        }
        return Task::eComplete;
      }

    private:
      Callback* mCallback;
      Arg1* mArgument;
    };

    virtual void signal(Arg1 arg)
    {
      Guard cbGuard(mCallbackLock);
      CallbacksMap::iterator it = mCallbacks.begin();
      while (it != mCallbacks.end())
      {
        Callback& cb = it->second;
        ++it;

        auto task = Engine::module<ObjectPoolManager>->grab<SignalTask>();
        task->set(cb, arg);
        Engine::module<TaskManager>()->add(task);
      }
    }
  };*/
}
#endif