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
  // rotation d'un angle a  autour du vecteur u
  void Mat4::initRotAxis( Point3d  u, PDouble a)
  {
    // Mat[ line ][ col ]
    long double c = ::cos(a);
    long double s = ::sin(a);
    long double x = u.cX;
    long double y = u.cY;
    long double z = u.cZ;
    long double ic = 1-c;
    long double xy=x*y;
    long double xz=x*z;
    long double yz=y*z;
    long double x2=x*x;
    long double y2=y*y;
    long double z2=z*z;
    long double xs=x*s;
    long double ys=y*s;
    long double zs=z*s;
    

    
    cMat[0][0] = x2*ic + c;
    cMat[0][1] = xy*ic - zs;			     
    cMat[0][2] = xz*ic + ys;

    cMat[1][0] = xy*ic+zs;
    cMat[1][1] = y2*ic+c;  
    cMat[1][2] = yz*ic-xs;
    
    cMat[2][0] = xz*ic-ys;
    cMat[2][1] = yz*ic+xs;  
    cMat[2][2] = z2*ic+c;
	
    initRotFor4x4();
  }
  //-----------------------------
  // aligne deux vecteurs  optimise
  
  void Mat4::rotateAlign( Vector3d d, Vector3d z)
  {
    Vector3d v = Vector3d::Cross( z, d );
    
    const double cosA = Vector3d::Dot( z, d );
    const double k = 1.0 / (1.0 + cosA);

    set(
	v.x() * v.x() * k + cosA,
	v.y() * v.x() * k - v.z(),
	v.z() * v.x() * k + v.y(),
	
	v.x() * v.y() * k + v.z(), 
	v.y() * v.y() * k + cosA,    
	v.z() * v.y() * k - v.x(),
	
	v.x() * v.z() * k  - v.y(),
	v.y() * v.z() * k + v.x(),
	v.z() * v.z() * k + cosA
	);    
  }

  
  // aligne deux vecteurs
  // bug 1 fois sur deux
  /*
  void Mat4::rotateAlign( Vector3d u1, Vector3d u2)
  {
    Vector3d axis = Vector3d::Cross( u1, u2 );
    axis.normalize();

    //   Vector3d axis = normalize(Vector3d::Cross( u1, u2 ); );
    
    double dotProduct = Vector3d::Dot( u1, u2 );
    if( dotProduct > 1.0 )
      dotProduct = 1.0;
    else
      if( dotProduct < -1.0 )
	dotProduct = -1.0;
    //    dotProduct = clamp( dotProduct, -1.0f, 1.0f );
    
    double angleRadians = acosf( dotProduct );

    const double sinA = sinf( angleRadians );
    const double cosA = cosf( angleRadians );
    const double invCosA = 1.0f - cosA;

    set( (axis.x() * axis.x() * invCosA) + cosA,
	 (axis.y() * axis.x() * invCosA) - (sinA * axis.z()), 
	 (axis.z() * axis.x() * invCosA) + (sinA * axis.y()),
	 (axis.x() * axis.y() * invCosA) + (sinA * axis.z()),  
	 (axis.y() * axis.y() * invCosA) + cosA,      
	 (axis.z() * axis.y() * invCosA) - (sinA * axis.x()),
	 (axis.x() * axis.z() * invCosA) - (sinA * axis.y()),  
	 (axis.y() * axis.z() * invCosA) + (sinA * axis.x()),  
	 (axis.z() * axis.z() * invCosA) + cosA 
	 );
    //   initRotFor4x4();
}
  */
  
  void Mat4::rotateAlignInv( Vector3d d, Vector3d z)
  {
    Vector3d axis = Vector3d::Cross( z, d );
    axis.normalize();

    double dotProduct = Vector3d::Dot( z, d );
    
    if( dotProduct > 1.0 )
      dotProduct = 1.0;
    else
      if( dotProduct < -1.0 )
	dotProduct = -1.0;

    double angleRadians = acosf( dotProduct );
      
    initRotAxis(  axis, -angleRadians );
  }
  
  
  //**************************************

}
