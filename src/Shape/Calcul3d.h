#ifndef H__Calcul3d__H
#define H__Calcul3d__H

#include "PP3dType.h"
#include "ObjProps.h"
#include "Entity.h"


#include <vector>
#include <set>


namespace PP3d{


  //*******************************************
  struct Calcul3d{
    static void Normal(  const PP3d::Point3d & p1,
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
  };
  //*******************************************

}

#endif
