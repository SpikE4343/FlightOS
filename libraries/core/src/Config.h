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

#ifndef FlightOS_Config_h_included
#define FlightOS_Config_h_included

//#include "Json.h"

namespace FlightOS
{
	class Config
	{
	public:
		
		bool load( const char* filename );
		bool loadString( const char* jsonStr );
		//const Json::Value& root() const { return mRoot; }

	protected:
		//Json::Value mRoot;
	};
}

#endif