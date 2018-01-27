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

#ifndef FlightOS_AssetManager_H_INCLUDED
#define FlightOS_AssetManager_H_INCLUDED

#include "ObjectPool.h"
#include "Asset.h"
#include "AssetTypes.h"
#include "Threading.h"
#include "EngineModule.h"
#include "IAssetManager.h"

#include <map>
#include <unordered_map>

namespace FlightOS
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