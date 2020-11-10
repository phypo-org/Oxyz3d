#include "MinMaxBox3d.h"

#include <stdio.h>

#include "PrimitivFactory.h"


namespace PP3d {

  /*---------------------------------------------------------------------*/
  /*    Rect3d::getCenter ...                                     */
  /*---------------------------------------------------------------------*/

  Point3d
  Rect3d::getCenter( ) const
  {
    return Point3d(	 (cMin.cX + cMax.cX) * 0.5,
			 (cMin.cY + cMax.cY) * 0.5,
			 (cMin.cZ + cMax.cZ) * 0.5 );
  }
  Double3
  Rect3d::getCenterDbl() const
  {
    return Double3(	 (cMin.cX + cMax.cX) * 0.5,
			 (cMin.cY + cMax.cY) * 0.5,
			 (cMin.cZ + cMax.cZ) * 0.5 );
  }
  //---------------------------
  Point3d 
  Rect3d::center() const
  {
    return Point3d( (cMin.cX + cMax.cX)* 0.5,
		    (cMin.cY + cMax.cY)* 0.5,
		    (cMin.cZ + cMax.cZ)* 0.5);
  }
  //---------------------------
  Double3 
  Rect3d::centerDbl() const
  {
    return Double3( (cMin.cX + cMax.cX)* 0.5,
		    (cMin.cY + cMax.cY)* 0.5,
		    (cMin.cZ + cMax.cZ)* 0.5);
  }
  //---------------------------
  Point3d 
  Rect3d::size() const
  {
    return Point3d( (cMax.cX -cMin.cX),
		    (cMax.cY -cMin.cY),
		    (cMax.cZ -cMin.cZ)
		    );
  }
  //---------------------------
	 
  PDouble Rect3d::distanceSquare() const
  {
    Point3d lPt( (cMax.cX -cMin.cX),
		 (cMax.cY -cMin.cY),
		 (cMax.cZ -cMin.cZ)
		 );
		
    return lPt.cX*lPt.cX*0.25 + lPt.cY*lPt.cY*0.25 +  lPt.cZ*lPt.cZ*0.25;
  }
  //---------------------------
	 
  PDouble Rect3d::distance() const
  {
    return std::sqrt( distanceSquare() );
  }
  //---------------------------
  bool Rect3d::isInRadius( Point3d pPt )
  {
    Point3d lDist = pPt - center();
    if( lDist.distanceSquare() <  size().distanceSquare() )
      return true;
		
    return false;
  }
  //---------------------------
  void Rect3d::drawGL()
  {
    if( cMin == cMax )
      return;
		
    Point3d lPoints[] = {
      Point3d( cMax.cX,  cMin.cY,  cMin.cZ), 
      Point3d( cMax.cX,  cMax.cY,  cMin.cZ),    
      Point3d( cMin.cX,  cMax.cY,  cMin.cZ),
      Point3d( cMin.cX,  cMin.cY,  cMin.cZ),

      Point3d( cMax.cX,  cMin.cY, cMax.cZ),
      Point3d( cMax.cX,  cMax.cY, cMax.cZ),
      Point3d( cMin.cX,  cMax.cY, cMax.cZ),
      Point3d( cMin.cX,  cMin.cY, cMax.cZ )
    };
		
    const int lA =0;
    const int lB =1;
    const int lC =2;
    const int lD =3;
    const int lE =4;
    const int lF =5;
    const int lG =6;
    const int lH =7;

			
    PrimFacet lFacets[]={
      PrimFacet( lA, lD, lC, lB ),
      PrimFacet( lE, lF, lG, lH ),
      PrimFacet( lA, lB, lF, lE ),					
      PrimFacet( lD, lH, lG, lC ),					
      PrimFacet( lB, lC, lG, lF ),
      PrimFacet( lA, lE, lH, lD )
    };
    for( const PrimFacet& lFacet : lFacets )
      {
	////				glNormal3dv( lFacet.cNorm.vectForGL() );
	glBegin(GL_QUADS);
		
	for( size_t p=0; p< lFacet.size(); p++ )
	  {
	    glVertex3dv( lPoints[ lFacet[p] ].vectForGL() );
	  }
	glEnd();
      }
		
    glDisable( GL_LIGHTING );
	
    glPointSize( 5 );
    glColor4f( 1, 0.8f, 0, 0.5f );
    glBegin( GL_POINTS );
    glVertex3dv( cMin.vectForGL() ); 
    glVertex3dv( cMax.vectForGL() ); 
    glEnd();
    glEnable(GL_LIGHTING);

		
  }
	
  /*---------------------------------------------------------------------*/
  /*    T3dObj::getInfoStr ...                                          */
  /*---------------------------------------------------------------------*/

  char*
  Rect3d::getInfoStr( char* pStr)
  {
    int lNb = sprintf( pStr, "(%f, %f, %f : %f, %f, %f )",
		       cMin.cX, cMin.cY, cMin.cZ, cMax.cX, cMax.cY, cMax.cZ );

    return &pStr[lNb];
  }

  //***********************************************
}
