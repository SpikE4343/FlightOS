/*************************************************************************************
g3::System
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3_System_H_INCLUDED
#define g3_System_H_INCLUDED

#include "g3Types.h"
#include "g3Singletons.h"
#include "g3ImUi.h"
#include <string>

namespace g3
{
  class System : public Singleton, public IImUiWindow
  {
    g3SingletonDecl;
  public:
    System();

    int initialize();
    int update();
    int shutdown();

    int createWindow( int width, int height, const char* title );

    void* getDeviceContext();
    void* getWindow();

    uint64 getSystemTimeMs();

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