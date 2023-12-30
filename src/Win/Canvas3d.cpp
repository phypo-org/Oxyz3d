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
 
   bool sDrawColorSelect=false;

  const char * const ANNULE_ACTION="e";
  
  const char * const CHG_AXIS="a";
  
  const char * const CENTER_ON_SELECTION="A";
 
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
  
  const char * const KEY_UNDO="/7a";

  
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
 
    cViewPropsTransform.cLineWidth = 4; 
    cViewPropsTransform.cColorLine.set(0.,1.0,0.);
     cViewPropsTransform.cColorLineSelect.set(0.,0.8,0.);
     
    cViewPropsTransform.cColorFacet.set(0.,0.7,0.);
    cViewPropsTransform.cColorFacetSelect.set(0.,1.0,0.);
    cViewPropsTransform.cColorLineHighlight.set(0.1,1.0,0.1);
    
   
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
	switch( getUserMode()  )
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
	  case ModeUser::MODE_DRAG :
	    std::cout << "MODE_DRAG " ;
	    break;
	  }
      }
  }
 
  //---------------------------
  void Canvas3d::userPrepareAction( int	pEvent )
  {	
    cMouseInitPosX = cMouseLastPosX = Fl::event_x();
    cMouseInitPosY = cMouseLastPosY = Fl::event_y();

    TheAppli.currentTransform().raz();
    TheAppli.currentValTransf()= 0;
    TheAppli.currentTransform().scaleTo(1);
  }
  //------------------------------
  void Canvas3d::userCancelAction(	int	pEvent )
  {  
    if(cVisitModifSelect!= nullptr )
      {
	cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::CANCEL, TheSelect );
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
    TheAppli.currentValTransf() = 0;
    TheAppli.currentTransform().raz();

    PP3d::Point3d lVoid;
    changeUserMode(  ModeUser::MODE_BASE );
    cancelDragSelect();					
    cSelectMode = PP3d::SelectMode::Undefine;
    if(cVisitModifSelect != nullptr )
      {
	delete cVisitModifSelect;
	cVisitModifSelect = nullptr;
      }
  }									 
  //---------------------------
  void Canvas3d::userChangeKameraView( int pEvent)
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
      }
    redraw();
    cMouseLastPosX = lX;
    cMouseLastPosY = lY;      
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
      case InputPlaneType::Free : {
	    double lH = pixel_h();
	    double lX = ((double )Fl::event_x());
	    double lY = lH-((double) Fl::event_y());
	    PP3d::Point3d lPt0;
	    PP3d::Point3d lResult0;
	    getKamera().projectObjectToWin( lPt0, lResult0, true);
	    getKamera().projectWinToObject( PP3d::Point3d( lX, lY, lResult0.cZ), iResult, true);
	return true;
      }
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


  //---------------------------------------------------------
  int Canvas3d::handle( int pEvent ) 
  {
     TheSelect.removeHightLightEntity( TheBase );
   
    //		cout << endl;
    if( pEvent == FL_NO_EVENT ) return 1;
    
    bool lFlagRedrawAll = false;
    
    traceMode();
    
    DBG_EVT( " <<<Event:" << pEvent << " " << fl_eventnames[pEvent] << ">>> "
	     << " button:" << Fl::event_button() 
	     << " ctrl: " << Fl::event_ctrl()
	     << " shift:" <<  Fl::event_shift()
	     << " alt:" <<  Fl::event_alt());    
    TheInput.hideCurrentPoint();
    
    switch( pEvent )
      {
	//===========================	
      case FL_PUSH :
	DBG_EVT( "-------------- Button Push "  << Fl::event_button()  << " " << Fl::event_shift());
	Fl::focus(this);     


	// SAISIE DE POINT
	if( Fl::event_button() == FL_LEFT_MOUSE
	    &&  Fl::event_ctrl() )
          {
            if(  getUserMode() == ModeUser::MODE_BASE)
            {
              if( getSubMode() == SubModeUser::SUBMODE_MAGNET )
                {
                   DBG_ACT(" **************** MAGNET ");
                  return 1;
                }
              if( getSubMode() == SubModeUser::SUBMODE_INPUT_PT )
                {
                  DBG_ACT(" **************** cUserActionSaisie " );
                  std::cout << " **************** cUserActionSaisie " << std::endl;
                  userInputPoint(true );              
                  return 1;
                }
            }
 
      }
	if( Fl::event_button() == FL_LEFT_MOUSE
	    &&  Fl::event_shift() &&  getUserMode() == ModeUser::MODE_BASE)
	  {
	    PP3d::EntityId lId =TheSelect.getLastHightLightEntityId();
	    
	    std::cout << " **************** cUserActionSaisie SHIFT " << lId  << std::endl;
	    DBG_ACT(" **************** cUserActionSaisie Hightlight  "  << lId );
	    
	    userInputPoint(  TheAppli.getDatabase()->findEntity( lId)  );

	    return 1;
	  }
				
	// SELECTION RECTANGLE : BUG
	if( Fl::event_button() == FL_MIDDLE_MOUSE
	    &&  Fl::event_shift() ) // &&  getUserMode() == ModeUser::MODE_BASE)
	  {	  
	    changeUserMode( ModeUser::MODE_SELECT_RECT );

	    userPrepareAction( pEvent );
	    TheAppli.redrawAllCanvas( PP3d::Compute::Nothing);
	    return 1;
	  }

	// DEBUT MODE DEPLACEMENT CAMERA
	if( Fl::event_button() == FL_MIDDLE_MOUSE )
	  {
	    userPrepareAction( pEvent );
	    if( getUserMode() == ModeUser::MODE_BASE )
	      changeUserMode( ModeUser::MODE_MOVE_CAMERA );
	    return 1;
	  }

	if( Fl::event_button() == FL_LEFT_MOUSE &&  getUserMode() == ModeUser::MODE_TRANSFORM )
	  {
	    userPrepareAction( pEvent );
	    return 1;
	  }
	


	// FIN DEPLACEMENT CAMERA OU SELECTION
	if( Fl::event_button() == FL_LEFT_MOUSE )
	  {
	    if( getUserMode() == ModeUser::MODE_MOVE_CAMERA )
	      {
		changeUserMode( ModeUser::MODE_BASE );
		userTerminateAction( pEvent );
		return 1;
	      }
	    else  //DEBUT MODE_SELECT
	      if( getUserMode() == ModeUser::MODE_BASE )
		{
		  cSelectMode = PP3d::SelectMode::Undefine;
		  userPrepareAction( pEvent );
		  if( userSelectionPointColor( pEvent, false ))
		    changeUserMode( ModeUser::MODE_SELECT);
		  else
		    changeUserMode( ModeUser::MODE_SELECT_RECT);

		    
		  return 1;
		}
	
	    userPrepareAction( pEvent );
						
	    return 1;
	  }

	// MENU
	if(  Fl::event_button() == FL_RIGHT_MOUSE )
	  {
	    cPopup->clear();						
						
						
	    if( getUserMode() == ModeUser::MODE_BASE )
	      {
		makeMenu( *cPopup); 
	      }
	  }
	cPopup->position( Fl::event_x() , Fl::event_y());
			
	cPopup->popup();

						
	return 1;
						
			
	break;
	//========================================
	//========================================
	//========================================

      case FL_RELEASE:
	DBG_EVT( "******** Button Release ");      
					
	switch(  Fl::event_button() )
	  {
	  case FL_LEFT_MOUSE :
	    if( getUserMode() == ModeUser::MODE_SELECT )
	      {
		userTerminateAction( pEvent );
	      }
	    else
	    if( getUserMode() == ModeUser::MODE_DRAG )
	      {
		cout << "MODE_DRAG - RELEASE " << endl;

		userDragInputPt(pEvent, true );
		userTerminateAction( pEvent );
		changeUserMode( ModeUser::MODE_BASE );
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
				
	if(  getUserMode() == ModeUser::MODE_TRANSFORM
	     && TheAppli.getCurrentTransformType() != Transform::Nothing )
	  {
	    userTransformSelection(pEvent, true );
	    userTerminateAction( pEvent );
	    TheAppli.redrawAllCanvas( PP3d::Compute::FacetAll);
	  }
				
	if( getUserMode() == ModeUser::MODE_SELECT_RECT )
	  {
	    userSelectionRectangle(pEvent, true );
	    userTerminateAction( pEvent );
	  }		
	break;
	//========================================
	//========================================
	//========================================

      case FL_DRAG:
	Fl::focus(this);
	DBG_EVT( " <DRAG> " << cMouseLastPosX );
	
	if( cMouseLastPosX != -1 )
	  {
	    if( getUserMode() == ModeUser::MODE_DRAG)
	      {
		userDragInputPt(pEvent, false);
	      }
	    else
	    if( getUserMode() == ModeUser::MODE_TRANSFORM )
	      {
		userTransformSelection(pEvent);
	      }
	    else
	      if( getUserMode() == ModeUser::MODE_MOVE_CAMERA )
		{
		  userChangeKameraView( pEvent );
		}					
	      else
		if( getUserMode() == ModeUser::MODE_SELECT )
		  {
		    userSelectionPointColor( pEvent, false );
		  }
		else
		  if( getUserMode() == ModeUser::MODE_SELECT_RECT )
		    {
		      cMouseLastPosX = Fl::event_x();
		      cMouseLastPosY = Fl::event_y();
		      userSelectionRectangle(pEvent);			
		    }	    
	  }
	setCursor3dPosition( Fl::event_x(), Fl::event_y());			 					
	TheAppli.redrawAllCanvas(PP3d::Compute::Nothing); // a cause du curseur ou 				break;
	break;
      				
	//====================================
	//====================================
	//====================================
				
      case FL_MOVE:
	{
	  DBG_EVT( " <MOVE> " << cMouseLastPosX );
	  if( cMouseLastPosX != -1 )
	    {					 	
	      if( getUserMode() == ModeUser::MODE_MOVE_CAMERA )
		{
		  userChangeKameraView( pEvent );
		  TheAppli.redrawAllCanvas(PP3d::Compute::Nothing); // a cause du curseur ou du rectangel etc
		}
	    }
	  else
	    if( getUserMode() == ModeUser::MODE_BASE )
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
		TheAppli.redrawAllCanvas(PP3d::Compute::Nothing); // a cause du curseur ou 				break;
		
	
		//			cout << " Move : x="<< lX << " y=" << lY <<  std::endl;
		//			cout << " Cursor :  x=" << pResult.cX << " y=" << pResult.cY << " z=" << pResult.cZ << std::endl;
	      }
	  break;
	}
	//====================================
	//====================================
	//====================================
			
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
	
	//====================================
	//====================================
	//====================================
				
      case FL_KEYDOWN:
	{
	  DBG_EVT_NL( " CTRL :" << Fl::event_ctrl() );
	  DBG_EVT_NL( " SHIFT :" << Fl::event_shift() );
	  DBG_EVT_NL( " KEYDOW " <<  Fl::event_key()  );

	  cout << " key<" << Fl::event_key() << "> " ;
	  
	  if(  Fl::event_key() >= 256 )
	    {
	      switch(  Fl::event_key() )
		{
		  //=======================
		case FL_Escape:
		  userCancelAction( pEvent );
		  break;
		  //=======================
		case	FL_Tab:
		  DBG_EVT_NL( " TAB" );

                  cout << "TAB " << endl;
		  if( getUserMode() == ModeUser::MODE_TRANSFORM )
		    {
                      //CallDialogKeepFloatInit( this );
		      userTransformSelectionInput(pEvent);
		    }		  
		  break;
		  //=======================
		case FL_Insert:
		  if( getUserMode() == ModeUser::MODE_DRAG )
		    {
		      DBG_ACT( "Canvas3d::handle FL_Insert" );
		      
		      PP3d::Point3d lResult;
		      if(  Fl::event_shift()  )
			{
			  PP3d::EntityId lId =TheSelect.getLastHightLightEntityId();
			  PP3d::Entity* iEntity = TheAppli.getDatabase()->findEntity( lId);
			  if( iEntity != nullptr)
			    {
			      PP3d::SortEntityVisitorPoint  lVisit;
			      iEntity->execVisitor( lVisit );

			      for( PP3d::PointPtr lPt : lVisit.cVectPoints) // les points sont uniques
				{
				  TheInput.insertCurrentLineSelectPoint( lPt->get(), TheBase );	
				}
			      lFlagRedrawAll = true;
			    }
	  
			}
		      else
			{		      
			  if( setCursor3dPosition(Fl::event_x(), Fl::event_y() ))
			    {
			      PP3d::Point3d lResult = TheAppli.getDatabase()->getCursorPosition();			
			      TheAppli.roundInput( lResult );
			      TheInput.insertCurrentLineSelectPoint( lResult, TheBase); 
			      lFlagRedrawAll = true;
			    }
			}
		    }
		  break;
		  //=======================
		case FL_BackSpace:
		case FL_Delete:
		  if( getUserMode() == ModeUser::MODE_DRAG )
		    {
		      TheInput.delCurrentLineSelectPoint(TheBase); 
		    }
		  else if( getUserMode() == ModeUser::MODE_BASE )
		    {		      
		      TheInput.delLastPoint ();
		    }
		  
		  lFlagRedrawAll = true;
		  break;
		  //=======================
		case FL_Up:
		case FL_Down:
		  break;

		case 0xffab:
		  SelFunct::SelectMore( TheSelect, TheBase );
		    
		  TheAppli.redrawAllCanvas(PP3d::Compute::FacetAll);
		  TheAppli.redrawObjectTree();
		  break;
		
		case 0xffad:
		  SelFunct::SelectLess( TheSelect, TheBase );
		    
		  TheAppli.redrawAllCanvas(PP3d::Compute::FacetAll);
		  TheAppli.redrawObjectTree();
		  break;
		  
		} // end case
	    }
	  else
	    //			if( Fl::event_ctrl() != 0 )
	    {
	      const char *lStr = Fl::event_text();
	      cout << " txt<" << lStr << "> " ;
					
	      if( strcmp( lStr, ANNULE_ACTION )==0)
		{
		  userTerminateAction( pEvent );
		  TheAppli.setCurrentTransformType( Transform::Nothing );
		}
	      else if( strcmp( lStr, UNSELECT_ALL) == 0 )
		{
		  if( getUserMode() == ModeUser::MODE_BASE )
		    {
                      PushHistory();
		      TheSelect.removeAll();
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
	      else if( strcmp( lStr, KEY_UNDO)==0)
		{
		  cout << "UNDO" << endl;
		  if(  Fl::event_ctrl()  )
		    {
		      cout << "CTRL" << endl;
		      if( Fl::event_shift() )
			{
			  cout << "Shift" << endl;
			  RedoCB( &cMyWin3d, nullptr ); 
			}
		      else
			{
			  UndoCB( &cMyWin3d, nullptr  ); 
			  
			}
		    }
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
	      else if( strcmp( lStr, CHG_AXIS )==0 || strcmp( lStr,CENTER_ON_SELECTION) == 0 )
		{
                  //                  if( Fl::event_shift() )
                    {
                      cMyWin3d.recenterOnSelection(cKamera);
                    }
                    /*     else
		  if( cAxisFlag )
		    cAxisFlag = false;
		  else 
		    cAxisFlag = true;
                    */
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
		  }
	    
	      /*
		else if( strcmp( lStr, STR_EXIT ) ==0)
		{
		::exit(0);
		}
	      */
	    }
	if( lFlagRedrawAll )
	  TheAppli.redrawAll(  PP3d::Compute::Nothing );
	else
	  redraw();
	}
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
	if( getUserMode() == ModeUser::MODE_MOVE_CAMERA )
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

