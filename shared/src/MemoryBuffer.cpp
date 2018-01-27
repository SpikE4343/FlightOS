/*************************************************************************************
g3::NetworkMessages
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#include "g3MemoryBuffer.h"

namespace g3
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
