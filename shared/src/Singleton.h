/*************************************************************************************
g3::Singleton
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3Singleton_h_INCLUDED
#define g3Singleton_h_INCLUDED

#include "g3Types.h"

#define g3SingletonDecl private: \
  friend class Singletons;

#define g3SingletonDef( x ) std::shared_ptr<x> g3::Singletons::SingletonHolder<x>::instance = nullptr;


namespace g3
{
  class Singleton
  {
  public:
    Singleton();

    //virtual uint32 getSingletonId() const =0;
    //virtual void setSingletonId(uint32)=0;

    virtual void reset(){}
    virtual ~Singleton(void);
  };
}

#endif

