#include "g3Types.h"
#include "g3Threading.h"

namespace g3 
{
  long TypeReferenceId::nextId = 0;

  TypeId TypeReferenceId::getNext()
  {
    return (TypeId)Atomic::increment(&nextId);
  }
}