
#include "g3Util.h"


namespace g3
{

namespace String
{

std::string replace( std::string& str, const std::string find, const std::string& newValue )
{
  std::string out;
  size_t last = 0, index = str.find_first_of( find, last );
  while( index != std::string::npos )
  {
    out.append( str.substr( last, index-last ) );
    out.append( newValue );
    index += find.length();
    last = index;
  }
    
  out.append( str.substr( last ) );

  return out;
}

}

}