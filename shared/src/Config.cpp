#include "g3Config.h"
#include "g3Log.h"

#include <fstream>

namespace g3
{

  bool Config::load( const char* filename )
  {
    Json::Reader reader;

    std::ifstream file(filename, std::ifstream::binary);
    if( !reader.parse( file, mRoot ) )
    {
      Log::error( "Error: unable to load: %s: %s", filename, reader.getFormattedErrorMessages().c_str() );
      return false;
    }

    if( !mRoot.isObject() )
    {
      Log::error( "Error: unable to load: %s: root is not an object value", filename );
      return false;
    }

    return true;
  }

  bool Config::loadString( const char* jsonStr )
  {
    Json::Reader reader;
    if( !reader.parse( jsonStr, mRoot ) )
    {
      Log::error( "Error: unable to load: %s: %s", jsonStr, reader.getFormattedErrorMessages().c_str() );
      return false;
    }

    if( !mRoot.isObject() )
    {
      Log::error( "Error: unable to load: %s: root is not an object value", jsonStr );
      return false;
    }

    return true;
  }

}
