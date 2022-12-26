#ifndef H__Point3d__H
#define H__Point3d__H

#include "PP3dType.h"


#include <ostream>

#include <cmath>

namespace PP3d {


  //********************************************************

  template <typename TFLOAT>
  class TPoint3d{
	  
  public:
    TFLOAT cX;
    TFLOAT cY;
    TFLOAT cZ;


    TPoint3d()	:cX(0), cY(0), cZ(0) {}
    TPoint3d( TFLOAT pX, TFLOAT pY, TFLOAT pZ)	:cX(pX), cY(pY), cZ(pZ) {}
    //    TPoint3d(  const TFLOAT &pX, const TFLOAT& pY, const TFLOAT& pZ)	:cX(pX), cY(pY), cZ(pZ) {}
    void set ( TFLOAT pX, TFLOAT pY, TFLOAT pZ)	{ cX = pX;	cY = pY;	cZ = pZ; }
    void zero(){ cX=	cY=	cZ=0;	}
    void raz() { zero();}
   TPoint3d(const TPoint3d& iSrc )
      :cX(iSrc.cX), cY(iSrc.cY), cZ(iSrc.cZ)
    {
      
    }

    TFLOAT x() const { return cX; }
    TFLOAT y() const { return cY; }
    TFLOAT z() const { return cZ; }
    
    TFLOAT & x() { return cX; }
    TFLOAT & y() { return cY; }
    TFLOAT & z() { return cZ; }

    TPoint3d& operator = ( const TPoint3d& pVar )
    {
      cX = pVar.cX;
      cY = pVar.cY;
      cZ = pVar.cZ;
      return *this;
    }
    void  operator += ( const TPoint3d& pVar )
    {
      cX += pVar.cX;
      cY += pVar.cY;
      cZ += pVar.cZ;
    }
    void  operator *= ( const TPoint3d& pVar )
    {
      cX *= pVar.cX;
      cY *= pVar.cY;
      cZ *= pVar.cZ;
    }
    void  operator -= ( const TPoint3d& pVar )
    {
      cX -= pVar.cX;
      cY -= pVar.cY;
      cZ -= pVar.cZ;
    }
    void  operator /= ( const TPoint3d& pVar )
    {
      cX /= pVar.cX;
      cY /= pVar.cY;
      cZ /= pVar.cZ;
    }

    void  operator += ( TFLOAT pVar )
    {
      cX += pVar;
      cY += pVar;
      cZ += pVar;
    }
    void  operator -= ( TFLOAT pVar )
    {
      cX -= pVar;
      cY -= pVar;
      cZ -= pVar;
    }
    void  operator *= ( TFLOAT pVar )
    {
      cX *= pVar;
      cY *= pVar;
      cZ *= pVar;
    }
    void  operator /= ( TFLOAT pVar )
    {
      cX /= pVar;
      cY /= pVar;
      cZ /= pVar;
    }
    void addX( TFLOAT pX) { cX += pX; }
    void addY( TFLOAT pY) { cY += pY; }
    void addZ( TFLOAT pZ) { cZ += pZ; }

    TPoint3d  operator - () const	{	return TPoint3d( -cX, -cY, -cZ );	}
    //--------------------------------------------
     bool isZero ( ) const
    {
      if( cX == 0 && cY == 0 && cZ == 0 )
	return true;
      return false;
    }		
    //--------------------------------------------
    bool sameEpsi ( long double iEpsi, const TPoint3d & B )
    {
      TPoint3d lDiff( *this-B );
      if( fabs( lDiff.cX ) <= iEpsi
	  && fabs( lDiff.cY ) <= iEpsi
	  && fabs( lDiff.cZ ) <= iEpsi )
	return true;

      return false;
    }
    //--------------------------------
  
    const TFLOAT* vectForGL() const
    {
      return &cX;
    }		
    TFLOAT * operator *() 
    {
      return &cX;
    }
    //--------------------------------------------		
    //--------------------------------------------		
    //--------------------------------------------		
    friend TPoint3d operator * ( const TPoint3d & A, TFLOAT pK )
    {
      return TPoint3d( A.cX*pK, A.cY*pK, A.cZ*pK );
    }
    //--------------------------------------------		
    friend TPoint3d operator / ( const TPoint3d & A, TFLOAT pK )
    {
      return TPoint3d( A.cX/pK, A.cY/pK, A.cZ/pK );
    }
    //--------------------------------------------		
    friend TPoint3d operator + ( const TPoint3d & A, TFLOAT pK )
    {
      return TPoint3d( A.cX+pK, A.cY+pK, A.cZ+pK );
    }
    //--------------------------------------------		
    friend TPoint3d operator - ( const TPoint3d & A, TFLOAT pK )
    {
      return TPoint3d( A.cX-pK, A.cY-pK, A.cZ-pK );
    }
    //--------------------------------------------
    friend bool operator == ( const TPoint3d  & A, const TPoint3d & B )
    {
      if( A.cX == B.cX && A.cY == B.cY && A.cZ == B.cZ )
	return true;
      return false;
    }
   //--------------------------------------------
    friend bool operator != ( const TPoint3d & A, const TPoint3d &  B )
    {
      return ! operator == (A,B);
    }
		
    friend TPoint3d operator - ( const TPoint3d & A, const TPoint3d & B )
    {
      return TPoint3d( A.cX-B.cX, A.cY-B.cY, A.cZ-B.cZ );
    }
    //--------------------------------------------
		
    friend TPoint3d operator + ( const TPoint3d & A, const TPoint3d & B )
    {
      return TPoint3d( A.cX+B.cX, A.cY+B.cY, A.cZ+B.cZ );	
    }

    //--------------------------------	
    friend std::ostream& operator <<( std::ostream & pOs, const TPoint3d & pPt)
    {
      pOs << "x:" << pPt.cX << " y:" << pPt.cY << " z:" << pPt.cZ; 
      return pOs;
    }
    //-----------------------------------------	
    //--------------- Vecteur -----------------
    //-----------------------------------------	
    TFLOAT square(){
      return  cX*cX + cY*cY +  cZ*cZ;
    }
  
    
    // si c'est une distance entre deux point
    TFLOAT distanceSquare()
    {
      return square();
    }

    //--------------------------------
    // si c'est une distance entre deux point
    TFLOAT distance()
    {
      return std::sqrt( distanceSquare() );
    }
    TFLOAT mag()
    {
      return distance();
    }
    //--------------------------------	
    static TFLOAT Dot(  TPoint3d a, TPoint3d b ) // produit scalaire
    {
      return a.cX * b.cX + a.cY * b.cY + a.cZ * b.cZ;
    }
    //--------------------------------
    // cos a = (XaXb+YaYb+ZaZb) / sqrt((Xa²+Ya²+Za²)(Xb²+Yb²+Zb² ))
    // sqrt(a*b) = sqrt(a)*sqrt(b)
    // O to PI 
    
    static double GetAngleRadBetween( TPoint3d a, TPoint3d b )
    {
      long double lProduitScalaire = Dot( a, b );
      long double lProduitNorme = std::sqrt( a.square()*b.square() );
      if( lProduitNorme != 0 )
	{
	  long double lResult  = lProduitScalaire/lProduitNorme;
	  //	  std::cout << "Dot:" << lProduitScalaire << " Norm:" << lProduitNorme << " -> "
	  //		    << lResult << std::endl;
	  
	  if( ! std::isnan(lResult) && !std::isinf(lResult) )
	    {
	      double lAngleRad = std::acos( lResult );
	      if( ! std::isnan(lAngleRad) && !std::isinf(lAngleRad) )
		{
		  return lAngleRad;
		}
	    }
	}
      return std::nan("");
    }
    //--------------------------------
    static double GetSignedAngleRadBetween( TPoint3d  from, TPoint3d to, TPoint3d axis)
    {
      double lAngle = GetAngleRadBetween(from, to);
      
      float cross_x = from.cY * to.cZ - from.cZ * to.cY;
      float cross_y = from.cZ * to.cX - from.cX * to.cZ;
      float cross_z = from.cX * to.cY - from.cY * to.cX;

      if( axis.cX * cross_x + axis.cY * cross_y + axis.cZ * cross_z < 0 )
	return lAngle*-1;
     
      return lAngle;
    }
    //--------------------------------
    // https://github.com/Unity-Technologies/UnityCsReference/blob/master/Runtime/Export/Math/Vector3.cs
    static bool isNegAngle( TPoint3d  from, TPoint3d to, TPoint3d axis)
    {       
      float cross_x = from.cY * to.cZ - from.cZ * to.cY;
      float cross_y = from.cZ * to.cX - from.cX * to.cZ;
      float cross_z = from.cX * to.cY - from.cY * to.cX;
      return ( axis.cX * cross_x + axis.cY * cross_y + axis.cZ * cross_z) < 0;
    }
     //--------------------------------
    // Mettre un epsilon !!!
    static bool isPositifAngle( TPoint3d  from, TPoint3d to, TPoint3d axis)
    {       
      float cross_x = from.cY * to.cZ - from.cZ * to.cY;
      float cross_y = from.cZ * to.cX - from.cX * to.cZ;
      float cross_z = from.cX * to.cY - from.cY * to.cX;
      return ( axis.cX * cross_x + axis.cY * cross_y + axis.cZ * cross_z) > 0;
    }
     //--------------------------------
    static double signedAngle( TPoint3d  from, TPoint3d to, TPoint3d axis)
    {       
      float cross_x = from.cY * to.cZ - from.cZ * to.cY;
      float cross_y = from.cZ * to.cX - from.cX * to.cZ;
      float cross_z = from.cX * to.cY - from.cY * to.cX;
      return axis.cX * cross_x + axis.cY * cross_y + axis.cZ * cross_z;
    }
    //--------------------------------	
    TFLOAT dot( TPoint3d b )
    {
      return cX * b.cX + cY * b.cY + cZ * b.cZ;
    }
    //--------------------------------	
    static TPoint3d Cross( TPoint3d u, TPoint3d v )   // produit vectoriel 
    {
      return TPoint3d( u.cY * v.cZ - u.cZ * v.cY,
		       u.cZ * v.cX - u.cX * v.cZ,    
		       u.cX * v.cY - u.cY * v.cX ) ;
    }
    //--------------------------------	
    TPoint3d cross( TPoint3d v )   // produit vectoriel 
    {
      return TPoint3d( cY * v.cZ - cZ * v.cY,
		       cZ * v.cX - cX * v.cZ,    
		       cX * v.cY - cY * v.cX ) ;
    }
    //--------------------------------	
    TFLOAT modulus()
    {
      return std::sqrt( dot( *this ) ) ;
    }
    //--------------------------------
    TPoint3d versor()
    {
      TFLOAT  lM = modulus();
      if( lM == 0 )
	return TPoint3d(0,0,0);

      return (*this) * (1/lM);			
    }
    //--------------------------------	
    static TFLOAT Distance( TPoint3d A, TPoint3d B)
    {
      TPoint3d D = A-B;
      return D.distance();
    }
    //--------------------------------	
    static TFLOAT DistanceToLine( TPoint3d P, TPoint3d A, TPoint3d B)
    {
      TPoint3d AB = B-A;
      TPoint3d AP = P-A;

      if( AP.dot( AB ) <= 0 )
	return AP.modulus( );

      TPoint3d BP = P-B;
      if( BP.dot( AB ) >= 0 )
	return BP.modulus();

      return (AB.cross( AP)).modulus()/AB.modulus();
    }
    //--------------------------------
    static TPoint3d Middle( TPoint3d A, TPoint3d B)
    {
      return TPoint3d( (A.cX+B.cX)*0.5,  (A.cY+B.cY)*0.5,  (A.cZ+B.cZ)*0.5 );
    }
    //--------------------------------
    static TPoint3d Middle( TPoint3d A, TPoint3d B, TPoint3d C )
    {
      return TPoint3d( (A.cX+B.cX+C.cX)/3.0,  (A.cY+B.cY+C.cY)/3.0,  (A.cZ+B.cZ+C.cZ)/3.0 );
    }
    //--------------------------------
    static TPoint3d Middle( TPoint3d A, TPoint3d B, TPoint3d C, TPoint3d D )
    {
      return TPoint3d( (A.cX+B.cX+C.cX+D.cX)*0.25,  (A.cY+B.cY+C.cY+D.cY)*0.25,  (A.cZ+B.cZ+C.cZ+D.cZ)*0.25 );
    }
    //--------------------------------
    static TPoint3d Middle( TPoint3d A, TPoint3d B, TPoint3d C, TPoint3d D, TPoint3d E  )
    {
      return TPoint3d( (A.cX+B.cX+C.cX+D.cX+E.cX)*0.2,  (A.cY+B.cY+C.cY+D.cY+E.cY)*0.2,  (A.cZ+B.cZ+C.cZ+D.cZ+E.cZ)*0.2 );
    }
    //--------------------------------
    static TPoint3d Middle( std::vector<TPoint3d> & iVect )
    {
      TPoint3d lSum;
      
      for( TPoint3d & lPt : iVect )
	{
	  lSum += lPt;
	}
      lSum /= iVect.size();
      
      
      return lSum;
    }
    //--------------------------------	
    void normalize( PDouble pVal=1.0)
    {				
      PDouble d = (PDouble) std::sqrt(cX*cX+cY*cY+cZ*cZ);  // norme 
      if (d == 0.0f) {
	d = 1.0f;
      }
      d = pVal / d;
      cX *= d;
      cY *= d;
      cZ *= d;		
    }

    void round( const double iVal )
    {
      if( iVal == 0 ) return;
	
      cX = cX - fmod(cX, iVal);      
      cY = cY - fmod(cY, iVal);
      cZ = cZ - fmod(cZ, iVal);
    }
  };
	
  using Point3d  = TPoint3d<PDouble>;
  using Point3dF = TPoint3d<GLfloat>;
	
  using Vector3d  = TPoint3d<PDouble>;
	
  //******************************************
  class VectPoint3d{
    
  protected:
    ²std::vector<Point3d> cVect;
  

  public:


    Point3d& operator []( PIndex pInd ) 
    {
      if( pInd >=  cVect.size() )
	throw std::out_of_range(" <Point3d[]> ");
      
      return cVect[pInd];
    }
    
    const Point3d& operator []( PIndex pInd) const
    {
      if( pInd >=  cVect.size() )
	throw std::out_of_range(" <Point3d[]const> ");
      
      return cVect[pInd];
    }
    //--------------------------------
    size_t size() const { return cVect.size(); }
    //--------------------------------    
    void add( Point3d pVal )
    {
      cVect.push_back( pVal);      
    }
    //--------------------------------    
    const std::vector<Point3d>& getVector() const { return cVect; }
    std::vector<Point3d>&       getVector() { return cVect; }
    void resize( size_t pSize )  { cVect.resize( pSize ); }
    void clear()                 { cVect.clear(); }
    //--------------------------------    

    void  operator += ( const Point3d pVar ) //  ne pas mettre de & a pVar
    {
      for( Point3d& lPt : cVect )
	{
	  lPt += pVar;
	}
    }
    //--------------------------------    
    void  operator *= ( const Point3d& pVar ) 
    {
      for( Point3d& lPt : cVect )
	{
	  lPt *= pVar;
	}
    }
    //--------------------------------    
    void  operator -= ( const Point3d& pVar ) 
    {
      for( Point3d& lPt : cVect )
	{
	  lPt -= pVar;
	}
    }
    //--------------------------------    
    void  operator /= ( const Point3d& pVar ) 
    {
      for( Point3d& lPt : cVect )
	{
	  lPt /= pVar;
	}
    }

    //--------------------------------    
	
    friend std::ostream& operator <<( std::ostream& pOs, VectPoint3d& pPts)
    {
      for( Point3d& lPt : pPts.cVect )
	{
	  pOs << lPt << std::endl;
	}

      return pOs;
    }
    //--------------------------------
   		
  };

  //********************************************************

}

#endif
