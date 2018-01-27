/*************************************************************************************
g3::Asset
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#include "g3Util.h"
#include "g3System.h"
#include "g3AssetManager.h"
#include "g3Log.h"
#include "g3ObjectPoolManager.h"
#include "g3TaskManager.h"
#include "g3MemoryBuffer.h"
#include "g3Json.h"

namespace g3
{
  AssetManager::AssetTypeMap AssetManager::sAssetTypeMap;

  AssetManager::AssetManager() 
    : mState(eInitial)
  {
    
  }

  int AssetManager::initialize(const InitInfo& info)
  {
    mRoot = info.mContentRoot;

    Engine::module<TaskManager>()->createQueue( 
      TaskManager::kAssetTaskQueueId, 
      info.mNumWorkerThreads );

    Engine::module<TaskManager>()->add(this);

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
      asset = Engine::module<ObjectPoolManager>()->grab<Asset>();
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
      Engine::module<ObjectPoolManager>()->release(it->second);
      mAssetMap.erase( it );
    }
  }

  bool AssetManager::loadIndex( const std::string& file )
  {
    Log::info("AssetManager loading index: %s", file.c_str());
    Guard mapGuard(mAssetMapLock);

    Json::Value root;
    if( !JSON::LoadFile( file, root ) )
    {
      return false;
    }

    for( auto it = root.begin(); it != root.end(); ++it )
    {
      ASSETID assetid = (*it).asUInt();
      mFileIndex[ assetid ] = it.key().asCString();
    }

    return true;
  }

  void AssetManager::DrawUI(float s)
  {
    if (!ImGui::CollapsingHeader("Assets"))
    {
      return;
    }

    ImGui::LabelText("Root", mRoot.c_str());
    ImGui::LabelText("File Index", "%u", mFileIndex.size());
    ImGui::LabelText("Asset Map", "%u", mAssetMap.size());
    ImGui::LabelText("Asset Type Map Q", "%u", sAssetTypeMap.size());
  }

}