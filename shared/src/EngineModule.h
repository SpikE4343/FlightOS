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

#ifndef g3EngineModule_h_INCLUDED
#define g3EngineModule_h_INCLUDED

#include <memory>

#include "Engine.h"
#include "ModuleManager.h"
#include "ImUiWindow.h"
#include "Task.h"

namespace FlightOS
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

