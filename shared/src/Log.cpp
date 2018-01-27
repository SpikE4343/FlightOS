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
#include "Threading.h"

#include <stdio.h>
#include <stdarg.h>

namespace FlightOS
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

