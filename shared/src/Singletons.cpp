#include "g3Singletons.h"

namespace g3
{
  uint32 Singletons::mNextId = 0;
  Lock Singletons::mInsertLock;
  Singletons::SingletonMap Singletons::singletons;
}
