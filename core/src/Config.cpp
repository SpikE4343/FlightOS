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

#include "Config.h"
#include "Log.h"

#include <fstream>

namespace FlightOS
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
