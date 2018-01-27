/*************************************************************************************
g3::Log
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#include "g3Log.h"
#include "g3Threading.h"

#include <stdio.h>
#include <stdarg.h>

namespace g3
{

  static Log* spLog;

  Log::Log()
  {
    spLog = this;
  }

  Log* Log::Instance()
  {
    return spLog;
  }

  void Log::error(const char* format, ... )
  {
    va_list args;
    va_start (args, format);
    vprintf (format, args);
    va_end (args);	
    printf("\n");
  }

  void Log::debug(const char* format, ... )
  {
    va_list args;
    va_start (args, format);
    vprintf (format, args);
    va_end (args);	
    printf("\n");
  }

  void Log::info( const char* format, ... )
  {
    //static Lock lock;
    //lock.grab();
    va_list args;
    va_start (args, format);
    vprintf (format, args);
    va_end (args);
    printf("\n");
    //lock.release();
  }

  int Log::initialize()
  {
    return 1;
  }

  int Log::update()
  {
    return 1;
  }

  int Log::shutdown()
  {
    return 1;
  }


}

