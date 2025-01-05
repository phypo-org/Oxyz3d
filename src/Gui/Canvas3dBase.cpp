#include "Canvas3dBase.h"

//#include <config.h>
#include <FL/Fl.H>

 
#include <FL/gl.h>

#include <FL/names.h>
#include <FL/Fl_Menu_Button.H>

#include <FL/fl_draw.H>

#include "Win3dBase.h"

#include "Shape/GLUtility.h"
#include "Shape/Light.h"


using namespace std;

namespace M3d {
   Canvas3dBase::Canvas3dBase(  Win3dBase & pW3d, int pX, int pY, int pW, int  pH, const char *l )
    :Fl_Gl_Window( pX, pY, pW, pH, l )
    ,cMyWin3d( pW3d )
    ,cScale(1.0)
    ,cUserMode( ModeUser::MODE_BASE )
    ,cAxisFlag( true )
    ,cFlagLightColor( false)
     //	 cGridMode( ModeGrid::GRID_3D )
    ,cGridMode( ModeGrid::GRID_2D )
     //   ,cDebug(false)	
  { 
    gl_font( FL_HELVETICA_BOLD, 24);
    cKamera.initGL();
    cKamera.raz45();



    mode( FL_RGB | FL_DOUBLE | FL_DEPTH | FL_STENCIL | FL_DOUBLE);
     

   
    cout << "************************************************************************"  << std::endl;
   cout << "******** CAN DO STENCIL: " << Fl_Gl_Window::can_do( FL_STENCIL )
	 << " " << hex <<  FL_STENCIL
	 << " " << Fl_Gl_Window::mode()
	 << " " <<  Fl_Gl_Window::mode()
	 << std::endl;
     cout << "************************************************************************"  << std::endl;
   cout << "************************************************************************"  << std::endl;
    
  }

  //---------------------------
  Canvas3dBase::~Canvas3dBase( )
  {
    delete &cMyWin3d;
  }

    

  //---------------------------
  //Draw three grids
  
  void Canvas3dBase::drawGrid()
  {
    // Echelle 1 -> 1mm correspond au petit carreau de 1mm
    // position 12 dans le tableau des unité de la caméra
		
    float lScale = 1.0/cKamera.scale().x();
	
    double lSz           = 2E-10;  // taille minimale en millimetre de la grille de chaque coté de l'axe
	
    long lPosScale =  (long)(log10(lScale*2.5)+12);  // On recupere l'echelle par rapport a 1E-6 nanometre (2.5 arbitraire, à changer pour l'effet de seuil)
    lSz  *= pow(10,lPosScale);      // on modifie la taille de la grille en fonction de l'echelle 

    std::string lMesurement= "";
    //    double lDivision = 0;
    getMyWin().setMeasurement( lScale, lPosScale );

    //    cout << "SCALE:" << cKamera.scale().x() << " -> " << lScale << " ==> " << lPosScale << " " << cKamera.GetMeasurement( lPosScale ) << endl;

    // On crée 3 grille Allant de 10 en 10
    if( cGridMode == ModeGrid::GRID_2D )
      {
	double lMul   = 10.0;

	// Couleur du tracé
	float col1 = 0.4f;
	float col2 = 0.6f;
	float col3 = 0.8f;

	// Largeur du tracé
	float sz1 = 0.05;
	float sz2 = 1.5;
	float sz3 = 2;

	int lNbDiv = 200;
	//	int lNbDiv = 10;   // debug
	//	lSz       /= 10;   // debug


	// on commence par la grille la plus fine 
	glColor4f( col1, col1, col1, col1 );
	PP3d::GLUtility::DrawGrid(  lSz,           lNbDiv, sz1 );

	glColor4f( col2, col2, col2, col2 );
	PP3d::GLUtility::DrawGrid(  lSz*lMul,      lNbDiv, sz2 );
	
	glColor4f( col3, col3, col3, col3 );
	PP3d::GLUtility::DrawGrid(  lSz*lMul*lMul, lNbDiv, sz3 );
	//	lDivision = lSz / lNbDiv;      

	
	/* RECADRAGE ?????
	   double lX=0, lZ=0;
	   double lPx = 0; //pixel_w()/2;
	   double lPz = 0; //pixel_h()/2;
	   PP3d::Point3d lPt = transform2Dto3D( lPx, lPz );
	
	   cout<< "lPx:=" << lPx << " lPz:=" <<lPz << " -> " << lPt << endl;
	
	   glColor4f( 0.8, 0.1, 0.1, 0.3 );
	   PP3d::GLUtility::DrawGrid( lX, lZ, lSz,           lNbDiv, sz1 );
	
	   glColor4f( col1, col2, col2, col2 );
	   PP3d::GLUtility::DrawGrid( lX, lZ, lSz*lMul,      lNbDiv, sz2 );
	
	   glColor4f( col1, col3, col3, col3 );
	   PP3d::GLUtility::DrawGrid( lX, lZ, lSz*lMul*lMul, lNbDiv, sz3 );
	   lDivision = lSz / lNbDiv;      
	*/
      }	
  }
  //---------------------------
  void Canvas3dBase::beginDraw()
  {
    glViewport(0,0,pixel_w(),pixel_h());		
    cKamera.setAspectRatio( pixel_w(), pixel_h());
    cKamera.initGL();
    
    //==========  On place les lumieres dans le monde, pas avec la camera =============
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
		
    PP3d::Light::DisableAll();
    PP3d::Light::AllGL();
    PP3d::Light::RazAll();


    // Transparence
    glEnable(GL_BLEND) ;
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;  
    // Transparence
    //    glBlendFunc( GL_SRC_ALPHA, GL_ONE );

 
    if( cFlagLightColor )
      {		
	PP3d::Light::RainbowOn();
      }
    else
      {
	PP3d::Light::GreyOn();
      }
	
    PP3d::Light::AllGL();
    glEnable(GL_LIGHTING);
	
 

    static float lModelAmbient[] {0.2f, 0.2f, 0.2f, 1 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lModelAmbient );
    //===================================================================================

    glClearDepth(1.0f);   
    glClearColor( 0.3f, 0.3f, 0.3f, 1 );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    cKamera.execGL();
    
 
  }
  //---------------------------
  void Canvas3dBase::endDraw()
  { 
    glFlush();
  }
  //---------------------------
  void Canvas3dBase::drawUtils()
  {
    drawGrid();
    if( cAxisFlag )
      PP3d::GLUtility::DrawOrtho( (float)8/cKamera.scale().x(), 1, 3 );         
  }
  
  //---------------------------
  void Canvas3dBase::draw()
  { 
    beginDraw();
    
    drawUtils();

    endDraw();
  }
  //---------------------------

} // namespace
