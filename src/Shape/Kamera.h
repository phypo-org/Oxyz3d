#ifndef H__Kamera__H
#define H__Kamera__H


#include "PP3dType.h"


namespace PP3d {

//***************************************

class Kamera : public Transf3d {

public:
	enum class ModeKamera {MODE_ORTHO, MODE_PERPECTIVE};
	
public:

	ModeKamera  cModeKamera;

	double      cBaseSize;
	double      cRatioWH;


	double      cFovY;
	double      cAspect;
	double      cZnear;
	double      cZfar;

	bool        cSmooth=false;


public:
	Kamera( double pSize = 10 ); //1000
	void initGL();
	void execGL( bool pSelect = false);
	void reset();
	double getSize() { return cBaseSize; }
	void setAspectRatio( int pWidth, int pHeight);
	void chgModeKamera();
	bool isPerspectiveOn() { return cModeKamera == ModeKamera::MODE_PERPECTIVE;}

	void setPerspective( double pFovY, double pAspect, double pZnear, double pZfar )
	{
		cFovY = pFovY; cAspect = pAspect; cZnear = pZnear; cZfar = pZfar;
	}
	bool getProject( const Point3d& pPos, Point3d & pPtResult );
	bool projectObjectToWin( Point3d pPos, Point3d & pPtResult, bool pExec  );
	bool projectWinToObject( Point3d pPos, Point3d & pPtResult, bool pExec  );

};
//***************************************
}

#endif
