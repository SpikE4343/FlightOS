/*************************************************************************************
  g3::Log
  Copyright (c) 2013 John Rohrssen
  Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3_Log_H_INCLUDED
#define g3_Log_H_INCLUDED

#include "g3Log.h"

namespace g3
{

class Log
{
public:
	Log();

	static Log* Instance();
	static void debug(const char* format, ... );
	static void info( const char* format, ... );
	static void error( const char* format, ... );
	int initialize();
	int update();
	int shutdown();
};

}


#endif