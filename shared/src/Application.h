/*************************************************************************************
g3::Application
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3_Application_H_INCLUDED
#define g3_Application_H_INCLUDED

#include "g3Config.h"
#include "g3ModuleManager.h"
#include "g3EngineModule.h"

namespace g3
{
  class Application : public EngineModule
  {
  public:
    Application();

    /// Setup application state
    virtual int initialize(int argc, char* argv[]);

    /// Begin application logic
    int run(int argc, char* argv[]);

    /// Per-frame method to progress application logic
    virtual int update();

    /// Stop all application logic and clean up resources
    virtual int shutdown();

    /// 
    const Config& getConfig() const { return mConfig; }

  protected:
    Config mConfig;
  };

}


#endif