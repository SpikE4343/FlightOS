/*************************************************************************************
g3::Json
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3_Json_H_INCLUDED
#define g3_Json_H_INCLUDED

#include "g3Types.h"
#include "g3Math.h"

#include "json/json.h"

namespace g3
{

  namespace JSON
  {
    bool LoadFile( const std::string& filename, Json::Value& root );
    bool SaveToFile( const std::string& filename, const Json::Value& root );
    bool ElementVector3( Json::Value& root, Vector3& vec );
  }

}

#endif