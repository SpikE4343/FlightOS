/*************************************************************************************
g3::Application
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3_Application_H_INCLUDED
#define g3_Application_H_INCLUDED

#include "g3Singletons.h"
#include "g3Config.h"
#include "g3ModuleManager.h"

namespace g3
{
  class Application : 
    public Singleton
  {
    g3SingletonDecl;
  public:
    Application();

    virtual int initialize(int argc, char* argv[]);
    int run(int argc, char* argv[]);
    virtual int update();
    virtual int shutdown();

    const Config& getConfig() const { return mConfig; }

  protected:
    Config mConfig;
  };

}


#endif