#include "Double3.h"
#include "Point3d.h"

namespace PP3d {
  

  const Double3 & Double3::operator = (const Point3d& pVal)
  {
    cVect[ 0 ] = const_cast<Point3d&>(pVal).x();
    cVect[ 1 ] =  const_cast<Point3d&>(pVal).y();
    cVect[ 2 ] =  const_cast<Point3d&>(pVal).z();
    return  *this;
  }
  
}
