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


namespace FlightOS
{

namespace String
{

std::string replace( std::string& str, const std::string find, const std::string& newValue )
{
  std::string out;
  size_t last = 0, index = str.find_first_of( find, last );
  while( index != std::string::npos )
  {
    out.append( str.substr( last, index-last ) );
    out.append( newValue );
    index += find.length();
    last = index;
  }
    
  out.append( str.substr( last ) );

  return out;
}

}

}