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
    Double3(){		
    }

    Double3( PDouble A, PDouble B=0, PDouble C = 0  ){
      set( A, B, C);			
    }
	
    Double3( const Double3&  pVal ) {
      *this = pVal;
    }
    Double3( const Point3d&  pVal ) {
      set( pVal);
    }
  
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
    void set( Double3 pVal ) {
      *this = pVal;
    }

    void set( Point3d pVal ) {
      x() = pVal.x();
      y() = pVal.y();
      z() = pVal.z();
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

    friend class VectDouble3;
  };
  //*********************************************
  class VectDouble3
  {
  protected:
    std::vector<PDouble> cVect[3];

  public:
    VectDouble3()
    {		
    }

    VectDouble3( PDouble iA, PDouble iB=0, PDouble iC = 0  )
    {
      add( iA, iB, iC );
    }
    VectDouble3( Double3 iPt )
    {
      add( iPt );
    }
    VectDouble3( const Point3d & iPt)
    {
      add( iPt );
    }  
    
    VectDouble3( const VectPoint3d & iVectPt)
    {    
      add( iVectPt );
    }
    /*
    const std::vector<PDouble> & operator [] (PIndex pos) const 
    {
      return cVect[pos];
    }
    std::vector<PDouble> & operator [] (PIndex pos)  
    {
      return cVect[pos];
    }
    */
    void clear() {
      for( int i=0; i<3; i++ )
        cVect[i].clear();
    }

    PIndex size() { return cVect[0].size(); }
    void resize( PIndex iSz ) {  for( int i = 0; i < 3; i++ ) cVect[i].resize( iSz ); }
      
    void reset(){ for( int i = 0; i < 3; i++ )cVect[i].clear(); }


    const std::vector<PDouble> & X() const { return cVect[0]; }
    const std::vector<PDouble> & Y() const { return cVect[1]; }
    const std::vector<PDouble> & Z() const { return cVect[2]; }
	
    std::vector<PDouble> & X() { return cVect[0]; }
    std::vector<PDouble> & Y() { return cVect[1]; }
    std::vector<PDouble> & Z() { return cVect[2]; }
    
    PDouble  x( PIndex iPos ) const { return cVect[0][iPos]; }
    PDouble  y( PIndex iPos ) const { return cVect[1][iPos]; }
    PDouble  z( PIndex iPos ) const { return cVect[2][iPos]; }
    
    PDouble & x( PIndex iPos )  { return cVect[0][iPos]; }
    PDouble & y( PIndex iPos )  { return cVect[1][iPos]; }
    PDouble & z( PIndex iPos )  { return cVect[2][iPos]; }

    Point3d  getPoint3d( int iPos ) const { return Point3d( x(iPos), y(iPos), z(iPos) ); }

    void add(  PDouble iA, PDouble iB, PDouble iC  ){
      cVect[0].push_back( iA );
      cVect[1].push_back( iB );
      cVect[2].push_back( iC );			 
    }
    
    void add( Double3 iPt ){
      cVect[0].push_back( iPt.x() );
      cVect[1].push_back( iPt.y() );
      cVect[2].push_back( iPt.z() );			 
    }
    
    void add( const Point3d & iPt) {
      cVect[0].push_back( iPt.x() );
      cVect[1].push_back( iPt.y() );
      cVect[2].push_back( iPt.z() );			 
    }
    
    void add( const VectPoint3d & iVectPt) {
      for( const Point3d & lPt :iVectPt.getVector() )
        {
          add( lPt );
        }
    }
    void set( const VectPoint3d & iVectPt) {
      reset();
      add( iVectPt );     
    }
    void set( PIndex iPos,  const Point3d & iPt) {
      reset();
      x( iPos ) = iPt.x();
      y( iPos ) = iPt.y();
      z( iPos ) = iPt.z();
    }
   void set( PIndex iPos,  const Double3 & iPt) {
      reset();
      x( iPos ) = iPt.x();
      y( iPos ) = iPt.y();
      z( iPos ) = iPt.z();
    }


    Double3 operator []( PIndex iPos  ) {
      
      return Double3( x( iPos ),  y( iPos ), z( iPos ));
    }
    
    std::vector<PDouble> &  getAxePoint( PIndex iPos  ) {
    
      return cVect[iPos];
    }
  
  };
  //*********************************************
}


#endif
