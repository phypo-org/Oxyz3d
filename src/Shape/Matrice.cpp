#include "Matrice.h"

#include <cmath>

namespace PP3d{


  //**************************************
  void Mat4::MulMat3( Mat4& matres, Mat4& mat1, Mat4& mat2 )
  {
    for ( int k = 0; k < 3; k++ )
      {
	for ( int j = 0; j<3; j++ )
	  {
	    for (int  i = 0; i<3; i++ )
	      {
		matres.cMat[k][j] = (mat1.cMat[k][i] * mat2.cMat[i][j]);
	      }
	  }
      }
  }
  //**************************************
  void Mat4::MulMat4( Mat4& matres, Mat4& mat1, Mat4& mat2 )
  {
    for ( int k = 0; k < 4; k++ )
      {
	for ( int j = 0; j<4; j++ )
	  {
	    for ( int i = 0; i<4; i++ )
	      {
		matres.cMat[k][j] = (mat1.cMat[k][i] * mat2.cMat[i][j]);
	      }
	  }
      }
  }
  //-----------------------------
  // Initialise les rotation pour la partie 3x3 de la matrice
  void Mat4::initRotZ_3x3( PDouble a)
  {
    cMat[0][0] = ::cos(a);
    cMat[1][1] = cMat[0][0];
    cMat[1][0] = ::sin(a);
    cMat[0][1] = -1 * cMat[1][0];
    cMat[0][2] = cMat[1][2] = cMat[2][0] = cMat[2][1] = 0;
    cMat[2][2] = 1;
			
    cMat[3][0] = cMat[3][1] = cMat[3][2] = 0;
    cMat[0][3] = cMat[1][3] = cMat[2][3] = 0;
    cMat[3][3] = 1;
  }
  //-----------------------------
  void Mat4::initRotY_3x3( PDouble b)
  {
    cMat[0][0] = ::cos(b);
    cMat[2][2] = cMat[0][0];
    cMat[2][0] = ::sin(b);
    cMat[0][2] = -1 * cMat[2][0];
    cMat[0][1] = cMat[1][0] = cMat[1][2] = cMat[2][1] = 0;
    cMat[1][1] = 1;
			
    cMat[3][0] = cMat[3][1] = cMat[3][2] = 0;
    cMat[0][3] = cMat[1][3] = cMat[2][3] = 0;
    cMat[3][3] = 1;
  }
  //-----------------------------
  void Mat4::initRotX_3x3( PDouble c)
  {
    cMat[1][1] = ::cos(c);
    cMat[2][2] = cMat[1][1];
    cMat[2][1] = ::sin(c);
    cMat[1][2] = -1 * (cMat[2][1]);
    cMat[0][1] = cMat[1][0] = cMat[0][2] = cMat[2][0] = 0;
    cMat[0][0] = 1;
			
    cMat[3][0] = cMat[3][1] = cMat[3][2] = 0;
    cMat[0][3] = cMat[1][3] = cMat[2][3] = 0;
    cMat[3][3] = 1;
  }
  //-----------------------------
  void Mat4::initRotFor4x4()
  {
    cMat[0][3] = cMat[1][3] = cMat[2][3] = cMat[3][0] =
      cMat[3][1] =  cMat[3][2] = 0;
    cMat[3][3] = 1;
  }
  //-----------------------------
  //-----------------------------
  //-----------------------------
  void Mat4::initRot( PDouble x, PDouble y, PDouble z)
  {
    Mat4 lRotX;
    lRotX.initRotX_3x3( x );

    Mat4 lRotY;
    lRotY.initRotY_3x3( y );
		
    Mat4 lRotZ;
    lRotZ.initRotZ_3x3( z );

    Mat4 lTmp;
		
    MulMat3( lTmp, lRotX, lRotY );
    MulMat3( *this, lTmp, lRotZ );
    initRotFor4x4();
  }
  //-----------------------------
  void Mat4::set( Transf3d& T)
  {
    Mat4 lTmp;
    lTmp.Identity();
    lTmp.initMove( T.position().x(), T.position().y(), T.position().z() );
    *this *= lTmp;

    lTmp.Identity();
    lTmp.initRot( T.angle().x(), T.angle().y(), T.angle().z() );
    *this *= lTmp;

    lTmp.Identity();
    lTmp.initScale( T.scale().x(), T.scale().y(), T.scale().z() );
    *this *= lTmp;
  }
  //-----------------------------
  void Mat4::initRotAxis( Point3d  u, PDouble a)
  {
    // Mat[ line ][ col ]
    long double c = ::cos(a);
    long double s = ::sin(a);
    long double x = u.cX;
    long double y = u.cY;
    long double z = u.cZ;
    long double cm1 = 1-c;
    long double xy=x*y;
    long double xz=x*z;
    long double yz=y*z;
    long double x2=x*x;
    long double y2=y*y;
    long double z2=z*z;
    long double xs=x*s;
    long double ys=y*s;
    long double zs=z*s;
    

    
    cMat[0][0] = x2*cm1 + c;
    cMat[0][1] = xy*cm1 - zs;			     
    cMat[0][2] = xz*cm1 + ys;

    cMat[1][0] = xy*cm1+zs;
    cMat[1][1] = y2*cm1+c;  
    cMat[1][2] = yz*cm1-xs;
    
    cMat[2][0] = xz*cm1-ys;
    cMat[2][1] = yz*cm1+xs;  
    cMat[2][2] = z2*cm1+c;
	
    initRotFor4x4();
  }

  //**************************************

}
