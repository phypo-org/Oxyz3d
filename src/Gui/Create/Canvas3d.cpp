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
#include "Gui/MyFlWidget.h"

#include "Dialogs.h"
#include "Win3d.h"

#include "Gui/Preference.h"
#include "Callback.h"


#include "SelFunct.h"


using namespace std;


namespace M3d {
 
   bool sDrawColorSelect=false;


  //***************************************
  Canvas3d::Canvas3d( Win3dBase & pW3d, int pX, int pY, int pW, int  pH, const char *l )
    :Canvas3dBase( pW3d, pX, pY, pW, pH, l)
  {   
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

    
    cViewInputObjectMagnet.cColorPoint.set ( 0.5, 0.5, 1, 0.1);
    cViewInputObjectMagnet.cColorLine.set  ( 0.4, 0.4, 1, 0.1);
    cViewInputObjectMagnet.cColorFacet.set ( 0.3, 0.3, 1, 0.05 );
    cViewInputObjectMagnet.cViewMode = PP3d::ViewMode::SKELETON;
     
  }

  //---------------------------
  Magnet  & Canvas3d::getMagnet()  { return TheCreat.getMagnet(); }  
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
  void Canvas3d::userActionPrepare()
  {	
    cMouseInitPosX = cMouseLastPosX = Fl::event_x();
    cMouseInitPosY = cMouseLastPosY = Fl::event_y();
    cMouseLastPosZ = TheCreat.getInputPlaneHeight(); // ????

    TheCreat.currentTransform().raz();
    TheCreat.currentValTransf()= 0;
    TheCreat.currentTransform().scaleTo(1);
  }
  //------------------------------
  void Canvas3d::userActionCancel()
  {  
    if(cVisitModifSelect!= nullptr )
      {
	cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::CANCEL, TheSelect );
      }
    userActionTerminate();
  }									 
  //------------------------------
  void Canvas3d::userActionTerminate()
  {
    std::cout << "TERMINATE" << std::endl;
  		
    cMouseInitPosX = cMouseLastPosX = -1;
    cMouseInitPosY = cMouseLastPosY = -1;
    cMouseLastPosY = -1;

    TheCreat.setCurrentTransformType( Transform::Nothing );
    TheCreat.currentValTransf() = 0;
    TheCreat.currentTransform().raz();

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


    //  cout <<"transform2Dto3D Plane Height:" << TheCreat.getInputPlaneHeight() << endl;
    bool lOk = false;

    // On 
    switch( TheCreat.getInputPlane() )
      {
      case InputPlaneType::XY : lOk = lLineV.intersectPlanZ( iResult, TheCreat.getInputPlaneHeight() );
	break;
      case InputPlaneType::XZ : lOk = lLineV.intersectPlanY( iResult, TheCreat.getInputPlaneHeight() );
	break;
      case InputPlaneType::ZY : lOk = lLineV.intersectPlanX( iResult, TheCreat.getInputPlaneHeight() );
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
	TheCreat.getDatabase()->setCursorPosition ( lResult );
	TheCreat.setCursorPosition( lResult );
	return true;
      }
    return false;
  }

  
  const char * const CHG_AXIS="a";
  
  const char * const CENTER_ON_SELECTION="A";
 
  const char * const CHG_GRID="g";
  
  const char * const CHG_ORTHO_PERS="p";
  
  const char * const SMOOTH="s";

  
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
  
#define  CMP_KEY(A) A[0] == std::tolower(lStr[0])
  
  //---------------------------------------------------------
  int Canvas3d::handle( int pEvent ) 
  {
    if( getGlobalMode() == GlobalMode::MAGNET )
      {
        PP3d::EntityPtr lLast = TheBase.findHightLightEntity();
        if( lLast )
          getMagnet().setLastHightLigth( lLast );
      }
    
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
    
    //======================================
   if( (lResult = handleCamera( pEvent )) !=0 )
      {
        AINFO( "Camera");        
        return lResult;
      }
    //======================================
   //   std::cout << "after hdl camera" << std::endl;
   if( (lResult = handleMenu( pEvent )) !=0 )
      {
        AINFO( "Menu");
        return lResult;
      }
   
    //======================================
   //    std::cout << "after hdl menu" << std::endl;
   if( (lResult = handleInput( pEvent )) !=0 )
      {
        AINFO( "Input " << TheCreat.getDatabase()->getCursorPosition() );

        
        return lResult;
      }
     //std::cout << "after hdl magnet" << std::endl;
    //======================================

    if( (lResult = handleMagnet( pEvent )) !=0 )
      {
        AINFO( "Magnet");
        return lResult;
      }
    //======================================
   if( (lResult = handleSelect( pEvent )) !=0 )
      {
        size_t lNbSel = TheSelect.getNbSelected();

        if( TheSelect.getSelectType() == PP3d::SelectType::Line ) // car il y a deux ligne pour les facette ? // Mais pour les vrai lignes ?          
          lNbSel /= 2;
        
        if( lNbSel == 1  )
         {
           AINFO( "Selection " << TheSelect.getStrSelectType() << '>' <<  TheSelect.getSelectionVect()[0]->getStringInfo());        
          }
        else
          {
            AINFO( "Selection " << TheSelect.getStrSelectType() << ":" << lNbSel);
          }        
    //        AINFO( "Select");
        return lResult;
      }
   //    std::cout << "after hdl select" << std::endl;
 
   if( (lResult = handleTransform( pEvent )) !=0 )
      {
        AINFO( "Transform");
       return lResult;
      }
   //   std::cout << "after hdl transform" << std::endl;
   
   //======================================
  

   
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
		  userActionCancel( );
                  AINFO( "---");

		  break;
		  //=======================
		case	FL_Tab:
  		  DBG_EVT_NL( " TAB" );

                  cout << "TAB Canvas3d " << endl;
		  if( getUserMode() == ModeUser::MODE_TRANSFORM )
		    {
                      AINFO( "Selection input");
             
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
			  PP3d::Entity* iEntity = TheCreat.getDatabase()->findEntity( lId);
			  if( iEntity != nullptr)
			    {
                             AINFO( "Insert Entity points");
 
			      PP3d::SortEntityVisitorPoint  lVisit;
			      iEntity->execVisitor( lVisit );

			      for( PP3d::PointPtr lPt : lVisit.cVectPoints) // les points sont uniques
				{
				  TheInput.insertCurrentLineSelectPoint( lPt->get(), TheBase );	
				}
			      lFlagRedrawAll = true;
			    }
                          else AINFO( "Entity not found");
			}
		      else
			{		      
			  if( setCursor3dPosition(Fl::event_x(), Fl::event_y() ))
			    {
                              AINFO( "Insert selected point");

                              
			      PP3d::Point3d lResult = TheCreat.getDatabase()->getCursorPosition();			
			      TheCreat.roundInput( lResult );
			      TheInput.insertCurrentLineSelectPoint( lResult, TheBase); 
			      lFlagRedrawAll = true;
			    }
			}
		    }
		  break;
		  //=======================
		case FL_BackSpace:
		  if( getUserMode() == ModeUser::MODE_DRAG )
		    {
                      AINFO( "BackSpace - Delete select input point");
		      TheInput.delCurrentLineSelectPoint(TheBase); 
		    }
		  else if( getUserMode() == ModeUser::MODE_BASE )
		    {		      
                      AINFO( "Delete last input point");
		      TheInput.delLastPoint ();
		    }
		  
		  lFlagRedrawAll = true;
		  break;
		  //=======================
		case FL_Delete:
                  if( TheSelect.getSelectType() ==  PP3d::SelectType::Facet )
                    {
                      AINFO( "Del - Delete facet");

                      mergeFacets(&TheBase, &TheSelect);
                    }
                  else
                    {
                      AINFO( "Del - Delete selection");
                      
                      TheSelect.deleteAllFromDatabase( *TheCreat.getDatabase());
                    }
		  lFlagRedrawAll = true;
		  break;
		  //=======================
		case FL_Up:
		case FL_Down:
		  break;

		case 0xffab:
		  SelFunct::SelectMore( TheSelect, TheBase );
		    
                      AINFO( "Select more");
		  TheCreat.redrawAllCanvas3d(PP3d::Compute::FacetAll);
                  TheCreat.redrawObjectTree();
		  break;
		
		case 0xffad:
                      AINFO( "Select less");
		  SelFunct::SelectLess( TheSelect, TheBase );
		    
		  TheCreat.redrawAllCanvas3d(PP3d::Compute::FacetAll);
                  TheCreat.redrawObjectTree();
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
                  AINFO( "Cancel action");

		  userActionTerminate( );
		  TheCreat.setCurrentTransformType( Transform::Nothing );
		}
	      else if( strcmp( lStr, UNSELECT_ALL) == 0 )
		{
		  if( getUserMode() == ModeUser::MODE_BASE )
		    {
                      AINFO( "Cancel selection");
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
                               AINFO( "Redo");

			  cout << "Shift" << endl;
			  RedoCB( &cMyWin3d, nullptr ); 
			}
		      else
			{
                          AINFO( "Undo");

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
                      getWin().recenterOnSelection(cKamera);
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
                else
                  if( CMP_KEY(SMOOTH))
                    {
                      AINFO( "Subdivide");

                      subdiveCatmullClark( true );                      
                    }
	    
	      /*
		else if( strcmp( lStr, STR_EXIT ) ==0)
		{
		::exit(0);
		}
	      */
	    }
	if( lFlagRedrawAll )
	  TheCreat.redrawAll(  PP3d::Compute::Nothing );
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

