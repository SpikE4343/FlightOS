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

#ifndef FlightOS_Application_H_INCLUDED
#define FlightOS_Application_H_INCLUDED

#include "Config.h"
#include "ModuleManager.h"
#include "SystemModule.h"

namespace FlightOS
{
  class Application : public SystemModule
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