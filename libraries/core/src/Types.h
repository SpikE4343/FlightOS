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

#ifndef FlightOS_Types_H_INCLUDED
#define FlightOS_Types_H_INCLUDED

#include "shiny/Shiny.h"

typedef unsigned int   uint32;
typedef          int   int32;
typedef          short int16;
typedef unsigned short uint16;
typedef unsigned char  uint8;

#if _WIN32
typedef          __int64 int64;
typedef unsigned __int64 uint64;
typedef unsigned long long uint64;

#define FlightOS_packed( s ) __pragma( pack(push, 1) ) s __pragma( pack(pop) )

#define FlightOS_LittleEndian
#else

typedef int64_t   int64;
typedef u_int64_t uint64;

//#error unsupported platform
#endif

typedef uint32 ASSETID;

namespace FlightOS
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
      //static TypeId id = 0;
      if (id == 0)
        id = TypeReferenceId::getNext();

      return id;
    }

  private:
    static TypeId id;
  };


  template<class TType>
  TypeId TypeReference<TType>::id = 0;
}
#endif