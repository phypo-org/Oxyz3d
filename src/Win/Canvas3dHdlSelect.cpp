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
  int Canvas3d::handleSelect(  int pEvent )
  {
    // GlobalMode quelconque    
      
    if( pEvent == FL_PUSH )
      {
        /*
        // SELECTION RECTANGLE : BUG
	if( Fl::event_button() == FL_MIDDLE_MOUSE
	    &&  Fl::event_shift() ) // &&  getUserMode() == ModeUser::MODE_BASE)
	  {	  
	    changeUserMode( ModeUser::MODE_SELECT_RECT );

	    userActionPrepare( pEvent );
            
	    TheAppli.redrawAllCanvas( PP3d::Compute::Nothing);
	    return 1;
	  }
        */
  	if( Fl::event_button() == FL_LEFT_MOUSE)
          {
            std::cout << "*** handleSelect LEFT_MOUSE" << std::endl;
            if( getUserMode() == ModeUser::MODE_BASE )    //DEBUT MODE_SELECT
              {
                cSelectMode = PP3d::SelectMode::Undefine;
                
                if( userSelectionPointColor( pEvent, false ))
                  {
                    changeUserMode( ModeUser::MODE_SELECT);
                  }
                else
                  {
                    changeUserMode( ModeUser::MODE_SELECT_RECT);
                    std::cout << "*** mode SELECT RECT" << std::endl;
                }
                
                userActionPrepare(  );
                TheAppli.redrawAllCanvas3d(PP3d::Compute::Nothing);
               return 1;
              }
          }
      }
    else if( pEvent == FL_RELEASE )
      {
        if( Fl::event_button() == FL_LEFT_MOUSE )
          {
            if( getUserMode() == ModeUser::MODE_SELECT )
	      {
		userActionTerminate(  );
                TheAppli.redrawAllCanvas3d(PP3d::Compute::Nothing);
                return 1;
	      }
          }
              			
	if( getUserMode() == ModeUser::MODE_SELECT_RECT )
	  {
	    userSelectionRectangle(pEvent, true );
	    userActionTerminate(  );
            TheAppli.redrawAllCanvas3d(PP3d::Compute::Nothing);
            return 1;
	  }
      }
    else if(  pEvent == FL_DRAG )
      {    
	if( userActionIsRun() )
          {
            if( getUserMode() == ModeUser::MODE_SELECT ) // Selection à la volée
              {
                TheSelect.removeHightLightEntity( TheBase );
              
                userSelectionPointColor( pEvent, false );
                setCursor3dPosition( Fl::event_x(), Fl::event_y());
                TheAppli.redrawAllCanvas3d(PP3d::Compute::Nothing);
                return 1;
              }
            else if( getUserMode() == ModeUser::MODE_SELECT_RECT )
              {         
                cMouseLastPosX = Fl::event_x();
                cMouseLastPosY = Fl::event_y();
                userSelectionRectangle(pEvent);			
                TheAppli.redrawAllCanvas3d(PP3d::Compute::Nothing);
                 return 1;
             }
         }
      }
    else if(  pEvent == FL_MOVE )
      {
        if( getUserMode() == ModeUser::MODE_BASE )       
          {
            if( MyPref.cSelectPassOverLighting )    // Lighting object when pass over
              {
                userSelectionPointColor( pEvent, true );
                setCursor3dPosition( Fl::event_x(), Fl::event_y());
                TheAppli.redrawAllCanvas3d(PP3d::Compute::Nothing);
                return 1;
              }
          }
      }
    return 0;
  }
  
} //namespace 

