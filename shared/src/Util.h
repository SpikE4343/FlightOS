/*************************************************************************************
g3::Util
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3_Util_H_INCLUDED
#define g3_Util_H_INCLUDED

#include <stddef.h>
#include <assert.h>
#include <unordered_map>
#include <stdlib.h>

#include "g3Types.h"
#include "g3Threading.h"

#define g3Assert(x) assert(x)

namespace g3
{

  namespace String
  {
    std::string replace( std::string& str, const std::string find, const std::string& newValue );
  };

  template< typename Ret, typename Arg1 >
  class Delegate
  {
  public:
    typedef Ret (*CallFuncPtr)(void*, Arg1);

    template< class Obj, Ret (Obj::*FuncPtr)(Arg1)> 
    static Ret Call(void* obj, Arg1 request)
    {
      Obj * o = (Obj*)obj;
      return (o->*FuncPtr)(request);
    }

    template< class Obj, Ret (Obj::*FuncPtr)(Arg1)>
    static Delegate Method( Obj* obj )
    {
      Delegate<Ret,Arg1> del;
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
    Event( ) 
    {

    }

    template< class Obj >
    static uint64 genId( Obj* obj, void* cbf )
    {
      int64 id = (uint32)obj;
      id <<= 32;
      id |= (uint32)cbf;

      return id;
    }

    template< class Obj, Ret (Obj::*MethodPtr)(Arg1) >
    void add(Obj*obj)
    {
      Callback cb = Callback::Method< Obj, MethodPtr >( obj );

      Guard cbGuard(mCallbackLock);
      mCallbacks[genId<Obj>(obj,(void*)cb.getCall())] = cb;
    }

    template< class Obj, Ret (Obj::*MethodPtr)(Arg1) >
    void remove(Obj*obj)
    {
      Callback cb = Callback::Method< Obj, MethodPtr >( obj );

      Guard cbGuard(mCallbackLock);
      mCallbacks.erase(genId<Obj>(obj,(void*)cb.getCall()));
    }

    void signal(Arg1 arg)
    {
      Guard cbGuard(mCallbackLock);
      CallbacksMap::iterator it = mCallbacks.begin();
      while(  it != mCallbacks.end() )
      {
        Callback& cb = it->second;
        ++it;
        cb( arg );
      }
    }

    bool isValid() const { return !mCallbacks.empty(); }

    CallbacksMap mCallbacks;
    Lock mCallbackLock;
  };
}
#endif