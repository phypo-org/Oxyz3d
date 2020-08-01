#ifndef H__Double3__H
#define H__Double3__H


#include <iostream>
#include <stdexcept>


#include "string.h"

#include "PP3dType.h"
#include "Point3d.h"

namespace PP3d {

  
  //*************************************
	
  class Double3
  {
  protected:
    PDouble cVect[3];

  public:
    Double3()
    {		
      for( int i=0; i<3; i++)			cVect[i] = 0;		
    }

    Double3( PDouble A, PDouble B=0, PDouble C = 0  )
    {
      cVect[0] = A;
      cVect[1] = B;
      cVect[2] = C;
			
    }
	
    Double3(PDouble pVect[ 3 ])
    {
      for( int i=0; i<3; i++)			cVect[i] = pVect[i];
    }

    Double3( const Double3&  pVal )
    {
      *this = pVal;
    }
    Double3( const Point3d&  pVal );
  

    PDouble x() const { return cVect[0]; }
    PDouble y() const { return cVect[1]; }
    PDouble z() const { return cVect[2]; }
	
    PDouble& x() { return cVect[0]; }
    PDouble& y() { return cVect[1]; }
    PDouble& z() { return cVect[2]; }
	
    void set( PDouble pX, PDouble pY, PDouble pZ )
    {
      cVect[ 0 ] = pX;
      cVect[ 1 ] = pY;
      cVect[ 2 ] = pZ;
    }
		
    PDouble* getVect() { return &cVect[0]; }

    const Double3 & operator = (const Double3& pVal)
    {
      for( int i=0; i<3; i++)			cVect[i] = pVal.cVect[i];
      return *this;
    }
    
    const Double3 & operator = (const Point3d& pVal);

    void raz (PDouble pVal=0)
    {
      for( int i=0; i<3; i++)		cVect[i] = pVal;
	
    }
   
    const Double3&  operator += (const  Double3& pVal)
    {
      for( int i = 0; i < 3; i++ )
	cVect[ i ] += pVal.cVect[ i ];
      return *this;
    }

    const Double3&  operator -= (const  Double3& pVal)
    {
      for( int i = 0; i < 3; i++ )
	cVect[i] -= pVal.cVect[i];
      return *this;
    }

    const Double3&  operator *= (const  Double3& pVal)
    {
      for( int i = 0; i < 3; i++ )
	cVect[i] *= pVal.cVect[i];
      return *this;
    }

    const Double3&  operator /= (const  Double3& pVal)
    {
      for( int i = 0; i < 3; i++ )
	cVect[i] /= pVal.cVect[i];
      return *this;
    }

    const Double3& operator += ( PDouble pVal)
    {
      for( int i = 0; i < 3; i++ )
	cVect[ i ] += pVal;

      return *this;
    }

    const Double3&  operator -= ( PDouble pVal)
    {
      for( int i = 0; i < 3; i++ )
	cVect[i] -= pVal;
      return *this;
    }

    const Double3&  operator *= (PDouble pVal)
    {
      for( int i = 0; i < 3; i++ )
	cVect[i] *= pVal;
      return *this;
    }

    const Double3&  operator /= (PDouble pVal)
    {
      for( int i = 0; i < 3; i++ )
	cVect[i] /= pVal;
      return *this;
    }
	
    void inverse()
    {
      for( int i = 0; i < 3; i++ )
	cVect[i] = -cVect[i];
    }
    PDouble& operator [] (int pos)
    {
      return cVect[pos];
    }

    PDouble operator [] (int pos) const 
    {
      return cVect[pos];
    }


    operator PDouble *(  )
    {
      return cVect;
    }

    std::ostream& print( std::ostream& os)
    {
      for( int i = 0; i < 3; i++ )
	os << cVect[i] << ", ";

      return os;
    }
    friend std::ostream& operator <<( std::ostream& pOs, Double3 pPt)
    {
      pOs << pPt.cVect[0] << ',' << pPt.cVect[1]  << ',' << pPt.cVect[2]; 
      return pOs;
    }
  };
}


#endif
