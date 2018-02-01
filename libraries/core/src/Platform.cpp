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

#include "Platform.h"

namespace FlightOS
{
  Platform::Platform()
  {

  }

  int Platform::initialize()
  {
    return 0;
  }

  int Platform::update()
  {
    return 0;
  }

  int Platform::shutdown()
  {
    return 0;
  }

  int Platform::createWindow(int width, int height, const char* title)
  {
    return 0;
  }

  void* Platform::getDeviceContext()
  {
    return 0;
  }

  void* Platform::getWindow()
  {
    return nullptr;
  }

  uint64 Platform::getTimeMs()
  {
    return 0L;
  }

  void Platform::CreateFilePath(const std::string& path)
  {

  }

  void Platform::DrawUI(float dt)
  {

  }
}