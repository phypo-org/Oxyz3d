#ifndef H__PP3dType__H
#define H__PP3dType__H

#if defined(_WIN32)
#include<GL/glut.h>
#else
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#endif

#include <iostream>
#include <stdexcept>
#include <vector>


#include "string.h"


#define INFO3( A ) if( PP3d::sVerbose > 3 )	\
  { \
    std::cout <<  "       " << A ) << std::endl;	\
  }
  
#define INFO2( A )  if( PP3d::sVerbose > 2 ) \
  { \
    std::cout <<  "    "<<A << std::endl;	\
  }
  
#define INFO1( A )  if( PP3d::sVerbose > 1 ) \
  { \
    std::cout<< "  " <<  A << std::endl;		\
  } 
   
#define INFO( A )  if( PP3d::sVerbose > 0) \
  { \
     std::cout <<  A << std::endl; \
  } 
   
#define DBG4( A )  if(PP3d::sDebug > 4 )	\
  { \
    std::cout <<  "%         " << A  << std::endl;	\
  }

#define DBG3( A )  if(PP3d::sDebug > 3 )	\
  { \
    std::cout <<  "%       " << A  << std::endl;	\
  }
  
#define DBG2( A )  if( PP3d::sDebug > 2 ) \
  { \
    std::cout <<  "%    "<< A << std::endl;	\
  }
  
#define DBG1( A )  if( PP3d::sDebug > 1 )	\
  { \
    std::cout<< "%  " <<  A << std::endl;  \
  } 
   
#define DBG( A )  if( PP3d::sDebug > 0) \
  { \
    std::cout << '%' << A << std::endl;	\
  } 
   
namespace PP3d {

  extern int sVerbose;
  extern int sDebug;
	
  using   PDouble =  GLdouble;
  using   PIndex  =  size_t ;
	using 	PP3dId  =   GLuint ;
	
	using   ComponentId = int;
	
	
	enum class SelectType {  Point=1, Line=2, Facet=4, Poly=8, Object=16, All=32};
	inline static const char* GetStrShapeType( SelectType pType )
	{
		switch( pType )
			{
			case SelectType::Point:  return "Point";
			case SelectType::Line:   return "Line";
			case SelectType::Facet:  return "Facet";
			case SelectType::Poly:   return "Poly";
			case SelectType::Object: return "Object";
			case SelectType::All:    return "";
		}
		return "SelectType::unknown";
	}
	inline std::ostream& operator << ( std::ostream& pOs, SelectType pType )
	{
		pOs <<   GetStrShapeType( pType );
		return pOs;
	}


	
	enum class GLMode { Select, Draw };

	enum class SelectMode { Undefine, Select, Unselect, Inverse };
	inline std::ostream& operator << ( std::ostream& pOs, SelectMode pVal)
	{
		switch( pVal )
			{
			case SelectMode::Undefine: pOs <<"Undefine"; break;
			case SelectMode::Select: pOs <<"Select"; break;
			case SelectMode::Unselect: pOs <<"Unselect"; break;
			case SelectMode::Inverse: pOs <<"Inverse"; break;
			}
		return pOs;
	}

	enum class ShapeType  {  Point, Line, Facet, Poly, Object, Null};
	inline static const char* GetStrShapeType( ShapeType pType )
	{
		switch( pType )
			{
			case ShapeType::Point:  return "Point";
			case ShapeType::Line:   return "Line";
			case ShapeType::Facet:  return "Facet";
			case ShapeType::Poly:   return "Poly";
			case ShapeType::Object: return "Object";
			case ShapeType::Null: return "";
		}
		return "ShapeType::unknown";
	}
	
	inline static ShapeType GetShapeTypeFromStr( const char* pStr )
	{
		if( ::strcmp( pStr, "Point" ) == 0 )
			return ShapeType::Point;
		else		if( ::strcmp( pStr, "Line" ) == 0 )
			return ShapeType::Line;
		else		if( ::strcmp( pStr, "Facet" ) == 0 )
			return ShapeType::Facet;
		else		if( ::strcmp( pStr, "Poly" ) == 0 )
			return ShapeType::Poly;
		else		if( ::strcmp( pStr, "Object" ) == 0 )
			return ShapeType::Object;
		return ShapeType::Null;
	}
	
	inline std::ostream& operator << ( std::ostream& pOs, ShapeType pType )
	{
		pOs <<   GetStrShapeType( pType );
		return pOs;
	}

};


#include "Point3d.h"
#include "ColorRGBA.h"

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

		const PDouble x() const { return cVect[0]; }
		const PDouble y() const { return cVect[1]; }
		const PDouble z() const { return cVect[2]; }
	
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

	//********************************************************
	class Rect3d;

	class Transf3d
	{

	protected:
		Double3 cPosition;
		Double3 cAngle;
		Double3 cScale;
	public:
		Transf3d();

		Double3& position() { return cPosition; }
		Double3& angle()    { return cAngle; }
		Double3& scale()    { return cScale; }

		void raz();
		void razX();
		void razY();
		void razZ();

		void centerTo( Rect3d& pBox);
		void scaleTo( Rect3d & pBox, Double3 & pSz );
		void scaleTo( Rect3d& pBox, double pSz );

		void scaleTo( double pSize );
		
		void execGL();
		//	void execBox( MinMaxBox3d & pBox );

	};


  //********************************************************

  //********************************************************

};



#endif
