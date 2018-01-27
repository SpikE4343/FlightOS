/*************************************************************************************
g3::EngineModule
 
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3EngineModule_h_INCLUDED
#define g3EngineModule_h_INCLUDED

#include <memory>

#include "g3Engine.h"
#include "g3ModuleManager.h"
#include "g3ImUiWindow.h"
#include "g3Task.h"

namespace g3
{

 class IImuUiWindow;

class EngineModule 
  : public Module<Engine>
  , public IImUiWindow
{
protected:
  /// short hand for modules
  Engine* engine() { return owner(); }

public:

  virtual void DrawUI(float dt){}
  

  virtual int moduleUnbind()
  {
    Module<Engine>::moduleUnbind();
    return 0;
  }


  EngineModule();
  virtual ~EngineModule();
};

} // g3

#endif

