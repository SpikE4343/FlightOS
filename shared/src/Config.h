/*************************************************************************************
  g3::Config
  Copyright (c) 2013 John Rohrssen
  Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3_Config_h_included
#define g3_Config_h_included

#include "g3Json.h"

namespace g3
{
	class Config
	{
	public:
		
		bool load( const char* filename );
		bool loadString( const char* jsonStr );
		const Json::Value& root() const { return mRoot; }

	protected:
		Json::Value mRoot;
	};
}

#endif