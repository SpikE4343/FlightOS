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
#include "Platform.h"
#include "DeviceManager.h"
#include "Log.h"
#include "TaskManager.h"
#include "ObjectPoolManager.h"

namespace FlightOS
{
  static std::string skDefaultConfig = "{ \"workerthreads\":4 }";

  DeviceManager::DeviceManager()
  {

  }

  int DeviceManager::initialize(int argc, char* argv[])
  {
    System::instance().addModule<DeviceManager>(this);

    system()->createModule<ObjectPoolManager>();
    system()->createModule<Platform>();
    system()->createModule<TaskManager>();

    if(system()->getModule<Platform>()->initialize() != 1 )
    {
      Log::info( "Failed to initialize platform");
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

    System::module<TaskManager>()->initialize(2);// mConfig.root()["workerthreads"].asInt() );

    return 1;
  }

  int DeviceManager::run(int argc, char* argv[])
  {
    int ret = initialize(argc, argv);
    if( ret != 1 )
    {
      return ret;
    }

    for(;;)
    {
      if(system()->getModule<Platform>()->update() != 1 )
        break;

      system()->getModule<TaskManager>()->update();

      if( update() != 1 )
        break;
    }

    shutdown();

    return 1;
  }

  int DeviceManager::update()
  {
    return 1;
  }

  int DeviceManager::shutdown()
  {
    system()->removeAllModules();

    return 1;
  }
}