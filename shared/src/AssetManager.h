/*************************************************************************************

g3::Asset
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com

*************************************************************************************/

#ifndef g3_AssetManager_H_INCLUDED
#define g3_AssetManager_H_INCLUDED

#include "g3ObjectPool.h"
#include "g3Asset.h"
#include "g3AssetTypes.h"
#include "g3Threading.h"
#include "g3EngineModule.h"
#include "g3IAssetManager.h"

#include <map>
#include <unordered_map>

namespace g3
{
  class MemoryBuffer;
  class Asset;
  class AssetData;
  class AssetInstance;

  class AssetManager 
    : public IAssetManager
    , public Task
    , public EngineModule
  {
  private:
    enum State
    {
      eInitial=0,
      eLoadingIndex,
      eLoaded
    };

  public:
    struct InitInfo
    {
      std::string mContentRoot;
      int mNumWorkerThreads;
    };

    typedef AssetData* (*CreateAssetPtr)();
    typedef AssetInstance* (*CreateSpecificAssetPtr)();

    struct AssetCreators
    {
      CreateAssetPtr mCreateAssetData;
      CreateSpecificAssetPtr mCreateSpecificAsset;
    };

    typedef std::unordered_map< AssetTypes::Types, AssetCreators > AssetTypeMap;
    typedef std::unordered_map< ASSETID, Asset* > AssetMap;
    typedef std::map< ASSETID, std::string > FileIndex;

    static void registerAssetType( AssetTypes::Types type, CreateAssetPtr cb);
    static AssetTypeMap sAssetTypeMap;
    static AssetData* createAssetData( AssetTypes::Types type );

    AssetManager();
    const std::string& getRoot() const { return mRoot; }

    int initialize(const InitInfo& info);
    Task::UpdateResult update();
    int shutdown();

    Asset* getAsset( ASSETID id );
    void releaseAsset( ASSETID id );

    void DrawUI(float s);

  private:
    bool loadIndex(const std::string& file);

    std::string     mRoot;

    Lock            mAssetMapLock;
    AssetMap        mAssetMap;

    FileIndex       mFileIndex;

    State           mState;
  };

}

#endif