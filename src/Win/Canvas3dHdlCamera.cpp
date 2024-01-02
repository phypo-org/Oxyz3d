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
  int Canvas3d::handleCamera(  int pEvent )
  {
    // GlobalMode quelconque            
       
    if( pEvent == FL_PUSH )
      {        
	// DEBUT MODE DEPLACEMENT CAMERA
	if( Fl::event_button() == FL_MIDDLE_MOUSE )
	  {
	    userPrepareAction( pEvent );
	    if( getUserMode() == ModeUser::MODE_BASE )
              {
                changeUserMode( ModeUser::MODE_MOVE_CAMERA );
              }
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
                userPrepareAction( pEvent );
                
		return 0; /////// L'EVENT PEUT ETRE TRAITER AILLEURS !!!!!!!
	      }
          }
      }
    
    if( pEvent ==FL_DRAG)
      {
        if( getUserMode() == ModeUser::MODE_MOVE_CAMERA )
          {
            userChangeKameraView( pEvent );
            setCursor3dPosition( Fl::event_x(), Fl::event_y());	
            TheAppli.redrawAllCanvas3d(PP3d::Compute::Nothing);
            return 1;
          }					
      }

    if( pEvent ==FL_MOVE)
      {
        if( getUserMode() == ModeUser::MODE_MOVE_CAMERA )
          {
            if( userActionRun() )
              {
                userChangeKameraView( pEvent );
                setCursor3dPosition( Fl::event_x(), Fl::event_y());
                TheAppli.redrawAllCanvas3d(PP3d::Compute::Nothing); 
                return 1;
              }
          }
      }
   else if( pEvent == FL_MOUSEWHEEL )
      {
        if( getUserMode() == ModeUser::MODE_MOVE_CAMERA )
	  {						
            Fl::focus(this);     
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
            return 1;
          }
      }
    return 0;
  }

} // namespace
