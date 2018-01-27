/*************************************************************************************
g3::Application
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#include "g3Util.h"
#include "g3System.h"
#include "g3Application.h"
#include "g3Log.h"
#include "g3TaskManager.h"
#include "g3Singletons.h"

namespace g3
{
  g3SingletonDef(Application);
  static std::string skDefaultConfig = "{ \"workerthreads\":4 }";

  Application::Application()
  {
  }

  int Application::initialize(int argc, char* argv[])
  {
    Singletons::create<System>();
    Singletons::create<TaskManager>();

    if( Singletons::get<System>()->initialize() != 1 )
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

    Singletons::get<TaskManager>()->initialize( mConfig.root()["workerthreads"].asInt() );

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
      if( Singletons::get<System>()->update() != 1 )
        break;

      Singletons::get<TaskManager>()->update();

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
    Singletons::get<System>()->shutdown();

    Singletons::reset();

    return 1;
  }

}