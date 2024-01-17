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
 //---------------------------
  void Canvas3dBase::userChangeKameraView( int pEvent)
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
  int Canvas3d::handleCamera(  int pEvent )
  {
    // GlobalMode quelconque            
       
    if( pEvent == FL_PUSH )
      {        
	// DEBUT MODE DEPLACEMENT CAMERA
	if( Fl::event_button() == FL_MIDDLE_MOUSE )
	  {
	    userActionPrepare( );
	    if( getUserMode() == ModeUser::MODE_BASE )
              {
                changeUserMode( ModeUser::MODE_MOVE_CAMERA );
              }
            userActionPrepare();        
	    return 1;
	  }

	// FIN DEPLACEMENT CAMERA OU SELECTION
	if( Fl::event_button() == FL_LEFT_MOUSE )
	  {
	    if( getUserMode() == ModeUser::MODE_MOVE_CAMERA )
	      {
		changeUserMode( ModeUser::MODE_BASE );
		userActionTerminate( );
                userActionPrepare( );
                
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
            TheCreat.redrawAllCanvas3d(PP3d::Compute::Nothing);
            return 1;
          }					
      }

    if( pEvent ==FL_MOVE)
      {
        if( getUserMode() == ModeUser::MODE_MOVE_CAMERA )
          {
            if( userActionIsRun() )
              {
                userChangeKameraView( pEvent );
                setCursor3dPosition( Fl::event_x(), Fl::event_y());
                TheCreat.redrawAllCanvas3d(PP3d::Compute::Nothing); 
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
