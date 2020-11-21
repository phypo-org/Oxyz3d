#include "Kamera.h"

#include "MinMaxBox3d.h"
#define __GNU_SOURCE
#include <math.h>

namespace PP3d{

  //***************************************

  Kamera::Kamera( double pSize  )
    :cModeKamera( ModeKamera::MODE_ORTHO )
    ,cBaseSize( pSize)
    ,cRatioWH(1.0)
  {
    reset();
  }
  //------------------------------------------------
  void Kamera::setAspectRatio( int pWidth, int pHeight)
  {
    cRatioWH = ((double)pWidth)/((double)pHeight);
  }
  //------------------------------------------------
  void Kamera::reset()
  {
    // Znear et Zfar doivent etre > 0 !
    raz();
    cFovY   =  20.0 ;
    cAspect = cRatioWH ; 
    cZnear  = 0.01 ;        // Mettre dans Application::Pparametres !
    cZfar   = 1000;
 
    // pour Viewer
    cPosition[0]= 0;	
    cPosition[1]= 0;
    cPosition[2]= 60;//cBaseSize;

    
    cPosition2.raz();
    
    //cScale.set( 0.1, 0.1, 0.1);
  }
  //------------------------------------------------
  void Kamera::initGL()
  {	
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable( GL_DEPTH_TEST );
    glDepthFunc(GL_LESS); // PHIPO 20201114 ???
		
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		
    glFrontFace( GL_CCW );    // facette visible si contre horaire
    glDisable( GL_CULL_FACE );


    //	glEnable( GL_TEXTURE_2D );
	
    //	glEnable( GL_AUTO_NORMAL);
    //	glEnable( GL_NORMALIZE );

    if( cSmooth )		
      {
	glShadeModel( GL_SMOOTH );
	glEnable( GL_POINT_SMOOTH );
	glEnable( GL_LINE_SMOOTH);
	glEnable( GL_POLYGON_SMOOTH );
      }     
  }	
  //------------------------------------------------
  void Kamera::execGL( bool pSelect )
  {
    if( ! pSelect )
      {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
      }
	
	
    if( cModeKamera == ModeKamera::MODE_PERPECTIVE )
      {
	cAspect =  cRatioWH;
	gluPerspective( cFovY, cAspect, cZnear, cZfar );
      }
    else
      {
	//			if( cRatioWH >1. )
	glOrtho( -cRatioWH*cBaseSize, cRatioWH*cBaseSize, -cBaseSize, cBaseSize, -cBaseSize*10, cBaseSize*10);
	//			else
	//				glOrtho( -cBaseSize, cBaseSize,  -cBaseSize*cRatioWH, cBaseSize/cRatioWH,  -cBaseSize*100, cBaseSize*100 );
 
	//KameraDouble::get(PERS_ZNEAR), KameraDouble::get(PERS_ZFAR));
      }
	
    // Inversion car c est l utilisateur 
    glTranslated( -cPosition[ 0 ], -cPosition[1], -cPosition[2] );
		
    glRotated( -cAngle[0], 1.0, 0.0, 0.0 );
    glRotated( -cAngle[1], 0.0, 1.0, 0.0 );
    glRotated( -cAngle[2], 0.0, 0.0, 1.0 );

    //glLoadMatrixd( Selection::Instance().getCurrentMatrice().vectForGL() );

    // ATTENTION POUR VIEW SEUL SCALEX EST UTILISE !!!!!!!
    glScaled( cScale[0], cScale[0], cScale[0] );
	
    glTranslated( -cPosition2[ 0 ], -cPosition2[1], -cPosition2[2] );
    
    if( ! pSelect )
      {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
      }
    
    position()[2]  =60;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  }
  //------------------------------------------------
  void Kamera::chgModeKamera()
  {
    if( cModeKamera == ModeKamera::MODE_PERPECTIVE )
      cModeKamera = ModeKamera::MODE_ORTHO;
    else
      cModeKamera = ModeKamera::MODE_PERPECTIVE;
  }
  //------------------------------------------------

  bool
  Kamera::projectObjectToWin( Point3d pPos, Point3d & pPtResult, bool pExec )
  {
    GLint lViewport[ 4 ];
    GLdouble lMvMatrix[ 16 ];
    GLdouble lProjMatix[ 16 ];

    if( pExec ) execGL();



    glGetIntegerv( GL_VIEWPORT, lViewport );

    //	std::cout <<"VIEWPORT:" << lViewport[0]<< " " <<  lViewport[1]<<  " " << lViewport[2]<< " " <<  lViewport[3]  << std::endl;
    glGetDoublev( GL_MODELVIEW_MATRIX, lMvMatrix ); 
    glGetDoublev( GL_PROJECTION_MATRIX, lProjMatix );

	
    if( gluProject( pPos.cX, pPos.cY, pPos.cZ,
		    lMvMatrix, lProjMatix, lViewport,
		    &pPtResult.cX, &pPtResult.cY, &pPtResult.cZ ) == GL_FALSE )
      return false;


    return true;
  }
  //------------------------------------------------
  bool
  Kamera::projectWinToObject( Point3d pPos, Point3d & pPtResult, bool pExec  )
  {
    GLint lViewport[ 4 ];
    GLdouble lMvMatrix[ 16 ];
    GLdouble lProjMatix[ 16 ];

    if( pExec )	execGL();

    glGetIntegerv( GL_VIEWPORT, lViewport );

    //	std::cout <<"VIEWPORT:" << lViewport[0]<< " " <<  lViewport[1]<<  " " << lViewport[2]<< " " <<  lViewport[3]  << std::endl;

    glGetDoublev( GL_MODELVIEW_MATRIX, lMvMatrix );
    glGetDoublev( GL_PROJECTION_MATRIX, lProjMatix );

	
	
    if( gluUnProject(  pPos.cX, pPos.cY, pPos.cZ,
		       lMvMatrix, lProjMatix, lViewport,
		       &pPtResult.cX, &pPtResult.cY, &pPtResult.cZ ) == GL_FALSE )
      return false;



    return true;

  }
  //------------------------------------------------
  void
  Kamera::zoomTo( Rect3d & iMMBox )
  {
    std::cout << "MinMaxBox=" << iMMBox <<  std::endl;
    Point3d lCenter = iMMBox.center();

     
    std::cout << "actuel:" << position() << " -> " << lCenter<< std::endl;
    position2().x() = lCenter.x();
    position2().y() = lCenter.y();
    position2().z() = lCenter.z();
    
    position().x() = 0;
    position().y() = 0;
    position().z() = 0;
      
    //    scaleTo(  iMMBox, 1 );   
  }
  //------------------------------------------------
  void Kamera::setScale( double iScale )
  {
    // 43 niveau comme le tableau
    // scale = 1, soit 1 mm est la valeur par default
    
    if(iScale  < 1E-30 ) 
      iScale   = 1E-30;
    else
      if( iScale > 1E13 )
	iScale   = 1E13;


    scaleTo(iScale);
  }
  //------------------------------------------------  
   void Kamera::setScaleToMeasurement( int iPosScale )
   {
     double lScale = exp10(iPosScale-12)/2.5;
     lScale = 1.0/lScale;
     setScale( lScale );
   }
  //------------------------------------------------
  #define lTabMesurementSize 43
  static  const char *lTabMesurement[lTabMesurementSize]={
    "1E-6 nm",   
    "1E-5 nm",   
    "1E-4 nm",   
    "1E-3 nm",    
    "1E-2 nm",    
    "1E-1 nm",  // atom size
    "1 nm",         
    "10 nm",
    "100 nm",
    "1 μm",
    "10 μm",
    "100 μm ", 
    "1 mm",                // 1
    "1 cm",
    "1 dm",
    "1 m",
    "1 dam",
    "1 hm",
    "1 km",
    "10 km",
    "100 km",
    "1000 km",
    "10.000 km",
    "100.000 km",
    "1 E6 km",
    "1 E7 km",
    "1 E8 km",
    "1 E9 km",
    "1 E10 km",
    "1 E11 km",
    "1 E12 km",
    "1 E13 km",
    "1 E14 km",
    "1 E15 km",
    "1 E16 km",
    "1 E17 km",
    "1 E18 km",
    "1 E19 km",
    "1 E20 km",
    "1 E21 km",
    "1 E22 km",
    "1 E23 km",
    "1 E24 km"  // more or less the size of the univers
  };
  const char *Kamera::GetMeasurement( int iPosScale )
  {
    static char sStr[33];    
    
    if( iPosScale < 0 || iPosScale >= lTabMesurementSize )     
      {
	// BUG for the size, if several windows at the same time !!!!!
	
	sprintf( sStr, "Off-limits - 1E%d m", (iPosScale -15));
	return sStr;
      }
       
    return lTabMesurement[iPosScale] ;
  }
  int Kamera::GetNbMeasurement() { return lTabMesurementSize; } // return the total number of unity
  int Kamera::GetDefaultMeasurement() { return 12; } // 1mm A mettre dans les preferences 

  /*
  //------------------------------------------------

  void 
  O3dKamera::setPick(  int pX, int pY, float pWidht, float pHeight )
  {
  // Code for picking
  GLint lViewport[ 4 ];
  glGetIntegerv( GL_VIEWPORT, lViewport );


  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Code for picking
  gluPickMatrix( (GLdouble)pX, 
  (lViewport[ 3 ]-pY - 1), pWidht, pHeight, lViewport );



  if( cPersFlag == GL_TRUE )
  {
  gluPerspective( KameraDouble::get( PERS_FOVY), KameraDouble::get(PERS_ASPECT), KameraDouble::get(PERS_ZNEAR), KameraDouble::get(PERS_ZFAR) );
  }
  else
  glOrtho( -1000,  + 1000, -1000,  +1000, -100000, 100000 );//KameraDouble::get(PERS_ZNEAR), KameraDouble::get(PERS_ZFAR));



  // Inversion car c est l utilisteur 
  glTranslated( -T3dTransf::get(POS)[ 0 ], -T3dTransf::get(POS)[1], -T3dTransf::get(POS)[2] );

  glRotated( -T3dTransf::get(ANGLE)[0], 1.0, 0.0, 0.0 );
  glRotated( -T3dTransf::get(ANGLE)[1], 0.0, 1.0, 0.0 );
  glRotated( -T3dTransf::get(ANGLE)[2], 0.0, 0.0, 1.0 );

  // ATTENTION POUR VIEW SEUL SCALEX EST UTILISE !!!!!!!
  glScaled( T3dTransf::get(SCALE)[0], T3dTransf::get(SCALE)[0], T3dTransf::get(SCALE)[0] );


  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();  	
  }
  //------------------------------------------------
  */


}
