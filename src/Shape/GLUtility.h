
#ifndef H__GLUtility__H
#define H__GLUtility__H

//#include <config.h>
//#include <FL/Fl.H>
#include "PP3dType.h"


namespace PP3d {
//*******************************************
	class Rect3d;
	
	class GLUtility{
		
	public:
		
		static void DrawOrtho(   float pSz, int  pNeg );
		static void DrawGrid( float pSz, int pNb, bool pNombre );
		static void DrawChar( Point3d pPos, float pSz,  char pChar );
		static void DrawText( bool pOrient, Point3d pPos, float pSz, const char* pStr, int  pReverse=0  );
		static void DrawCursor( Point3d pPt, float pSz );
		static void DrawCursorCruz( Point3d pPt, float pSz );
		static void DrawMinMaxBox(Rect3d&lMMb); 
		
	protected:
		static void DrawMinus( float pX, float pY, float pZ, float pSz );
		static void DrawX( float pX, float pY, float pZ, float pSz );
		static void DrawY( float pX, float pY, float pZ, float pSz );
		static void DrawZ( float pX, float pY, float pZ, float pSz );
	};
	
//*******************************************
};

#endif


