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
    
    cViewInputObject.cColorPoint.set( 0.4, 1, 0.4, 0.5);
    cViewInputObject.cColorLine.set( 0.4, 0.8, 0.3, 0.4);
    cViewInputObject.cColorFacet.set( 0.2, 0.4, 0.2, 0.3 );

    
    cViewInputObjectMagnet.cColorPoint.set ( 0.5, 0.5, 1, 0.5);
    cViewInputObjectMagnet.cColorLine.set  ( 0.4, 0.4, 1, 0.4);
    cViewInputObjectMagnet.cColorFacet.set ( 0.3, 0.3, 1, 0.3 );
    cViewInputObjectMagnet.cViewMode = PP3d::ViewMode::SKELETON;

      

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
  Magnet  & Canvas3d::getMagnet()  { return TheAppli.getMagnet(); }  
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
  void Canvas3d::userActionPrepare( int	pEvent )
  {	
    cMouseInitPosX = cMouseLastPosX = Fl::event_x();
    cMouseInitPosY = cMouseLastPosY = Fl::event_y();
    cMouseLastPosZ = TheAppli.getInputPlaneHeight(); // ????

    TheAppli.currentTransform().raz();
    TheAppli.currentValTransf()= 0;
    TheAppli.currentTransform().scaleTo(1);
  }
  //------------------------------
  void Canvas3d::userActionCancel(	int	pEvent )
  {  
    if(cVisitModifSelect!= nullptr )
      {
	cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::CANCEL, TheSelect );
      }
    userActionTerminate(pEvent);
  }									 
  //------------------------------
  void Canvas3d::userActionTerminate(	int	pEvent )
  {
    std::cout << "TERMINATE" << std::endl;
  		
    cMouseInitPosX = cMouseLastPosX = -1;
    cMouseInitPosY = cMouseLastPosY = -1;
    cMouseLastPosY = -1;

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
  // pX, pY sont des coordonnees "ecran"
  
  bool Canvas3d::transform2Dto3D(  int pX, int pY,  PP3d::Point3d & iResult )
  {
    // On lance une droite à partir de la position de la souris
    // le Z n'est pas vraiment important (attention a la precision des doubles quand meme)
    double lH = pixel_h();
  
    // on prend 2 point avec deux Z differents
    PP3d::Point3d lPt0( pX, lH-pY,   lH);
    PP3d::Point3d lPt1( pX, lH-pY, - lH );
    
    
    PP3d::Point3d lR0;
    PP3d::Point3d lR1;
    
    // On projette les deux points dans la base 3D
    getKamera().projectWinToObject( lPt0, lR0, true);
    getKamera().projectWinToObject( lPt1, lR1, true);

    // Calcul du vecteur de la droite
    PP3d::Line3dVect lLineV( lR0, lR1 );


    //  cout <<"transform2Dto3D Plane Height:" << TheAppli.getInputPlaneHeight() << endl;
    bool lOk = false;

    // On 
    switch( TheAppli.getInputPlane() )
      {
      case InputPlaneType::X : lOk = lLineV.intersectPlanX( iResult, TheAppli.getInputPlaneHeight() );
	break;
      case InputPlaneType::Y : lOk = lLineV.intersectPlanY( iResult, TheAppli.getInputPlaneHeight() );
	break;
      case InputPlaneType::Z : lOk = lLineV.intersectPlanZ( iResult, TheAppli.getInputPlaneHeight() );
	break;
        
      case InputPlaneType::Free : { //??????????????????
        double lX = ((double )pX);      
        double lY = lH-((double)pY);
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
    
    if( transform2Dto3D( pX,  pY, lResult ) )
      {
        //        std::cout << "Canvas3d::setCursor3dPosition " << lResult << std::endl;
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
    
    //    MyPref.cDbgEvt = 3;
    
    DBG_EVT( " <<<Event:" << pEvent << " " << fl_eventnames[pEvent] << ">>> "
	     << " button:" << Fl::event_button()
	     << " ctrl: " << Fl::event_ctrl()
	     << " shift:" <<  Fl::event_shift()
	     << " alt:" <<  Fl::event_alt());    
    TheInput.hideCurrentPoint();


    int lResult = 0;
    
    if( pEvent == FL_PUSH
        || pEvent == FL_DRAG )
      {
   	Fl::focus(this);     
      }
    
    if( pEvent ==  FL_ENTER )
      {
        Fl::focus(Fl::belowmouse());
      }
    

    // ATTENTION L'ORDRE D'APPEL EST IMPORTANT !!!!!
    
   if( (lResult = handleCamera( pEvent )) !=0 )
      {
        return lResult;
      }
   //   std::cout << "after hdl camera" << std::endl;
   if( (lResult = handleMenu( pEvent )) !=0 )
      {
        return lResult;
      }
   
   //    std::cout << "after hdl menu" << std::endl;
   if( (lResult = handleInput( pEvent )) !=0 )
      {
        return lResult;
      }
     //std::cout << "after hdl magnet" << std::endl;

    if( (lResult = handleMagnet( pEvent )) !=0 )
      {
        return lResult;
      }
   
   if( (lResult = handleSelect( pEvent )) !=0 )
      {
        return lResult;
      }
   //    std::cout << "after hdl select" << std::endl;
 
   if( (lResult = handleTransform( pEvent )) !=0 )
      {
        return lResult;
      }
   //   std::cout << "after hdl transform" << std::endl;
   
   

   
   switch( pEvent )
      {				
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
		  userActionCancel( pEvent );
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
		    
		  TheAppli.redrawAllCanvas3d(PP3d::Compute::FacetAll);
                  TheAppli.redrawObjectTree();
		  break;
		
		case 0xffad:
		  SelFunct::SelectLess( TheSelect, TheBase );
		    
		  TheAppli.redrawAllCanvas3d(PP3d::Compute::FacetAll);
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
		  userActionTerminate( pEvent );
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

      }
						
    return 1;
  }
  //***************************************


} //namespace 

