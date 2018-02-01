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

#ifndef FlightOS_Platform_H_INCLUDED
#define FlightOS_Platform_H_INCLUDED

#include "Types.h"
#include "SystemModule.h"
#include <string>

namespace FlightOS
{
  class IPlatform
  {
  public:
    virtual uint64 getTimeMs() = 0;
  };

  class Platform : /*public IPlatform,*/ public SystemModule
  {
  public:
    Platform();

    int initialize();
    int update();
    int shutdown();

    int createWindow( int width, int height, const char* title );

    void* getDeviceContext();
    void* getWindow();

    uint64 getTimeMs();

    void CreateFilePath( const std::string& path );

    void DrawUI(float dt);

  private:

    float mFrameTimeSecs;
    uint64 mStartSysTimeMs;
    uint64 mLastSysTimeMs;
    float mClockFreq;
  };

}
#endif