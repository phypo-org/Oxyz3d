#include "Canvas3d.h"


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

  //---------------------------
  // nouveau point venant de la position de la souris (ctrl)
  PP3d::Point3d  Canvas3d::userInputPoint( int x, int y, bool iFinalize )
  {
    PP3d::Point3d lResult;
    
    if( setCursor3dPosition( x, y ) )
      {
	lResult = TheAppli.getDatabase()->getCursorPosition();

	TheAppli.roundInput( lResult );
	
	if( iFinalize )
	  TheInput.addPointToCurrentLine( lResult );
	else
	  TheInput.viewCurrentPoint( lResult );
	
	
	TheAppli.redrawAllCanvas3d( PP3d::Compute::Nothing);
      }
    return lResult;
  }
  //---------------------------
  // nouveau point venant de la position de la souris (ctrl)
  PP3d::Point3d Canvas3d::userInputPoint( bool iFinalize )
  {     
    return userInputPoint( Fl::event_x(), Fl::event_y(),iFinalize );    
  }
  //---------------------------
  // recuperation d'un point d'un objet (shift)
  void Canvas3d::userInputPoint( PP3d::Entity* iEntity )
  {
    AINFO( "Point input mode (ESC to cancel)");
    
    if( iEntity == nullptr)
      {
	DBG_ACT(" **************** userInputPoint Hightlight NOT FOUND" );
	return;
      }
    DBG_ACT(" **************** userInputPoint Hightlight " << iEntity->getType() );
    
    PP3d::SortEntityVisitorPoint  lVisit;
    iEntity->execVisitor( lVisit );

    for( PP3d::PointPtr lPt : lVisit.cVectPoints) // les points sont uniques
      {
	TheInput.addPointToCurrentLine( lPt->get() );	
      }

    TheAppli.redrawAllCanvas3d(PP3d::Compute::Nothing);
  }
  //---------------------------------------------------------
  bool Canvas3d::userDragInputPt(int  pEvent, bool iFinalize )
  {
    cout << "Canvas3d::userDragInputPt " << iFinalize << endl;

    AINFO( "Drag input mode (ESC to quit)");
    
    if( setCursor3dPosition(Fl::event_x(), Fl::event_y() ))
      {
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
		    lResult = lPt->get();	
		  }
	      }
	  }
	else
	  {
	    lResult = TheAppli.getDatabase()->getCursorPosition();
	    TheAppli.roundInput( lResult );
	  }
		
	if( iFinalize )
	  {
	    TheInput.changeCurrentLineSelectPoint( lResult );
	    changeUserMode( ModeUser::MODE_BASE );
	  }
	else	  
	  TheInput.viewCurrentPoint( lResult );
	
	
	TheAppli.redrawAllCanvas3d( PP3d::Compute::Nothing);
      }
    return true;
  }

  //---------------------------
  int Canvas3d::handleInput( int pEvent )
  {
    if( getGlobalMode() != GlobalMode::INPUT)
      {
        return 0;
      }

    if(  getUserMode() == ModeUser::MODE_BASE)
      {
        if( pEvent == FL_PUSH )
          {
            
            if(  Fl::event_button() == FL_LEFT_MOUSE )
              {               
                if( Fl::event_ctrl() ) // CTRL
                  {
                    DBG_ACT(" **************** cUserActionSaisie " );
                    std::cout << " **************** cUserActionSaisie " << std::endl;
                    userInputPoint(true );              
                    userActionPrepare( pEvent );        
                    return 1;  
                  }
               
                if( Fl::event_shift() ) // SHIFT
                  { 
                    PP3d::EntityId lId =TheSelect.getLastHightLightEntityId();
                    
                    std::cout << " **************** cUserActionSaisie SHIFT " << lId  << std::endl;
                    DBG_ACT(" **************** cUserActionSaisie Hightlight  "  << lId );
                    
                    userInputPoint(  TheAppli.getDatabase()->findEntity( lId)  );
                    userActionPrepare( pEvent );        
            
                    return 1;
                  }
              } //   FL_LEFT_MOUSE
            
          } // FL_PUSH
      } // MODE_BASE

    if( getUserMode() == ModeUser::MODE_DRAG )
      {
        if( pEvent == FL_RELEASE )
          {
            if( Fl::event_button() == FL_LEFT_MOUSE )
              {
                userDragInputPt(pEvent, true );
                userActionTerminate( pEvent );
                changeUserMode( ModeUser::MODE_BASE );
                return 1;
              }
          }

        if(  pEvent == FL_DRAG )
          {
            if( userActionIsRun() )
              {
                userDragInputPt(pEvent, false);
                return 1;
              }
          }
      }


    // EXPERIMENTAL
    if(  pEvent == FL_MOVE )
      {
        if(  Fl::event_ctrl() )
          {
            if(  Fl::event_shift() )   // ctrl et shift !!!!!!
              {
                /* std::cout << "SHIFT "
                   << " X: " << cMouseLastPosX
                   << " Y: "  << cMouseLastPosY
                   << " Z:" << cMouseLastPosZ 
                   << std::endl;*/
              
                if( cMouseLastPosZ != -1 )
                  {
                    TheAppli.setInputPlaneHeight( TheAppli.getInputPlaneHeight()
                                                  + (Fl::event_y()-cMouseLastPosZ)/10.0) ; // ?????????????
                    userInputPoint( cMouseLastPosX, cMouseLastPosY, false ); // just view the possible position of point
                  
                  }
              
                cMouseLastPosZ = Fl::event_y();
                return 1;
              }
            else
              {
                //   std::cout << " NO " << std::endl;           
              
                cMouseLastPosX = Fl::event_x();
                cMouseLastPosY = Fl::event_y();
                cMouseLastPosZ = -1;
              
                userInputPoint( false ); // just view the possible position of point
              }
            setCursor3dPosition( Fl::event_x(), Fl::event_y());
            TheAppli.redrawAllCanvas3d(PP3d::Compute::Nothing);
            return -1;
          }                
      }
  
    
    return 0;
  }
  
} //namespace 

