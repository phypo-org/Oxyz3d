#include "Canvas3d.h"

#include <FL/Fl.H>


#include <FL/gl.h>

#include <FL/names.h>
#include <FL/Fl_Menu_Button.H>


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

#include "Application.h"
#include "MyFlWidget.h"

#include "Dialogs.h"

using namespace std;


namespace M3d {

  const char * const ANNULE_ACTION="e";

  const char * const CHG_AXIS="a";

  const char * const CHG_GRID="g";

  const char * const CHG_ORTHO_PERS="p";


  const char * const RESET_VIEW_TO_X="x";
  const char * const RESET_VIEW_TO_Y="y";
  const char * const RESET_VIEW_TO_Z="z";

  const char * const STR_CURSOR_3D="c";
  const char * const STR_EXIT="q";

  const char * const MOVE_Z_N="-";
  const char * const MOVE_Z_P="+";

  const char * const UNSELECT_ALL=" ";

  //***************************************
  Canvas3d::Canvas3d(  Win3d& pW3d, int pX, int pY, int pW, int  pH,   const char *l )
    :Fl_Gl_Window( pX, pY, pW, pH, l )
    ,cMyWin3d(pW3d)
    ,cScale(1.0)
    ,cMode( ModeUser::MODE_BASE )
    ,cAxisFlag( true )
    ,cFlagLightColor( true)
     //	 cGridMode( ModeGrid::GRID_3D )
    ,cGridMode( ModeGrid::GRID_2D )
    ,cDebug(false)

    ,cFlagCursor3d(false)
  {
    gl_font( FL_HELVETICA_BOLD, 24);
    cKamera.initGL();
  }
  //---------------------------
  Canvas3d::~Canvas3d( )
  {

  }
  //---------------------------
  void Canvas3d::traceMode() const
  {
    switch( cMode )
      {
      case ModeUser::MODE_BASE:
	DBG( "MODE_BASE  ");
	break;
      case ModeUser::MODE_SELECT:
	DBG( "MODE_SELECT  ");
	break;
      case ModeUser::MODE_SELECT_RECT:
	DBG( "MODE_SELECT_RECT  ");
	break;
      case ModeUser::MODE_MOVE_CAMERA:
	DBG( "MODE_MOVE_CAMERA  ");
	break;
      case ModeUser::MODE_TRANSFORM :
	DBG( "MODE_TRANSFORM  " );
	break;
      }
  }
  //---------------------------
  PP3d::DataBase& Canvas3d::getDataBase() { return *Application::Instance().getDatabase(); }
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
    static float lModelAmbient[] {0.2, 0.2, 0.2, 1 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lModelAmbient );
    //===================================================================================

    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable( GL_DEPTH_TEST );


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cKamera.execGL();


    //===========================================================
    glLineWidth( 1 );

    if( cGridMode == ModeGrid::GRID_2D )
      {
	glColor4f( 0.4, 0.4, 0.4, 0.5 );
	PP3d::GLUtility::DrawGrid( cKamera.getSize()*10, 5, false);
	//	if( cGridMode == ModeGrid::GRID_3D )
	//				T3dPrimitiv::DrawNappeT3d( 1200, 6);
      }

    if( cAxisFlag )
      PP3d::GLUtility::DrawOrtho( cKamera.getSize(), 1 );


    if( cFlagCursor3d )
      PP3d::GLUtility::DrawCursorCruz( Application::Instance().getDatabase()->getCursorPosition(), 50);



    cViewProps.cDebug = cDebug;

    Application::Instance().getDatabase()->recomputeAll();
    Application::Instance().getDatabase()->drawGL( cViewProps, PP3d::GLMode::Draw );

    if( cMode == ModeUser::MODE_SELECT_RECT )
      {

	glEnable( GL_BLEND );
	glDepthMask( GL_FALSE );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );

	glColor4f(0.3, 0.1, 0.1, 0.3);

	Application::Instance().getDatabase()->getSelectionRectanglePosition().drawGL();

	glDepthMask( GL_TRUE );
	glDisable( GL_BLEND );

      }
    glFlush();
  }
  //---------------------------
  void Canvas3d::processHits( GLuint pNbHits, GLuint*  pSelectBuf, bool pFlagMove)
  {
    cout << " processHits=" <<  pNbHits <<  endl;

    GLuint*	ptr = (GLuint *) pSelectBuf;

    //		std::vector< std::unique_ptr<PP3d::PickingHit> > lVectHits;
    //		std::vector< PP3d::PickingHit* > lVectHits;
    std::vector< PP3d::PickingHit > lVectHits;

    for(GLuint i = 0; i < pNbHits; i++) // for each hit
      {
	GLuint lNbNames = *ptr;
	ptr++;

	cout << "Nb names :" << lNbNames << " >>> "  ;

	float lZ1 = (float) *ptr/0x7fffffff;		ptr++;
	float lZ2 = (float) *ptr/0x7fffffff;		ptr++;

	for (GLuint j = 0; j < lNbNames; j++) // for each name
	  {
	    GLuint lName = *ptr; ptr++;

	    //					cout << lName  << " Z:" << lZ1 << " -> " << lZ2 << endl;

	    if( lName != 0)
	      {
		//								PP3d::PickingHit* lHit = new PP3d::PickingHit( lName, lZ1, lZ2 );
		PP3d::PickingHit lHit( lName, lZ1, lZ2 );
		lVectHits.push_back(lHit);
	      }
	  }
      }

    if( lVectHits.size() )
      {										////				std::sort( lVectHits.begin(), lVectHits.end(), []( PP3d::PickingHit &A, PP3d::PickingHit &B) { return A.cZ1 < B.cZ2; });

	if( PP3d::Selection::Instance().selectPickingHit( lVectHits,
							  *Application::Instance().getDatabase(),
							  cSelectMode, pFlagMove ))
	  {
	    Application::Instance().validate( History::SaveMode::Mini, Application::DeferRedraw::DeferTrue );  /// PHIPO_TEST
	  }
      }
  }
  //---------------------------
  void Canvas3d::picking( int pX, int pY, bool pFlagMove )
  {
    //    DBG( "=== picking:" << pX << " " << pY << " SM:" << cSelectMode <<  std::endl;

    GLint lViewport[4];
    glGetIntegerv(GL_VIEWPORT, lViewport);


    int BUFSIZE = 4096;
    GLuint lSelectBuf[BUFSIZE];
    memset(&lSelectBuf, 0, BUFSIZE );

    glSelectBuffer(BUFSIZE, lSelectBuf);

    glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(0);

    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluPickMatrix((GLdouble) pX, (GLdouble) (lViewport[3] - pY),
		  10, 10, lViewport );

    cKamera.execGL( true );

    glMatrixMode(GL_MODELVIEW);

    Application::Instance().getDatabase()->drawGL( cViewProps, PP3d::GLMode::Select );

    glPopMatrix();
    glFlush();
    GLuint lNbHits = glRenderMode(GL_RENDER);
    processHits( lNbHits, lSelectBuf, pFlagMove  );

    //glPopMatrix();
    //glMatrixMode(GL_MODELVIEW);
    //GLuint lNbHits = glRenderMode(GL_RENDER);
    //glFlush();
    //processHits( lNbHits, lSelectBuf );
  }
  //---------------------------
  void Canvas3d::userPrepareAction( int	pEvent )
  {
    cMouseInitPosX = cMouseLastPosX = Fl::event_x();
    cMouseInitPosY = cMouseLastPosY = Fl::event_y();

    Application::Instance().currentTransform().raz();
  }
  //------------------------------
  void Canvas3d::userCancelAction(	int	pEvent )
  {
    if(cVisitModifSelect!= nullptr )
      {
	cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::CANCEL);
      }
    userTerminateAction(pEvent);
  }
 //------------------------------
  void Canvas3d::userTerminateAction(	int	pEvent )
  {
    DBG( "TERMINATE action" );

    cMouseInitPosX = cMouseLastPosX = -1;
    cMouseInitPosY = cMouseLastPosY = -1;
    cRectBeginX = -1;
    cRectBeginY = -1;
    Application::Instance().setCurrentTransformType( Transform::Nothing );
    Application::Instance().currentTransform().raz();

    PP3d::Point3d lVoid;
    Application::Instance().getDatabase()->setSelectionRectanglePosition( lVoid, lVoid );
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
	getKamera().angle()[1] += (cMouseLastPosX	-lX)/10.0;
	getKamera().angle()[0] += (lY-cMouseLastPosY)/10.0;

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


	cout << "angle X: " <<	getKamera().angle()[0]
	     << "angle Y: " <<	getKamera().angle()[1] ;
	redraw();
	cMouseLastPosX = lX;
	cMouseLastPosY = lY;
      }
  }
  //---------------------------
  bool Canvas3d::initDragSelect()
  {
    if( cDragPoints.size() == 0
	&& PP3d::Selection::Instance().getNbSelected() >0 )
      {
	// We keep all the adress of points of selected entities
	PP3d::GetPoints< PP3d::EntityPtrHash, PP3d::PointPtrSet>( PP3d::Selection::Instance().getSelection(),
								  cDragPoints );


	// Save the original coordinates of points
	cDragSavPoints.resize( cDragPoints.size() );
	int i=0;
	for( PP3d::PointPtr lPoint : cDragPoints )
	  {
	    cDragSavPoints[i++] = lPoint->get();
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
	int i=0;
	for( PP3d::PointPtr lPoint : cDragPoints )
	  {
	    lPoint->get() = 	cDragSavPoints[i++] * pMat;
	  }
      }
  }
  //---------------------------
  void Canvas3d::validDragSelect( PP3d::Mat4 &pMat )
  {
    dragSelect( pMat ); // Inutile ?
    cDragPoints.clear();
    cDragSavPoints.clear();
  }
  //---------------------------
  void Canvas3d::cancelDragSelect()
  {
    if( cDragPoints.size() != 0 )
      {
	int i=0;
	for( PP3d::PointPtr lPoint : cDragPoints )
	  {
	    lPoint->get() = 	cDragSavPoints[i++];
	  }
	cDragPoints.clear();
	cDragSavPoints.clear();
      }
  }
  //---------------------------
  void Canvas3d::userTransformSelection(int	pEvent, bool pFlagFinalize)
  {
    int lX = Fl::event_x();
    int lY = Fl::event_y();

    float lDx = (lX - cMouseLastPosX);
    float lDy = (cMouseLastPosY - lY);
    cMouseLastPosX = lX;
    cMouseLastPosY = lY;

    DBG( "***** Canvas3d::userTransformSelection **** " << lDx << " " << lDy );


    PP3d::Mat4 lMatTran;
    lMatTran.Identity();

    switch( Application::Instance().getCurrentTransformType() )
      {
      case Transform::Nothing:
	DBG( "Nothing to do !!!" );
	return ;

	//================
      case Transform::MoveNormal:
	{
	  if(cVisitModifSelect == nullptr )
	    {
	      if( PP3d::Selection::Instance().getSelectType() >=  PP3d::SelectType::Facet)
		{
		  cVisitModifSelect = new PP3d::VisitorFacetMoveNormal();
		}
	      else
		{
		  cVisitModifSelect = new PP3d::VisitorPointLineMoveNormal();
		}

	      cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::SAV);
	    }

	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::CANCEL); // remise a zero des modifs
	  cVisitModifSelect->setCoef( (cMouseLastPosX-cMouseInitPosX)/300.0 );
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::MODIF);
	  return; //////////// ATTENTION
	}
	break;
	//================

      case Transform::MoveX:
	{
	  Application::Instance().currentTransform().position().x() += lDx/10;
	  lMatTran.initMove( Application::Instance().currentTransform().position().x(), 0, 0 );
	}
	break;

      case Transform::MoveY:
	{
	  Application::Instance().currentTransform().position().y() += lDy/10;
	  lMatTran.initMove( 0, Application::Instance().currentTransform().position().y(), 0 );
	}
	break;

      case Transform::MoveZ:
	{
	  Application::Instance().currentTransform().position().z() += lDx/10;
	  lMatTran.initMove( 0, 0, Application::Instance().currentTransform().position().z() );
	}
	break;


      case Transform::CenterRotX :
      case Transform::CenterRotY :
      case Transform::CenterRotZ :
	{
	  PP3d::Point3d lCenter =    PP3d::Selection::Instance().getCenter( *Application::Instance().getDatabase() );

	  DBG( "Center:" << lCenter );
	  PP3d::Mat4 lMatRecenter;
	  lMatRecenter.initMove( lCenter ); //on revient au centre;


	  PP3d::Point3d lNCenter =  -lCenter;
	  DBG( " Neg:" << lNCenter );
	  PP3d::Mat4 lMatZero;
	  lMatZero.initMove( lNCenter ); //on se positionne en zero;

	  PP3d::Mat4 lMatRot;
	  switch( Application::Instance().getCurrentTransformType() )
	    {
	    case Transform::CenterRotX :
	      Application::Instance().currentTransform().angle().x() += lDx/360;
	      lMatRot.initRotX( Application::Instance().currentTransform().angle().x() );
	      break;

	    case Transform::CenterRotY :
	      Application::Instance().currentTransform().angle().y() += lDy/360;
	      lMatRot.initRotY( Application::Instance().currentTransform().angle().y() );
	      break;
	    case Transform::CenterRotZ :
	      Application::Instance().currentTransform().angle().z() += lDx/360;
	      lMatRot.initRotZ( Application::Instance().currentTransform().angle().z() );
	      break;
	    default:;
	    }

	  lMatTran = lMatRecenter * lMatRot *  lMatZero;

	}
	break;

      default:
	DBG( "Nothing to do 2!!!" );
      }

    //		PP3d::Mat4 lTmp;
    //		lTmp.set( Application::Instance().getCurrentTransform() );
    // Faire une sauvegarde de la selection
    // ou alors utiliser des matrice opengl pour chaque objet a modifier (y compris les points !!!!)?
    DBG1( "pos:" << Application::Instance().currentTransform().position() );


    if( pFlagFinalize == false )
      {
	//				PP3d::Selection::Instance().setCurrentMatrice( &lMatTran );
	dragSelect( lMatTran );
      }
    else
      {
	DBG1( "============= VALIDATE ===================" );
	//			cout << lMatTran << endl;
	validDragSelect( lMatTran );
	DBG1( "============= VALIDATE ===================" );

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


    DBG( "==========================================================================" );
    // DBG( "1----> (" << lResult0.cX<< "  " << lResult0.cY << "  " << lResult0.cZ << " ) " );

    // On projette ensuite ce point dans l'espace 3d

    PP3d::Point3d pResult;

    DBG( "lX:" << lX << " lY:" << lY );
    getKamera().projectWinToObject( PP3d::Point3d( lX, lY, lResult0.cZ), pResult, true);
    DBG( " --> (" << pResult.cX<< "  " << pResult.cY << "  " << pResult.cZ << " ) " );

    // Pour verifier on fait l'operation inverse
    getKamera().projectObjectToWin( pResult, lResult0, true);
    DBG( " ==> (" << lResult0.cX<< "  " << lResult0.cY << "  " << lResult0.cZ << " ) " );

    DBG( "==========================================================================" );


    Application::Instance().getDatabase()->setCursorPosition(	pResult );
    Application::Instance().setCursorPosition( pResult );
    {
      std::ostringstream lOsLuaCode;
      std::ostringstream lOsLuaOut;

      //      lOsLuaCode << "ShapeAddCurrentPoint("<<  pResult.cX << ',' << pResult.cY << ',' <<  pResult.cZ <<')');
      //      lOsLuaCode << "OxyzRedrawCanvas()");
      if( Application::Instance().execLuaHisto(lOsLuaCode, lOsLuaOut) !=0)
				{
				}
    }
    //		cDataBase.addPointToCurrentLine( pResult );
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
    Application::Instance().getDatabase()->setCursorPosition(	pResult );
    Application::Instance().setCursorPosition( pResult );
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

    Application::Instance().getDatabase()->setSelectionRectanglePosition( lBegin, lCurrent );
    if( pFlagFinalize )
      {
	//			PP3d::Selection::Instance().selectRect( cDataBase.getSelectionRectanglePosition() );
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


	cDataBase.selectPoint( PP3d::Point3d(lX, lY, lResult0.cZ), cKamera, PP3d::SelectType::Object );*/

    picking( Fl::event_x(),  Fl::event_y(), pFlagMove )	;
  }
  //---------------------------------------------------------
  int Canvas3d::handle( int pEvent	)
  {
    //		cout << endl;
    if( pEvent == FL_NO_EVENT ) return 1;

    traceMode();


    DBG( " <<<Event:" << pEvent << " " << fl_eventnames[pEvent] << ">>> " );


    switch( pEvent )
      {
	//===========================

      case FL_PUSH :
	DBG( " Button Push"  );
	Fl::focus(this);


	//		DBG( " ctrl: " << Fl::event_ctrl() << " " ;

	// SAISIE DE POINT
	if( Fl::event_button() == FL_LEFT_MOUSE
	    &&  Fl::event_ctrl() &&  cMode == ModeUser::MODE_BASE)
	  {
	    DBG( " **************** cUserActionSaisie " );

	    userInputPoint( pEvent );

	    return 1;
	  }

	// SELECTION RECTANGLE : BUG
	if( Fl::event_button() == FL_LEFT_MOUSE
	    &&  Fl::event_shift() &&  cMode == ModeUser::MODE_BASE)
	  {
	    DBG( " cUserActionRectangle " );
	    cMode = ModeUser::MODE_SELECT_RECT;

	    userPrepareAction( pEvent );

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
	cout << "******** Button Release ";

	switch(  Fl::event_button() )
	  {
	  case FL_LEFT_MOUSE :
	    if( cMode == ModeUser::MODE_SELECT )
	      {
		userTerminateAction( pEvent );
	      }

	    DBG( "LEFT ");
	    break;
	  case FL_MIDDLE_MOUSE :
	    DBG( "MIDDLE ");
	    break;
	  case FL_RIGHT_MOUSE :
	    DBG( "RIGHT ");

	    break;
	  }

	if(  cMode == ModeUser::MODE_TRANSFORM
	     && Application::Instance().getCurrentTransformType() != Transform::Nothing )
	  {
	    userTransformSelection(pEvent, true );
	    userTerminateAction( pEvent );
	    Application::Instance().validate( History::SaveMode::Diff, Application::DeferRedraw::DeferTrue ); // PHIPO_TEST
	  }

	if( cMode == ModeUser::MODE_SELECT_RECT )
	  {
	    userSelectionRectangle(pEvent, true );
	    userTerminateAction( pEvent );
	  }


	break;
	//==============================

      case FL_DRAG:
	Fl::focus(this);
	cout << " <DRAG> " << cMouseLastPosX;
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
	  }
	setCursor3dPosition( Fl::event_x(), Fl::event_y());
	Application::Instance().redrawAllCanvas3d(); // a cause du curseur ou 				break;

	//==============================

      case FL_MOVE:
	{
	  cout << " <MOVE> " << cMouseLastPosX <<  std::endl;
	  if( cMouseLastPosX != -1 )
	    {
	      if( cMode == ModeUser::MODE_MOVE_CAMERA )
		{
		  userChangeKameraView( pEvent );
		  Application::Instance().redrawAllCanvas3d(); // a cause du curseur ou du rectangel etc
		}
	      else if( cMode == ModeUser::MODE_SELECT_RECT )
		{
		  userSelectionRectangle(pEvent);
		  Application::Instance().redrawAllCanvas3d(); // a cause du curseur ou du rectangel etc
		}

	    }
	  else
	    if( cMode == ModeUser::MODE_BASE )
	      {
		userSelectionPoint( pEvent, true );
	      }

	  setCursor3dPosition( Fl::event_x(), Fl::event_y());
	  Application::Instance().redrawAllCanvas3d(); // a cause du curseur ou 				break;


	  //			cout << " Move : x="<< lX << " y=" << lY <<  std::endl;
	  //			cout << " Cursor :  x=" << pResult.cX << " y=" << pResult.cY << " z=" << pResult.cZ );
	}
	break;
	//==============================

      case FL_ENTER:
	cout << " ENTER " << Fl::belowmouse();
	//			Fl::focus(Fl::belowmouse());
	break;
      case FL_LEAVE:
	cout << " LEAVE " << Fl::belowmouse();
	break;

      case FL_FOCUS:
	cout << " FOCUS " ;
	break;
      case FL_UNFOCUS:
	cout << " UNFOCUS " ;
	break;

      case FL_KEYDOWN:
	{
	  cout << " CTRL :" << Fl::event_ctrl() << " " ;
	  cout << " KEYDOW " <<  Fl::event_key()  ;

	  if(  Fl::event_key() >= 256 )
	    {
	      switch(  Fl::event_key() )
		{
		case FL_Escape:
		  userCancelAction( pEvent );
		  break;
		case	FL_Tab:
		  cout << " TAB";
		  break;
		case FL_BackSpace:
		case FL_Delete:
		  Application::Instance().getDatabase()->delPointToCurrentLine();
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

	      if( strcmp( lStr, ANNULE_ACTION )==0)
		{
		  userTerminateAction( pEvent );
		  Application::Instance().setCurrentTransformType( Transform::Nothing );
		}
	      else if( strcmp( lStr, UNSELECT_ALL) == 0 )
		{
		  if( cMode == ModeUser::MODE_BASE )
		    PP3d::Selection::Instance().removeAll();
		}
	      else if( strcmp( lStr, MOVE_Z_N )==0)
		{
		  cKamera.position()[2] -= 1;
		}
	      else if( strcmp( lStr, MOVE_Z_P )==0)
		{
		  cKamera.position()[2] += 1;
		}
	      else if( strcmp( lStr, RESET_VIEW_TO_X )==0)
		{
		  cKamera.razX();
		}
	      else if( strcmp( lStr, RESET_VIEW_TO_Y )==0)
		{
		  cKamera.razY();
		}
	      else if( strcmp( lStr, RESET_VIEW_TO_Z )==0)
		{
		  cKamera.razZ();
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
	      else if( strcmp( lStr, STR_EXIT ) ==0)
		{
		  ::exit(0);
		}
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
	    cout << " WHEEL:" << Fl::event_dx() << ":" <<  Fl::event_dy() ;

	    cScale = cKamera.scale().x();

	    cScale += Fl::event_dy()/10.0;

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


}; //namespace PP3d

