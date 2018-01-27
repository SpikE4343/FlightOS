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

#include "Log.h"
#include "Json.h"

#include <fstream>

namespace FlightOS
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
