#ifndef g3Profiler_h_included 
#define g3Profiler_h_included

#include "g3singletons.h"

namespace g3
{
  class Profiler : public Singleton
  {
    g3SingletonDecl;
  public:
    Profiler(void);
    ~Profiler(void);
  };
}

#endif

