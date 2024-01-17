
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

#include "Creation.h"
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
  void Canvas3d::draw() 
  {
    //   cout << "        " << this << " draw " << endl;
    if( sDrawColorSelect  )
      {
	drawForSelect();
	return ;
      }
    
    beginDraw();
   
    
    PP3d::ViewProps * lTmpViewInputPolyObject = &cViewInputObject;
    
    if( getGlobalMode() == GlobalMode::MAGNET )
      //        && ( getUserMode() == ModeUser::MODE_DRAG || getUserMode() == ModeUser::MODE_TRANSFORM) )

      {
        lTmpViewInputPolyObject =   & cViewInputObjectMagnet;
      }
    
    //===========================================================	
    drawUtils();  //Grid, cursor ...

    if( cFlagCursor3d )
      {
	PP3d::GLUtility::DrawCursorCruz2( TheCreat.getDatabase()->getCursorPosition(), 50);
      }
		
    //  cViewProps.cDebug = cDebug;
    cViewGen.cFlagViewNormal = cFlagViewNormal;
 

    // Draw the 3d view of object
    ///////////    TheCreat.getDatabase()->recomputeAll( );      // AFAIRE : optimiser c'est couteux de le faire a chaque fois
    
    TheCreat.getDatabase()->drawGL( cViewGen, cViewInputCursor, cViewInputPoly,  * lTmpViewInputPolyObject,  PP3d::GLMode::Draw  , TheSelect.getSelectType(), PP3d::ClassTypeObj  );


    // Draw transformation
    if( cFlagViewGeo )
      {
	////////	TheCreat.getDatabaseTransform()->recomputeAll();
	TheCreat.getDatabase()->drawGL( cViewPropsTransform, cViewInputCursor, cViewInputPoly , cViewInputObject, PP3d::GLMode::Draw, TheSelect.getSelectType(),  PP3d::ClassTypeGeo ); 
      }
    
    // 
    if( TheCreat.getDatabaseTmp() != nullptr )
      {
	////////	TheCreat.getDatabaseTransform()->recomputeAll();
	TheCreat.getDatabaseTmp()->drawGL( cViewPropsTransform, cViewInputCursor,  cViewInputPoly, cViewInputObject, PP3d::GLMode::Draw, TheSelect.getSelectType(), PP3d::ClassTypeObj); 
        if( cFlagViewGeo )
	TheCreat.getDatabaseTmp()->drawGL( cViewPropsTransform, cViewInputCursor,  cViewInputPoly, cViewInputObject, PP3d::GLMode::Draw, TheSelect.getSelectType(), PP3d::ClassTypeGeo); 
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
    endDraw();
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
	TheCreat.getDatabase()->recomputeAll(PP3d::Compute::Nothing);
	TheCreat.getDatabase()->drawGL( cViewPropsTransform, cViewPropsTransform, cViewPropsTransform, cViewPropsTransform, PP3d::GLMode::Select, TheSelect.getSelectType(), PP3d::ClassTypeGeo); 
      }
       else
      {
	TheCreat.getDatabase()->recomputeAll(PP3d::Compute::Nothing);     
	TheCreat.getDatabase()->drawGL( cViewGen, cViewGen, cViewGen, cViewGen, PP3d::GLMode::Select, TheSelect.getSelectType(), PP3d::ClassTypeObj  );
      }
    
    glFlush();
    //glFinish();  

    //Faire ce qu'il faut pour trouver la selection
  }  

} //namespace 
