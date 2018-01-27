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

#ifndef FlightOS_MemoryBuffer_H_INCLUDED
#define FlightOS_MemoryBuffer_H_INCLUDED

#include "Util.h"
#include "ObjectPool.h"

#include <vector>

namespace FlightOS
{

  class RefObject;

  class MemoryBuffer : public RefObject
  {
  public:
    MemoryBuffer();

    template< typename wT >
    int write( wT& v );

    template< typename wT >
    int write( const wT& v );

    template< typename rT >
    int read( rT& v );

    int write( uint8* pData, int len );
    int read( uint8* pData, int len );

    int size() const;

    int getPos() const { return mPos; }
    void setPos( int pos );

    uint8* data();
    const uint8* data() const;
    uint8* current();
    void resize(int size );
    void assign( const MemoryBuffer& from );

    void reset();

  private:
    typedef std::vector<uint8> Buffer;
    Buffer mBuffer;

    int mPos;
  };

  template< typename wT >
  inline int MemoryBuffer::write( wT& v )
  {
    int size = sizeof(wT);
    int len = (int)mBuffer.size() - mPos;

    if( size > len )
      mBuffer.resize( mBuffer.size() + size );

    *((wT*)&mBuffer[mPos]) = v;
    mPos += size;

    return size;
  }

  template< typename wT >
  inline int MemoryBuffer::write( const wT& v )
  {
    int size = sizeof(wT);
    int len = (int)mBuffer.size() - mPos;

    if( size > len )
      mBuffer.resize( mBuffer.size() + size );

    *((wT*)&mBuffer[mPos]) = v;
    mPos += size;

    return size;
  }

  template< typename rT >
  inline int MemoryBuffer::read( rT& v )
  {
    int size = sizeof(rT);
    int len = (int)mBuffer.size() - mPos;

    if( size > len )
      return 0;

    v = *((rT*)(&mBuffer[mPos]));
    mPos += size;

    return size;
  }


  template<>
  inline int MemoryBuffer::read( std::string& v )
  {
    int size = (int)v.length() + sizeof( uint16 );
    int len = (int)mBuffer.size() - mPos;

    if( size > len )
      return 0;

    uint16 strlen = 0;
    read( strlen );
    v.resize( strlen );

    assert( strlen == read( (uint8*)v.data(), strlen ) );
    v.shrink_to_fit();

    return size;
  }

  template<>
  inline int MemoryBuffer::write( std::string& v )
  {
    uint16 strlen = (uint16)v.length();
    write( strlen );
    write( (uint8*)v.data(), strlen );
    return strlen;
  }

}


#endif