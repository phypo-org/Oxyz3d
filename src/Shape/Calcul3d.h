#ifndef H__Calcul3d__H
#define H__Calcul3d__H

#include "PP3dType.h"
#include "ObjProps.h"
#include "Entity.h"


#include <vector>
#include <set>


#define _USE_MATH_DEFINES // for C++
#define ___USE_GNU
#include <cmath>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


namespace PP3d{


  //*******************************************
  struct Calcul3d{
    static void Normal(  const  PP3d::Point3d & p1,
			 const  PP3d::Point3d & p2,
			 const  PP3d::Point3d & p3, 
			 PP3d::Point3d & pNorm ) ;
    static void NoNorm(  const  PP3d::Point3d & p1,
			 const  PP3d::Point3d & p2,
			 const  PP3d::Point3d & p3, 
			 PP3d::Point3d & pNorm ) ;
    
    static void GetMiddle( const std::vector<PP3d::Point3d>& pPoints,
			   PP3d::Point3d & pMiddle );
    
    static void GetMiddle( const std::vector<size_t> & pFacet,
			   const std::vector<PP3d::Point3d> & pPoints,
			   PP3d::Point3d & pMiddle );
    
    static void GetMinMaxAndCenter( PP3d::ObjProps&pProps,
				    const  std::vector<PP3d::Point3d> & pPoints );
    
    static constexpr long double Rad2Deg =180/M_PI;
    static constexpr long double Deg2Rad =M_PI/180;
    static constexpr long double M_PIx2 = M_PI*2;

    static double Rad2Degre( double iRad ) { return iRad * Rad2Deg;}
    static double Degre2Rad( double iDeg ) { return iDeg * Deg2Rad;}
  };
  //*******************************************

}

#endif
