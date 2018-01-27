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
#include "System.h"
#include "Application.h"
#include "Log.h"
#include "TaskManager.h"
#include "ObjectPoolManager.h"

namespace FlightOS
{
  static std::string skDefaultConfig = "{ \"workerthreads\":4 }";

  Application::Application()
  {

  }

  int Application::initialize(int argc, char* argv[])
  {
    Engine::instance().addModule<Application>(this);

    engine()->createModule<ObjectPoolManager>();
    engine()->createModule<System>();
    engine()->createModule<TaskManager>();

    if(engine()->getModule<System>()->initialize() != 1 )
    {
      Log::info( "Failed to initialize system");
      return 0;
    }

    mConfig.loadString( skDefaultConfig.c_str() );

    int arg = 0;
    while( arg < argc )
    {
      char* pArg = argv[arg];
      char* pValue = (arg < argc-1) ? argv[arg+1] : NULL;

      if( strncmp( pArg, "-config", 7 ) == 0 )
      {
        if( !mConfig.load( pValue ) )
        {
          Log::error( "Unable to load config file: %s", pValue );
          return 0;
        }
      }
      ++arg;
    }

    Engine::module<TaskManager>()->initialize( mConfig.root()["workerthreads"].asInt() );

    return 1;
  }

  int Application::run(int argc, char* argv[])
  {
    int ret = initialize(argc, argv);
    if( ret != 1 )
    {
      return ret;
    }

    for(;;)
    {
      if(engine()->getModule<System>()->update() != 1 )
        break;

      engine()->getModule<TaskManager>()->update();

      if( update() != 1 )
        break;
    }

    shutdown();

    return 1;
  }

  int Application::update()
  {
    return 1;
  }

  int Application::shutdown()
  {
    engine()->removeAllModules();

    return 1;
  }
}