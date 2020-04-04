#ifndef h_T3dBox_h
#define h_T3dBox_h


/*---------------------------------------------------------------------*/
/*    include ...                                                      */
/*---------------------------------------------------------------------*/


#include "PP3dType.h"


#include <string.h>



namespace PP3d{
//******************************************
  class Rect3d
{
  Point3d cMin;
  Point3d cMax;
	
public:
	Rect3d()
	{	
		reset();
	}
	void reset()
	{		
		cMin.cX = cMin.cY = cMin.cZ =  1e10;
		cMax.cX = cMax.cY = cMax.cZ = -1e10;		 
	}
	
	void set( Point3d pA, Point3d pB )
	{
		/*
		if(pA.cX < pB.cX )
			{
				cMin.cX = pA.cX;
				cMax.cX =pB.cX;
			}
		else
			{
				cMin.cX = pB.cX;
				cMax.cX =pA.cX;
			}
		
		if(pA.cY < pB.cY )
			{
				cMin.cY = pA.cY;
				cMax.cY =pB.cY;
			}
		else
			{
				cMin.cY = pB.cY;
				cMax.cY =pA.cY;
			}
		
		if(pA.cZ < pB.cZ )
			{
				cMin.cZ = pA.cZ;
				cMax.cZ =pB.cZ;
			}
		else
			{
				cMin.cZ = pB.cZ;
				cMax.cZ =pA.cZ;
			}
		*/
			cMin = pA;
				cMax = pB;
	}
	Point3d &getMin()
	{
		return cMin;
	}
	Point3d &getMax()
	{
		return cMax;
	}

	Point3d size() const;
	Point3d center() const ;
	Double3 centerDbl() const ;
	Point3d getCenter( ) const;
	Double3 getCenterDbl( ) const;

	virtual char *getInfoStr( char *pStr );
	 //-------------------------------
	// Only use X and Y !!!
	bool isIntoXY( const Point3d  pPt ) const
	{
		if( pPt.cX < cMin.cX
 			  ||  pPt.cX>cMax.cX )
			return false;

		if( pPt.cY < cMin.cY
 			  ||  pPt.cY>cMax.cY )
			return false;
	
		return true;
	}
	bool isInto( const Point3d  pPt ) const
	{
		if( pPt.cX < cMin.cX
 			  ||  pPt.cX>cMax.cX )
			return false;

		if( pPt.cY < cMin.cY
 			  ||  pPt.cY>cMax.cY )
			return false;
	
		if( pPt.cZ < cMin.cZ
 			  ||  pPt.cZ>cMax.cZ )
			return false;

		return true;
	}
	//-------------------------------
	void add( Point3d pPt )
	{
		if( pPt.cX < cMin.cX )
			cMin.cX = pPt.cX;
		else	if( pPt.cX > cMax.cX )
			cMax.cX = pPt.cX;
		
		if( pPt.cY < cMin.cY )
			cMin.cY = pPt.cY;
		else	if( pPt.cY > cMax.cY )
			cMax.cY = pPt.cY;

		if( pPt.cZ < cMin.cZ )
			cMin.cZ = pPt.cZ;
		else	if( pPt.cZ > cMax.cZ )
			cMax.cZ = pPt.cZ;

	}
	PDouble distanceSquare() const;
	PDouble distance() const;
	bool    isInRadius( Point3d pPt );
	//-------------------------------
	void drawGL();

	//-------------------------------

	friend class T3dVect3D;
	friend std::ostream& operator <<( std::ostream& pOs, const Rect3d& pMM)
	{
	  pOs << '[' << pMM.cMin << ' ' << pMM.cMax << ']';	 
	 return pOs;
	}

};
	
	typedef Rect3d  MinMaxBox3d;
	
//***************************************
}


#endif
