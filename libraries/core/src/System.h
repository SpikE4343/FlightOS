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

#ifndef System_h_INCLUDED
#define System_h_INCLUDED

#include <memory>

#include "Types.h"
#include "ModuleManager.h"

namespace FlightOS
{

class System :
  public ModuleManager<System>
{
public:
  
  virtual ~System();

  static System& instance() 
  {
    if (mInstance == NULL)
    {
      mInstance = new System();
    }
    return *mInstance; 
  }

  template <typename TModuleType>
  static TModuleType* module() 
  {
    return instance().getModule<TModuleType>();
  }

private:
  static System* mInstance;
};

} // g3

#endif

