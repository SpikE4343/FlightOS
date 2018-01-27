/*************************************************************************************
g3::Client
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3Singletons_h_INCLUDED
#define g3Singletons_h_INCLUDED

#include <map>
#include <memory>

#include "g3Types.h"
#include "g3Singleton.h"
#include "g3Threading.h"



namespace g3
{
  class Singletons
  {
  private:
    typedef std::map<uint64, std::shared_ptr<Singleton>> SingletonMap;
    static SingletonMap singletons;
    static uint32 mNextId;
    static Lock mInsertLock;

    template< class TSingleton >
    class SingletonHolder
    {
    public:
      static std::shared_ptr<TSingleton> instance;
    };

  public:

    static void reset()
    {
      // try to release in reverse set order
      for( auto &i = singletons.rbegin(); i != singletons.rend(); ++i  )
      {
        i->second.reset();
      }

      singletons.clear();
      mNextId=0;
    }

    template <class TSingleton> 
    static std::shared_ptr<TSingleton> create()
    {
      return set( std::make_shared<TSingleton>() );
    }

    template <class TSingleton> 
    static std::shared_ptr<TSingleton> set( std::shared_ptr<TSingleton> singleton )
    {
      if( SingletonHolder<TSingleton>::instance != nullptr )
      {
        return nullptr;
      }

      Guard g( mInsertLock );

      SingletonHolder<TSingleton>::instance = singleton;

      auto pr = singletons.insert( std::make_pair( ++mNextId, singleton ));

      if( !pr.second )
      {
        return nullptr;
      }

      return singleton;
    }

    template <class TSingleton> 
    static TSingleton* get()
    {
      return SingletonHolder<TSingleton>::instance.get();
    }
  };
}

#endif

