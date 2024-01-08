
#include "Canvas3d.h"

//#include <config.h>
#include <FL/Fl.H>

 
#include <FL/gl.h>

#include <FL/names.h>
#include <FL/Fl_Menu_Button.H>

#include <FL/fl_draw.H>

#include "Service.h"


#include <iostream>
#include <memory>


#include "Shape/ObjectLine.h"
#include "Shape/ObjectPoly.h"
#include "Shape/GLUtility.h"
#include "Shape/PrimitivFactory.h"
#include "Shape/Light.h"
#include "Shape/ViewProps.h"
#include "Shape/Selection.h"
#include "Shape/SortVisitor.h"
#include "Shape/PP3dUtils.h"
#include "Shape/MinMaxBox3d.h"
#include "Shape/Line3dVect.h"

#include "Application.h"
#include "MyFlWidget.h"

#include "Dialogs.h"
#include "Win3d.h"

#include "Preference.h"
#include "Callback.h"


#include "SelFunct.h"


using namespace std;



namespace M3d {

  extern  bool sDrawColorSelect;
 

 //---------------------------
  void Canvas3d::drawSelectRect()  // 2D !
  {

    std::cout << "drawSelectRect" << std::endl;
    
    glMatrixMode (GL_PROJECTION); // Tell opengl that we are doing project matrix work
    glLoadIdentity(); // Clear the matrix
        
    int lW = pixel_w();
    int lH = pixel_h();

    glOrtho( 0, lW, 0, lH, -100.0, 100.0); // Setup an Ortho view
    glViewport(0, 0, lW, lH);

    glMatrixMode(GL_MODELVIEW); // Tell opengl that we are doing model matrix work. (drawing)
    glLoadIdentity(); // Clear the model matrix
    //    glEnable( GL_BLEND );
    glDisable(GL_DEPTH_TEST);
    
    glDepthFunc(GL_LESS); 


    glDepthMask( GL_FALSE );
    //    glBlendFunc( GL_SRC_ALPHA, GL_ONE );
				
    glColor4f(0.9f, 0.1f, 0.1f, 0.1f);
    glDisable( GL_LIGHTING );


    PP3d::Point3d lMin( cMouseInitPosX, lH-cMouseInitPosY, 0 );
    PP3d::Point3d lMax( cMouseLastPosX, lH- cMouseLastPosY, 0);

    //    cout << "min:" << lMin << " max:" << lMax << endl;
       
    PP3d::Rect3d lRect(lMin, lMax);
    lRect.drawGL();

    glColor4f(1.0f, 0.2f, 0.2f, 0.2f);
    glBegin( GL_LINES );
       

    glLineWidth( 1 );
    // Diagonale
    //       glVertex3dv( lMin.vectForGL() ); 
    //       glVertex3dv( lMax.vectForGL() );

    glVertex3f( (float)lMin.cX, (float)lMin.cY, 0 );
    glVertex3f( (float)lMin.cX, (float)lMax.cY, 0 );
       
    glVertex3f( (float)lMin.cX, (float)lMin.cY, 0 );
    glVertex3f( (float)lMax.cX, (float)lMin.cY, 0 );

       
    glVertex3f( (float)lMax.cX, (float)lMax.cY, 0);
    glVertex3f( (float)lMin.cX, (float)lMax.cY, 0);
  
    glVertex3f( (float)lMax.cX, (float)lMax.cY, 0);
    glVertex3f( (float)lMax.cX, (float)lMin.cY, 0);      
	
    glEnd();
    glEnable(GL_LIGHTING);
      
    glDepthMask( GL_TRUE );
    glDisable( GL_BLEND );             
  }
  

  //--------------------------- 
  void Canvas3d::drawSelectCircle(  PP3d::Point3d iPos, int iSz, bool iUseAlternColor )  // 2D !
  {   
    glMatrixMode (GL_PROJECTION); // Tell opengl that we are doing project matrix work
    glLoadIdentity(); // Clear the matrix
        
    int lW = pixel_w();
    int lH = pixel_h();

    glOrtho( 0, lW, 0, lH, -100.0, 100.0); // Setup an Ortho view
    glViewport(0, 0, lW, lH);

    glMatrixMode(GL_MODELVIEW); // Tell opengl that we are doing model matrix work. (drawing)
    glLoadIdentity(); // Clear the model matrix
    //    glEnable( GL_BLEND );
    glDisable(GL_DEPTH_TEST);
    
    glDepthFunc(GL_LESS); 


    glDepthMask( GL_FALSE );
    //    glBlendFunc( GL_SRC_ALPHA, GL_ONE );
				
    glColor4f(0.4f, 0.6f, (iUseAlternColor?0.5f:1.0f), 0.2f);
    glDisable( GL_LIGHTING );


    PP3d::Point3d lCenter( iPos );

    PP3d::Point3d lSz( iSz, 0, 0);

    //    cout << "min:" << lMin << " max:" << lMax << endl;
   

    glLineWidth( 2 );
    glBegin(GL_POLYGON);

    // Diagonale
    //       glVertex3dv( lMin.vectForGL() ); 
    //       glVertex3dv( lMax.vectForGL() );
    int lNb = 128;
    double lStepAngle = 2*M_PI/lNb;
    double lAngle = 0;
    for( int i=0; i< lNb; i++ )
      {
        lAngle += lStepAngle;
        double x = cos( lAngle )*lSz.cX+lCenter.cX;
        double y = sin( lAngle )*lSz.cX+lCenter.cY;
        
        glVertex3f( (float)x, (float)y, 0 );
      }
    glEnd();
    
 
    glColor4f(0.2f, 0.5f, (iUseAlternColor?0.5f:1.0f), 0.2f);

    glLineWidth( 2 );
    glBegin(GL_POLYGON);

    // Diagonale
    //       glVertex3dv( lMin.vectForGL() ); 
    //       glVertex3dv( lMax.vectForGL() );
    lSz -=5;
    lAngle = 0;
    for( int i=0; i< lNb; i++ )
      {
        lAngle += lStepAngle;
        double x = cos( lAngle )*lSz.cX+lCenter.cX;
        double y = sin( lAngle )*lSz.cX+lCenter.cY;
        
        glVertex3f( (float)x, (float)y, 0 );
      }
    glEnd();

    
    glEnable(GL_LIGHTING);
      
    glDepthMask( GL_TRUE );
    //    glDisable( GL_BLEND );     
  }
  

  //---------------------------
  //Draw three grids
  
  void Canvas3d::drawGrid()
  {
    // Echelle 1 -> 1mm correspond au petit carreau de 1mm
    // position 12 dans le tableau des unité de la caméra
		
    float lScale = 1.0/cKamera.scale().x();
	
    double lSz           = 2E-10;  // taille minimale en millimetre de la grille de chaque coté de l'axe
	
    long lPosScale =  (long)(log10(lScale*2.5)+12);  // On recupere l'echelle par rapport a 1E-6 nanometre (2.5 arbitraire, à changer pour l'effet de seuil)
    lSz  *= exp10(lPosScale);      // on modifie la taille de la grille en fonction de l'echelle 

    std::string lMesurement= "";
    //    double lDivision = 0;
    cMyWin3d.setMeasurement( lScale, lPosScale );

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
  void Canvas3d::draw() 
  {
    //   cout << "        " << this << " draw " << endl;
    if( sDrawColorSelect  )
      {
	drawForSelect();
	return ;
      }
    
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

    PP3d::ViewProps * lTmpViewInputPolyObject = &cViewInputObject;
    
    if( getGlobalMode() == GlobalMode::MAGNET )
      //        && ( getUserMode() == ModeUser::MODE_DRAG || getUserMode() == ModeUser::MODE_TRANSFORM) )

      {
        lTmpViewInputPolyObject =   & cViewInputObjectMagnet;
      }
    
    //===========================================================	
    drawGrid();

    if( cAxisFlag )
      PP3d::GLUtility::DrawOrtho( (float)8/cKamera.scale().x(), 1, 3 );  
		
		
    if( cFlagCursor3d )
      {
	PP3d::GLUtility::DrawCursorCruz2( TheAppli.getDatabase()->getCursorPosition(), 50);
      }

		
    //  cViewProps.cDebug = cDebug;
    cViewGen.cFlagViewNormal = cFlagViewNormal;
 

    // Draw the 3d view of object
    ///////////    TheAppli.getDatabase()->recomputeAll( );      // AFAIRE : optimiser c'est couteux de le faire a chaque fois
    
    TheAppli.getDatabase()->drawGL( cViewGen, cViewInputCursor, cViewInputPoly,  * lTmpViewInputPolyObject,  PP3d::GLMode::Draw  , TheSelect.getSelectType(), PP3d::ClassTypeObj  );


    // Draw transformation
    if( cFlagViewGeo )
      {
	////////	TheAppli.getDatabaseTransform()->recomputeAll();
	TheAppli.getDatabase()->drawGL( cViewPropsTransform, cViewInputCursor, cViewInputPoly , cViewInputObject, PP3d::GLMode::Draw, TheSelect.getSelectType(),  PP3d::ClassTypeGeo ); 
      }
    
    // 
    if( TheAppli.getDatabaseTmp() != nullptr )
      {
	////////	TheAppli.getDatabaseTransform()->recomputeAll();
	TheAppli.getDatabaseTmp()->drawGL( cViewPropsTransform, cViewInputCursor,  cViewInputPoly, cViewInputObject, PP3d::GLMode::Draw, TheSelect.getSelectType(), PP3d::ClassTypeObj); 
        if( cFlagViewGeo )
	TheAppli.getDatabaseTmp()->drawGL( cViewPropsTransform, cViewInputCursor,  cViewInputPoly, cViewInputObject, PP3d::GLMode::Draw, TheSelect.getSelectType(), PP3d::ClassTypeGeo); 
      }

    // draw rectangle selection if needed
    if( getUserMode() == ModeUser::MODE_SELECT_RECT )
      {	
	drawSelectRect();
      }
    /*
     // draw magnet
    if( getGlobalMode() == GlobalMode::MAGNET
      && ( getUserMode() == ModeUser::MODE_DRAG || getUserMode() == ModeUser::MODE_TRANSFORM) )
      {
	drawMagnet( cMagnet );
      }
    */
  
    glFlush();    
  }  



  //---------------------------
  void Canvas3d::drawForSelect()  // AJOUTER LA TAILLE
  {    
    glViewport(0,0,pixel_w(),pixel_h());            //A CHANGER POUR PICKING MOUSE, METTRE LA TAILLE		
    cKamera.setAspectRatio( pixel_w(), pixel_h());
    cKamera.initGL();	 // CREER un initSelectGL

    
    glShadeModel(GL_FLAT); //******

    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
		
    PP3d::Light::DisableAll();    
    PP3d::Light::AllGL();
    PP3d::Light::RazAll();
	
    glDisable(GL_LIGHTING); 
    //    glEnable(GL_LIGHTING); 
    //===================================================================================

    glClearDepth(1.0f);

    glClearColor( 0, 0, 0, 1 );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
 

    cKamera.execGL();
		       
    
    //===========================================================	
		
    //  cViewProps.cDebug = cDebug;
    cViewGen.cFlagViewNormal = cFlagViewNormal;
 

    // Draw the 3d view of object
    if(  getSelectGeo())  // pour les transformations
      {
	////////
	TheAppli.getDatabase()->recomputeAll(PP3d::Compute::Nothing);
	TheAppli.getDatabase()->drawGL( cViewPropsTransform, cViewPropsTransform, cViewPropsTransform, cViewPropsTransform, PP3d::GLMode::Select, TheSelect.getSelectType(), PP3d::ClassTypeGeo); 
      }
       else
      {
	TheAppli.getDatabase()->recomputeAll(PP3d::Compute::Nothing);     
	TheAppli.getDatabase()->drawGL( cViewGen, cViewGen, cViewGen, cViewGen, PP3d::GLMode::Select, TheSelect.getSelectType(), PP3d::ClassTypeObj  );
      }
    
    glFlush();
    //glFinish();  

    //Faire ce qu'il faut pour trouver la selection
  }  

} //namespace 
