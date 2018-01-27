/*************************************************************************************
g3::Asset
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3_Asset_H_INCLUDED
#define g3_Asset_H_INCLUDED

#include "g3Util.h"
#include "g3Math.h"
#include "g3Task.h"
#include "g3TaskManager.h"
#include "g3AssetManager.h"
#include "g3AssetTypes.h"
#include "g3MemoryBuffer.h"

namespace g3
{
  class MemoryBuffer;
  class AssetManager;
  class AssetInstance;

  class AssetData
  {
  public:
    g3_packed( struct Header
    {
      Header();

      enum Flags
      {
        eCompressed = 1 << 0
      };

      char   mAsset[4];
      uint32 mType;
      uint32 mVersion;
      uint64 mId;
      uint32 mFlags;
      uint32 mDeflateSize;
      uint32 mSize;
      uint32 mNumAssetRefs;
    });

    g3_packed( struct AssetRef
    {
      uint32 mAssetId;
    });

    virtual ~AssetData();

    virtual uint32 getType() const =0;

    virtual bool read( MemoryBuffer& buf )=0;
    virtual bool write( MemoryBuffer& buf ) =0;

    MemoryBuffer* mData;
  };


  class Asset : public Task, public RefObject
  {
  public:

    Asset();

    enum Status
    {
      eNone,

      // loading
      eStartLoading,
      eLoading,
      eDataLoading,
      eDataLoadingComplete,
      eReady,

      // reloading
      eStartReloading,
      eReloading,

      // unloading
      eStartUnloading,
      eUnloading,
      eUnloaded,

      // saving
      eStartSaving,
      eSaving,

      // error states
      eError
    };

    typedef Event< void, Asset* > AssetEvent;
    typedef std::unordered_map< uint32, Asset* > AssetMap;
    typedef std::unordered_map< uint32, AssetInstance* > AssetInstanceMap;

    AssetEvent DataLoaded;
    AssetEvent DataReloaded;
    AssetEvent DataSaved;

    AssetEvent Ready;

    void setName( const char* name ) { mName = name; }
    void setData( AssetData* data ) { mData = data; }
    const AssetData* getData() const { return mData; }
    AssetData* getData() { return mData; }

    template<class TAssetData>
    TAssetData* getData() { return (TAssetData*)mData; }

    Status getStatus() const { return mStatus; }
    bool isLoading() const { return mStatus == eLoading; }
    bool isReady() const { return mStatus == eReady; }
    bool areChildrenReady() const { return mChildrenReady; }

    uint32 assetid() const { return mAssetId; }
    bool canLoad() const;

    void load();
    bool save(const std::string& path, bool compress=false);

    void setAssetId( uint32 assetid ) { mAssetId = assetid; }

    uint32 addInstance( AssetInstance* inst );
    void removeInstance( uint32 instid );

    void reset();

    virtual uint32 getQueue() const { return TaskManager::kAssetTaskQueueId; }

  private:
    Task::UpdateResult update();
    bool loadFromFile();
    bool loadFromClump();

    bool saveToFile();

    uint32					mNextInstanceId;
    Status          mStatus;
    std::string     mName;
    ASSETID         mAssetId;
    AssetData*      mData;
    AssetInstanceMap  mInstances;

    AssetMap        mDependencies;
    bool						mChildrenReady;

  };

  class AssetInstance
  {
  public:
    AssetInstance() : mAsset(NULL), mReady(false) {}
    virtual void DataLoaded( Asset* asset )=0;
    virtual void DataReloaded( Asset* asset )=0;
    virtual void DataSaved( Asset* asset )=0;

    bool isReady() const { return mReady; }

    AssetData* getData() { return mAsset->getData(); }
    void setAsset( Asset* asset ) { mAsset = asset; }
    Asset* getAsset() { return mAsset; }
    ASSETID getAssetId() const { return mAsset->assetid(); }

    void setInstanceId( uint32 id ){ mAssetInstanceId = id; }
    uint32 getInstanceId() const { return mAssetInstanceId; }

    uint32                             mId;

  protected:

    uint32 mAssetInstanceId;
    Asset* mAsset;
    bool	 mReady;
  };


#define dec_asset( type ) uint32 getType() const { return (uint32)AssetTypes::##type; } \
public: \
  bool read( MemoryBuffer& buf ); \
  bool write( MemoryBuffer& buf ); \
  //bool readJSON(); \
  //bool writeJSON() const;

#define define_asset( t, c ) class c : public AssetData { \
  dec_asset( t ); \

#define end_define_asset() }

#define asset_attr( t, a ) t a;

#define asset_attr_read( a ) buf.read(a)
#define asset_attr_write( a ) buf.write(a)

#define asset_read( c ) bool c##::read( MemoryBuffer& buf )
#define asset_write( c ) bool c##::write( MemoryBuffer& buf )

#define asset_attr_Vector3_write( v3 ) { buf.write( v3.x() ); buf.write( v3.y() ); buf.write( v3.z() ); }
#define asset_attr_Vector3_read( v3 ) { float temp=0.0f; buf.read( temp ); v3.setX(temp); buf.read( temp ); v3.setY(temp); buf.read( temp ); v3.setZ(temp); }

#define reg_asset_creator(t, adc)                                 \
  static AssetData* s_assetdata_creator_##t() { return new adc (); }        \
  struct s_asset_creator_class_##t {                              \
  s_asset_creator_class_##t(){ AssetManager::registerAssetType( AssetTypes::##t, &s_assetdata_creator_##t);  } \
  }; static s_asset_creator_class_##t t##_creator_obj;                   \

#define asset_attr_std_vector_write( v ) { uint32 size = (uint32)v.size(); \
  buf.write( size ); \
  if( size > 0 ) buf.write( (uint8*)&v[0], ((uint32)v.size()) * sizeof(v[0]) ) ; }

#define asset_attr_std_vector_read( v ) { uint32 size = 0; \
  buf.read(size); \
  v.resize( size ); \
  if( size > 0 ) buf.read( (uint8*)&v[0], size * sizeof(v[0]) ); }

}


#endif
