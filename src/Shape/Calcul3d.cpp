#include "Calcul3d.h"


#include <cmath>

#include "ObjProps.h"




namespace PP3d{
  
  //------------------------------------------------
	
  void Calcul3d::Normal(  const PP3d::Point3d & p1,
			  const  PP3d::Point3d & p2,
			  const  PP3d::Point3d & p3, 
			  PP3d::Point3d & pNorm ) 
  {				
    double coa, cob, coc ;
    double px1, py1, pz1 ;
    double px2, py2, pz2 ;
    double px3, py3, pz3 ;
				
    double absvec ;
				
    px1 = p1.cX ;
    py1 = p1.cY ;
    pz1 = p1.cZ ;
				
    px2 = p2.cX ;
    py2 = p2.cY ;
    pz2 = p2.cZ ;
				
    px3 = p3.cX ;
    py3 = p3.cY ;
    pz3 = p3.cZ ;
				
    coa = (py1 * (pz2-pz3) + py2*(pz3-pz1) + py3*(pz1-pz2)) ;
    cob = (pz1 * (px2-px3) + pz2*(px3-px1) + pz3*(px1-px2)) ;
    coc = (px1 * (py2-py3) + px2*(py3-py1) + px3*(py1-py2)) ;
				
    // Normalisation
    absvec = sqrt ((double)((coa *coa) + (cob *cob) + (coc *coc)));
				
    if( absvec == 0.0  )
      pNorm.cX  = absvec = 1.0;
				
    absvec = 1.0 / absvec;
				
    pNorm.cX = coa*absvec ;
    pNorm.cY = cob*absvec ;
    pNorm.cZ = coc*absvec ;
  }
  //------------------------------------------------
  void Calcul3d::NoNorm(  const PP3d::Point3d & p1,
			  const  PP3d::Point3d & p2,
			  const  PP3d::Point3d & p3, 
			  PP3d::Point3d & pNorm ) 
  {				
    double coa, cob, coc ;
    double px1, py1, pz1 ;
    double px2, py2, pz2 ;
    double px3, py3, pz3 ;
				
    double absvec ;
				
    px1 = p1.cX ;
    py1 = p1.cY ;
    pz1 = p1.cZ ;
				
    px2 = p2.cX ;
    py2 = p2.cY ;
    pz2 = p2.cZ ;
				
    px3 = p3.cX ;
    py3 = p3.cY ;
    pz3 = p3.cZ ;
				
    pNorm.cX = (py1 * (pz2-pz3) + py2*(pz3-pz1) + py3*(pz1-pz2)) ;
    pNorm.cY = (pz1 * (px2-px3) + pz2*(px3-px1) + pz3*(px1-px2)) ;
    pNorm.cZ = (px1 * (py2-py3) + px2*(py3-py1) + px3*(py1-py2)) ;    
  }
  //------------------------------------------------
  void Calcul3d::GetMiddle( const std::vector<PP3d::Point3d>& pPoints,
			    PP3d::Point3d & pMiddle )
  {
    pMiddle.zero();

    for(  const PP3d::Point3d& pPoint : pPoints )
      {
	pMiddle += pPoint;
      }

    pMiddle /= (double)pPoints.size();
  }
  //------------------------------------------------
  void Calcul3d::GetMiddle( const std::vector<size_t> & pFacet,
			    const std::vector<PP3d::Point3d>& pPoints,
			    PP3d::Point3d & pMiddle )
  {
    pMiddle.zero();

    for( size_t p=0; p< pFacet.size(); p++ )
      {
	pMiddle += pPoints[ pFacet[p] ];
      }

    pMiddle /= (double)pFacet.size();
  }
  //------------------------------------------------
  void Calcul3d::GetMinMaxAndCenter( PP3d::ObjProps&pProps,
				     const std::vector<PP3d::Point3d>& pPoints )
  {
    pProps.cCenter.zero();
    pProps.cMinMaxBox.reset();
    for( const PP3d::Point3d& pPoint : pPoints )
      {
	pProps.cCenter += pPoint;
	pProps.cMinMaxBox.add( pPoint );
      }

    pProps.cCenter /= (double)pPoints.size();
  }
  //------------------------------------------------

}
