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


#include "Application.h"
#include "MyFlWidget.h"

#include "Dialogs.h"
#include "Win3d.h"

#include "Preference.h"

using namespace std;


namespace M3d {
  
  const char * const ANNULE_ACTION="e";
  
  const char * const CHG_AXIS="a";
  
  const char * const CHG_GRID="g";
  
  const char * const CHG_ORTHO_PERS="p";
  
  
  const char * const RESET_VIEW_TO_X="x";
  const char * const RESET_VIEW_TO_Y="y";
  const char * const RESET_VIEW_TO_Z="z";
  const char * const RESET_VIEW_TO_X2="X";
  const char * const RESET_VIEW_TO_Y2="Y";
  const char * const RESET_VIEW_TO_Z2="Z";
  

  const char * const RESET_VIEW_SCALE_ORIGIN="O";
  
  const char * const RESET_VIEW="O";
  const char * const RESET_VIEW_SCALE_0="0";
  const char * const RESET_VIEW_SCALE_1="1";
  const char * const RESET_VIEW_SCALE_2="2";
  const char * const RESET_VIEW_SCALE_3="3";
  const char * const RESET_VIEW_SCALE_4="4";
  const char * const RESET_VIEW_SCALE_5="5";
  const char * const RESET_VIEW_SCALE_6="6";

  
  const char * const STR_CURSOR_3D="c";
  const char * const STR_EXIT="q";

  const char * const MOVE_Z_N="-";
  const char * const MOVE_Z_P="+";
	
  const char * const UNSELECT_ALL=" ";

  //***************************************
  Canvas3d::Canvas3d(  Win3d& pW3d, int pX, int pY, int pW, int  pH, const char *l )
    :Fl_Gl_Window( pX, pY, pW, pH, l )
    ,cMyWin3d(pW3d)
    ,cScale(1.0)
    ,cMode( ModeUser::MODE_BASE )
    ,cAxisFlag( true )
    ,cFlagLightColor( false)
     //	 cGridMode( ModeGrid::GRID_3D )
    ,cGridMode( ModeGrid::GRID_2D )
     //   ,cDebug(false)	
  { 
    gl_font( FL_HELVETICA_BOLD, 24);
    cKamera.initGL();
    cKamera.raz45();

    cViewPropsTransform.cLineWidth = 2;
    cViewPropsTransform.cColorLine.set(1.0,0.0,0.0);
	 

  }
  //---------------------------
  Canvas3d::~Canvas3d( )
  {
	
  }
  //---------------------------
  void Canvas3d::traceMode() const
  {
    if( MyPref.cDbgEvt )
      {
	switch( cMode )
	  {
	  case ModeUser::MODE_BASE:
	    std::cout << "MODE_BASE  ";
	    break;
	  case ModeUser::MODE_SELECT:
	    std::cout << "MODE_SELECT  ";
	    break;
	  case ModeUser::MODE_SELECT_RECT:
	    std::cout << "MODE_SELECT_RECT  ";
	    break;
	  case ModeUser::MODE_MOVE_CAMERA:
	    std::cout << "MODE_MOVE_CAMERA  ";
	    break;
	  case ModeUser::MODE_TRANSFORM :
	    std::cout << "MODE_TRANSFORM  " ;
	    break;
	  }
      }
  }

  
  //---------------------------
  void Canvas3d::drawRect(int x1, int y1, int x2, int y2)
  {
    std::cout << "____________________________" << std::endl;

    
    
    glPushMatrix(); 
    glLoadIdentity();
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
 
    glDrawBuffer(GL_FRONT );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
   //	glOrtho(0, glw.getWidth(),0, glw.getHeight(),-1,1);
    // cKamera.orthoForRect();
    
    //    glMatrixMode(GL_MODELVIEW);
    
    //  glEnable(GL_COLOR_LOGIC_OP);
    //  glLogicOp(GL_XOR);

    float dx = pixel_w()/2.0;
    float dy = pixel_h()/2.0;
    std::cout << "dx:" << dx << " dy:" << dy;
    
    glColor4f(0.3f, 0.3f, 0.3f, 0.3f);
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //  glRectf(x1, y1, x2, y2);

    float X1 = x1 - dx;
    float X2 = x2 - dx;
    float Y1 = y1 - dy;
    float Y2 = y2 - dy;
    std::cout << " X1:" << X1 << " Y1:" << Y1 << " X2:" << X2 << " Y2:" << Y2 << std::endl;
    
    glColor4f( 1, 1, 1, 0.3);
    glBegin(GL_LINE_LOOP);
    // Top left
    glVertex3f( X1, Y1, 0.0);
    // top right
    glVertex3f( X2,  Y1, 0.0);
    // Bottom right
    glVertex3f( X2,  Y2,  0.0);
    // bottom left
    glVertex3f( X1, Y2,  0.0);
    
    glVertex3f( X1, Y1, 0.0);
    glVertex3f( X2, Y2, 0.0);
    glVertex3f( X2, Y1, 0.0);
    glVertex3f( X1, y2, 0.0);

    glEnd();
		
    /*
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    */
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);    
    glEnable(GL_LIGHTING);
    //  glDisable(GL_COLOR_LOGIC_OP);
   //    glFlush();
    /*
    //   glDrawBuffer(GL_BACK);
    */
  }
 
  //---------------------------
  void Canvas3d::draw() 
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
    glDepthFunc(GL_LEQUAL);
    glEnable( GL_DEPTH_TEST );


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    cKamera.execGL();

    
    getKamera().position()[2]  =60;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //===========================================================
    glLineWidth( 1 );
	
    if( cGridMode == ModeGrid::GRID_2D )
      {
	glColor4f( 0.4f, 0.4f, 0.4f, 0.5f );
	PP3d::GLUtility::DrawGrid( (float)cKamera.getSize()*10, 5, false);	 
	//	if( cGridMode == ModeGrid::GRID_3D )
	//				T3dPrimitiv::DrawNappeT3d( 1200, 6);
      }
		
    if( cAxisFlag )
      PP3d::GLUtility::DrawOrtho( (float)cKamera.getSize(), 1 );  
		
		
    if( cFlagCursor3d )
      {
	PP3d::GLUtility::DrawCursorCruz( TheAppli.getDatabase()->getCursorPosition(), 50);
      }

		
    //  cViewProps.cDebug = cDebug;
    cViewProps.cFlagViewNormal = cFlagViewNormal;
 

     TheAppli.getDatabase()->recomputeAll();     
     TheAppli.getDatabase()->drawGL( cViewProps, PP3d::GLMode::Draw, TheSelect.getSelectType() );


     if( cFlagViewTransform )
       {
	 TheAppli.getDatabaseTransform()->recomputeAll();
	 TheAppli.getDatabaseTransform()->drawGL( cViewPropsTransform, PP3d::GLMode::Draw, TheSelectTransform.getSelectType() ); 
       }

     
     
    if( cMode == ModeUser::MODE_SELECT_RECT )
      {
	std::cout << "[[[[[[[[[[[[[[[(DrawREct]]]]]]]]]]]]]])"
		  << " x:" << cMouseInitPosX
		  << " y:" << cMouseInitPosY
		  << " -> x:" << cMouseLastPosX
		  << " -> y:" << cMouseLastPosY
		  << std::endl;
	
	drawRect( cMouseInitPosX, cMouseInitPosY, cMouseLastPosX, cMouseLastPosY );
 /*
	glEnable( GL_BLEND );
	glDepthMask( GL_FALSE );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
				
	glColor4f(0.3f, 0.1f, 0.1f, 0.3f);

	TheAppli.getDatabase()->getSelectionRectanglePosition().drawGL();

	glDepthMask( GL_TRUE );
	glDisable( GL_BLEND );
	  */						
      }
    glFlush();
  }  

  //---------------------------
  void Canvas3d::processHits( GLuint pNbHits, GLuint*  pSelectBuf, bool pFlagMove)
  {    
    DBG_SEL( " processHits=" <<  pNbHits );
    //   std::cout <<  "======== processHits=" <<  pNbHits  << std::endl;
		
    GLuint*	ptr = (GLuint *) pSelectBuf;
		
    //		std::vector< std::unique_ptr<PP3d::PickingHit> > lVectHits;
    //		std::vector< PP3d::PickingHit* > lVectHits;
    std::vector< PP3d::PickingHit > lVectHits;
	
    for(GLuint lH = 0; lH < pNbHits; lH++) // for each hit  
      { 
	GLuint lNbNames = *ptr;
	ptr++;
				
	DBG_SEL_NL( "Nb names :" << lNbNames << " >>> ") ;
      	
	//	double lZ1 = ((float)*ptr) /0x7fffffff;		ptr++;	
	//	double lZ2 = ((float)*ptr)/0x7fffffff;		ptr++;
													 
	double lZ1 = ((float)*ptr);		ptr++;	
	double lZ2 = ((float)*ptr);		ptr++;
													 
	for (GLuint j = 0; j < lNbNames; j++) // for each name 
	  {
	    GLuint lName = *ptr; ptr++;
						
	    //	    cout << "---" <<  lName  << " Z:" << lZ1 << " -> " << lZ2 << endl;
	    if( lName != 0)
	      {
		PP3d::PickingHit lHit( lName, lZ1, lZ2 );
		lVectHits.push_back(lHit);
	      }
	  }
      }
		
    if( lVectHits.size() )
      {										////				std::sort( lVectHits.begin(), lVectHits.end(), []( PP3d::PickingHit &A, PP3d::PickingHit &B) { return A.cZ1 < B.cZ2; });				

	if( TheSelectTransform.getSelectType() != PP3d::SelectType::Null )
	  {
	    if( TheSelectTransform.selectPickingHit( lVectHits,
						     *TheAppli.getDatabaseTransform(),
						     cSelectMode, pFlagMove ))
	      {
		TheAppli.redrawAllCanvas3d();
		//		TheAppli.redrawObjectTree();			
	      }
	  }
	else
	  {
	    if( TheSelect.selectPickingHit( lVectHits,
					    *TheAppli.getDatabase(),
					    cSelectMode, pFlagMove ))
	      {
		TheAppli.redrawAllCanvas3d();
		//	TheAppli.redrawObjectTree();			
	      }
	  }
      }
  }
  //---------------------------
  void Canvas3d::picking( int pX, int pY, bool pFlagMove )
  {
    DBG_SEL( "=== picking:" << pX << " " << pY << " SM:" << cSelectMode );
    //    std::cout <<  "=== picking:" << pX << " " << pY << " SM:" << cSelectMode << std::endl;

    GLint lViewport[4];
    glGetIntegerv(GL_VIEWPORT, lViewport);

	 
    const int BUFSIZE = 4096;
    GLuint lSelectBuf[BUFSIZE];
    ::memset(&lSelectBuf, 0, sizeof(lSelectBuf)  );
	 
    glSelectBuffer(BUFSIZE, lSelectBuf);
	 
    glRenderMode(GL_SELECT);
	 
    glInitNames();
    glPushName(0);

    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
	 
    gluPickMatrix((GLdouble) pX, (GLdouble) (lViewport[3] - pY),
		  MyPref.cSelectPickingSize, MyPref.cSelectPickingSize, lViewport );
	 
    cKamera.execGL( true );
	
    glMatrixMode(GL_MODELVIEW);
    
    if( TheSelectTransform.getSelectType() != PP3d::SelectType::Null )
      TheAppli.getDatabaseTransform()->drawGL( cViewPropsTransform, PP3d::GLMode::Select, TheSelectTransform.getSelectType() );
    else
      TheAppli.getDatabase()->drawGL( cViewProps, PP3d::GLMode::Select, TheSelect.getSelectType() );

    glPopMatrix();
    glFlush();
    GLuint lNbHits = glRenderMode(GL_RENDER);
    processHits( lNbHits, lSelectBuf, pFlagMove  ); 	
  }
  //---------------------------
  void Canvas3d::userPrepareAction( int	pEvent )
  {	
    cMouseInitPosX = cMouseLastPosX = Fl::event_x();
    cMouseInitPosY = cMouseLastPosY = Fl::event_y();

    TheAppli.currentTransform().raz();
    TheAppli.currentTransform().scaleTo(1);
  }
  //------------------------------
  void Canvas3d::userCancelAction(	int	pEvent )
  {  
    if(cVisitModifSelect!= nullptr )
      {
	cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::CANCEL, TheSelect );
  	cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::CANCEL, TheSelectTransform );
    }
    userTerminateAction(pEvent);
  }									 
  //------------------------------
  void Canvas3d::userTerminateAction(	int	pEvent )
  {
    std::cout << "TERMINATE" << std::endl;
		
    cMouseInitPosX = cMouseLastPosX = -1;
    cMouseInitPosY = cMouseLastPosY = -1;
    cRectBeginX = -1;
    cRectBeginY = -1;
    TheAppli.setCurrentTransformType( Transform::Nothing );
    TheAppli.currentTransform().raz();

    PP3d::Point3d lVoid;
     TheAppli.getDatabase()->setSelectionRectanglePosition( lVoid, lVoid );
    cMode = ModeUser::MODE_BASE;
    cancelDragSelect();					
    cSelectMode = PP3d::SelectMode::Undefine;
    if(cVisitModifSelect != nullptr )
      {
	delete cVisitModifSelect;
	cVisitModifSelect = nullptr;
      }
  }									 
  //---------------------------
  void Canvas3d::userChangeKameraView( int	pEvent)
  {
    if( cMouseLastPosX == -1 )
      return;

    //    std::cout << " ZKam:" << getKamera().position()[2] << " " << std::flush;
    
    int lX = Fl::event_x();
    int lY = Fl::event_y();
    //		if( Fl::event_button2() )
    if( Fl::event_button2() )
      {
	getKamera().position()[0] += (cMouseLastPosX	-lX)/10.0;
	getKamera().position()[1] += (lY-cMouseLastPosY)/10.0;
	redraw();
	cMouseLastPosX = lX;
	cMouseLastPosY = lY;
      }
    else
      //			if( Fl::event_button1() )
      //	if( Fl::event_button1() )
      {
	getKamera().angle()[1] += (cMouseLastPosX-lX)/1.0;
	getKamera().angle()[0] += (lY-cMouseLastPosY)/1.0;

	if( getKamera().angle()[0] < 0 )
	  getKamera().angle()[0] += 360;
	else
	  if( getKamera().angle()[0] > 360 )
	    getKamera().angle()[0] -= 360;

	if( getKamera().angle()[1] < 0 )
	  getKamera().angle()[1] += 360;
	else
	  if( getKamera().angle()[1] > 360 )
	    getKamera().angle()[1] -= 360;

								
	//	cout << "angle X: " <<	getKamera().angle()[0]  
	//	     << "angle Y: " <<	getKamera().angle()[1] ;
	redraw();
	cMouseLastPosX = lX;
	cMouseLastPosY = lY;
      }
  }
  //---------------------------
  bool Canvas3d::initDragSelect()
  {
    if( cDragPoints.size() == 0
	&&TheSelect.getNbSelected() >0 )
      {
	std::cout << "******************* Canvas3d::initDragSelect " << TheSelect.getNbSelected() << std::endl;
	std::cout << "******************* Canvas3d::initDragSelect " << TheSelect.getSelection().size() << std::endl;

	cDragCenter =   TheSelect.getCenter( *TheAppli.getDatabase() );			
	// We keep all the adress of points of selected entities
	PP3d::GetPoints< PP3d::EntityPtrHash, PP3d::PointPtrSet>(TheSelect.getSelection(),
								  cDragPoints );

	// Save the original coordinates of points       	  
	cDragSavPoints.resize( cDragPoints.size() );
	int lP=0;
	for( PP3d::PointPtr lPoint : cDragPoints )
	  {
	    cDragSavPoints[lP++] = lPoint->get();
	  }
	return true;
      }
    return false;
  }
  //---------------------------
  void Canvas3d::dragSelect( PP3d::Mat4 &pMat )
  {
    if( initDragSelect() == false )
      {				
	int lP=0;
	for( PP3d::PointPtr lPoint : cDragPoints )
	  {
	    lPoint->get() = cDragSavPoints[lP++] * pMat;  // 
	  }				
      }
  }
  //---------------------------
  void Canvas3d::validDragSelect( PP3d::Mat4 &pMat )
  {
    std::cout << "validDragSelect PushHistory " << std::endl;
    
    dragSelect( pMat ); // Inutile ? 
    cDragPoints.clear();
    cDragSavPoints.clear();
    PushHistory();
  }
  //---------------------------
  void Canvas3d::cancelDragSelect()
  {
    if( cDragPoints.size() != 0 )
      {
	int lP=0;
	for( PP3d::PointPtr lPoint : cDragPoints )
	  {
	    lPoint->get() = 	cDragSavPoints[lP++]; // restore original values 
	  }
	cDragPoints.clear();
	cDragSavPoints.clear();
      }
  }

  //---------------------------
  void Canvas3d::userTransformSelection(int pEvent, bool pFlagFinalize)
  {
    int lX = Fl::event_x();
    int lY = Fl::event_y();
 
    float lDx = (float)(lX - cMouseLastPosX);
    float lDy = (float)(cMouseLastPosY - lY);
    cMouseLastPosX = lX;
    cMouseLastPosY = lY;
    
    long double lScale= lDx*0.01;
    
  
    std::cout << "***** Canvas3d::userTransformSelection **** " << lDx << " " << lDy << " Scale:" <<  lScale << std::endl;

		
    PP3d::Mat4 lMatTran;
    lMatTran.Identity();
    
  
    switch( TheAppli.getCurrentTransformType() )
      {
	//================
      case Transform::Nothing:
	std::cout << "Nothing to do !!!" << std::endl;
	break;
	//	return ;

	//================
      case Transform::MoveNormal:
	{
	 
	  std::cout << "======== Grab Normal " << std::endl;
 
	  
	  if(cVisitModifSelect == nullptr )
	    {
	      cVisitModifSelect = new PP3d::VisitorMoveNormal(TheSelect);// DIFFERENCE !!!
	      cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::SAV, TheSelect);
	    }
	  
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::CANCEL, TheSelect); // remise a zero des modifs
	  cVisitModifSelect->setCoef( ((double)cMouseLastPosX-cMouseInitPosX)/30 );// DIFFERENCE !!!
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::MODIF, TheSelect);
	  
	  if( pFlagFinalize)
	      validDragSelect( lMatTran );
	  return;    //////////// ATTENTION 
	}
	//================
	
      case Transform::ScaleNormal:
	{
	 
	  std::cout << "======== Grab Normal " << std::endl;
 
	  
	  if(cVisitModifSelect == nullptr )
	    {
	      cVisitModifSelect = new PP3d::VisitorScaleNormal(TheSelect); // DIFFERENCE !!!
	      cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::SAV, TheSelect);
	    }
	  
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::CANCEL, TheSelect); // remise a zero des modifs
	  cVisitModifSelect->setCoef( ((double)cMouseLastPosX-cMouseInitPosX)/100.0 );// DIFFERENCE !!!
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::MODIF, TheSelect);
	  
	  if( pFlagFinalize)
	      validDragSelect( lMatTran );
	  return;    //(double)/////////// ATTENTI suricata.yamlON 
	}
	//================
	// Chaque facette tourne autor de sa normale
      case Transform::CenterRotFacetNorm:
	{
	 
	  std::cout << "======== Grab Normal " << std::endl;	  
	   
	  if(cVisitModifSelect == nullptr )
	    {
	      cVisitModifSelect = new PP3d::VisitorRotNormal(TheSelect); // DIFFERENCE !!!
	      cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::SAV, TheSelect);
	    }
	  
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::CANCEL, TheSelect); // remise a zero des modifs
	  TheAppli.currentTransform().angle().x() += M_PI*lDx*0.01;             // DIFFERENCE !!!	
	  cVisitModifSelect->setCoef( TheAppli.currentTransform().angle().x() );// DIFFERENCE !!!
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::MODIF, TheSelect);
	  
	  if( pFlagFinalize)
	      validDragSelect( lMatTran );
	  return;    //(double)/////////// ATTENTlON 
	}
	//================

	
			
      case Transform::MoveX:
	{
	  TheAppli.currentTransform().position().x() += lDx/10;
	  cMyWin3d.setCurrentVal(  "move x" , TheAppli.currentTransform().position().x() );
	  lMatTran.initMove( TheAppli.currentTransform().position().x(), 0, 0 );
	}
	break;
				
      case Transform::MoveY:
	{
	  TheAppli.currentTransform().position().y() += lDx/10;
	  cMyWin3d.setCurrentVal( "move y" ,  TheAppli.currentTransform().position().y() );
	  lMatTran.initMove( 0, TheAppli.currentTransform().position().y(), 0 );
	}
	break;
				
      case Transform::MoveZ:
	{
	  TheAppli.currentTransform().position().z() += lDx/10;
	  cMyWin3d.setCurrentVal(  "move z" , TheAppli.currentTransform().position().z() );
	  lMatTran.initMove( 0, 0, TheAppli.currentTransform().position().z() );
	}
	break;

     case Transform::MoveAxis:
	{
	  PP3d::Point3d lPtZero;
	  PP3d::Point3d lAxis;
	  if( TheAppli.getAxis( lPtZero, lAxis ) )	{
	    {
	      TheAppli.currentTransform().position().x() += lAxis.cX*lDx/100;
	      TheAppli.currentTransform().position().y() += lAxis.cY*lDx/100;
	      TheAppli.currentTransform().position().z() += lAxis.cZ*lDx/100;
	      
	      cMyWin3d.setCurrentVal(  "move z" , TheAppli.currentTransform().position().z() );
	      lMatTran.initMove( TheAppli.currentTransform().position() );
	    }
	  }
	}
	break;

	//========== SCALE =========================

	case Transform::ScaleUniform :
	case Transform::ScaleX :
	case Transform::ScaleY :
	case Transform::ScaleZ :
	case Transform::ScaleRX :
	case Transform::ScaleRY :
	case Transform::ScaleRZ :
	  {
	    PP3d::Mat4 lMatRecenter;
	    lMatRecenter.initMove( cDragCenter ); //on revient au centre;
	    
	    PP3d::Point3d lNCenter =  -cDragCenter;					
	    PP3d::Mat4 lMatZero;
	    lMatZero.initMove( lNCenter ); //on se positionne en zero;
	    
	    PP3d::Mat4 lMatScale;	  
	    switch(  TheAppli.getCurrentTransformType() )
	      {		
	      case Transform::ScaleUniform :
		{
		  TheAppli.currentTransform().scale().x() += lScale;
		  TheAppli.currentTransform().scale().y() += lScale;
		  TheAppli.currentTransform().scale().z() += lScale;
		  break;
		}
	      case Transform::ScaleX :
		  TheAppli.currentTransform().scale().x() += lScale;
		  break;
	      case Transform::ScaleY :
		  TheAppli.currentTransform().scale().y() += lScale;
		  break;
	      case Transform::ScaleZ :
		  TheAppli.currentTransform().scale().z() += lScale;
		  break;
	      case Transform::ScaleRX :
		  TheAppli.currentTransform().scale().y() += lScale;
		  TheAppli.currentTransform().scale().z() += lScale;
		  break;
	      case Transform::ScaleRY :
		  TheAppli.currentTransform().scale().x() += lScale;
		  TheAppli.currentTransform().scale().z() += lScale;
		  break;
	      case Transform::ScaleRZ :
		  TheAppli.currentTransform().scale().x() += lScale;
		  TheAppli.currentTransform().scale().y() += lScale;
		  break;
	      case Transform::ScaleAxis :
		{
		  PP3d::Point3d lPtZero;
		  PP3d::Point3d lAxis;
		  if( TheAppli.getAxis( lPtZero, lAxis ) )
		    {
		      // normalize ?
		    } // A FAIRE
		}
		break;
	      default:;
	      }
	     
	    //	  CallDialogKeepFloat( TheAppli.currentTransform().scale.x());
	    lMatScale.initScale( TheAppli.currentTransform().scale() );
	    lMatTran = lMatRecenter * lMatScale *  lMatZero;					
	  }
	break;
	//============ ROTATE =======================
      case Transform::CenterRotX :
      case Transform::CenterRotY :
      case Transform::CenterRotZ :
      case Transform::CenterRotNorm :
      case Transform::CenterRotAxis :
      case Transform::RotAxis :
	{		
	  std::cout << "Center:" << cDragCenter  << std::endl;
	  
	  PP3d::Mat4 lMatRecenter;
	  lMatRecenter.initMove( cDragCenter ); //on revient au centre;
					
	  PP3d::Point3d lNCenter =  -cDragCenter;					
	  PP3d::Mat4 lMatZero;
	  lMatZero.initMove( lNCenter ); //on se positionne en zero;

	  
	  PP3d::Mat4 lMatRot;	  
	  switch( TheAppli.getCurrentTransformType() )
	    {
	    case Transform::CenterRotX :
	      TheAppli.currentTransform().angle().x() += M_PI*lDx*0.01;
	      CallDialogKeepFloat( TheAppli.currentTransform().angle().x());
	      
	      std::cout << " Angle:" << TheAppli.currentTransform().angle().x()
			<< " : " << (TheAppli.currentTransform().angle().x()*180)/M_PI
			<< std::endl;	  

	      lMatRot.initRotX( TheAppli.currentTransform().angle().x() );
	      break;
							
	    case Transform::CenterRotY :
	      TheAppli.currentTransform().angle().y() += M_PI*lDx*0.01;
	      CallDialogKeepFloat( TheAppli.currentTransform().angle().y());
	      
	      std::cout << " Angle:" << TheAppli.currentTransform().angle().y()
			<< " : " << (TheAppli.currentTransform().angle().y()*180)/M_PI
			<< std::endl;	  

	      lMatRot.initRotY( TheAppli.currentTransform().angle().y() );
	      break;
	      
	    case Transform::CenterRotZ :
	      TheAppli.currentTransform().angle().z() += M_PI*lDx*0.01;
	      CallDialogKeepFloat( TheAppli.currentTransform().angle().z());
	      
	      std::cout << " Angle:" << TheAppli.currentTransform().angle().z()
			<< " : " << (TheAppli.currentTransform().angle().z()*180)/M_PI
			<< std::endl;	  
	      
	      lMatRot.initRotZ( TheAppli.currentTransform().angle().z() );
	      break;
	      
	    case Transform::CenterRotNorm :
	      {
		if( TheSelect.getNbSelected() >0
		    && (TheSelect.getSelectType() == PP3d::SelectType::Object
			|| TheSelect.getSelectType() ==  PP3d::SelectType::Facet))
		  {
		    PP3d::SortEntityVisitorPointFacet lVisit;		    
		    TheSelect.execVisitorOnEntity(lVisit);
		
		    PP3d::Point3d lCenter;
		    PP3d::Point3d lNorm;
		    
		    if( PP3d::GetVectorFromFacets( lVisit, lNorm, lCenter ))
		      {			
			//	      std::cout << "VERIFICATION VECTEUR NORM:"
			//	      << lAxis.cX*lAxis.cX+lAxis.cY*lAxis.cY+lAxis.cZ*lAxis.cZ
			//	      << std::endl;
			// On prend x mais on pourrait prendre ce que l'on veut
			TheAppli.currentTransform().angle().x() += M_PI*lDx*0.01;
			CallDialogKeepFloat( TheAppli.currentTransform().angle().x());
			 
			//			std::cout << " Angle:" << TheAppli.currentTransform().angle().x()
			//				  << " : " << (TheAppli.currentTransform().angle().x()*180)/M_PI
			//	  << std::endl;	 
			lNorm.normalize();
			lMatRot.initRotAxis( lNorm, TheAppli.currentTransform().angle().x() );
		      }
		  }
	      }
	      break;
	      
	    case Transform::CenterRotAxis :
	      {
		  PP3d::Point3d lPtZero;
		  PP3d::Point3d lAxis;
		  if( TheAppli.getAxis( lPtZero, lAxis ))
		    {
		      lAxis -= lPtZero;
		      lAxis.normalize();		      
		      
		      PP3d::SortEntityVisitorPointFacet lVisit;		    
		      TheSelect.execVisitorOnEntity(lVisit);		    
		      
		      TheAppli.currentTransform().angle().x() += M_PI*lDx*0.01;
		      CallDialogKeepFloat( TheAppli.currentTransform().angle().x());
		      
		      lMatRot.initRotAxis( lAxis, TheAppli.currentTransform().angle().x() );		      
		    }		      
	      }
	      break;
	      
	    case Transform::RotAxis :
	      {
		  PP3d::Point3d lPtZero;
		  PP3d::Point3d lAxis;
		  if( TheAppli.getAxis( lPtZero, lAxis ))
		    {
		      lAxis -= lPtZero;
		      lAxis.normalize();		      
		      
		      lMatRecenter.initMove( lPtZero ); //on revient au centre;
	    
		      PP3d::Point3d lNCenter =  -lPtZero;					
		      lMatZero.initMove( lNCenter ); //on se positionne en zero;
	    
		      PP3d::SortEntityVisitorPointFacet lVisit;		    
		      TheSelect.execVisitorOnEntity(lVisit);		    
		      
		      TheAppli.currentTransform().angle().x() += M_PI*lDx*0.01;
		      CallDialogKeepFloat( TheAppli.currentTransform().angle().x());
		      
		      lMatRot.initRotAxis( lAxis, TheAppli.currentTransform().angle().x() );		      
		    }		      
	      }
	      break;

	    default:;
	    }					
					
	  lMatTran = lMatRecenter * lMatRot *  lMatZero;					
	}
	break;
	//===================================

      default:
	std::cout << "Nothing to do 2!!!" << std::endl;
      }

    std::cout << "pos:" << TheAppli.currentTransform().position() << std::endl;


    if( pFlagFinalize == false )
      {
	//				PP3d::Selection::Instance().setCurrentMatrice( &lMatTran );
	dragSelect( lMatTran );
      }
    else
      {
	cout << "============= VALIDATE ===================" << endl;
	//			cout << lMatTran << endl;
	validDragSelect( lMatTran );
	cout << "============= VALIDATE ===================" << endl;
				
	//				PP3d::Selection::Instance().execModificationMatrice( lMatTran );
      }
  }
  //---------------------------
  void Canvas3d::userInputPoint(int	pEvent)
  {
    double lH = pixel_h();
    double lX = ((double )Fl::event_x());
    double lY = lH-((double) Fl::event_y());

    // On projette le point 0,0,0 de la 3d vers la 2d pour recuperer le Z a ajouter a notre x et y
		
		
    PP3d::Point3d lPt0;
    PP3d::Point3d lResult0;
    getKamera().projectObjectToWin( lPt0, lResult0, true);
		

    std::cout << "==========================================================================" << std::endl;
    // std::cout << "1----> (" << lResult0.cX<< "  " << lResult0.cY << "  " << lResult0.cZ << " ) " << std::endl;
		
    // On projette ensuite ce point dans l'espace 3d
		
    PP3d::Point3d pResult;
		
    std::cout << "lX:" << lX << " lY:" << lY ;
    getKamera().projectWinToObject( PP3d::Point3d( lX, lY, lResult0.cZ), pResult, true);
    std::cout << " --> (" << pResult.cX<< "  " << pResult.cY << "  " << pResult.cZ << " ) " << std::endl;

    // Pour verifier on fait l'operation inverse
    getKamera().projectObjectToWin( pResult, lResult0, true);				
    std::cout << " ==> (" << lResult0.cX<< "  " << lResult0.cY << "  " << lResult0.cZ << " ) " << std::endl;
		
    std::cout << "==========================================================================" << std::endl;
			 
	
    TheAppli.getDatabase()->setCursorPosition(	pResult );
    TheAppli.setCursorPosition( pResult );
    /*
    {
      std::ostringstream lOsLuaCode;
      std::ostringstream lOsLuaOut;
			
      lOsLuaCode << "ShapeAddCurrentPoint("<<  pResult.cX << ',' << pResult.cY << ',' <<  pResult.cZ <<')'<< std::endl;
      lOsLuaCode << "OxyzRedrawCanvas()"<< std::endl;
      if( TheAppli.execLuaHisto(lOsLuaCode, lOsLuaOut) !=0)
	{
	}
    }
    */
    
    TheAppli.getDatabase()->addPointToCurrentLine( pResult );
				
    TheAppli.redrawAllCanvas3d();
  }
  //---------------------------
  void Canvas3d::userInputPoint( PP3d::Entity* iEntity )
  {
    if( iEntity == nullptr)
      {
	DBG_ACT(" **************** userInputPoint Hightlight NOT FOUND" );
	std::cout << " **************** userInputPoint Hightlight NOT FOUND" << std::endl;
	return;
      }
    DBG_ACT(" **************** userInputPoint Hightlight " << iEntity->getType() );
    std::cout << " **************** userInputPoint Hightlight " << iEntity->getType() << std::endl;
    
    PP3d::SortEntityVisitorPoint  lVisit;
    iEntity->execVisitor( lVisit );

    for( PP3d::PointPtr lPt : lVisit.cVectPoints) // les points sont uniques
      {
	TheAppli.getDatabase()->addPointToCurrentLine( lPt->get() );	
      }

    TheAppli.redrawAllCanvas3d();
  }
  //---------------------------
  PP3d::Point3d Canvas3d::tranform2Dto3D(  int pX, int pY, int pZ )
  {
    double lH = pixel_h();				
    double lX = ((double )pX);
    double lY = lH-((double) pY);
    double lZ = ((double ) pZ);

		
    PP3d::Point3d pResult;
    cKamera.projectWinToObject( PP3d::Point3d(lX,lY,lZ), pResult, true);
    return pResult;
  }
  //---------------------------
  void Canvas3d::setCursor3dPosition( int pX, int pY, int pZ )
  {
    PP3d::Point3d pResult = tranform2Dto3D( pX, pY, pZ );
     TheAppli.getDatabase()->setCursorPosition(	pResult );
    TheAppli.setCursorPosition( pResult );
  }
  //---------------------------
  void Canvas3d::userSelectionRectangle(int	pEvent, bool pFlagFinalize )
  {
		
    if( cRectBeginX == -1 )
      {
	cRectBeginX = Fl::event_x();
	cRectBeginY = Fl::event_y();
	return;
      }
    PP3d::Point3d lCurrent = tranform2Dto3D( Fl::event_x(),  Fl::event_y() );
    PP3d::Point3d lBegin   = tranform2Dto3D( cRectBeginX, cRectBeginY);
			
     TheAppli.getDatabase()->setSelectionRectanglePosition( lBegin, lCurrent );
    if( pFlagFinalize )
      {
	//			PP3d::Selection::Instance().selectRect(  TheAppli.getDatabase()->getSelectionRectanglePosition() );
      }
  }
  //---------------------------------------------------------
  void Canvas3d::userSelectionPoint(int	pEvent, bool pFlagMove)
  {
    /*	double lH = pixel_h();
	double lX = ((double )Fl::event_x());
	double lY = lH-((double) Fl::event_y());
		
	// On projette le point 0,0,0 de la 3d vers la 2d pour recuperer le Z a ajouter a notre x et y
	PP3d::Point3d lPt0;
	PP3d::Point3d lResult0;
	cKamera.projectObjectToWin( lPt0, lResult0, true);
		

	 TheAppli.getDatabase()->selectPoint( PP3d::Point3d(lX, lY, lResult0.cZ), cKamera, PP3d::SelectType::Object );*/
		
    picking( Fl::event_x(),  Fl::event_y(), pFlagMove )	;
  }
  //---------------------------------------------------------
  int Canvas3d::handle( int pEvent	) 
  {
    //		cout << endl;
    if( pEvent == FL_NO_EVENT ) return 1;

    traceMode();
    
    DBG_EVT( " <<<Event:" << pEvent << " " << fl_eventnames[pEvent] << ">>> "
	     << " button:" << Fl::event_button() 
	     << " ctrl: " << Fl::event_ctrl()
	     << " shift:" <<  Fl::event_shift()
	     << " alt:" <<  Fl::event_alt());	


    switch( pEvent )
      {
	//===========================
				
      case FL_PUSH :
	DBG_EVT( "-------------- Button Push "  << Fl::event_button()  << " " << Fl::event_shift());
	Fl::focus(this);



	// SAISIE DE POINT
	if( Fl::event_button() == FL_LEFT_MOUSE
	    &&  Fl::event_ctrl() &&  cMode == ModeUser::MODE_BASE)
	  {
	    DBG_ACT(" **************** cUserActionSaisie " );
	    std::cout << " **************** cUserActionSaisie " << std::endl;
	    userInputPoint( pEvent );
						
	    return 1;
	  }
	if( Fl::event_button() == FL_LEFT_MOUSE
	    &&  Fl::event_shift() &&  cMode == ModeUser::MODE_BASE)
	  {
	    PP3d::EntityId lId =TheSelect.getLastHightLightEntityId();
	    
	    std::cout << " **************** cUserActionSaisie SHIFT " << lId  << std::endl;
	    DBG_ACT(" **************** cUserActionSaisie Hightlight  "  << lId );
	    
	    userInputPoint(  TheAppli.getDatabase()->findEntity( lId)  );	    						
	    return 1;
	  }
				
	// SELECTION RECTANGLE : BUG
	if( Fl::event_button() == FL_MIDDLE_MOUSE
	    &&  Fl::event_shift() ) // &&  cMode == ModeUser::MODE_BASE)
	  {
	    std::cout << "SELECT_RECT" << std::endl;
	
	     
	    DBG_EVT( "******************* cUserActionRectangle " );
	    DBG_EVT( "******************* cUserActionRectangle " );
	    
	    DBG_ACT( "******************* cUserActionRectangle " );
	    DBG_ACT( "******************* cUserActionRectangle " );
	    DBG_ACT( "******************* cUserActionRectangle " );
	    DBG_ACT( "******************* cUserActionRectangle " );
	    DBG_ACT( "******************* cUserActionRectangle " );
	    
	    cMode = ModeUser::MODE_SELECT_RECT;

	    userPrepareAction( pEvent );
	    TheAppli.redrawAllCanvas3d();

	    return 1;
	  }

	// DEBUT MODE DEPLACEMENT CAMERA
	if( Fl::event_button() == FL_MIDDLE_MOUSE )
	  {
	    userPrepareAction( pEvent );
	    if( cMode == ModeUser::MODE_BASE )
	      cMode = ModeUser::MODE_MOVE_CAMERA;
	    return 1;
	  }

	if( Fl::event_button() == FL_LEFT_MOUSE &&  cMode == ModeUser::MODE_TRANSFORM )
	  {
	    userPrepareAction( pEvent );
	    return 1;
	  }

	// FIN DEPLACEMENT CAMERA OU SELECTION
	if( Fl::event_button() == FL_LEFT_MOUSE )
	  {
	    if( cMode == ModeUser::MODE_MOVE_CAMERA )
	      {
		cMode = ModeUser::MODE_BASE;
		userTerminateAction( pEvent );
		return 1;
	      }
	    else  //DEBUT MODE_SELECT
	      if( cMode == ModeUser::MODE_BASE )
		{
		  cSelectMode = PP3d::SelectMode::Undefine;
		  userPrepareAction( pEvent );
		  userSelectionPoint( pEvent, false );
		  cMode = ModeUser::MODE_SELECT;																	
		  return 1;
		}
	
	    userPrepareAction( pEvent );
						
	    return 1;
	  }

	// MENU
	if(  Fl::event_button() == FL_RIGHT_MOUSE )
	  {
	    cPopup->clear();						
						
						
	    if( cMode == ModeUser::MODE_BASE )
	      {
		makeMenu( *cPopup); 
	      }
	  }
	cPopup->position( Fl::event_x() , Fl::event_y());
			
	cPopup->popup();

						
	return 1;
						
			
	break;
	//==============================

      case FL_RELEASE:
	DBG_EVT( "******** Button Release ");
					
	switch(  Fl::event_button() )
	  {
	  case FL_LEFT_MOUSE :
	    if( cMode == ModeUser::MODE_SELECT )
	      {
		userTerminateAction( pEvent );
	      }

	    DBG_EVT(  "LEFT ");
	    break;
	  case FL_MIDDLE_MOUSE :
	    DBG_EVT( "MIDDLE ");
	    break;
	  case FL_RIGHT_MOUSE :
	    DBG_EVT(  "RIGHT ");							
	    break;
	  }
				
	if(  cMode == ModeUser::MODE_TRANSFORM
	     && TheAppli.getCurrentTransformType() != Transform::Nothing )
	  {
	    userTransformSelection(pEvent, true );
	    userTerminateAction( pEvent );
	    TheAppli.redrawAllCanvas3d();
	  }
				
	if( cMode == ModeUser::MODE_SELECT_RECT )
	  {
	    std::cout << "{{{{{{{{{{{{{{{{{{ RELEASE RECT }}}}}}}}}}}}}}" << std::endl;
	    userSelectionRectangle(pEvent, true );
	    userTerminateAction( pEvent );
	  }		
	break;
	//==============================

      case FL_DRAG:
	Fl::focus(this);
	DBG_EVT( " <DRAG> " << cMouseLastPosX );
	if( cMouseLastPosX != -1 )
	  {
	    if( cMode == ModeUser::MODE_TRANSFORM )
	      {
		userTransformSelection(pEvent);
	      }
	    else
	      if( cMode == ModeUser::MODE_MOVE_CAMERA )
		{
		  userChangeKameraView( pEvent );
		}					
	      else
		if( cMode == ModeUser::MODE_SELECT )
		  {
		    userSelectionPoint( pEvent, false );
		  }
		else
		  if( cMode == ModeUser::MODE_SELECT_RECT )
		    {
		      cMouseLastPosX = Fl::event_x();
		      cMouseLastPosY = Fl::event_y();
		    }	    
	  }
	setCursor3dPosition( Fl::event_x(), Fl::event_y());			 					
	TheAppli.redrawAllCanvas3d(); // a cause du curseur ou 				break;
	break;
				
	//==============================
				
      case FL_MOVE:
	{
	  DBG_EVT( " <MOVE> " << cMouseLastPosX );
	  
	  if( cMouseLastPosX != -1 )
	    {					 	
	      if( cMode == ModeUser::MODE_MOVE_CAMERA )
		{
		  userChangeKameraView( pEvent );
		  TheAppli.redrawAllCanvas3d(); // a cause du curseur ou du rectangel etc
		}
	      else if( cMode == ModeUser::MODE_SELECT_RECT)
		{
		  userSelectionRectangle(pEvent);			
		  TheAppli.redrawAllCanvas3d(); // a cause du curseur ou du rectangel etc
		}	
	
	    }
	  else
	    if( cMode == ModeUser::MODE_BASE )
	      {
		if( MyPref.cSelectPassOverLighting ) 
		  userSelectionPoint( pEvent, true );
	      }
					
	  setCursor3dPosition( Fl::event_x(), Fl::event_y());
	  TheAppli.redrawAllCanvas3d(); // a cause du curseur ou 				break;
		
	
	  //			cout << " Move : x="<< lX << " y=" << lY <<  std::endl;
	  //			cout << " Cursor :  x=" << pResult.cX << " y=" << pResult.cY << " z=" << pResult.cZ << std::endl;
	}
	break;
	//==============================
			
      case FL_ENTER:
	DBG_EVT( " ENTER " << Fl::belowmouse());
	Fl::focus(Fl::belowmouse()); 
	break;
      case FL_LEAVE:
	DBG_EVT( " LEAVE " << Fl::belowmouse());
	break;

      case FL_FOCUS:
	DBG_EVT(  " FOCUS " );
	break;
      case FL_UNFOCUS:
	DBG_EVT( " UNFOCUS " );
	break;
	
			
      case FL_KEYDOWN:
	{
	  DBG_EVT_NL( " CTRL :" << Fl::event_ctrl() << " " );
	  DBG_EVT_NL( " KEYDOW " <<  Fl::event_key()  );

	  if(  Fl::event_key() >= 256 )
	    {
	      switch(  Fl::event_key() )
		{
		case FL_Escape:
		  userCancelAction( pEvent );								
		  break;
		case	FL_Tab:
		  DBG_EVT_NL( " TAB" );
		  if( cMode == ModeUser::MODE_TRANSFORM )
		    {
		      userTransformSelection(pEvent);			
		    }		  
		  break;
		case FL_BackSpace:
		case FL_Delete:
		   TheAppli.getDatabase()->delPointToCurrentLine();
		  break;

		case FL_Up:
		case FL_Down:;
		}
	    }
	  else
	    //			if( Fl::event_ctrl() != 0 )
	    {
	      cout << ":" << Fl::event_text() << ":" ;
	      const char *lStr = Fl::event_text();
	      cout << " TXT <" << lStr << "> " ;
					
	      if( strcmp( lStr, ANNULE_ACTION )==0)
		{
		  userTerminateAction( pEvent );
		  TheAppli.setCurrentTransformType( Transform::Nothing );
		}
	      else if( strcmp( lStr, UNSELECT_ALL) == 0 )
		{
		  if( cMode == ModeUser::MODE_BASE )
		   TheSelect.removeAll();
		}
	      else if( strcmp( lStr, MOVE_Z_N )==0)
		{
		  cScale = cKamera.scale().x();
		  cKamera.position()[2] -= cScale;
		  cout << " MOVE_Z_N<" << cScale << "> Z<" << cKamera.position()[2]<<">";

		}
	      else if( strcmp( lStr, MOVE_Z_P )==0)
		{
		  cScale = cKamera.scale().x();
		  cKamera.position()[2] += cScale;
		  cout << " MOVE_Z_P<" << cScale << "> " ;
		}
	      else if( strcmp( lStr, RESET_VIEW_TO_X )==0
		       || strcmp( lStr, RESET_VIEW_TO_X2 )==0)
		{
		  if( Fl::event_shift() )
		    cKamera.razXInv();
		  else
		    cKamera.razX(); 
		}
	      else if( strcmp( lStr, RESET_VIEW_TO_Y )==0
		       || strcmp( lStr, RESET_VIEW_TO_Y2 )==0 )
		{
		  if( Fl::event_shift() )
		    cKamera.razYInv(); 
		  else
		    cKamera.razY(); 
		}
	      else if( strcmp( lStr, RESET_VIEW_TO_Z )==0
		       || strcmp( lStr, RESET_VIEW_TO_Z2 )==0)
		{
		  if( Fl::event_shift() )
		    cKamera.razZInv(); 
		  else
		    cKamera.razZ(); 
		} 
	      else if( strcmp( lStr, RESET_VIEW )==0)
		{
		  cKamera.reset(); 
		  cKamera.raz45();
		  cKamera.scaleTo(1.0);
		}
	      else if( strcmp( lStr, RESET_VIEW_SCALE_0 )==0)
		{
		  cKamera.scaleTo(0.1);
		}
	      else if( strcmp( lStr, RESET_VIEW_SCALE_1 )==0)
		{
		  cKamera.scaleTo(1.0);
		}
	      else if( strcmp( lStr, RESET_VIEW_SCALE_2 )==0)
		{
		  cKamera.scaleTo(2.0);
		}	      
	      else if( strcmp( lStr, RESET_VIEW_SCALE_3 )==0)
		{
		  cKamera.scaleTo(4.0);
		}
	      else if( strcmp( lStr, RESET_VIEW_SCALE_4 )==0)
		{
		  cKamera.scaleTo(8.0);
		}
	      
	      else if( strcmp( lStr, RESET_VIEW_SCALE_5 )==0)
		{
		  cKamera.scaleTo(16.0);
		}
	      else if( strcmp( lStr, RESET_VIEW_SCALE_6 )==0)
		{
		  cKamera.scaleTo(32.0);
		}	      	      	      
	      else if( strcmp( lStr, CHG_AXIS )==0)
		{
		  if( cAxisFlag )
		    cAxisFlag = false;
		  else 
		    cAxisFlag = true;
		}
	      else if( strcmp( lStr, CHG_GRID )==0)
		{
		  if( cGridMode == ModeGrid::NO_GRID )
		    {
		      cGridMode = ModeGrid::GRID_2D;
		    }
		  else 
		    {
		      cGridMode = ModeGrid::NO_GRID;
		    }
		}
	      else if( strcmp( lStr, CHG_ORTHO_PERS ) ==0)
		{
		  cout << "Changement kamera clavier" << endl;
		  cKamera.chgModeKamera();									
		}
	      else  if( strcmp( lStr, STR_CURSOR_3D ) ==0)
		{
		  cFlagCursor3d = ! cFlagCursor3d;
		}
	      /*
	      else if( strcmp( lStr, STR_EXIT ) ==0)
		{
		  ::exit(0);
		}
	      */
	    }
	}
	redraw();
	break;

      case FL_KEYUP:
	cout << " KEYUP " ;
	break;
	
      case FL_SHOW:
	cout << "SHOW" ;
	break;

      case FL_HIDE:
	cout << " HIDE";
	break;

      case FL_ACTIVATE:
	cout << " ACTIVATE ";
	break;


      case FL_MOUSEWHEEL:
	Fl::focus(this);
	if( cMode == ModeUser::MODE_MOVE_CAMERA )
	  {
	    cout << " WHEEL x:" << Fl::event_dx() << " y:" <<  Fl::event_dy() ;
						
	    cScale = cKamera.scale().x();
	    if( Fl::event_dy() != 0)
	      {
	  
		//	    cScale += Fl::event_dy()/10.0;
		cScale *= 1.0-Fl::event_dy()/(MyPref.cMouseWheel+0.01);
	      }
			
	    if(cScale  < 0.0000000001 ) 
	      cScale = 0.0000000001;
	    if( cScale > 100000000 )
	      cScale = 100000000;
						
	    Service::Warning << "Scale out of range !";
						
	    cKamera.scaleTo(cScale);
	    redraw();
	  } 
	break;
      }
						
    return 1;
  }
  //***************************************


} //namespace 

