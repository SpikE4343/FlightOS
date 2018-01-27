/*************************************************************************************
g3::Types
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3_Types_H_INCLUDED
#define g3_Types_H_INCLUDED

#include "Shiny.h"

typedef unsigned int   uint32;
typedef          int   int32;
typedef          short int16;
typedef unsigned short uint16;
typedef unsigned char  uint8;

#if _WIN32
typedef          __int64 int64;
typedef unsigned __int64 uint64;
typedef unsigned long long uint64;

#define g3_packed( s ) __pragma( pack(push, 1) ) s __pragma( pack(pop) )

#define g3_LittleEndian
#else

#error unsupported platform
#endif

typedef uint32 ASSETID;

namespace g3
{
  typedef uint32 TypeId;

  class TypeReferenceId
  {
  public:
    static TypeId getNext();

  private:
    static long nextId;
  };

  //
  // Create a static variable for each type
  //
  template< class TType >
  class TypeReference
  {
  public:
    static TypeId getId()
    {
      if (id == 0)
        id = TypeReferenceId::getNext();

      return id;
    }

  private:
    static TypeId id;
  };
}
#endif