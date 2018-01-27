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

#ifndef FlightOS_AssetTypes_H_INCLUDED
#define FlightOS_AssetTypes_H_INCLUDED

namespace FlightOS {
namespace AssetTypes {

enum Types
{
	eDataAsset = 1024,
  
  // graphics
  eGraphicsMeshAsset,
	eGraphicsShaderAsset,

  eGraphicsObjectAsset,
  
  // physics
  ePhysicsObject,

  // game
  eWorldAsset,
  eEntityAsset,
  eEntityGraphicsModule,
};

} // namespace AssetTypes
} // namespace FlightOS

#endif