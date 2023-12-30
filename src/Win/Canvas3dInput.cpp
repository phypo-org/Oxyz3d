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
  void Canvas3d::userInputPoint( bool iFinalize )
  {     
    if( setCursor3dPosition(Fl::event_x(), Fl::event_y() ))
      {
	PP3d::Point3d lResult = TheAppli.getDatabase()->getCursorPosition();

	TheAppli.roundInput( lResult );
	
	if( iFinalize )
	  TheInput.addPointToCurrentLine( lResult );
	else
	  TheInput.viewCurrentPoint( lResult );
	
	
	TheAppli.redrawAllCanvas( PP3d::Compute::Nothing);
      }
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

    TheAppli.redrawAllCanvas(PP3d::Compute::Nothing);
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
	
	
	TheAppli.redrawAllCanvas( PP3d::Compute::Nothing);
      }
    return true;
  }
} //namespace 

