#ifndef H__Line3d__H
#define H__Line3d__H

#include "Point3d.h"


#include <ostream>

#include <cmath>

namespace PP3d {
  
  
  
  //******************************************************** 
  class  Line3dVect{

  public:
    Point3d cA;  // point
    Point3d cV;  // vecteur 


    Line3dVect( Point3d iA, Point3d iB ) :cA(iA), cV(iB-iA) {}

    bool intersectPlanX( Point3d & iRes, double iPos=0 ) {
      
      if( cV.cX  == 0 ) return false;
      
      long double t = (iPos-cA.cX)/cV.cX;
      
      iRes.set( iPos,  cA.cY+cV.cY*t, cA.cZ+cV.cZ*t );
      return true;
    }
   
    bool intersectPlanY( Point3d & iRes, double iPos=0 ) {
      
      if( cV.cY  == 0 ) return false;
      
      long double t = (iPos-cA.cY)/cV.cY;
      
      iRes.set( cA.cX+cV.cX*t,  iPos, cA.cZ+cV.cZ*t );
      return true;
    }
 
    bool intersectPlanZ( Point3d & iRes, double iPos=0 ) {
      
      if( cV.cZ  == 0 ) return false;
      
      long double t = (iPos-cA.cZ)/cV.cZ;
      
      iRes.set( cA.cX+cV.cX*t, cA.cY+cV.cY*t, iPos );
      return true;
    }
      
  };
  
} //namespace

#endif
