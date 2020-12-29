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




using namespace std;


namespace M3d {

  static bool sDrawColorSelect=false;

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
  const char * const BASCULE_DRAW_SELECT_COLOR="!";
  const char * const BASCULE_TEST_SELECT_COLOR=":";

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
    
    cViewInputCursor.cColorPoint.set( 1, 0.9, 0.1, 0.8);
    cViewInputCursor.cColorLine.set( 0.9, 0.5, 0.3, 0.8);
    
    cViewInputPoly.cColorPoint.set( 0.7, 0.4, 0.1, 0.7);
    cViewInputPoly.cColorLine.set( 0.6, 0.5, 0.2, 0.7 );    
    
    cViewInputObject.cColorPoint.set( 0.4, 1, 0.4, 0.7);
    cViewInputObject.cColorLine.set( 0.4, 0.8, 0.3, 0.6);
    cViewInputObject.cColorFacet.set( 0.2, 0.4, 0.2, 0.5 );
    

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
  void Canvas3d::drawSelectRect()  // 2D !
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
				
    glColor4f(0.9f, 0.1f, 0.1f, 0.1f);
    glDisable( GL_LIGHTING );


    PP3d::Point3d lMin( cMouseInitPosX, lH-cMouseInitPosY, 0 );
    PP3d::Point3d lMax( cMouseLastPosX, lH- cMouseLastPosY, 0);

    //    cout << "min:" << lMin << " max:" << lMax << endl;
       
    PP3d::Rect3d lRect(lMin, lMax);
    lRect.drawGL();

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
    

    //===========================================================	
    drawGrid();

    if( cAxisFlag )
      PP3d::GLUtility::DrawOrtho( (float)8/cKamera.scale().x(), 1, 2 );  
		
		
    if( cFlagCursor3d )
      {
	PP3d::GLUtility::DrawCursorCruz2( TheAppli.getDatabase()->getCursorPosition(), 50);
      }

		
    //  cViewProps.cDebug = cDebug;
    cViewGen.cFlagViewNormal = cFlagViewNormal;
 

    // Draw the 3d view of object
    TheAppli.getDatabase()->recomputeAll();     
    TheAppli.getDatabase()->drawGL( cViewGen, cViewInputCursor, cViewInputPoly, cViewInputObject,  PP3d::GLMode::Draw  , TheSelect.getSelectType() );


    // Draw transformation
    if( cFlagViewTransform )
      {
	TheAppli.getDatabaseTransform()->recomputeAll();
	TheAppli.getDatabaseTransform()->drawGL( cViewPropsTransform, cViewInputCursor, cViewInputPoly, cViewInputObject, PP3d::GLMode::Draw, TheSelectTransform.getSelectType() ); 
      }

    // draw rectangle selection if needed
    if( cMode == ModeUser::MODE_SELECT_RECT )
      {	
	drawSelectRect();
      }
  
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
    if( TheSelectTransform.getSelectType() == PP3d::SelectType::Null )  // pour les transformations
      {
	TheAppli.getDatabase()->recomputeAll();     
	TheAppli.getDatabase()->drawGL( cViewGen, cViewGen, cViewGen, cViewGen, PP3d::GLMode::Select, TheSelect.getSelectType() );
      }
    /*   else
      {
	TheAppli.getDatabaseTransform()->recomputeAll();
	TheAppli.getDatabaseTransform()->drawGL( cViewPropsTransform, PP3d::GLMode::Select, TheSelectTransform.getSelectType() ); 
      }
    */
    glFlush();
    //glFinish(); 

    //Faire ce qu'il faut pour trouver la selection
  }  
  //---------------------------
  bool Canvas3d::selectEntity( PP3d::EntityId iId, bool pFlagMove  )
  {
    bool lFlagRedraw = false;
    
    ////	cout << "Find Id : " << lId << endl;
    PP3d::EntityPtr lEntity = TheAppli.getDatabase()->findEntity( iId );
    if( lEntity != nullptr )
      {
	//	cout << "********** Find Entity : " << iId <<  endl;	       	
	PP3d::Selection * lSelect = &TheSelect;
	PP3d::DataBase  * lBase   = TheAppli.getDatabase();
	
	if( TheSelectTransform.getSelectType() != PP3d::SelectType::Null )
	  { // Selection de transformation
	    lSelect = &TheSelectTransform;
	    lBase   = TheAppli.getDatabaseTransform();		
	  }
	////    cout <<" processHits before Select" <<endl;
	
	//	PP3d::SelectMode lSelectMode = 	PP3d::SelectMode::Select;
	
	lFlagRedraw = lSelect->selectPickingColor(  lEntity,
						    *lBase,
						     cSelectMode,
						    pFlagMove );	
	////	    cout << " processHits after Select" <<endl;
	
	if( lFlagRedraw )	    
	  TheAppli.redrawAllCanvas3d();
      }
    return lFlagRedraw;
  }  
  //---------------------------
  bool Canvas3d::pickingColor( int pX, int pY, bool pFlagMove, int iSizeX, int iSizeY, bool pFlagRect  )
  {
    //      cout << ":pickingColor x:" << pX << " y:" << pY << " sX:" << iSizeX << " sY:" << iSizeY << " R:"<<  pFlagRect<< endl;
  
    drawForSelect(); // AJOUTER LA TAILLE
    glFinish(); 
  
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    if( pFlagRect == false )
      {
	unsigned char lData[4];
	glReadPixels( pX, pY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, lData);
	////	cout << "Data: " << std::hex << (int)lData[0] <<" " <<   (int)lData[1]<<" "  <<  (int)lData[2] <<" " <<  (int)lData[3]  << endl;


	PP3d::EntityId lId = PP3d::ColorRGBA::GetId( lData );

	if( lId == 0 )
	  {
	    return false;
	  }
	
	selectEntity( lId, pFlagMove); 
      }
    else
      {
	if( iSizeX <= 0 || iSizeY <=  0 )
	  {
	    return false;
	  }
	
	/*	cout << "Canvas3d::pickingColor move:" << pFlagMove
	     << " Pos X:"<< pX << " Y:" <<pY
	     << " SzX:"  << iSizeX << " SzY:" << iSizeY
	     << " pFlagRect:" << pFlagRect
	     << endl;
	*/
	
	auto luData = std::make_unique<unsigned char[]>( iSizeX*4 );
	PP3d::EntityId lMemoId = 0;
	
	for( int lY = pY; lY < pY+iSizeY; lY++ )
	  {
	    ::memset( luData.get(), 0xFF, iSizeX*4 );
	    //    cout << std::endl << "pickingColor lY:" << lY << std::endl;
	    glReadPixels( pX, lY, iSizeX, 1, GL_RGBA, GL_UNSIGNED_BYTE, luData.get() );
	    
	    for( int lX = 0; lX< iSizeX; lX++ )
	      {
		PP3d::EntityId lId = PP3d::ColorRGBA::GetId(  &(luData)[lX*4]);
		if( lMemoId != lId )
		  {
		    //	    cout << "pickingColor lY:" << lY << " lId:" << lId << std::endl;

		    lMemoId = lId;
		    selectEntity( lId, pFlagMove );
		  }
	      }
	  }
      }
    return true;
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
    
    TheAppli.setCurrentTransformType( Transform::Nothing );
    TheAppli.currentTransform().raz();

    PP3d::Point3d lVoid;
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
  void Canvas3d::userInputPoint( bool iFinalize )
  {     
    if( setCursor3dPosition(Fl::event_x(), Fl::event_y() ))
      {
	PP3d::Point3d lResult = TheAppli.getDatabase()->getCursorPosition();

	TheAppli.roundInput( lResult );
	
	if( iFinalize )
	  TheAppli.getDatabase()->addPointToCurrentLine( lResult );
	else
	  TheAppli.getDatabase()->viewCurrentPoint( lResult );
	
	
	TheAppli.redrawAllCanvas3d();
      }
  }
  /*
    //    cout << "---------------------------------------" << endl;
    
    //    cout << "P0 :" << lPt0 << " P1 :" << lPt1 << "-> R0:" << lR0 << " R1:" << lR1 	 <<  " Vn" << lVn << endl;
    //    cout << "P0W:" << lPt0W << " P1W:" << lPt1W << endl; 
    

    // On cherche l'intersection sur le plan xz, donc y==0
    // En parametrique l'equation de la droite : A=lR0+lVn * t
    // Pour y donc lR.cY+lVn.cY*t = 0 donc t = -lPT.cY/lVn.cY; il faut que lVn.cY!=0

 
    double lX = lR0.cX+lVn.cX*t;      
    double lY = lR0.cY+lVn.cY*t; // normalemnet 0 !
    double lZ = lR0.cZ+lVn.cZ*t;

    
    PP3d::Point3d lResult( lX, lY, lZ );
    PP3d::Point3d lResultW( lX, lY, lZ );
    getKamera().projectObjectToWin( lResult, lResultW , true);
    cout << "==>lResult:" << lResult <<  " W:" <<lResultW  << endl;

		 	
    TheAppli.getDatabase()->setCursorPosition (	lResult );
    TheAppli.setCursorPosition( lResult );
 
    if( iFinalize )
      TheAppli.getDatabase()->addPointToCurrentLine( lResult );
    else
      TheAppli.getDatabase()->viewCurrentPoint( lResult );

				
    TheAppli.redrawAllCanvas3d();
  }
  */
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
  bool Canvas3d::transform2Dto3D(  int pX, int pY,  PP3d::Point3d & iResult )
  {
    // On lance une droite à partir de la position de la souris
    // le Z n'est pas vraiment important (attention a la precision des doubles quand meme)
    
    PP3d::Point3d lPt0( Fl::event_x(), pixel_h()-Fl::event_y(),  pixel_h());
    PP3d::Point3d lPt1( Fl::event_x(), pixel_h()-Fl::event_y(), - pixel_h() );
    
    PP3d::Point3d lR0;
    PP3d::Point3d lR1;
    
    // On projette les deux points dans la base 3D
    getKamera().projectWinToObject( lPt0, lR0, true);
    getKamera().projectWinToObject( lPt1, lR1, true);

    // Calcul du vecteur de la droite
    PP3d::Line3dVect lLineV( lR0, lR1 );


    //  cout <<"transform2Dto3D Plane Height:" << TheAppli.getInputPlaneHeight() << endl;
    bool lOk = false;
    switch( TheAppli.getInputPlane() )
      {
      case InputPlaneType::X : lOk = lLineV.intersectPlanX( iResult, TheAppli.getInputPlaneHeight() );
	break;
      case InputPlaneType::Y : lOk = lLineV.intersectPlanY( iResult, TheAppli.getInputPlaneHeight() );
	break;
      case InputPlaneType::Z : lOk = lLineV.intersectPlanZ( iResult, TheAppli.getInputPlaneHeight() );
	break;  
      case InputPlaneType::Free : ;
      }

    return lOk;
  }
  //---------------------------
  bool Canvas3d::setCursor3dPosition( int pX, int pY )
  {
    PP3d::Point3d lResult;    
    
    if( transform2Dto3D( pX, pixel_h()-pY, lResult ) )
      {
	TheAppli.getDatabase()->setCursorPosition ( lResult );
	TheAppli.setCursorPosition( lResult );
	return true;
      }
    return false;
  }
  //---------------------------
  bool Canvas3d::userSelectionRectangle(int pEvent, bool pFlagFinalize )
  {
    //  cout << "userSelectionRectangle x:" <<  cMouseInitPosX  << " y:" << cMouseInitPosY
    //	 << "   x:" <<  cMouseLastPosX  << " y:" <<  cMouseLastPosY << endl;

    
    if( cMouseInitPosX == -1 )
      {
	cMouseInitPosX = Fl::event_x();
	cMouseInitPosY = Fl::event_y();			
      }     
    
    //   cout << "userSelectionRectangle x:" <<  cMouseInitPosX  << " y:" << cMouseInitPosY
    //	 << "   x:" <<  cMouseLastPosX  << " y:" <<  cMouseLastPosY << endl;

    int lLastY = pixel_h()-cMouseLastPosY;
    int lInitY = pixel_h()-cMouseInitPosY;
      
    //    float lMiddleX = (cMouseInitPosX + cMouseLastPosX)/2.0f;
    //    float lMiddleY = (lInitY + lLastY)/2.0f;
    float lMinX  = std::min( cMouseInitPosX , cMouseLastPosX );
    float lMinY  = std::min( lLastY , lInitY );
    float lMaxX  = std::max( cMouseInitPosX , cMouseLastPosX );
    float lMaxY  = std::max( lLastY , lInitY );
    float lSzX   = lMaxX - lMinX;
    float lSzY   = lMaxY - lMinY;
    
    //   cout << "userSelectionRectangle Middle x:" <<  lMinX<< " y:" <<lMinY
    //	 << "  Sz  x:" <<  lSzX  << " y:" <<  lSzY << endl;

    if( lSzX> 0 && lSzY > 0 )
      {
	if( pFlagFinalize )
	  {
	    pickingColor( lMinX, lMinY, false, lSzX, lSzY, true );
	    cMouseInitPosX = cMouseLastPosX = -1;
	    cMouseInitPosY = cMouseLastPosY = -1;
 	  }
	else
	  {
	    pickingColor( lMinX, lMinY, true, lSzX, lSzY, true );
	  }
      }
    return true;
  }
  //---------------------------------------------------------
  bool Canvas3d::userSelectionPointColor(int pEvent, bool pFlagMove)
  {
    return pickingColor( Fl::event_x(),   pixel_h() - Fl::event_y(), pFlagMove, MyPref.cSelectPickingSize, MyPref.cSelectPickingSize, false );
  }

  //---------------------------------------------------------
  int Canvas3d::handle( int pEvent ) 
  {
    //		cout << endl;
    if( pEvent == FL_NO_EVENT ) return 1;
    
    traceMode();
    
    DBG_EVT( " <<<Event:" << pEvent << " " << fl_eventnames[pEvent] << ">>> "
	     << " button:" << Fl::event_button() 
	     << " ctrl: " << Fl::event_ctrl()
	     << " shift:" <<  Fl::event_shift()
	     << " alt:" <<  Fl::event_alt());
    
    TheAppli.getDatabase()->hideCurrentPoint();
    
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
	    userInputPoint(true );
						
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
		  if( userSelectionPointColor( pEvent, false ))
		    cMode = ModeUser::MODE_SELECT;
		  else
		    cMode = ModeUser::MODE_SELECT_RECT;

		    
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
		    userSelectionPointColor( pEvent, false );
		  }
		else
		  if( cMode == ModeUser::MODE_SELECT_RECT )
		    {
		      cMouseLastPosX = Fl::event_x();
		      cMouseLastPosY = Fl::event_y();
		      userSelectionRectangle(pEvent);			
		      TheAppli.redrawAllCanvas3d(); // a cause du curseur ou du rectangel etc
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
	    }
	  else
	    if( cMode == ModeUser::MODE_BASE )
	      {
		if(  Fl::event_ctrl() )
		  { 
		    userInputPoint( false ); // just view th possible position of point
		  }
		else
		if( MyPref.cSelectPassOverLighting )
		  {
		    userSelectionPointColor( pEvent, true );
		  }
		

					
		setCursor3dPosition( Fl::event_x(), Fl::event_y());
		TheAppli.redrawAllCanvas3d(); // a cause du curseur ou 				break;
		
	
		//			cout << " Move : x="<< lX << " y=" << lY <<  std::endl;
		//			cout << " Cursor :  x=" << pResult.cX << " y=" << pResult.cY << " z=" << pResult.cZ << std::endl;
	      }
	  break;
	}
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
		    {
		      TheSelect.removeAll();
		      TheSelectTransform.removeAll();
		    }
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
	      else
		if( strcmp( lStr, BASCULE_DRAW_SELECT_COLOR ) ==0)
		  {
		    if( sDrawColorSelect )
		      sDrawColorSelect= false;
		    else
		      sDrawColorSelect = true;
		    redraw();		
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
	    cScale = cKamera.scale().x();
	    if( Fl::event_dy() != 0)
	      {
		//	    cout << " WHEEL x:" << Fl::event_dx() << " y:" <<  Fl::event_dy() << " " ;	  
		//	    cScale += Fl::event_dy()/10.0;
		// cScale *= 1.0-Fl::event_dy()/(MyPref.cMouseWheel+0.01);
		//		cout << 1.0-Fl::event_dy()/(MyPref.cMouseWheel+0.0001) << " " << endl;
		cScale *= 1.0-(Fl::event_dy()/(MyPref.cMouseWheel+0.000001));
	      }		
						
	    cKamera.setScale(cScale); // verify limits
	    redraw();
	  } 
	break;
      }
						
    return 1;
  }
  //***************************************


} //namespace 

