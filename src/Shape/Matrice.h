#ifndef H__Matrice__H
#define H__Matrice__H

#include <ostream>

#include "Point3d.h"
#include "Double3.h"
#include "Transf3d.h"

#include <utility>
#include <iostream>



namespace PP3d {

	
  // Aucune optimisation n'est faite !
	
  //***************************************
		
  class Mat4;


  //***************************************
	
  class Mat4 {
    PDouble cMat[4][4];

  public:
    Mat4() {identity();}
    //-----------------------------
    void set( PDouble x1, PDouble x2,  PDouble x3,
	      PDouble y1, PDouble y2,  PDouble y3,
	      PDouble z1, PDouble z2,  PDouble z3 )
    {
      cMat[0][0] = x1;
      cMat[0][1] = x2;
      cMat[0][2] = x3;
      cMat[0][3] = 0;
 
      cMat[1][0] = y1; 
      cMat[1][1] = y2;     
      cMat[1][2] = y3;
      cMat[1][3] = 0;
  
      cMat[2][0] = z1;
      cMat[2][1] = z2;
      cMat[2][2] = z3;  
      cMat[2][3] = 0;
      
      cMat[3][0] = 0;
      cMat[3][1] = 0;
      cMat[3][2] = 0;  
      cMat[3][3] = 1;      
    }
    //-----------------------------
    Mat4( const Mat4& iSrc) 
    {
      ::memcpy( cMat, iSrc.cMat, sizeof( cMat ) );	    
    }
   //-----------------------------
    void operator = ( const Mat4& iSrc)
    {
      ::memcpy( cMat, iSrc.cMat, sizeof( cMat ) );
    }
    //-----------------------------
    void raz()
    {
      ::memset( cMat, 0, sizeof( cMat ) );
    }
    //-----------------------------
    void identity()
    {
      for ( int j = 0; j < 4; j++ )
	{
	  for ( int i = 0; i < 4; i++ )
	    cMat[i][j] = 0;
	}
      /*  place les 1 sur la diagonale  */
      for ( int i = 0; i < 4; i++ )
	cMat[i][i] = 1;
    }
    //-----------------------------

    void transpose()
    {
      std::swap( cMat[0][1], cMat[1][0] );
      std::swap( cMat[0][2], cMat[2][0] );
      std::swap( cMat[0][3], cMat[3][0] );
      std::swap( cMat[1][2], cMat[2][1] );
      std::swap( cMat[1][3], cMat[3][1] );
      std::swap( cMat[2][3], cMat[3][2] );
    }
    //-----------------------------
    void initMove( PDouble pX, PDouble pY, PDouble pZ )
    {
      identity();
      cMat[0][3] += pX;
      cMat[1][3] += pY;
      cMat[2][3] += pZ;
    }
    void initMove( const  Point3d & pPt ) { initMove( pPt.cX, pPt.cY, pPt.cZ); }
    void initMove( const  Double3 & pPt ) { initMove( pPt.x(), pPt.y(), pPt.z()); }
    //-----------------------------
    void initScale( PDouble pX, PDouble pY, PDouble pZ )
    {
      identity();
      cMat[0][0] *= pX;
      cMat[1][1] *= pY;
      cMat[2][2] *= pZ;			
    }
     void initScale( PDouble pVal )
    {
      identity();
      cMat[0][0] *= pVal;
      cMat[1][1] *= pVal;
      cMat[2][2] *= pVal;			
    }
   void initScale( const  Point3d & pPt ) { initScale( pPt.cX, pPt.cY, pPt.cZ); }
    void initScale( const  Double3 & pPt ) { initScale( pPt.x(), pPt.y(), pPt.z()); }
    //-----------------------------

  private:
    // voir Sculptor/tranbase.c
    static void MulMat3( Mat4& matres, Mat4& mat1, Mat4& mat2 );
    static void MulMat4( Mat4& matres, Mat4& mat1, Mat4& mat2 );
										
    void initRotX_3x3( PDouble A);
    void initRotY_3x3( PDouble A);
    void initRotZ_3x3( PDouble A);
    void initRotFor4x4();

  public:
    void initRotX( PDouble A) { initRotX_3x3( A); initRotFor4x4(); }
    void initRotY( PDouble A) { initRotY_3x3( A); initRotFor4x4(); }
    void initRotZ( PDouble A) { initRotZ_3x3( A); initRotFor4x4(); }
    void initRot( PDouble A, PDouble B, PDouble C);
    void initRotAxis( Point3d  u, PDouble a);

			
    //-----------------------------
    //-----------------------------
    //-----------------------------
    PDouble operator()(size_t k, size_t j ) const { return cMat[k][j]; }
    //-----------------------------
    friend std::ostream&  operator << ( std::ostream& pOut, Mat4 A)
    {
      pOut << '(';
      for ( int k = 0; k < 4; k++ )
	{
          if( k != 0 )
            pOut << ' ';
	  pOut << '(';
	  for ( int j = 0; j<4; j++ )
	    {
	      pOut << A.cMat[k][j] ;
	      if ( j != 4-1 )
		pOut	<< ',';
	    }
	  pOut << ')';
	  if( k != 4-1 )
	    pOut << std::endl;
	}
      pOut << ')';
      return pOut;
    }
    //-----------------------------
    friend Mat4 operator *( const Mat4& A, const Mat4& B) 
    {
      Mat4 lRes;
      lRes.raz();
			
      for ( int row = 0; row < 4; row++ )
	{
	  for ( int col = 0; col<4; col++ )
	    {
	      for ( int i = 0; i<4; i++ )
		{ 
		  lRes.cMat[row][col] += ( A.cMat[row][i] * B.cMat[i][col]);
		}
	    }
					
	}
      return lRes;
    }
		
		
    friend Point3d operator * ( Point3d P, const Mat4& M)
    {
      // pour un point avec donc avec w a 1 
      return Point3d( P.cX * M(0,0) + P.cY * M(0,1) + P.cZ * M(0,2) + M(0,3),			
		      P.cX * M(1,0) + P.cY * M(1,1) + P.cZ * M(1,2) + M(1,3),
		      P.cX * M(2,0) + P.cY * M(2,1) + P.cZ * M(2,2) + M(2,3));
    }
    //-----------------------------

    friend  void operator *= ( Point3d& P, const Mat4& M)
    {
      //			std::cout << " *= " << P ;
      P = Point3d( P.cX * M(0,0) + P.cY * M(0,1) + P.cZ * M(0,2) + M(0,3),			
		   P.cX * M(1,0) + P.cY * M(1,1) + P.cZ * M(1,2) + M(1,3),
		   P.cX * M(2,0) + P.cY * M(2,1) + P.cZ * M(2,2) + M(2,3));
		
      //			std::cout << " ==>> " << P << std::endl;
    }
		
    //-----------------------------
    friend  void operator *= ( VectPoint3d& P, const Mat4& M)
    {
      for( Point3d& lPt : P.getVector() )
	{
	  lPt *= M;
	}
    }
    //-----------------------------
    friend bool operator ==( const Mat4 & A, const Mat4& B )
    {
      for ( int j = 0; j < 4; j++ )
	{
	  for ( int i = 0; i < 4; i++ )
	    {
	      if( A.cMat[i][j] != B.cMat[i][j] )
		return false;
	    }
	}
      return true;			
    }
    //-----------------------------
    void set( Transf3d& pTransform);

    //-----------------------------
    void  operator *= ( const Mat4 & B )
    {
      const Mat4& A = *this;
      *this = A * B;
    }
    //-----------------------------
    const PDouble* vectForGL() const
    {
      return &cMat[0][0];
    }
    
    void rotateAlign( Point3d u1, Point3d u2);
    void rotateAlignInv( Point3d u1, Point3d u2);

    static Vector3d GetCross( Vector3d u1, Vector3d u2)
  {
    Vector3d axis = Vector3d::Cross( u1, u2 );
    return axis;
  }
  
    static Vector3d GetCrossNorm( Vector3d u1, Vector3d u2)
  {
    Vector3d axis = Vector3d::Cross( u1, u2 );
    axis.normalize();
    return axis;
  }
 		
  };


  //***************************************


	
}



#endif
