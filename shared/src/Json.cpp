/*************************************************************************************
g3::Json
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#include "g3Log.h"
#include "g3Json.h"

#include <fstream>

namespace g3
{

  namespace JSON
  {
    bool LoadFile( const std::string& filename, Json::Value& root )
    {
      Json::Reader reader;
      std::ifstream file(filename.c_str(), std::ifstream::binary);
      if( !reader.parse( file, root ) )
      {
        Log::error( "JSON::LoadFile: Error: unable to load file: %s, %s", filename.c_str(), reader.getFormattedErrorMessages().c_str() );
        file.close();
        return false;
      }

      file.close();
      return true;
    }

    bool SaveToFile( const std::string& filename, const Json::Value& root )
    {
      Json::StyledStreamWriter writer("  ");

      std::ofstream file( filename.c_str() );

      writer.write( file, root );

      file.close();

      return true;
    }

    bool ElementVector3( Json::Value& root, Vector3& vec )
    {
      vec.setX((float)root[0U].asDouble());
      vec.setY((float)root[1U].asDouble());
      vec.setZ((float)root[2U].asDouble());
      return true;
    }
  }

}
