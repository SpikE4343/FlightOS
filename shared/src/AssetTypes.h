/*************************************************************************************
  g3::AssetTypes
  Copyright (c) 2013 John Rohrssen
  Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3_AssetTypes_H_INCLUDED
#define g3_AssetTypes_H_INCLUDED

namespace g3 {
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
} // namespace g3

#endif