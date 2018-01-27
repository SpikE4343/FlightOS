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

#include "Util.h"
#include "Platform.h"
#include "Asset.h"
#include "Log.h"
#include "TaskManager.h"
#include "MemoryBuffer.h"

namespace FlightOS
{

  AssetData::~AssetData()
  {

  }

  AssetData::Header::Header() :
    mType(0),
    mVersion(0),
    mId(0),
    mFlags(0),
    mDeflateSize(0),
    mSize(0)
  {
    mAsset[0] = 'A';
    mAsset[1] = 'S';
    mAsset[2] = 'E';
    mAsset[3] = 'T';
  }

  Asset::Asset() :
    mStatus(eNone),
    mAssetId(0),
    mData(0),
    mChildrenReady(false),
    mNextInstanceId(1)
  {
  }

  bool Asset::canLoad() const 
  {
    switch( mStatus )
    {
    case eReady:
    case eStartLoading:
    case eDataLoading:
    case eDataLoadingComplete:
    case eLoading:

    case eStartReloading:
    case eReloading:

    case eStartUnloading:
    case eUnloading:

    case eStartSaving:
    case eSaving:
    case eError:
      return false;
    }

    return true;
  }

  void Asset::load()
  {
    if( !isReady() && canLoad() )
    {
      mStatus = eStartLoading;
    }

    System::module<TaskManager>()->add( this );
  }

  bool Asset::save(const std::string& path, bool compress)
  {
    AssetData::Header header;
    header.mType = mData->getType();
    header.mNumAssetRefs = mDependencies.size();
    header.mVersion = 1;
    header.mDeflateSize = 0;
    header.mFlags = compress ? AssetData::Header::eCompressed : 0;

    MemoryBuffer buf;

    // save asset references
    AssetMap::iterator it = mDependencies.begin();
    for( uint32 i =0; i < header.mNumAssetRefs && it != mDependencies.end(); ++i,++it )
    {
      uint32 id = it->first;
      buf.write( id );
    }

    mData->write( buf );
    header.mSize = buf.size();

    System::module<Platform>()->CreateFilePath( path );

    char file[256];
    _snprintf_s( file, sizeof(file), "%s/%08x.g3a", path.c_str(), mAssetId );

    FILE* fp = NULL;
    fopen_s( &fp, file, "wb" );
    if( fp == NULL )
    {
      Log::error( "Asset::save: Unable to open file: %s", file );
      return false;
    }

    int written = fwrite( &header, 1, sizeof(header), fp );
    written += fwrite( buf.data(), 1, buf.size(), fp );
    fclose(fp);

    return written == ( buf.size() + sizeof(header) );
  }

  Task::UpdateResult Asset::update()
  {
    Task::UpdateResult result = Task::eContinue;

    switch( mStatus )
    {

    case eStartLoading:
      mStatus = eLoading;
      if( loadFromFile() || loadFromClump() )
      {
        mStatus = eReady;
      }
      else 
      {
        mStatus = eError;
        result = Task::eComplete;
      }
      break;

    case eReady:
      Ready.signal(this);
      result = Task::eComplete;
      break;
    };

    return result;
  }

  bool Asset::loadFromFile()
  {
    mStatus = eDataLoading;
    std::string fullpath = System::module<AssetManager>()->getRoot();

    char file[256];
    _snprintf_s( file, sizeof(file), "%s/%08x.g3a", fullpath.c_str(), mAssetId );

    FILE* fp = NULL;
    fopen_s( &fp, file, "rb" );
    if( fp == NULL )
    {
      Log::error( "LoadAssetTask: Unable to open file: %s", file );
      mStatus = eError;
      return false;
    }

    AssetData::Header header;

    int read = fread( (void*)&header, 1, sizeof(header), fp );
    if( read != sizeof(header ) )
    {
      Log::error( "LoadAssetTask: Unable to read asset header from file: %s", fullpath.c_str() );
      fclose(fp);
      mStatus = eError;
      return false;
    }

    AssetData* asset = AssetManager::createAssetData( (AssetTypes::Types)header.mType );
    if( asset == NULL )
    {
      Log::error( "LoadAssetTask: Unable to create asset of type: %u from file: %s", header.mType, fullpath.c_str() );
      fclose(fp);
      mStatus = eError;
      return false;
    }

    MemoryBuffer buf;
    buf.resize( header.mSize );

    read = fread( buf.data(), 1, header.mSize+header.mNumAssetRefs*(sizeof(AssetData::AssetRef)), fp );
    if( read != header.mSize )
    {
      delete asset;
      Log::error( "LoadAssetTask: Unable to read asset data of size: %u from file: %s", header.mSize, fullpath.c_str() );
      fclose(fp);
      mStatus = eError;
      return false;
    }

    for( int i=0; i < (int)header.mNumAssetRefs; ++i )
    {
      uint32 assetid=0;
      buf.read( assetid );
      mDependencies[ assetid ] = NULL;
    }

    fclose(fp);

    asset->read( buf );
    mData = asset;

    return true;
  }

  bool Asset::loadFromClump()
  {
    return false;
  }

  void Asset::reset()
  {

  }

}
