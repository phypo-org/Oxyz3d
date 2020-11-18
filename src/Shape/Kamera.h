#ifndef H__Kamera__H
#define H__Kamera__H


#include "Transf3d.h"

#include "Point3d.h"

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
    Double3 cPosition2;

  public:
    Kamera( double pSize = 10 ); //1000
    void initGL();
    void execGL( bool pSelect = false);
    void reset();
    double getSize() { return cBaseSize; }
    void setAspectRatio( int pWidth, int pHeight);
    void chgModeKamera();
    bool isPerspectiveOn() { return cModeKamera == ModeKamera::MODE_PERPECTIVE;}

    void orthoForRect(){ 	glOrtho( -cRatioWH*cBaseSize, cRatioWH*cBaseSize, -cBaseSize, cBaseSize, -cBaseSize*10, cBaseSize*10); }


    void setPerspective( double pFovY, double pAspect, double pZnear, double pZfar )
    {
      cFovY = pFovY; cAspect = pAspect; cZnear = pZnear; cZfar = pZfar;
    }
    bool getProject( const Point3d& pPos, Point3d & pPtResult );
    bool projectObjectToWin( Point3d pPos, Point3d & pPtResult, bool pExec  );
    bool projectWinToObject( Point3d pPos, Point3d & pPtResult, bool pExec  );
  
    void zoomTo( Rect3d & iMMBox );
    void setPosition2( Double3 & iPos )
    {
      for( int i= 0; i < 3; i++ )
	cPosition2[i] = iPos[i]; 
    }
    Double3& position2() { return cPosition2;}
    
    void setScale( double iScale );


    void setScaleToMeasurement( int iPosScale );
    static const char *GetMeasurement( int iPosScale ); // return the univeral unity 
    static int         GetNbMeasurement( ); // return the total number of unity
    static int         GetDefaultMeasurement();
  };
  //***************************************
}

#endif
