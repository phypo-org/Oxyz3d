#ifndef H__PP3D_UTILS__H
#define H__PP3D_UTILS__H

#include "Entity.h"

namespace PP3d {
  

  //-------------------------------------------
  inline bool  GetVectorFromFacets( PP3d::SortEntityVisitor & iVisit, PP3d::Point3d & oNorm, PP3d::Point3d & oCenter )
  {
    oCenter.raz();
    
    size_t lSz = iVisit.cVectFacets.size();
    if( lSz >= 1 )
      {
	oNorm = iVisit.cVectFacets[lSz-1]->getNormal();
	for( size_t i=1; i< lSz; i++ )
	  {
	    lNorm += iVisit.cVectFacets[i]->getNormal();
	  }
	if( lSz > 0)
	  lNorm /= lSz;
	
	
	for(  const PP3d::PointPtr pPoint : iVisit.cVectPoints )
	  {
	    lCenter += pPoint->get();
	  }
	lCenter /= (double)iVisit.cVectPoints.size();
      }
    else
      {
	oNorm.raz();
	return false;
      }
    return true;
  }
  //-------------------------------------------
 
}

#endif
