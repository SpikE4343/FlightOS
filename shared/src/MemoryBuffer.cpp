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

#include "MemoryBuffer.h"

namespace FlightOS
{

  MemoryBuffer::MemoryBuffer() :
    mPos(0)
  {

  }

  int MemoryBuffer::size() const
  {
    return mBuffer.size();
  }

  void MemoryBuffer::setPos( int pos )
  {
    mPos = pos;
  }

  uint8* MemoryBuffer::data()
  {
    if( mBuffer.empty() )
      return NULL;

    return &mBuffer[0];
  }

  const uint8* MemoryBuffer::data() const
  {
    if( mBuffer.empty() )
      return NULL;

    return &mBuffer[0];
  }

  uint8* MemoryBuffer::current()
  {
    return &mBuffer[mPos];
  }

  void MemoryBuffer::resize(int size )
  {
    mBuffer.resize(size);
  }

  void MemoryBuffer::assign( const MemoryBuffer& from )
  {
    resize( from.size() );
    memcpy( mBuffer.data(), from.data(), size());
  }

  int MemoryBuffer::write( uint8* pData, int len )
  {
    int remain = mBuffer.size() - mPos;
    if( remain < len )
      mBuffer.resize( mBuffer.size() + (len - remain) );

    memcpy( &mBuffer[mPos], pData, len );
    mPos += len;
    return len;
  }

  int MemoryBuffer::read( uint8* pData, int len )
  {
    int remain = mBuffer.size() - mPos;
    if( remain < len )
      return 0;

    memcpy( pData, &mBuffer[mPos], len );
    mPos += len;
    return len;
  }

  void MemoryBuffer::reset()
  {
    mPos = 0;
  }
}
