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
#include "AssetManager.h"
#include "Log.h"
#include "ObjectPoolManager.h"
#include "TaskManager.h"
#include "MemoryBuffer.h"
#include "Json.h"

namespace FlightOS
{
  AssetManager::AssetTypeMap AssetManager::sAssetTypeMap;

  AssetManager::AssetManager() 
    : mState(eInitial)
  {
    
  }

  int AssetManager::initialize(const InitInfo& info)
  {
    mRoot = info.mContentRoot;

    System::module<TaskManager>()->createQueue( 
      TaskManager::kAssetTaskQueueId, 
      info.mNumWorkerThreads );

    System::module<TaskManager>()->add(this);

    Log::info( "AssetManager initialized" );
    return 1;
  }

  Task::UpdateResult AssetManager::update()
  {
    switch (mState)
    {
    case eInitial:
      mState = eLoadingIndex;
      break;

    case eLoadingIndex:

      if (loadIndex(mRoot))
      {
        mState = eLoaded;
      }
      break;

    case eLoaded:
      return Task::ePause;
    }

    return Task::eContinue;
  }

  int AssetManager::shutdown()
  {
    Log::info( "AssetManager shutdown" );
    return 1;
  }

  void AssetManager::registerAssetType( AssetTypes::Types type, CreateAssetPtr cb)
  {
    auto& creators = sAssetTypeMap[ type ];
    creators.mCreateAssetData = cb;
  }

  AssetData* AssetManager::createAssetData( AssetTypes::Types type )
  {
    auto it = sAssetTypeMap.find( type );
    if( it == sAssetTypeMap.end() )
      return NULL;

    return (it->second.mCreateAssetData)();
  }

  Asset* AssetManager::getAsset( ASSETID id )
  {
    // TODO: only block reading when writing
    Guard mapGuard(mAssetMapLock);

    AssetMap::iterator it = mAssetMap.find( id );
    Asset* asset = NULL;

    if( it == mAssetMap.end() )
    {
      asset = System::module<ObjectPoolManager>()->grab<Asset>();
      asset->setAssetId( id );
      mAssetMap[ id ] = asset;
    }
    else
    {
      asset = it->second;
    }

    return asset;
  }

  void AssetManager::releaseAsset( ASSETID id )
  {
    Guard mapGuard(mAssetMapLock);
    
    AssetMap::iterator it = mAssetMap.find( id );
    if( it != mAssetMap.end() )
    {
      System::module<ObjectPoolManager>()->release(it->second);
      mAssetMap.erase( it );
    }
  }

  bool AssetManager::loadIndex( const std::string& file )
  {
    Log::info("AssetManager loading index: %s", file.c_str());
   /* Guard mapGuard(mAssetMapLock);

    Json::Value root;
    if( !JSON::LoadFile( file, root ) )
    {
      return false;
    }

    for( auto it = root.begin(); it != root.end(); ++it )
    {
      ASSETID assetid = (*it).asUInt();
      mFileIndex[ assetid ] = it.key().asCString();
    }*/

    return true;
  }

  void AssetManager::DrawUI(float s)
  {
    /*if (!ImGui::CollapsingHeader("Assets"))
    {
      return;
    }

    ImGui::LabelText("Root", mRoot.c_str());
    ImGui::LabelText("File Index", "%u", mFileIndex.size());
    ImGui::LabelText("Asset Map", "%u", mAssetMap.size());
    ImGui::LabelText("Asset Type Map Q", "%u", sAssetTypeMap.size());*/
  }

}