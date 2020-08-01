#include "Kamera.h"

#include "MinMaxBox3d.h"

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
    cFovY   =  100.0 ;
    cAspect = cRatioWH ; 
    cZnear  = 0.01 ;        // Mettre dans Application::Pparametres !
    cZfar   = 1000;
 
    // pour Viewer
    cPosition[0]= 0;
	
    cPosition[1]= 0;
    cPosition[2]= 20;//cBaseSize;

    
    cPosition2.raz();
    
    //cScale.set( 0.1, 0.1, 0.1);
  }
  //------------------------------------------------
  void Kamera::initGL()
  {	
    glClearColor( 0.3f, 0.3f, 0.3f, 1 );
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable( GL_DEPTH_TEST );
		
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		
    glFrontFace( GL_CCW );    // facette visible si contre horaire
    glDisable( GL_CULL_FACE );

			
    if( cSmooth )
      {
	glShadeModel( GL_SMOOTH); // blends colors nicely, and smoothes out lighting
      }

    //	   glShadeModel(GL_FLAT);
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
